#include <string>

#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>

#include "Bounds.h"
#include "GameState.h"

#include "Game.h"
#include "GameLoop.h"
#include "Menu.h"
#include "LevelSelection.h"
#include "Win.h"
#include "Lose.h"

/**
*   @brief   Default Constructor.
*   @details Consider setting the game's width and height
             and even seeding the random number generator.
*/
AngryBirdsGame::AngryBirdsGame()
{
	std::srand(time(NULL));

	menu = std::make_unique<Menu>();
	level_selection = std::make_unique<LevelSelection>();
	core_game = std::make_unique<GameLoop>();
	win_screen = std::make_unique<WinScreen>();
	lose_screen = std::make_unique<LoseScreen>();

	for (int i = 0; i < numOfLevels ; i++)
	{
		level_completed.push_back(false);
		level_score.push_back(0);
	}
}

/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
AngryBirdsGame::~AngryBirdsGame()
{
	this->inputs->unregisterCallback(key_callback_id);
	this->inputs->unregisterCallback(mouse_callback_id);

	level_completed.clear();
	level_score.clear();
}

/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The keyHandler and clickHandler
			 callback should also be set in the initialise function.
*   @return  True if the game initialised correctly.
*/
bool AngryBirdsGame::init()
{
	setupResolution();
	if (!initAPI())
	{
		return false;
	}

	toggleFPS();
	renderer->setWindowTitle("Angry Penguins!");
	renderer->setWindowedMode(ASGE::Renderer::WindowMode::BORDERLESS);
	renderer->setClearColour(ASGE::COLOURS::BLACK);

	// input handling functions
	inputs->use_threads = false;

	key_callback_id = inputs->addCallbackFnc(
		ASGE::E_KEY, &AngryBirdsGame::keyHandler, this);
	
	mouse_callback_id =inputs->addCallbackFnc(
		ASGE::E_MOUSE_CLICK, &AngryBirdsGame::clickHandler, this);

	menu->init(renderer.get());
	level_selection->init(renderer.get(), this);
	win_screen->init(renderer.get());
	lose_screen->init(renderer.get());

	return true;
}

/**
*   @brief   Sets the game window resolution
*   @details This function is designed to create the window size, any 
             aspect ratio scaling factors and safe zones to ensure the 
			 game frames when resolutions are changed in size.
*   @return  void
*/
void AngryBirdsGame::setupResolution()
{
	// how will you calculate the game's resolution?
	// will it scale correctly in full screen? what AR will you use?
	// how will the game be framed in native 16:9 resolutions?
	// here are some abritrary values for you to adjust as you see fit
	// https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
	game_width = 1920;
	game_height = 1080;
}

/**
*   @brief   Processes any key inputs
*   @details This function is added as a callback to handle the game's
			 keyboard input. For this game, calls to this function
			 are thread safe, so you may alter the game's state as you
			 see fit.
*   @param   data The event data relating to key input.
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::keyHandler(const ASGE::SharedEventData data)
{
	auto key = static_cast<const ASGE::KeyEvent*>(data.get());
	
	if (key->key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}
	
	else if (key->key == ASGE::KEYS::KEY_ENTER && 
		     key->action == ASGE::KEYS::KEY_PRESSED &&
		     key->mods == 0x0004)
	{
		if (renderer->getWindowMode() == ASGE::Renderer::WindowMode::WINDOWED)
		{
			renderer->setWindowedMode(ASGE::Renderer::WindowMode::FULLSCREEN);
		}
		else
		{
			renderer->setWindowedMode(ASGE::Renderer::WindowMode::WINDOWED);
		}
	}

	switch (game_state)
	{
		case GAME_STATE::MENU:
			menu->keyboardControl(key, this);
		break;
		case GAME_STATE::LEVEL_SELECTION:
			level_selection->keyboardControl(key, this);
			break;
		case GAME_STATE::GAME:
			core_game->keyInput(key, this);
		break;
		case GAME_STATE::Win:
			win_screen->keyboardControl(key, this);
		break;
		case GAME_STATE::Lost:
			lose_screen->keyboardControl(key, this);
			break;
	}
}

/**
*   @brief   Processes any click inputs
*   @details This function is added as a callback to handle the game's
		     mouse button input. For this game, calls to this function
             are thread safe, so you may alter the game's state as you
             see fit.
*   @param   data The event data relating to key input.
*   @see     ClickEvent
*   @return  void
*/
void AngryBirdsGame::clickHandler(const ASGE::SharedEventData data)
{
	auto click = static_cast<const ASGE::ClickEvent*>(data.get());

	double x_pos, y_pos;
	inputs->getCursorPos(x_pos, y_pos);

	Bounds mouse;
	mouse.bottom_x = x_pos;
	mouse.top_x = x_pos;
	mouse.bottom_y = y_pos;
	mouse.top_y = y_pos;

	switch (game_state)
	{
		case GAME_STATE::MENU:
			menu->mouseControl(mouse, click, this);
		break;
	}
}


/**
*   @brief   Updates the scene
*   @details Prepares the renderer subsystem before drawing the
		     current frame. Once the current frame is has finished
		     the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void AngryBirdsGame::update(const ASGE::GameTime& us)
{


	auto dt_sec = us.delta_time.count() / 1000.0;

	// Calcuate score
	score = 0;

	for (int i = 0; i < numOfLevels; i++)
	{
		score += level_score[i];
	}

	//make sure you use delta time in any movement calculations!
	switch (game_state)
	{
		case GAME_STATE::MENU:
			menu->update(dt_sec);
		break;
		case GAME_STATE::LEVEL_SELECTION:
			level_selection->update(dt_sec);
			break;
		case GAME_STATE::GAME:
			if (level_setup == false)
			{
				core_game->tidyUp();
				core_game->init(renderer.get(), game_level);
				level_setup = true;
			}
			else
			{
				core_game->update(us, this);
			}
			break;
	}

}

/**
*   @brief   Renders the scene
*   @details Renders all the game objects to the current frame.
	         Once the current frame is has finished the buffers are
			 swapped accordingly and the image shown.
*   @return  void
*/
void AngryBirdsGame::render(const ASGE::GameTime &)
{
	renderer->setFont(0);
	
	switch (game_state)
	{
		case GAME_STATE::MENU:
			menu->render(renderer.get());
		break;
		case GAME_STATE::LEVEL_SELECTION:
			level_selection->render(renderer.get());
		break;
		case GAME_STATE::GAME:
			if (level_setup == true)
			{
				core_game->render(renderer.get(), 0, 0);
			}
		break;
		case GAME_STATE::Win:
			win_screen->render(renderer.get());
		break;
		case GAME_STATE::Lost:
			lose_screen->render(renderer.get());
		break;

	}
}

bool AngryBirdsGame::levelUnlocked(int level)
{
	if (level == 0)
	{
		return true;
	}
	else if (level_completed[level - 1] == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
