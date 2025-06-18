#include "EditorScene.h"

EditorScene::EditorScene(sf::RenderTarget *hwnd) : Scene(hwnd)
{
	//debug for generating props
	/*PhysicsObject p({}, { 64, 64 }, 50);
	p.setFillColor(sf::Color::Red);
	saveProp("./EDITOR/data/Prop/box.prop", p);
	p.setFillColor(sf::Color::Green);
	p.setSize({ 32, 96 });
	saveProp("./EDITOR/data/Prop/tall.prop", p);
	p.setFillColor(sf::Color::Blue);
	p.setSize({ 128, 32 });
	saveProp("./EDITOR/data/Prop/wide.prop", p);*/

	// debug palette props
	for (const auto& file : std::filesystem::directory_iterator("./EDITOR/data/Prop"))
	{
		if (file.path().extension() == ".prop")
		{
			loadProp(file.path().string());
		}
	}

	// debug textures
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

	currentProp = paletteProps[0];

	camera = Camera(midWin, winSize);
	window->setView(camera);

	commander.addHeld(sf::Keyboard::LShift, new GenericCommand([=]{ camera.pan(window->mapPixelToCoords(Input::getIntMousePos()) - camera.getCenter()); }));
	commander.addPressed(sf::Keyboard::Delete, new GenericCommand([=]{ deleteSelectedObject(); }));
	//mode switching
	commander.addPressed(sf::Keyboard::L, new GenericCommand([=] { placingLight ? placeState = PlaceState::OBJECT : placeState = PlaceState::LIGHT; }));
	commander.addPressed(sf::Keyboard::R, new GenericCommand([=] { placingRoom ? placeState = PlaceState::OBJECT : placeState = PlaceState::ROOM; }));
	commander.addPressed(sf::Keyboard::C, new GenericCommand([=] { placingRoom ? placeState = PlaceState::OBJECT : placeState = PlaceState::CREATURE; }));
	//for save load
	commander.addPressed(sf::Keyboard::PageUp, new GenericCommand([=] { saveToFile("levels/level.json"); }));
	commander.addPressed(sf::Keyboard::PageDown, new GenericCommand([=] { loadFromFile("levels/level.json"); }));
	//for scroll
	commander.addHeld(sf::Keyboard::Up, new GenericCommand([=] { scrollOffset += 1.f; }));
	commander.addHeld(sf::Keyboard::Down, new GenericCommand([=] { scrollOffset -= 1.f;}));
	//for scale
	commander.addHeld(sf::Keyboard::Q, new GenericCommand([=] { objects[selectedIndex].obj.scale(1.1f, 1.1f); }));
	commander.addHeld(sf::Keyboard::E, new GenericCommand([=] { objects[selectedIndex].obj.scale(1.f/1.1f, 1.f/1.1f); }));

	lighting.setTarget(dynamic_cast<sf::RenderTexture *>(window));
	lighting.create();

	//init windows
	paletteWin = new sf::RenderWindow(sf::VideoMode(200, 400), "Palette", sf::Style::Titlebar | sf::Style::Close);
	pickerWin = new sf::RenderWindow(sf::VideoMode(320, 380), "HSV Colour Picker", sf::Style::Titlebar | sf::Style::Close);
	texWin = new sf::RenderWindow(sf::VideoMode(60, 400), "Texture Palette", sf::Style::Titlebar | sf::Style::Close);
	paletteWin->setPosition(paletteWin->getPosition() + sf::Vector2i(700, 0));
	pickerWin->setPosition(pickerWin->getPosition() + sf::Vector2i(-700, 0));
	texWin->setPosition(texWin->getPosition() + sf::Vector2i(920, 0));
	paletteWin->setFramerateLimit(15);
	pickerWin->setFramerateLimit(15);
	texWin->setFramerateLimit(15);

	std::cout << "Welcome to the editor!\nWindows:\nColour picker: pick colours for lights/objects\nProp Palette: pick props to place\nTexture palette: pick textures to apply to props\n\n\nControls:\nLeft Click: place\nLeft Click Drag: move selected\nRight Click: rotate selected\nPgUp: Save\nPgDn: Load\nUp/Down: scroll texture palette\nQ/E: scale selected\n\nMODE SWITCHING:\nL: toggle between placing lights/objects\nR: toggle between rooms/objects\nC: toggle between creatures/objects\n\n\n";
	std::cout << "WARNING: placed creatures sizes are purely illustrative placeholders, their sizes in editor probably won't match their actual sizes in game.\n\n";
}

