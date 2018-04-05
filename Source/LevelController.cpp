#include <vector>
#include <Engine/Sprite.h>

#include "Vector2.h"
#include "GameObject.h"
#include "LevelController.h"
#include "WorldController.h"
#include "GameObjectBlueprint.h"

LevelController::LevelController()
{
	world_controller = std::make_unique<WorldController>();
	player = new GameObject;

}

LevelController::~LevelController()
{
	delete player;
}

void LevelController::makeLevel(int level_number, ASGE::Renderer* renderer)
{
	level_loaded = false;

	createLevel(renderer);

	createPlayer(renderer);

	level_loaded = true;
}

void LevelController::positionCamera(float x, float y)
{
	vector2 difference = vector2(0, 0);

	difference.x = camera_position.x - x;
	difference.y = camera_position.y - y;
	
	for (int i = 0; i < object_list->size(); i++)
	{
		object_list->at(i).moveObjectBy(difference.x, difference.y);
	}

	for (int i = 0; i < background_list->size(); i++)
	{
		background_list->at(i).moveObjectBy(difference.x, difference.y);
	}

	//player->moveObjectBy(difference.x, difference.y);

	camera_position.x = x;
	camera_position.y = y;
}

vector2 LevelController::getCameraPosition() const
{
	return camera_position;
}

void LevelController::renderLevel(ASGE::Renderer* renderer, int game_width, int game_height)
{
	renderBackgrounds(renderer, game_width, game_height);
	renderObjects(renderer, game_width, game_height);
	renderPlayer(renderer);
}

void LevelController::renderBackgrounds(ASGE::Renderer* renderer, int game_width, int game_height)
{
	for (int i = 0; i < background_list->size(); i++)
	{
		GameObject* current_object = &background_list->at(i);

		if (current_object->getBound().bottom_x > 0 || current_object->getBound().bottom_y > 0)
		{
			if (current_object->getBound().top_x < game_width || current_object->getBound().top_y > game_height)
			{
				renderer->renderSprite(*current_object->getObject());
			}

		}
	}
}

void LevelController::renderObjects(ASGE::Renderer* renderer, int game_width, int game_height)
{	
	for (int i = 0; i < object_list->size(); i++)
	{
		GameObject* current_object = &object_list->at(i);

		if (current_object->getBound().bottom_x > 0 || current_object->getBound().bottom_y > 0)
		{
			if (current_object->getBound().top_x < game_width || current_object->getBound().top_y > game_height)
			{
				renderer->renderSprite(*current_object->getObject());
			}

		}
	}
}

void LevelController::renderPlayer(ASGE::Renderer* renderer) const
{
	renderer->renderSprite(*player->getObject());
}

GameObject* LevelController::getPlayer() const
{
	return player;
}

bool LevelController::collision(GameObject* object_a)
{
	bool collision = false;

	for (int i = 0; i < object_list->size(); i++)
	{
		GameObject* object_b = &object_list->at(i);
		
		collision = world_controller->collision(object_a, object_b);

		if (collision == true)
		{
			if (object_b->getType() == OBJECT_TYPE::MISSION_ITEM)
			{
				level_completed = true;
			}

			if (object_b->passableCheck() == false)
			{
				return true;
			}
			else
			{
				return false;
			}
			
			return true;
		}
	}
	
	return false;

}

bool LevelController::isGrounded(GameObject* object_a, const ASGE::GameTime& time_data)
{
	bool collision = false;

	ASGE::Sprite* object_a_sprite = object_a->getObject();
	
	for (int i = 0; i < object_list->size(); i++)
	{
		GameObject* object_b = &object_list->at(i);

		collision = world_controller->zoneCollision(object_a_sprite->xPos(), 
			object_a_sprite->xPos() + object_a_sprite->width(),
			object_a_sprite->yPos() + (object_a_sprite->height() + 1), object_b);

		if (collision == true)
		{

			return true;
		}
	}

	return false;
}

bool LevelController::passableGround(GameObject* object_a, const ASGE::GameTime& time_data)
{
	bool collision = false;

	ASGE::Sprite* object_a_sprite = object_a->getObject();

	for (int i = 0; i < object_list->size(); i++)
	{
		GameObject* object_b = &object_list->at(i);

		collision = world_controller->zoneCollision(object_a_sprite->xPos(),
			object_a_sprite->xPos() + object_a_sprite->width(),
			object_a_sprite->yPos() + (object_a_sprite->height() + 1), object_b);

		if (collision == true)
		{

			if (object_b->passableCheck() == true)
			{
				return true;
			}

			return false;
		}
	}

	return false;
}

void LevelController::checkGravity(GameObject* current, const ASGE::GameTime& time_data)
{
	static float timer;
	timer += (time_data.delta_time.count() / 1000.f);

	bool grounded = isGrounded(current, time_data);
	current->setGrounded(grounded);

	if (grounded != true)
	{
		if (timer >= 0.025f) // The time between each addition of gravity is added
		{
			world_controller->applyGravity(current, time_data);
			timer = 0;
		}
	}
	else
	{
		if (current->getVelocity() != 0)
		{
			if (collision(current) == true)
			{
				current->moveDown(-(current->getVelocity() + 1), time_data);
			}

			current->setVelocity(0);
		}
	}
}

bool LevelController::levelLoaded()
{
	return level_loaded;
}

bool LevelController::levelWon()
{
	return level_completed;
}

void LevelController::debugSetLevelWon()
{
	level_completed = true;
}

void LevelController::createLevel(ASGE::Renderer* renderer)
{
	
}

void LevelController::makeObjectFromBlueprint(GameObjectBlueprint blueprint, vector2 current_offset, ASGE::Renderer* renderer)
{
	object_list->push_back(GameObject());

	object_list->at(object_list->size() - 1).loadObject(renderer, blueprint.object);
	object_list->at(object_list->size() - 1).setPassable(blueprint.passable);
	object_list->at(object_list->size() - 1).setType(blueprint.type);
	object_list->at(object_list->size() - 1).changeFacing(blueprint.facing);
	object_list->at(object_list->size() - 1).positionObject(current_offset.x + blueprint.position.x + world_offset.x,
		current_offset.y + blueprint.position.y + world_offset.y);
}

void LevelController::createPlayer(ASGE::Renderer* renderer)
{
	player->loadObject(renderer, "..\\..\\Resources\\Textures\\Player\\Player.png");
	player->positionObject(60, area_size.y - player->getObject()->height() - 10);
	player->setPassable(false);
}