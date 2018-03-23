#pragma once
#include <vector>

#include "GameObject.h"

class WorldController
{
	public:
		WorldController();
		~WorldController() = default;

		bool collision(GameObject*, GameObject*);
		bool collision(GameObject::bounds, GameObject::bounds);
		bool zoneCollision(float, float, float, GameObject*);
		void applyGravity(GameObject*, const ASGE::GameTime&);
		float getGravityStrength();
	private:
		float gravity_strength = 0.7f;

};