void EditorScene::handleInput(float dt)
{
	//before anything else, update the windows 
	/*Input::handleEvents(paletteWin);
	Input::handleEvents(pickerWin);
	Input::handleEvents(texWin);*/

	commander.handleInput();

	sf::Vector2f mousePos = window->mapPixelToCoords(Input::getIntMousePos());

	switch (placeState)
	{
	case PlaceState::OBJECT:
	{
		// left click: select or add
		if (Input::isLeftMousePressed())
		{
			selectObjectAt(mousePos);
			if (selectedIndex == -1)
			{
				addObject(mousePos);
			}
		}
		// left click and hold: drag
		else if (Input::isLeftMouseDown() && selectedIndex != -1)
		{
			if (!dragging)
			{
				dragging = true;
				dragOffset = objects[selectedIndex].obj.getPosition() - mousePos;
			}
		}
		// right click and hold: rotate object
		else if (Input::isRightMouseDown() && selectedIndex != -1)
		{
			const sf::Vector2f dir = mousePos - objects[selectedIndex].obj.getPosition();
			objects[selectedIndex].obj.setRotation(Deg(VectorHelper::angle(dir)));
		}

		// drag selected object
		if (dragging && selectedIndex != -1 && Input::isLeftMouseDown())
		{
			objects[selectedIndex].obj.setPosition(mousePos + dragOffset);
		}

		// release drag
		if (dragging && !Input::isLeftMouseDown())
		{
			dragging = false;
		}
		break;
	}

	case PlaceState::LIGHT:
	{
		//drag existing light
		if (draggingLight && selectedLightIndex != -1 && Input::isLeftMouseDown())
		{
			std::get<0>(placedLights[selectedLightIndex]) = mousePos + lightDragOffset;
			return;
		}
		// Release drag
		if (draggingLight && !Input::isLeftMouseDown())
		{
			draggingLight = false;
			selectedLightIndex = -1;
			return;
		}

		//on left click: select light for dragging or start placing new
		if (Input::isLeftMousePressed() && !midLightPlace)
		{
			//check if clicking on an existing light
			selectedLightIndex = -1;
			for (size_t i = 0; i < placedLights.size(); ++i)
			{
				float dist = VectorHelper::magnitude(mousePos - std::get<0>(placedLights[i]));
				if (dist < std::get<1>(placedLights[i]))
				{
					selectedLightIndex = static_cast<int>(i);
					break;
				}
			}
			if (selectedLightIndex != -1)
			{
				draggingLight = true;
				lightDragOffset = std::get<0>(placedLights[selectedLightIndex]) - mousePos;
				return;
			}
			//otherwise, start placing new light
			midLightPlace = true;
			placedLights.push_back(std::make_tuple(mousePos, 0.f, currentColour));
		}
		else if (midLightPlace)
		{
			float radius = VectorHelper::magnitude(mousePos - std::get<0>(placedLights.back()));
			std::get<1>(placedLights.back()) = radius;

			if (Input::isLeftMousePressed())
			{
				lighting.addLight(placedLights.back());
				midLightPlace = false;
			}
		}
		return; //skip normal prop/object placement when in light mode
	}
		break;


	case PlaceState::ROOM:
	{
		//just like light placing, first click is where the top left corner of the room is, second click is bottom right corner
		if (Input::isLeftMousePressed() && !midLightPlace)
		{
			//check if clicking on an existing room
			activeRoomIndex = -1;
			for (size_t i = 0; i < placedRooms.size(); ++i)
			{
				if (placedRooms[i].contains(mousePos))
				{
					std::cout << "Dragging existing room at index: " << i << std::endl;
					activeRoomIndex = static_cast<int>(i);
					break;
				}
			}

			if (activeRoomIndex != -1)
			{
				//drag existing room
				if (!dragging)
				{
					dragging = true;
				}
				return; //skip normal prop/object placement when in room mode
			}

			//else new room
			midLightPlace = true;
			placedRooms.push_back(sf::FloatRect(mousePos, { 0.f, 0.f }));
			activeRoomIndex = static_cast<int>(placedRooms.size()) - 1; //set active room to the last one added
			return;
		}
		else if (midLightPlace) //place bottom corner
		{
			sf::FloatRect& room = placedRooms[activeRoomIndex];
			room.width = mousePos.x - room.left;
			room.height = mousePos.y - room.top;
			if (Input::isLeftMousePressed())
			{
				midLightPlace = false;
			}
			return;
		}

		if (dragging && activeRoomIndex != -1 && Input::isLeftMouseDown() && !midLightPlace)
		{
			//dragging existing room
			sf::FloatRect& room = placedRooms[activeRoomIndex];
			sf::Vector2f newPos = mousePos - sf::Vector2f(room.width / 2.f, room.height / 2.f);
			room.left = newPos.x;
			room.top = newPos.y;
			return;
		}

		return;
	}
		break;

	case PlaceState::CREATURE:
	{
		// left click: select or add
		if (Input::isLeftMousePressed())
		{
			selectObjectAt(mousePos);
			if (selectedCreatureIndex == -1)
			{
				addObject(mousePos);
			}
		}
		// left click and hold: drag
		else if (Input::isLeftMouseDown() && selectedCreatureIndex != -1)
		{
			if (!dragging)
			{
				dragging = true;
				dragOffset = creatures[selectedCreatureIndex].obj.getPosition() - mousePos;
			}
		}
		// right click and hold: rotate object
		else if (Input::isRightMouseDown() && selectedCreatureIndex != -1)
		{
			const sf::Vector2f dir = mousePos - creatures[selectedCreatureIndex].obj.getPosition();
			creatures[selectedCreatureIndex].obj.setRotation(Deg(VectorHelper::angle(dir)));
		}

		// drag selected object
		if (dragging && selectedCreatureIndex != -1 && Input::isLeftMouseDown())
		{
			creatures[selectedCreatureIndex].obj.setPosition(mousePos + dragOffset);
		}

		// release drag
		if (dragging && !Input::isLeftMouseDown())
		{
			dragging = false;
		}
		break;
	}
	}
}

