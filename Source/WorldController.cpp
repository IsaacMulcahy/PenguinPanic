#include "WorldController.h"
#include <Engine/Sprite.h>
#include <vector>
#include "Bounds.h"

WorldController::WorldController()
{

}

bool WorldController::collision(GameObject* current, GameObject* world)
{
	// Setup Data for collision detection
	Bounds current_bounds = current->getBound();
	Bounds world_bounds = world->getBound();

	// Check
	if (current_bounds.top_x < world_bounds.bottom_x)
	{
		if (current_bounds.bottom_x > world_bounds.top_x)
		{
			if (current_bounds.top_y < world_bounds.bottom_y)
			{
				if (current_bounds.bottom_y > world_bounds.top_y)
				{
					return true; // The object has been hit
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return false;

}

bool WorldController::collision(Bounds current, Bounds world)
{

	// Check
	if (current.top_x < world.bottom_x)
	{
		if (current.bottom_x > world.top_x)
		{
			if (current.top_y < world.bottom_y)
			{
				if (current.bottom_y > world.top_y)
				{
					return true; // The object has been hit
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return false;

}


bool WorldController::zoneCollision(float x1, float x2, float y, GameObject* world)
{
	Bounds world_bounds = world->getBound();
	
	if (x1 < world_bounds.bottom_x)
	{
		if (x2 > world_bounds.top_x)
		{
			if (y-1 < world_bounds.bottom_y)
			{
				if (y+1 > world_bounds.top_y)
				{
					return true; // The object has been hit
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return false;
}

void WorldController::applyGravity(GameObject* current, const ASGE::GameTime& time_data)
{
	vector2 current_velocity = current->getVelocity();
	
	current_velocity.y += gravity_strength;
	
	current->moveDown(current_velocity.y, time_data);
	current->setVelocity(current_velocity);
}

float WorldController::getGravityStrength()
{
	return gravity_strength;
}