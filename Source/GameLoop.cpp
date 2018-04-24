#include <Engine\GameTime.h>
#include <Engine\InputEvents.h>
#include <Engine/Sprite.h>
#include <string>

#include "Vector2.h"
#include "GameLoop.h"
#include "GameObject.h"
#include "LevelController.h"

bool GameLoop::init(ASGE::Renderer* render_ptr, int level)
{

	world_controller = std::make_unique<WorldController>();
	lvl_controller = std::make_unique<LevelController>();

	lvl_controller->makeLevel(level, render_ptr);

	render_ptr->setWindowTitle("Angry Birds - Level");

	background = render_ptr->createUniqueSprite();

	switch(level)
	{
		case 1:
			background->loadTexture("..\\..\\Resources\\Textures\\lvl1.png");
	}

	return true;
}

void GameLoop::render(ASGE::Renderer* render_ptr, int game_width, int game_height)
{
	
	render_ptr->renderSprite(*background);
	lvl_controller->renderLevel(render_ptr, game_width, game_height);

	std::string text = "Score: " + std::to_string(score);

	render_ptr->renderText(text, (game_width/2) - (20 * (text.size()/2)), 140, 1.5, ASGE::COLOURS::ORANGE);

	render_ptr->renderText(std::to_string(lvl_controller->getCannon()->getAngle()), 500, 300, 1.5, ASGE::COLOURS::ORANGE);

}

void GameLoop::mouseInput(const ASGE::ClickEvent* click, double mouse_x, double mouse_y)
{
	Bounds mouse;

	mouse.bottom_x = mouse_x;
	mouse.bottom_y = mouse_y;
	mouse.top_x = mouse_x;
	mouse.top_y = mouse_y;

}

void GameLoop::keyInput(const ASGE::KeyEvent* key_event, AngryBirdsGame* main)
{
	if (key_event->key == ASGE::KEYS::KEY_D)
	{
		
	}
	else if (key_event->key == ASGE::KEYS::KEY_A)
	{
		
	}
	else if (key_event->key == ASGE::KEYS::KEY_S)
	{
		current_action = GameAction::DOWN;
	}
	else if (key_event->key == ASGE::KEYS::KEY_W)
	{
		current_action = GameAction::UP;
	}
	else if (key_event->key == ASGE::KEYS::KEY_SPACE)
	{
		
	}
	else if (key_event->key == ASGE::KEYS::KEY_ENTER)
	{
		lvl_controller->debugSetLevelWon();
	}
	else
	{
		current_action = GameAction::NONE;
	}

	if (key_event->action == 0)
	{
		current_action = GameAction::NONE;
	}
}

void GameLoop::update(const ASGE::GameTime& time_data)
{
	
	vector2 camera_position = lvl_controller->getCameraPosition();

	processGameActions(time_data);

	//lvl_controller->checkGravity(lvl_controller->getPlayer(), time_data);


	if (lvl_controller->levelWon() == true)
	{
		
	}
}

void GameLoop::processGameActions(const ASGE::GameTime& time_data)
{
	float rotation_value = 0;
	
	switch (current_action)
	{
	case GameAction::UP:
		if (lvl_controller->getCannon()->getAngle() > -0.1f)
		{
			rotation_value = -3 * (time_data.delta_time.count() / 1000.f);
			lvl_controller->getCannon()->rotateObject(rotation_value);
		}
		break;
	case GameAction::DOWN:
		if (lvl_controller->getCannon()->getAngle() < 0.5f)
		{
			rotation_value = 3 * (time_data.delta_time.count() / 1000.f);
			lvl_controller->getCannon()->rotateObject(rotation_value);
		}
		break;
	case GameAction::SHOOT:
		
		break;
	}
}