void EditorScene::update(float dt)
{
	camera.update(dt);

	// Sync lights with DeferredIllumination
	lighting.clearLights();
	for (const auto &l : placedLights)
	{
		lighting.addLight(l);
	}

	//if a light or object is selected and colourChanged, set colour of that obect/light to currentColour
	if (placingLight)
	{
		if (selectedLightIndex != -1 && colourChanged)
		{
			std::get<2>(placedLights[selectedLightIndex]) = currentColour;
			colourChanged = false;
		}
	}
	else {
		if (selectedIndex != -1)
		{
			if (colourChanged)
			{
				SceneDataLoader::setColour(&objects[selectedIndex].obj, currentColour);
				colourChanged = false;
			}

			if (texChanged)
			{
				objects[selectedIndex].tex = selectedTex;
				SceneDataLoader::setTexture(&objects[selectedIndex].obj, selectedTex);
				texChanged = false;
			}
		}
	}
}

void EditorScene::render()
{
	window->setView(camera);

	lighting.beginDraw(sf::Color(130, 112, 148));

	for (auto &placed : objects)
	{
		if (placed.selected)
			SceneDataLoader::setOutlineColour(&placed.obj, sf::Color::Yellow, 2.f);
		else
			SceneDataLoader::setOutlineColour(&placed.obj, sf::Color::Black, 2.f);
	   lighting.draw(&placed.obj);
	}

	for (auto& placed : creatures)
	{
		if (placed.selected)
			SceneDataLoader::setOutlineColour(&placed.obj, sf::Color::Yellow, 2.f);
		else
			SceneDataLoader::setOutlineColour(&placed.obj, sf::Color::Black, 2.f);
		lighting.draw(&placed.obj);
	}
	lighting.endDraw();

	// Draw placed lights as circles for visualization
	for (const auto& l : placedLights) 
	{
		sf::CircleShape c(std::get<1>(l)); // radius
		c.setOrigin(std::get<1>(l), std::get<1>(l));
		c.setPosition(std::get<0>(l));
		c.setFillColor(sf::Color::Transparent);
		if (midLightPlace)
		{
			sf::Color fill = std::get<2>(l); fill.a = 20;
			c.setFillColor(fill);
			c.setOutlineColor(std::get<2>(l));
			c.setOutlineThickness(2.f);
		}
		window->draw(c);
	}

	// Draw placed rooms
	for (const auto& room : placedRooms)
	{
		sf::RectangleShape rect(sf::Vector2f(room.width, room.height));
		rect.setPosition(room.left, room.top);
		rect.setFillColor(sf::Color(100, 100, 100, 50));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1.f);
		window->draw(rect);
	}
}

