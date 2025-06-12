#include "DeferredIllumination.h"

DeferredIllumination::DeferredIllumination()
{
	
}

DeferredIllumination::~DeferredIllumination()
{
}

void DeferredIllumination::create()
{
	renderQueue = new std::vector<renderPair>;
	illuminator.loadFromFile("shaders/illuminator.frag", sf::Shader::Fragment);

	lightPos.reserve(MAX_LIGHTS);
	lightRadius.reserve(MAX_LIGHTS);
	lightTint.reserve(MAX_LIGHTS);

	lightRect.setSize(sf::Vector2f(target->getSize()));
	lightRect.setOrigin(lightRect.getSize() / 2.f);
	illuminator.setUniform("screenSize", lightRect.getSize());
}

void DeferredIllumination::beginDraw(const sf::Color& clear)
{
	target->clear(clear);
}

void DeferredIllumination::draw(GameObject* drawable, const sf::RenderStates* state)
{
	if (drawable->getDrawType() == drawType::COLLISION) {
		draw(drawable->getRAWCollisionShape(), state);
		return;
	}

	if (drawable->getDrawType() == drawType::BOTH_CR)
	{
		draw(drawable->getRAWCollisionShape(), state);
	}
	
	renderQueue->push_back(
		renderPair(drawable, state ? state : &defaultState)
	);

	for (int i = 0; i < 4; i++) //4 edges on a rect
	{
		auto p1 = drawable->getTransform().transformPoint(drawable->getPoint(i));
		auto p2 = drawable->getTransform().transformPoint(drawable->getPoint(i == 3 ? 0 : i + 1));
		edges.push_back(sf::Glsl::Vec4(p1.x, p1.y, p2.x, p2.y));
	}

	if (drawable->getDrawType() == drawType::BOTH_RC)
	{
		draw(drawable->getRAWCollisionShape(), state);
	}
}

void DeferredIllumination::draw(const sf::ConvexShape* drawable, const sf::RenderStates* state)
{
	renderQueue->push_back(
		renderPair(drawable, state ? state : &defaultState)
	);

	int m = drawable->getPointCount()-1;
	for (int i = 0; i < m+1; i++)
	{
		auto p1 = drawable->getTransform().transformPoint(drawable->getPoint(i));
		auto p2 = drawable->getTransform().transformPoint(drawable->getPoint(i == m ? 0 : i + 1));
		edges.push_back(sf::Glsl::Vec4(p1.x, p1.y, p2.x, p2.y));
	}
}

void DeferredIllumination::endDraw()
{
	//manually bind texture
	segTex = uploadSegmentTex(edges);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, segTex);

	//setup lighting pass uniforms
	lightRect.setPosition(target->getView().getCenter());
	illuminator.setUniform("viewPos", lightRect.getPosition());
	illuminator.setUniform("segmentTex", 0);
	illuminator.setUniform("numSegments", static_cast<float>(edges.size()));

	//render each object with shadow
	//shadowCaster.setUniform("projectionViewMatrix", sf::Glsl::Mat4(target->getView().getTransform().getMatrix()));
	for (auto& p : *renderQueue)
	{
		target->draw(*p.first, *p.second); //draw object over its shadow
	}

	target->draw(lightRect, &illuminator);

	//clear the render queue
	renderQueue->clear();
	edges.clear();
	renderQueue->resize(0);
	edges.resize(0);
}

int DeferredIllumination::addLight(sf::Vector2f const& pos, float const& radius, sf::Color const& tint)
{
	lightPos.push_back(pos);
	lightRadius.push_back(radius);
	lightTint.push_back(tint);

	//update shader
	illuminator.setUniformArray("lightPos", &lightPos[0], lightPos.size());
	illuminator.setUniformArray("lightRadius", &lightRadius[0], lightRadius.size());
	illuminator.setUniformArray("lightTint", &lightTint[0], lightTint.size());
	illuminator.setUniform("numLights", static_cast<float>(lightPos.size()));

	return lightPos.size()-1; //return position of that light in the vector
}

int DeferredIllumination::addLight(Light const& l)
{
	lightPos.push_back(std::get<0>(l));
	lightRadius.push_back(std::get<1>(l));
	lightTint.push_back(std::get<2>(l));

	//update shader
	illuminator.setUniformArray("lightPos", &lightPos[0], lightPos.size());
	illuminator.setUniformArray("lightRadius", &lightRadius[0], lightRadius.size());
	illuminator.setUniformArray("lightTint", &lightTint[0], lightTint.size());
	illuminator.setUniform("numLights", static_cast<float>(lightPos.size()));

	return lightPos.size() - 1; //return position of that light in the vector
}

void DeferredIllumination::setLight(int i, Light const& l)
{
	if (i >= lightPos.size()) return; //avoid out of range exceptions
	lightPos[i] = std::get<0>(l);
	lightRadius[i] = std::get<1>(l);
	lightTint[i] = std::get<2>(l);

	//update shader
	illuminator.setUniformArray("lightPos", &lightPos[0], lightPos.size());
	illuminator.setUniformArray("lightRadius", &lightRadius[0], lightRadius.size());
	illuminator.setUniformArray("lightTint", &lightTint[0], lightTint.size());
	illuminator.setUniform("numLights", static_cast<float>(lightPos.size()));
}

GLuint DeferredIllumination::uploadSegmentTex(const std::vector<sf::Glsl::Vec4>& edges)
{
	std::vector<float> segmentData;

	for (const auto& seg : edges)
	{
		segmentData.push_back(seg.x);
		segmentData.push_back(seg.y);
		segmentData.push_back(seg.z);
		segmentData.push_back(seg.w);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_1D, textureID);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, edges.size(), 0, GL_RGBA, GL_FLOAT, segmentData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	return textureID;
}
