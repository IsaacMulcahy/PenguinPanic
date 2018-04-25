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


// ------------------ Update ------------------
void LevelController::updateObjects(const ASGE::GameTime& time)
{
	for (int i = 0; i < object_list.size(); i++)
	{
		if (object_list[i]->getPhysicsEnabled() == true)
		{
			object_list[i]->update(time);
			checkGravity(object_list[i].get(), time);
			checkNarwhales(object_list[i].get());
		}
	}

	for (int i = 0; i < penguin.size(); i++)
	{
		if (penguin[i]->getPhysicsEnabled() == true)
		{
			penguin[i]->update(time);
			checkGravity(penguin[i].get(), time);
			if (collision(penguin[i].get()) == true)
			{
				penguin[i]->setVelocity(0, penguin[i]->getVelocity().y);
			}
			checkNarwhales(penguin[i].get());
		}
	}

	for (int i = 0; i < narwhale.size(); i++)
	{
		if (narwhale[i]->getPhysicsEnabled() == true)
		{
			narwhale[i]->update(time);
			checkGravity(narwhale[i].get(), time);
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

Penguin* LevelController::getCurrentPenguin()
{
	for(int i = 0; i < penguin.size(); i++)
	{
		if (penguin[i]->getFired() == false)
		{
			return penguin[i].get();
		}
	}

	return nullptr;
}

int LevelController::penguinCount()
{
	int count = 0;

	for (int i = 0; i < penguin.size(); i++)
	{
		if (penguin[i]->getVisable() == true)
		{
			count++;
		}
	}

	return count;
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

			if (object_b->passableCheck() == false || object_b->getVisable() == true)
			{
				if (object_b->getType() == OBJECT_TYPE::BREAKABLE)
				{
					current_score += 10;
					object_b->setVisability(false);
				}

				if (object_a->getType() == OBJECT_TYPE::BREAKABLE)
				{
					current_score += 10;
					object_a->setVisability(false);
				}
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

void LevelController::checkGravity(GameObject* current, const ASGE::GameTime& time_data)
{

	bool grounded = isGrounded(current, time_data);
	current->setGrounded(grounded);

	if (grounded != true)
	{
		world_controller->applyGravity(current, time_data);
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

			float friction_effect = current->getVelocity().x * (400 * (time_data.delta_time.count() / 1000));
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
			current_score += 10;
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
		case 2:
			levelTwo(renderer);
		break;
		case 3:
			levelThree(renderer);
			break;
	}
}

void LevelController::levelOne(ASGE::Renderer* renderer)
{
	// Penguins
	std::unique_ptr<Penguin> current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 938);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 868);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 798);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 728);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 658);
	penguin.push_back(std::move(current_penguin));

	// World
	std::unique_ptr<GameObject> current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\SnowFloor.png");
	current_object->positionObject(0, 1010);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1280, 878);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1260, 850);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1392, 850);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1500, 878);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1525, 850);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1656, 850);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1732, 878);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));
	

	// Narwhales
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1370, 900);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1600, 900);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1485, 750);
	narwhale.push_back(std::move(current_object));
}

void LevelController::levelTwo(ASGE::Renderer* renderer)
{
	// Penguins
	std::unique_ptr<Penguin> current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 938);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 868);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 798);
	penguin.push_back(std::move(current_penguin));

	// World
	std::unique_ptr<GameObject> current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\SnowFloor.png");
	current_object->positionObject(0, 1010);
	object_list.push_back(std::move(current_object));

	// Floor 1
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1550, 878);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1525, 850);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1656, 850);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1732, 878);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	// Floor 2
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1550, 718);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1732, 718);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1525, 689);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1656, 689);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	// Floor 3
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1550, 557);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1732, 557);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1525, 528);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1656, 528);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	// Wall
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Snow4x4.png");
	current_object->positionObject(1100, 870);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Snow.png");
	current_object->positionObject(1030, 941);
	object_list.push_back(std::move(current_object));

	// Narwhales
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1630, 760);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1630, 900);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1630, 580);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1135, 700);
	narwhale.push_back(std::move(current_object));

}

void LevelController::levelThree(ASGE::Renderer* renderer)
{
	// Penguins
	std::unique_ptr<Penguin> current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 938);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 868);
	penguin.push_back(std::move(current_penguin));

	current_penguin = std::make_unique<Penguin>();
	current_penguin->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Penguin.png");
	current_penguin->positionObject(10, 798);
	penguin.push_back(std::move(current_penguin));

	// World
	std::unique_ptr<GameObject> current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\SnowFloor.png");
	current_object->positionObject(0, 1010);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Snow6x1.png");
	current_object->positionObject(890, 940);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Snow6x1.png");
	current_object->positionObject(1498, 940);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1535, 808);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1539, 779);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1671, 779);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1763, 808);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1580, 649);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceFloor.png");
	current_object->positionObject(1591, 620);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\IceWall.png");
	current_object->positionObject(1703, 650);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->setPhysics(true);
	object_list.push_back(std::move(current_object));

	// Narwhales
	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1360, 900);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1635, 850);
	narwhale.push_back(std::move(current_object));

	current_object = std::make_unique<GameObject>();
	current_object->loadObject(renderer, "..\\..\\Resources\\Textures\\Level\\Object\\Narwhale.png");
	current_object->setPhysics(true);
	current_object->setType(OBJECT_TYPE::BREAKABLE);
	current_object->positionObject(1620, 535);
	narwhale.push_back(std::move(current_object));

}