void EditorScene::selectObjectAt(const sf::Vector2f &pos)
{
	if (placingObject)
	{
		selectedIndex = -1;
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (objects[i].obj.getGlobalBounds().contains(pos))
			{
				selectedIndex = static_cast<int>(i);
				break;
			}
		}
		for (size_t i = 0; i < objects.size(); ++i)
			objects[i].selected = (i == selectedIndex);
	}
	else if (placingCreature)
	{
		selectedCreatureIndex = -1;
		for (size_t i = 0; i < creatures.size(); ++i)
		{
			if (creatures[i].obj.getGlobalBounds().contains(pos))
			{
				selectedCreatureIndex = static_cast<int>(i);
				break;
			}
		}
		for (size_t i = 0; i < creatures.size(); ++i)
			creatures[i].selected = (i == selectedCreatureIndex);
	}
}

void EditorScene::setCurrentProp(const PhysicsObject &prop)
{
	currentProp = prop;
}

void EditorScene::addObject(const sf::Vector2f &pos)
{
	if (placingObject)
	{
		PlacedObject placed;
		placed.obj = currentProp;
		placed.obj.setCanMove(false);
		SceneDataLoader::setColour(&placed.obj, currentColour); // set the colour of the object
		placed.tex = selectedTex;
		placed.obj.setPosition(pos);
		placed.selected = true;
		//placed.roomIndex = activeRoomIndex; // set the room index to the currently active room (don't do this, only creatures added to rooms not props)
		objects.push_back(placed);
		selectedIndex = static_cast<int>(objects.size()) - 1;
		for (size_t i = 0; i < objects.size(); ++i)
			objects[i].selected = (i == selectedIndex);
	}
	else if (placingCreature && activeRoomIndex != -1)
	{
		PlacedCreature placed;
		placed.obj = currentProp;
		placed.obj.setCanMove(false);
		SceneDataLoader::setColour(&placed.obj, currentColour); // set the colour of the object
		placed.tex = selectedTex;
		placed.obj.setPosition(pos);
		placed.selected = true;
		placed.roomIndex = activeRoomIndex;
		//placed.roomIndex = activeRoomIndex; // set the room index to the currently active room (don't do this, only creatures added to rooms not props)
		creatures.push_back(placed);
		selectedCreatureIndex = static_cast<int>(creatures.size()) - 1;
		for (size_t i = 0; i < creatures.size(); ++i)
			creatures[i].selected = (i == selectedCreatureIndex);
	}
}

