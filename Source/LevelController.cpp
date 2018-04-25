#include <vector>
#include <Engine/Sprite.h>

#include "Vector2.h"
#include "GameObject.h"
#include "LevelController.h"
#include "WorldController.h"

LevelController::LevelController()
{
	world_controller = std::make_unique<WorldController>();

}

// ------------------ Camera ------------------
void LevelController::positionCamera(float x, float y)
{

}

vector2 LevelController::getCameraPosition() const
{
	return camera_position;
}

// ------------------ Update ------------------
void LevelController::updateObjects(const ASGE::GameTime& time)
{
	for (int i = 0; i < object_list.size(); i++)
	{
		if (object_list[i]->getPhysicsEnabled() == true)
		{
			object_list[i]->update(time);
		}
	}

	for (int i = 0; i < penguin.size(); i++)
	{
		if (penguin[i]->getPhysicsEnabled() == true)
		{
			penguin[i]->update(time);
			checkGravity(penguin[i].get(), time);
			collision(penguin[i].get());
			checkNarwhales(penguin[i].get());
		}
	}
}

// ------------------ Rendering ------------------
void LevelController::renderLevel(ASGE::Renderer* renderer, int game_width, int game_height)
{
	cannon->render(renderer);
	renderObjects(renderer, game_width, game_height);
	renderPenguins(renderer, game_width, game_height);
	renderNarwhales(renderer, game_width, game_height);
}

void LevelController::renderObjects(ASGE::Renderer* renderer, int game_width, int game_height)
{	
	for (int i = 0; i < object_list.size(); i++)
	{
		GameObject* current_object = object_list[i].get();
		
		if (current_object->getVisable() == true)
		{
			if (current_object->getBound().bottom_x > 0 || current_object->getBound().bottom_y > 0)
			{
				if (current_object->getBound().top_x < game_width || current_object->getBound().top_y > game_height)
				{
					renderer->renderSprite(*current_object->getObject());
				}

			}
		}
	}
}

void LevelController::renderPenguins(ASGE::Renderer* renderer, int game_width, int game_height)
{
	for (int i = 0; i < penguin.size(); i++)
	{
		GameObject* current_object = penguin[i].get();

		if (current_object->getVisable() == true)
		{
			if (current_object->getBound().bottom_x > 0 || current_object->getBound().bottom_y > 0)
			{
				if (current_object->getBound().top_x < game_width || current_object->getBound().top_y > game_height)
				{
					renderer->renderSprite(*current_object->getObject());
				}

			}
		}
	}
}

void LevelController::renderNarwhales(ASGE::Renderer* renderer, int game_width, int game_height)
{
	for (int i = 0; i < narwhale.size(); i++)
	{
		GameObject* current_object = narwhale[i].get();

		if (current_object->getVisable() == true)
		{
			if (current_object->getBound().bottom_x > 0 || current_object->getBound().bottom_y > 0)
			{
				if (current_object->getBound().top_x < game_width || current_object->getBound().top_y > game_height)
				{
					renderer->renderSprite(*current_object->getObject());
				}

			}
		}
	}
}

// ------------------ Getters ------------------
Cannon* LevelController::getCannon() const
{
	return cannon.get();
}

GameObject* LevelController::getCurrentPenguin()
{
	return penguin[penguin.size()-1].get();
}

// ------------------ Physics ------------------
bool LevelController::collision(GameObject* object_a)
{
	bool collision = false;

	for (int i = 0; i < object_list.size(); i++)
	{
		GameObject* object_b = object_list[i].get();
		
		collision = world_controller->collision(object_a, object_b);

		if (collision == true)
		{

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
	
	for (int i = 0; i < object_list.size(); i++)
	{
		GameObject* object_b = object_list[i].get();

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

	for (int i = 0; i < object_list.size(); i++)
	{
		GameObject* object_b = object_list[i].get();

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
		if (current->getVelocity().y != 0)
		{
			if (collision(current) == true)
			{
				current->moveDown(-(current->getVelocity().y + 1), time_data);
			}

			vector2 new_velocity = current->getVelocity();

			float friction_effect = current->getVelocity().x * (300 * (time_data.delta_time.count() / 1000));
			new_velocity.x = friction_effect;
			new_velocity.y = 0;
			current->setVelocity(new_velocity);
		}
	}
}

void LevelController::checkNarwhales(GameObject* current)
{
	bool collision = false;

	for (int i = 0; i < narwhale.size(); i++)
	{
		GameObject* object_b = narwhale[i].get();

		collision = world_controller->collision(current, object_b);

		if (collision == true)
		{
			narwhale[i]->setVisability(false);
		}
	}

}


// ------------------ Level Control ------------------
void LevelController::makeLevel(int level_number, ASGE::Renderer* renderer)
{
	level_loaded = false;

	createLevel(renderer, level_number);

	level_loaded = true;
}

bool LevelController::levelLoaded()
{
	return level_loaded;
}

bool LevelController::levelWon()
{
	int tally = 0;

	for (int i = 0; i < narwhale.size(); i++)
	{
		if (narwhale[i]->getVisable() == false)
		{
			tally++;
		}
	}

	if (tally == narwhale.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LevelController::createLevel(ASGE::Renderer* renderer, int level_number)
{
	cannon = std::make_unique<Cannon>(renderer);
	cannon->positionObject(100, 830);

	// Centre angle of cannon
	//cannon->setupAngle(0.2f);
	
	switch (level_number)
	{
		case 1:
			levelOne(renderer);
		break;
	}
}

void LevelController::levelOne(ASGE::Renderer* renderer)
{
	// Penguins
	std::unique_ptr<GameObject> current_penguin = std::make_unique<GameObject>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Penguin.png");
	current_penguin->positionObject(10, 938);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<GameObject>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Penguin.png");
	current_penguin->positionObject(100, 938);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<GameObject>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Penguin.png");
	current_penguin->positionObject(50, 868);
	penguin.push_back(std::move(current_penguin));

	// World
	std::unique_ptr<GameObject> current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\SnowFloor.png");
	current_object->positionObject(0, 1010);

	object_list.push_back(std::move(current_object));
	

	// Narwhales
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Narwhale.png");
	current_object->positionObject(1700, 941);

	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Narwhale.png");
	current_object->positionObject(1400, 941);

	narwhale.push_back(std::move(current_object));
}