#pragma once
#include "GameObject.h"
#include "ObjectType.h"

class Bounds;

struct GameObjectBlueprint
{
	bool passable = false;
	std::string object;
	OBJECT_TYPE type = OBJECT_TYPE::SOLID;
	vector2 facing;
	Bounds object_bound;
	vector2 position;
};