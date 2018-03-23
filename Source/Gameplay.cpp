#include <Engine\GameTime.h>
#include <Engine\InputEvents.h>
#include <Engine/Sprite.h>
#include <string>
#include "GameFont.h"

#include "Gameplay.h"
#include "State.h"
#include "LevelController.h"

Gameplay::Gameplay()
{
	
}

Gameplay::~Gameplay()
{
	delete background;
}

bool Gameplay::init(ASGE::Renderer* render_ptr, std::shared_ptr<State> state, std::shared_ptr<LevelController> level)
{

	score = 25000;
	world_controller = std::make_unique<WorldController>();
	state_controller = state;
	lvl_controller = std::make_unique<LevelController>();
	renderer = render_ptr;

	lvl_controller->makeLevel(Mission::DIFFICULTY::NORMAL, Mission::THEME::MAIN_STREET, Mission::TYPE::EVESDROP, renderer);

	renderer->setWindowTitle("Endless Runner - Level");

	// Backdrop
	background = renderer->createRawSprite();
	background->loadTexture("..\\..\\Resources\\Textures\\World\\Backdrop\\Sky.png");

	return true;
}


void Gameplay::render(int game_width, int game_height)
{
	renderer->renderSprite(*background);
	lvl_controller->renderLevel(renderer, game_width, game_height);

	std::string text = "Score: " + std::to_string(score);

	renderer->renderText(text, (game_width/2) - (20 * (text.size()/2)), 140, 1.5, ASGE::COLOURS::ORANGE);

}


void Gameplay::mouseInput(const ASGE::ClickEvent* click, double mouse_x, double mouse_y)
{
	WorldObject::bounds mouse;

	mouse.bottom_x = mouse_x;
	mouse.bottom_y = mouse_y;
	mouse.top_x = mouse_x;
	mouse.top_y = mouse_y;

}

void Gameplay::keyInput(int key, int action)
{
	if (key == ASGE::KEYS::KEY_D)
	{
		current_action = GameAction::FACE_LEFT;
	}
	else if (key == ASGE::KEYS::KEY_A)
	{
		current_action = GameAction::FACE_RIGHT;
	}
	else if (key == ASGE::KEYS::KEY_S)
	{
		current_action = GameAction::GO_DOWN;
	}
	else if (key == ASGE::KEYS::KEY_SPACE)
	{
		current_action = GameAction::JUMP;
	}
	else if (key == ASGE::KEYS::KEY_ENTER)
	{
		lvl_controller->debugSetLevelWon();
	}
	else
	{
		current_action = GameAction::NONE;
	}

	if (action == 0)
	{
		current_action = GameAction::NONE;
	}
}

void Gameplay::update(const ASGE::GameTime& time_data)
{
	
	WorldObject::vector2 camera_position = lvl_controller->getCameraPosition();

	lvl_controller->positionCamera(lvl_controller->getPlayer()->getObject()->xPos(), camera_position.y);

	processGameActions(time_data);

	lvl_controller->getPlayer()->moveForward(1, time_data);

	lvl_controller->checkGravity(lvl_controller->getPlayer(), time_data);

	if (lvl_controller->collision(lvl_controller->getPlayer()) == true)
	{
		lvl_controller->getPlayer()->moveForward(-1, time_data);
	}

	if (lvl_controller->levelWon() == true)
	{
		state_controller->addScore(score);
		state_controller->setGameState(State::GAME_STATE::WORLD_MENU);
	}
	else
	{
		score -= (0.2 * time_data.delta_time.count());
	}
}

void Gameplay::processGameActions(const ASGE::GameTime& time_data)
{
	switch (current_action)
	{
	case GameAction::FACE_LEFT:
		if (lvl_controller->getPlayer()->getFacing().x < 0)
		{
			WorldObject::vector2 new_direction;
			new_direction.x = 1;
			new_direction.y = 0;
			lvl_controller->getPlayer()->changeFacing(new_direction);
		}
		break;
	case GameAction::FACE_RIGHT:
		if (lvl_controller->getPlayer()->getFacing().x > 0)
		{
			WorldObject::vector2 new_direction;
			new_direction.x = -1;
			new_direction.y = 0;
			lvl_controller->getPlayer()->changeFacing(new_direction);
		}
		break;
	case GameAction::JUMP:
		if (lvl_controller->getPlayer()->getGrounded() == true)
		{
			lvl_controller->getPlayer()->moveDown(-lvl_controller->getPlayer()->getJumpStrength(), time_data);
			lvl_controller->getPlayer()->setVelocity(-lvl_controller->getPlayer()->getJumpStrength());
		}
		break;
	case GameAction::GO_DOWN:
		if (lvl_controller->passableGround(lvl_controller->getPlayer(), time_data) == true)
		{
			lvl_controller->getPlayer()->moveForward(-1, time_data);
			lvl_controller->getPlayer()->moveDown(1, time_data);
			lvl_controller->getPlayer()->setVelocity(0);
		}
		break;
	}
}