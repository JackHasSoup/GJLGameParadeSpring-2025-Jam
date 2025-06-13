#include <iostream>
#include "TestScene.h"
#include "StoredMoveBufferTestScene.h"
#include "EDITOR/EditorScene.h"
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <thread>

#ifndef FORCE_EDITOR
#define FORCE_EDITOR false
#endif // !FORCE_EDITOR

int main(int argc, char *argv[])
{
	//Create the window
	float SCALE = 1;
	sf::VideoMode mode = sf::VideoMode(1200, 640);
	if (!FORCE_EDITOR)
		mode = sf::VideoMode::getDesktopMode();

	sf::RenderWindow window(mode, "[WHIMSY ENGINE - GAME]");
	window.setFramerateLimit(120);
	window.setActive(true);

	sf::RenderTexture tex;
	tex.create(mode.width/ SCALE, mode.height/ SCALE);
	sf::Sprite sprite;
	sprite.setTexture(tex.getTexture());
	sprite.setScale(SCALE, -SCALE);
	sprite.setPosition(sf::Vector2f(0, mode.height));

	//init glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW INIT FAILED!\n";
		return -1;
	}

	//fullscreen shader
	bool USEASCII = false;
	sf::Shader ascii;
	ascii.loadFromFile("shaders/ascii.frag", sf::Shader::Fragment);

	//FOR ASCII
	sf::Texture asciiLUT;
	asciiLUT.loadFromFile("gfx/lut/ascii_katakana.png");
	ascii.setUniform("res", sf::Vector2f(window.getSize()));
	ascii.setUniform("ASCIILookup", asciiLUT);

	//FOR GRADIENT
	sf::Shader gradient;
	gradient.loadFromFile("shaders/gradientOverlay.frag", sf::Shader::Fragment);
	gradient.setUniform("opacity", 0.15f);
	gradient.setUniform("topRight", sf::Glsl::Vec4(sf::Color(255, 66, 152)));
	gradient.setUniform("bottomLeft", sf::Glsl::Vec4(sf::Color(83, 255, 246)));
	

	// Initialise input and level objects.
	AudioManager::init();
	Input::init();
	StoredMoveBufferTestScene level(&tex);

	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime;

	if (argc > 1 || FORCE_EDITOR)
	{
		window.setTitle("[WHIMSY ENGINE - EDITOR]");
		bool editing = true;
		EditorScene editor(&tex);

		std::thread paletteThread(&EditorScene::runPaletteWindow, &editor);
		std::thread colourPickerThread(&EditorScene::runColourPickerWindow, &editor);
		std::thread texturePickerThread(&EditorScene::runTextureWindow, &editor);
		paletteThread.detach();
		colourPickerThread.detach();
		texturePickerThread.detach();

		while (window.isOpen() && editing)
		{
			Input::handleEvents(&window);
			deltaTime = clock.restart().asSeconds();

			editor.handleInput(deltaTime);
			editor.update(deltaTime);
			editor.render();

			window.draw(sprite);
			window.display();

			Input::update();
		}
		return 2;
	}

	// Game Loop
	while (window.isOpen())
	{
		//Process window events
		Input::handleEvents(&window, &tex);

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		// Call standard game loop functions (input, update and render)
		switch (GameState::getCurrentState())
		{
		default:
			level.handleInput(deltaTime);
			level.update(deltaTime);
			level.render();
			window.draw(sprite);//USEASCII ? window.draw(sprite, &ascii) : window.draw(sprite);
			window.display();
			break;
		}

		// Update input class, handle pressed keys
		// Must be done last.
		Input::update();
	}
	return 0;
}
