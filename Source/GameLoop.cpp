#include <Engine\GameTime.h>
#include <Engine\InputEvents.h>
#include <Engine/Sprite.h>
#include <string>

#include "Vector2.h"
#include "GameLoop.h"
#include "GameObject.h"
#include "Cannon.h"
#include "LevelController.h" 
#include "Game.h"

bool GameLoop::init(ASGE::Renderer* render_ptr, int level)
{

	world_controller = std::make_unique<WorldController>();
	lvl_controller = std::make_unique<LevelController>();

	lvl_controller->makeLevel(level, render_ptr);

	render_ptr->setWindowTitle("Angry Penguins - Level");

	background = render_ptr->createUniqueSprite();

	switch(level)
	{
		case 1:
			background->loadTexture("..\\..\\Resources\\Textures\\Level\\Backdrop\\lvl1.png");
		break;
		case 2:
			background->loadTexture("..\\..\\Resources\\Textures\\Level\\Backdrop\\lvl1.png");
		break;
		case 3:
			background->loadTexture("..\\..\\Resources\\Textures\\Level\\Backdrop\\lvl1.png");
		break;
	}

	return true;
}

void GameLoop::render(ASGE::Renderer* render_ptr, int game_width, int game_height)
{
	
	render_ptr->renderSprite(*background);
	lvl_controller->renderLevel(render_ptr, game_width, game_height);

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
	else if (key_event->key == ASGE::KEYS::KEY_S || key_event->key == ASGE::KEYS::KEY_DOWN)
	{
		current_action = GameAction::DOWN;
	}
	else if (key_event->key == ASGE::KEYS::KEY_W || key_event->key == ASGE::KEYS::KEY_UP)
	{
		current_action = GameAction::UP;
	}
	else if (key_event->key == ASGE::KEYS::KEY_D || key_event->key == ASGE::KEYS::KEY_RIGHT)
	{
		current_action = GameAction::POWER_UP;
	}
	else if (key_event->key == ASGE::KEYS::KEY_A || key_event->key == ASGE::KEYS::KEY_LEFT)
	{
		current_action = GameAction::POWER_DOWN;
	}
	else if (key_event->key == ASGE::KEYS::KEY_SPACE)
	{
		current_action = GameAction::SHOOT;
	}
	else if (key_event->key == ASGE::KEYS::KEY_ENTER)
	{

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

void GameLoop::update(const ASGE::GameTime& time_data, AngryBirdsGame* main)
{
	levelCheck(main);

	processGameActions(time_data);

	lvl_controller->updateObjects(time_data);

	positionPenguinInCannon();
}

void GameLoop::processGameActions(const ASGE::GameTime& time_data)
{
	float rotation_value = 0;
	
	static float timer;
	timer += (time_data.delta_time.count() / 1000.f);

	switch (current_action)
	{
	case GameAction::UP:
		if (lvl_controller->getCannon()->getAngle() > -0.1f)
		{
			rotation_value = -0.5 * (time_data.delta_time.count() / 1000.f);
			lvl_controller->getCannon()->rotateObject(rotation_value);
		}
		break;
	case GameAction::DOWN:
		if (lvl_controller->getCannon()->getAngle() < 0.5f)
		{
			rotation_value = 0.5 * (time_data.delta_time.count() / 1000.f);
			lvl_controller->getCannon()->rotateObject(rotation_value);
		}
		break;
	case GameAction::POWER_UP:
		if (lvl_controller->getCurrentPenguin() != nullptr)
		{
			if (timer >= 0.3f)
			{
				if (power == 10)
				{
					lvl_controller->getCannon()->setState(CANNON_STATE::LOADED_HEAVY);
					power = 11;
				}
				else if (power == 11)
				{
					lvl_controller->getCannon()->setState(CANNON_STATE::LOADED_LIGHT);
					power = 9;
				}
				else
				{
					lvl_controller->getCannon()->setState(CANNON_STATE::LOADED);
					power = 10;
				}
				timer = 0;
			}
		}
		break;

	case GameAction::POWER_DOWN:
		if (lvl_controller->getCurrentPenguin() != nullptr)
		{
			if (timer >= 0.3f)
			{
				if (power == 10)
				{
					lvl_controller->getCannon()->setState(CANNON_STATE::LOADED_LIGHT);
					power = 9;
				}
				else if (power == 9)
				{
					lvl_controller->getCannon()->setState(CANNON_STATE::LOADED_HEAVY);
					power = 11;
				}
				else
				{
					lvl_controller->getCannon()->setState(CANNON_STATE::LOADED);
					power = 10;
				}
				timer = 0;
			}
		}
		break;
	case GameAction::SHOOT:
		if (timer >= 0.5f)
		{
			if (lvl_controller->getCannon()->getState() == CANNON_STATE::LOADED
				|| lvl_controller->getCannon()->getState() == CANNON_STATE::LOADED_LIGHT
				|| lvl_controller->getCannon()->getState() == CANNON_STATE::LOADED_HEAVY)
			{
				if (lvl_controller->getCurrentPenguin() != nullptr)
				{
					// Angle Calculation
					float angle_height = 0;

					angle_height = ((lvl_controller->getCannon()->getAngle() - 0.500547) * 10);

					lvl_controller->getCurrentPenguin()->setVelocity(power, angle_height);
					lvl_controller->getCurrentPenguin()->setPhysics(true);
					lvl_controller->getCannon()->setState(CANNON_STATE::UNLOADED);
					lvl_controller->getCurrentPenguin()->setVisability(true);
					lvl_controller->getCurrentPenguin()->setFired(true);
					current_action = GameAction::NONE;
					timer = 0;

				}
			}
			else
			{
				if (lvl_controller->getCurrentPenguin() != nullptr)
				{
					lvl_controller->getCannon()->setPrevState();
					current_action = GameAction::NONE;
					timer = 0;
				}
			}
		}
		break;
	}
}

void GameLoop::positionPenguinInCannon()
{
	
	if (lvl_controller->getCannon()->getState() == CANNON_STATE::LOADED
		|| lvl_controller->getCannon()->getState() == CANNON_STATE::LOADED_LIGHT
		|| lvl_controller->getCannon()->getState() == CANNON_STATE::LOADED_HEAVY)
	{
		vector2 position = lvl_controller->getCannon()->getObjectPosition();
		position.x += 150;

		if (lvl_controller->getCannon()->getAngle() > 0.3f)
		{
			position.y += 40;
		}

		lvl_controller->getCurrentPenguin()->positionObject(position);
		lvl_controller->getCurrentPenguin()->setVisability(false);
	}
}

void GameLoop::levelCheck(AngryBirdsGame* main)
{
	if (lvl_controller->levelWon() == true)
	{
		lvl_score += lvl_controller->penguinCount() * 100;
		lvl_score += lvl_controller->currentSubScore();

		main->addScore(lvl_score);
		main->setGameState(GAME_STATE::Win);
	}

	if (current_action == GameAction::SHOOT)
	{
		if (lvl_controller->getCurrentPenguin() == nullptr)
		{
			current_action = GameAction::NONE;
			main->setGameState(GAME_STATE::Lost);
		}
	}
}