void EditorScene::deleteSelectedObject()
{
	if (placingObject)
	{
		if (selectedIndex >= 0 && selectedIndex < static_cast<int>(objects.size()))
		{
			objects.erase(objects.begin() + selectedIndex);
			selectedIndex = -1;
		}
	}
	else {
		if (selectedCreatureIndex >= 0 && selectedCreatureIndex < static_cast<int>(creatures.size()))
		{
			creatures.erase(creatures.begin() + selectedCreatureIndex);
			selectedCreatureIndex = -1;
		}
	}
}

void EditorScene::runPaletteWindow()
{
	while (paletteWin->isOpen())
	{
		sf::Event event;
		while (paletteWin->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				paletteWin->close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				for (size_t i = 0; i < paletteProps.size(); ++i)
				{
					sf::FloatRect rect(30, 30 + (scrollOffset * 5.f)  + i * 80, paletteProps[i].getSize().x, 80.f);
					if (rect.contains(mousePos))
					{
						setCurrentProp(paletteProps[i]);
						break;
					}
				}
			}
		}
		paletteWin->clear(sf::Color(40, 40, 40));

		for (size_t i = 0; i < paletteProps.size(); ++i)
		{
			PhysicsObject rect = paletteProps[i];
			rect.setPosition(30, 30 + (scrollOffset*5.f) + i * 80);
			SceneDataLoader::setOutlineColour(&rect, sf::Color::White, 2.f);

			if (rect.getSize().y > 80)
			{
				float const& scale = 80.f / rect.getSize().y;
				rect.setScale(scale, scale);
			}
			paletteWin->draw(rect);
		}
		paletteWin->display();
	}
}

