#pragma once
#include "GameObject.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include "SFML/OpenGL.hpp"
#include <vector>
#include <algorithm>
#include <tuple>

#ifndef renderPair
	#define renderPair std::pair<const sf::Drawable*, const sf::RenderStates*>
	//if theres no renderPair, nothing else is defined in this class either
	#define Light std::tuple<sf::Vector2f, float, sf::Color> //pos, radius, tint
	#define MAX_LIGHTS 15
#endif // !renderPair


//if this name sounds pointlessly pretentious thats because it is :)
//deferred means that a shadow/lighting pass it composited (drawn) over the top of a regularly rendered scene
//I use the word Illumination instead of Lighting because it sounds better ¯\_(ツ)_/¯
//any problems with this please email the whimsical wizards

//how lighting works:
/*
shadow texture
objects drawn to shadow texture with a vert shader that stretches them correctly according to light position and radius and frag that makes them black
used as mask to subtract from the lights pass texture
draw regular scene (base pass)
draw lighting pass overtop(with that mask)
draw shadow mask at 50% opacity over that (to darken areas in shadow)
*/
class DeferredIllumination {
public:
	DeferredIllumination();
	~DeferredIllumination();

	void setTarget(sf::RenderTexture* target) { this->target = target; };
	void create();
	//clears the target with a colour
	void beginDraw(const sf::Color& clear);
	//misleading name to stay consistent with drawing to a target. adds drawable to render stack for later processing
	void draw(GameObject* drawable, const sf::RenderStates* state = nullptr);
	void draw(const sf::ConvexShape* drawable, const sf::RenderStates* state = nullptr);
	//where the real magic happens. objects are sorted top down, then shadows and lighting applied
	void endDraw();

	int addLight(sf::Vector2f const& pos, float const& radius, sf::Color const& tint);
	int addLight(Light const& l);
	void setLight(int i, Light const& l);
	void clearLights() {lightPos.clear();lightRadius.clear();lightTint.clear();}

	void setAmbientColour(const sf::Color& ambient);
	void setAmbientLight(float const& strength); //0-1 darkness value

	GLuint uploadSegmentTex(const std::vector<sf::Glsl::Vec4>& edges);
private:
	//not exactly memory efficient, every object drawn without a state will reserve ~8 bytes of unused memory
	std::vector<renderPair>* renderQueue = nullptr;
	std::vector<sf::Glsl::Vec4> edges;

	std::vector<sf::Vector2f> lightPos;
	std::vector<float> lightRadius;
	std::vector<sf::Glsl::Vec4> lightTint;

	sf::RenderTexture* target = nullptr; //alternatively think about it as the "base" pass (unlit)

	sf::Shader illuminator;
	sf::RenderStates defaultState;
	sf::RectangleShape lightRect;

	GLuint segTex;
	sf::Color ambientColour = sf::Color(0,0,0,255.f*0.65f); //ambient light colour, shadow colour
};