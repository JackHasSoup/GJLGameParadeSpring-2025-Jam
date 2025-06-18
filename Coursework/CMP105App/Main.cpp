#include <iostream>
#include "TutorialScene.h"
#include "TestScene.h"
#include "MenuScene.h"
#include "PauseScene.h"
#include "EDITOR/EditorScene.h"
#include "RoomTestScene.h"
#include "Framework/SceneTransition.h"
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <thread>
#include "BaseEnemyTestScene.h"

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

	GameState::setRenderTarget(&tex);

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

	BaseEnemyTestScene testScene(&tex);
	TutorialScene tutorialScene(&tex);

	MenuScene menu(&tex, &window);
	PauseScene pause(&tex);
	SceneTransition sceneTrans(&tex);


	// Call a scene's render function given its gamestate enum. so you don't have to write a long switch statement

	#define RENDER_SCENE(inputState)\
	switch(inputState){\
	case State::MENU: menu.render(); break;\
	case State::PAUSE: pause.render(); break;\
	case State::TUTORIAL: tutorialScene.render(); break;\
	case State::TEST: testScene.render(); break;\
	}; 

#define UPDATE_SCENE(inputState, dt)\
	switch(inputState){\
	case State::MENU: menu.update(dt); break;\
	case State::PAUSE: pause.update(dt); break;\
	case State::TUTORIAL: tutorialScene.update(dt); break;\
	case State::TEST: testScene.update(dt); break;\
	}; 


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

		GameState::setLastState(GameState::getCurrentState());

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		// Call standard game loop functions (input, update and render)
		switch (GameState::getCurrentState())
		{
		case State::MENU: {
			menu.handleInput(deltaTime);
			menu.update(deltaTime);
			menu.render();
			window.draw(sprite);
			window.display();
			break;
		}
		case State::PAUSE: {
			pause.handleInput(deltaTime);
			pause.update(deltaTime);

			pause.beginDraw();
			RENDER_SCENE(pause.getPausedState());
			pause.render();

			window.draw(sprite);
			window.display();
			break;
		}
		case State::TUTORIAL: {
			tutorialScene.handleInput(deltaTime);
			tutorialScene.update(deltaTime);
			tutorialScene.render();
			window.draw(sprite);
			window.display();
			break;
		}
		case State::TEST: {
			testScene.handleInput(deltaTime);
			testScene.update(deltaTime);
			testScene.render();
			window.draw(sprite);//USEASCII ? window.draw(sprite, &ascii) : window.draw(sprite);
			window.display();
			break;
		}
		case State::TRANSITION: {
			if (sceneTrans.getInTimer() >= 0.f) { // If in the first half of fading
				RENDER_SCENE(sceneTrans.getStartState());
				sceneTrans.fadeIn(deltaTime);
			}
			else if (sceneTrans.getOutTimer() >= 0.f && sceneTrans.getInTimer() <= 0.f) { // If in the second half of fading
				RENDER_SCENE(sceneTrans.getEndState());
				sceneTrans.fadeOut(deltaTime);
			}
			else { // If scene transition is done
				GameState::setCurrentState(sceneTrans.getEndState());
				GameState::setLastState(sceneTrans.getEndState());
			}
			sceneTrans.render();
			window.draw(sprite);
			window.display();
			break;
			}
		}

		if (GameState::getCurrentState() != GameState::getLastState()) { // Call once when a gamestate switches from one to the other
			switch (GameState::getCurrentState()) {
			case State::PAUSE:
				pause.setPausedState(GameState::getLastState());
				break;
			case State::TUTORIAL: case State::TEST: // New levels added here
				if (GameState::getLastState() != State::PAUSE) {
					sceneTrans.setTransition(GameState::getLastState(), GameState::getCurrentState()); // FROM scene TO other scene
					// Call update function for one frame to load in sprites etc.
					UPDATE_SCENE(sceneTrans.getStartState(), deltaTime);
					UPDATE_SCENE(sceneTrans.getEndState(), deltaTime);
					GameState::setCurrentState(State::TRANSITION);
				}
			break;
			}
		}

		// Update input class, handle pressed keys
		// Must be done last.
		Input::update();
	}
	return 0;
}