void EditorScene::runColourPickerWindow()
{
	//OKLAB values (L: 0-1, a: -0.4 to 0.4, b: -0.4 to 0.4 approximately)
	float lightness = 0.5f, a_axis = 0.0f, b_axis = 0.0f;

	//convert current color to OKLAB
	OKLab currentOKLab = VectorHelper::RGBtoOKLAB(currentColour);
	lightness = currentOKLab.L;
	a_axis = currentOKLab.a;
	b_axis = currentOKLab.b;

	colourChanged = false;

	//UI geometry
	sf::Vector2f lightnessBarPos(20, 20), lightnessBarSize(20, 320);
	sf::Vector2f abBoxPos(60, 20), abBoxSize(240, 320);

	while (pickerWin->isOpen())
	{
		sf::Event event;
		while (pickerWin->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				pickerWin->close();

			if (event.type == sf::Event::MouseButtonPressed ||
				(event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)))
			{
				sf::Vector2i mouse = sf::Mouse::getPosition(*pickerWin);
				sf::Vector2f mouseF(mouse.x, mouse.y);

				//lightness bar
				if (mouseF.x >= lightnessBarPos.x && mouseF.x < lightnessBarPos.x + lightnessBarSize.x &&
					mouseF.y >= lightnessBarPos.y && mouseF.y < lightnessBarPos.y + lightnessBarSize.y)
				{
					float relY = mouseF.y - lightnessBarPos.y;
					lightness = 1.0f - std::clamp(relY / lightnessBarSize.y, 0.0f, 1.0f); //inverted so white is at top
				}

				//A-B box (chroma plane)
				if (mouseF.x >= abBoxPos.x && mouseF.x < abBoxPos.x + abBoxSize.x &&
					mouseF.y >= abBoxPos.y && mouseF.y < abBoxPos.y + abBoxSize.y)
				{
					float relX = mouseF.x - abBoxPos.x;
					float relY = mouseF.y - abBoxPos.y;

					//map to A-B space (centered at 0, ranging approximately -0.4 to 0.4)
					a_axis = (relX / abBoxSize.x - 0.5f) * 0.8f; // -0.4 to 0.4
					b_axis = (0.5f - relY / abBoxSize.y) * 0.8f; // -0.4 to 0.4 (inverted Y)
				}
			}
		}

		//draw lightness bar
		sf::Image lightnessImg;
		lightnessImg.create((unsigned)lightnessBarSize.x, (unsigned)lightnessBarSize.y);
		for (unsigned y = 0; y < (unsigned)lightnessBarSize.y; ++y)
		{
			float L = 1.0f - (float)y / lightnessBarSize.y; //white at top, black at bottom
			OKLab lab(L, a_axis, b_axis); //use current a,b values
			sf::Color c = VectorHelper::OKLABtoRGB(lab);

			for (unsigned x = 0; x < (unsigned)lightnessBarSize.x; ++x)
				lightnessImg.setPixel(x, y, c);
		}
		sf::Texture lightnessTex;
		lightnessTex.loadFromImage(lightnessImg);
		sf::Sprite lightnessSprite(lightnessTex);
		lightnessSprite.setPosition(lightnessBarPos);

		//draw A-B box (chroma plane)
		sf::Image abImg;
		abImg.create((unsigned)abBoxSize.x, (unsigned)abBoxSize.y);
		for (unsigned y = 0; y < (unsigned)abBoxSize.y; ++y)
		{
			float b = (0.5f - (float)y / abBoxSize.y) * 0.8f; // -0.4 to 0.4
			for (unsigned x = 0; x < (unsigned)abBoxSize.x; ++x)
			{
				float a = ((float)x / abBoxSize.x - 0.5f) * 0.8f; // -0.4 to 0.4
				OKLab lab(lightness, a, b);
				sf::Color c= VectorHelper::OKLABtoRGB(lab);
				abImg.setPixel(x, y, c);
			}
		}
		sf::Texture abTex;
		abTex.loadFromImage(abImg);
		sf::Sprite abSprite(abTex);
		abSprite.setPosition(abBoxPos);

		//update currentColour
		OKLab currentLab(lightness, a_axis, b_axis);
		sf::Color newColor = VectorHelper::OKLABtoRGB(currentLab);

		if (newColor.r != currentColour.r || newColor.g != currentColour.g || newColor.b != currentColour.b)
			colourChanged = true;
		currentColour = newColor;

		//draw preview
		sf::RectangleShape preview(sf::Vector2f(280, 30));
		preview.setPosition(20, 350);
		preview.setFillColor(currentColour);
		preview.setOutlineColor(sf::Color::Black);
		preview.setOutlineThickness(2.f);

		//draw crosshairs to show current selection
		sf::RectangleShape lightnessIndicator(sf::Vector2f(lightnessBarSize.x + 4, 2));
		lightnessIndicator.setPosition(lightnessBarPos.x - 2, lightnessBarPos.y + (1.0f - lightness) * lightnessBarSize.y - 1);
		lightnessIndicator.setFillColor(sf::Color::White);
		lightnessIndicator.setOutlineColor(sf::Color::Black);
		lightnessIndicator.setOutlineThickness(1.f);

		sf::CircleShape abIndicator(4);
		float abX = abBoxPos.x + (a_axis / 0.8f + 0.5f) * abBoxSize.x - 4;
		float abY = abBoxPos.y + (0.5f - b_axis / 0.8f) * abBoxSize.y - 4;
		abIndicator.setPosition(abX, abY);
		abIndicator.setFillColor(sf::Color::Transparent);
		abIndicator.setOutlineColor(sf::Color::White);
		abIndicator.setOutlineThickness(2.f);

		pickerWin->clear(sf::Color(60, 60, 60));
		pickerWin->draw(lightnessSprite);
		pickerWin->draw(abSprite);
		pickerWin->draw(preview);
		pickerWin->draw(lightnessIndicator);
		pickerWin->draw(abIndicator);
		pickerWin->display();
	}
}

