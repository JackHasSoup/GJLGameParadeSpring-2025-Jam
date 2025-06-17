#include "SceneDataLoader.h"

SceneData SceneDataLoader::loadScene(std::string const& filename)
{
	//load textures used by editor into asset manager
	for (const auto& file : std::filesystem::directory_iterator("./gfx/materials"))
	{
		if (file.path().extension() == ".png" || file.path().extension() == ".jpg")
		{
			sf::Texture* tex = AssetManager::registerNewTex(file.path().stem().string());
			if (!tex->loadFromFile(file.path().string()))
			{
				std::cerr << "Failed to load texture: " << file.path() << std::endl;
			}
		}
	}

	std::vector<PhysicsObject*> objs;
	std::vector<Light> lights;
	std::vector<sf::FloatRect> rooms;
	std::ifstream in(filename);
	json j;
	in >> j;

	for (auto const& o : j["objects"])
	{
		auto* obj = new PhysicsObject(
			vecFromJson(o["position"]),
			vecFromJson(o["size"]) * o["scale"].get<float>(),
			o.value("mass", 1.f)
		);
		obj->setRotation(o.value("rotation", 0.f)); //set rotation if specified
		obj->setCanMove(false);
		//obj->setScale(o["scale"], o["scale"]);
		//obj->getRAWCollisionShape()->setScale(o["scale"], o["scale"]);

		if (o.contains("drawType"))
			obj->setDrawType(static_cast<drawType>(o["drawType"].get<int>()));
		else
			obj->setDrawType(drawType::BOTH_RC); //default draw type if not specified

		if (o.contains("hull") && o["hull"].is_array())
		{
			sf::ConvexShape hull;
			hull.setPointCount(o["hull"].size());
			for (int i = 0; i < o["hull"].size(); ++i)
			{
				hull.setPoint(i, vecFromJson(o["hull"][i]) * o["scale"].get<float>());
			}

			obj->setCollisionShape(hull);
		}

		setColour(obj, colourFromJson(o["colour"]));
		setTexture(obj, o["textureKey"].get<std::string>());
		objs.push_back(obj);
	}

	//load lights
	for(auto const& lj : j["lights"])
	{
		lights.push_back(Light(
			vecFromJson(lj["position"]),
			lj.value("radius", 0.f),
			colourFromJson(lj["colour"])
		));
	}

	//load rooms
	for (auto const& r : j["rooms"])
	{
		sf::FloatRect room(
			r["left"].get<float>(),
			r["top"].get<float>(),
			r["width"].get<float>(),
			r["height"].get<float>()
		);
		rooms.push_back(room);
	}

	return SceneData(objs, lights, rooms);
}

void SceneDataLoader::setColour(PhysicsObject* obj, sf::Color const& c)
{
	switch (obj->getDrawType())
	{
	case drawType::RECT:
		obj->setFillColor(c);
		break;
	case drawType::COLLISION:
		obj->getRAWCollisionShape()->setFillColor(c);
		break;
	case drawType::BOTH_RC:
		obj->setFillColor(c);
		obj->getRAWCollisionShape()->setFillColor(c);
		break;
	case drawType::BOTH_CR:
		obj->setFillColor(c);
		obj->getRAWCollisionShape()->setFillColor(c);
		break;
	}
}

void SceneDataLoader::setOutlineColour(PhysicsObject* obj, sf::Color const& c, float const& thickness)
{
	switch (obj->getDrawType())
	{
	case drawType::RECT:
		obj->setOutlineColor(c);
		if (thickness > -1.f) obj->setOutlineThickness(thickness);
		break;
	case drawType::COLLISION:
		obj->getRAWCollisionShape()->setOutlineColor(c);
		if (thickness > -1.f) obj->getRAWCollisionShape()->setOutlineThickness(thickness);
		break;
	case drawType::BOTH_RC:
		obj->setOutlineColor(c);
		obj->getRAWCollisionShape()->setOutlineColor(c);
		if (thickness > -1.f) obj->getRAWCollisionShape()->setOutlineThickness(thickness);
		if (thickness > -1.f) obj->setOutlineThickness(thickness);
		break;
	case drawType::BOTH_CR:
		obj->setOutlineColor(c);
		obj->getRAWCollisionShape()->setOutlineColor(c);
		if (thickness > -1.f) obj->setOutlineThickness(thickness);
		if (thickness > -1.f) obj->getRAWCollisionShape()->setOutlineThickness(thickness);
		break;
	}
}

void SceneDataLoader::setTexture(PhysicsObject* obj, sf::String const& name)
{
	auto* t = AssetManager::getTex(name);
	if (!t)return;
	switch (obj->getDrawType())
	{
	case drawType::RECT:
		obj->setTexture(t);
		obj->setTextureRect(sf::IntRect(0, 0, t->getSize().x, t->getSize().y));
		break;
	case drawType::COLLISION:
		obj->getRAWCollisionShape()->setTexture(t);
		obj->getRAWCollisionShape()->setTextureRect(sf::IntRect(0, 0, t->getSize().x, t->getSize().y));
		break;
	case drawType::BOTH_RC:
		obj->setTexture(t);
		obj->getRAWCollisionShape()->setTexture(t);
		obj->setTextureRect(sf::IntRect(0, 0, t->getSize().x, t->getSize().y));
		obj->getRAWCollisionShape()->setTextureRect(sf::IntRect(0, 0, t->getSize().x, t->getSize().y));
		break;
	case drawType::BOTH_CR:
		obj->setTexture(t);
		obj->getRAWCollisionShape()->setTexture(t);
		obj->setTextureRect(sf::IntRect(0, 0, t->getSize().x, t->getSize().y));
		obj->getRAWCollisionShape()->setTextureRect(sf::IntRect(0, 0, t->getSize().x, t->getSize().y));
		break;
	}
}