void EditorScene::runTextureWindow()
{
	while (texWin->isOpen())
	{
		sf::Event event;
		while (texWin->pollEvent(event))
		{
			//not exactly safe, but only one window is recieving events at a time so shouldn't cause problems
			switch (event.type)
			{
			case sf::Event::Closed:
				texWin->close();
				break;
			case sf::Event::KeyPressed:
				// update input class
				Input::setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				//update input class
				Input::setKeyUp(event.key.code);
				break;
			default:
				break;
			}
				
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				int totalHeight = 30;
				for (const auto& tex : AssetManager::getAllTextures())
				{
					sf::FloatRect rect(20, totalHeight + scrollOffset*5.f, 40, 40);
					totalHeight += 50;
					if (rect.contains(mousePos))
					{
						selectedTex = tex.first;
						texChanged = true;
						break;
					}
				}
			}
		}
		texWin->clear(sf::Color(40, 40, 40));
		int totalHeight = 30;
		for (const auto& tex : AssetManager::getAllTextures())
		{
			sf::RectangleShape rect({ 40.f, 40.f });
			rect.setTexture(tex.second);
			rect.setPosition(20, totalHeight + scrollOffset * 5.f);
			rect.setOutlineColor(sf::Color::White);
			rect.setOutlineThickness(2.f);

			texWin->draw(rect);
			totalHeight += 50;
		}
		texWin->display();
	}
}

void EditorScene::saveToFile(const std::string& filename)
{
	json j;
	//save objects
	for (auto& obj : objects)
	{
		json o;
		o["position"] = SceneDataLoader::vecToJson(obj.obj.getPosition());
		o["size"] = SceneDataLoader::vecToJson(obj.obj.getSize());
		o["mass"] = 1.f;
		o["drawType"] = static_cast<int>(obj.obj.getDrawType());
		o["textureKey"] = obj.tex.toAnsiString();
		o["scale"] = obj.obj.getScale().x;
		o["rotation"] = obj.obj.getRotation();
		//o["roomIndex"] = obj.roomIndex; // save room index (don't do this, only creatures are assigned to a room)
		if (obj.obj.getCollisionShape().getPointCount() > 0)
		{
			o["hull"] = json::array();
			for (size_t i = 0; i < obj.obj.getCollisionShape().getPointCount(); ++i)
			{
				o["hull"].push_back(SceneDataLoader::vecToJson(obj.obj.getCollisionShape().getPoint(i)));
			}
		}

		if (obj.obj.getDrawType() == drawType::COLLISION)
		{
			o["colour"] = SceneDataLoader::colourToJson(obj.obj.getRAWCollisionShape()->getFillColor());
		}
		else {
			o["colour"] = SceneDataLoader::colourToJson(obj.obj.getFillColor());
		}

		j["objects"].push_back(o);
	}
	// save lights
	for (const auto& l : placedLights) 
	{
		json lj;
		lj["position"] = SceneDataLoader::vecToJson(std::get<0>(l));
		lj["radius"] = std::get<1>(l);
		lj["colour"] = SceneDataLoader::colourToJson(std::get<2>(l));
		j["lights"].push_back(lj);
	}

	// save rooms
	j["rooms"] = json::array();
	for (const auto& room : placedRooms)
	{
		json r;
		r["left"] = room.left;
		r["top"] = room.top;
		r["width"] = room.width;
		r["height"] = room.height;
		j["rooms"].push_back(r);
	}

	std::ofstream out(filename);
	out << j.dump(4);
}

void EditorScene::loadFromFile(const std::string& filename)
{
	std::ifstream in(filename);
	if (!in) return;
	json j;
	in >> j;

	objects.clear();
	placedLights.clear();

	//load objects
	for (const auto& o : j["objects"]) 
	{
		PlacedObject obj;
		obj.obj = PhysicsObject(
			SceneDataLoader::vecFromJson(o["position"]),
			SceneDataLoader::vecFromJson(o["size"]),
			o.value("mass", 1.f)
		);
		obj.obj.setRotation(o.value("rotation", 0.f)); //set rotation if specified
		obj.obj.setCanMove(false);

		if (o.contains("drawType"))
			obj.obj.setDrawType(static_cast<drawType>(o["drawType"].get<int>()));
		else
			obj.obj.setDrawType(drawType::BOTH_RC); //default draw type if not specified

		if (o.contains("hull") && o["hull"].is_array())
		{
			sf::ConvexShape hull;
			hull.setPointCount(o["hull"].size());
			for (size_t i = 0; i < o["hull"].size(); ++i)
			{
				hull.setPoint(i, SceneDataLoader::vecFromJson(o["hull"][i]));
			}
			obj.obj.setCollisionShape(hull);
		}

		obj.obj.setScale(o["scale"], o["scale"]);
		obj.obj.getRAWCollisionShape()->setScale(o["scale"], o["scale"]);

		SceneDataLoader::setColour(&obj.obj, SceneDataLoader::colourFromJson(o["colour"]));

		if (o.value("textureKey", "").length() > 2)
			obj.tex = o["textureKey"].get<std::string>();
			SceneDataLoader::setTexture(&obj.obj, o["textureKey"].get<std::string>());

		obj.selected = false;
		//obj.roomIndex = o.value("roomIndex", -1); // load room index, default to -1 if not present (don't do this, only creatures are assigned to a room)
		objects.push_back(obj);
	}
	//load lights
	for (const auto& lj : j["lights"]) 
	{
		placedLights.push_back(std::make_tuple(
			SceneDataLoader::vecFromJson(lj["position"]),
			lj.value("radius", 0.f),
			SceneDataLoader::colourFromJson(lj["colour"])
		));
	}

	//load rooms
	placedRooms.clear();
	for (const auto& r : j["rooms"])
	{
		sf::FloatRect room(
			r["left"].get<float>(),
			r["top"].get<float>(),
			r["width"].get<float>(),
			r["height"].get<float>()
		);
		placedRooms.push_back(room);
	}
}

void EditorScene::loadProp(std::string const& filename)
{
	std::ifstream in(filename);
	if (!in) return;
	json j;
	in >> j;

	PhysicsObject obj(
		{},
		SceneDataLoader::vecFromJson(j["size"]),
		j.value("mass", 1.f)
	);
	obj.setDrawType(static_cast<drawType>(j.value("drawType", 0)));

	if (j["hull"].is_array())
	{
		sf::ConvexShape hull;
		hull.setPointCount(j["hull"].size());
		for (size_t i = 0; i < j["hull"].size(); ++i)
		{
			hull.setPoint(i, SceneDataLoader::vecFromJson(j["hull"][i]) + obj.getOrigin()); //+origin to account for prop editor centring hull on 0,0 (top left)
		}
		obj.setCollisionShape(hull);
	}
	SceneDataLoader::setColour(&obj, SceneDataLoader::colourFromJson(j["colour"]));

	paletteProps.push_back(obj);
}

void EditorScene::saveProp(std::string const& filename, PhysicsObject propToBe)
{
	//takes in a filepath and a physics object, then saves to a .prop file (json format).
	json j;

	//basic data
	j["size"] = SceneDataLoader::vecToJson(propToBe.getSize());
	j["colour"] = SceneDataLoader::colourToJson(propToBe.getFillColor());
	j["mass"] = propToBe.getMass();
	j["drawType"] = (int)propToBe.getDrawType();

	//collision shape
	if (propToBe.getCollisionShape().getPointCount())
	{
		j["hull"] = json::array();
		for (size_t i = 0; i < propToBe.getCollisionShape().getPointCount(); ++i)
		{
			j["hull"].push_back(SceneDataLoader::vecToJson(propToBe.getCollisionShape().getPoint(i)));
		}
	}

	std::ofstream out(filename);
	if (!out) return; //check if file opened successfully
	out << j.dump(4); //write json to file with 4 spaces indentation
	out.close();
	std::cout << "Saved prop to " << filename << std::endl;

}
