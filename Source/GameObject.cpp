#include <Engine/Sprite.h>
#include <vector>

#include "Vector2.h"
#include "GameObject.h"
#include "Bounds.h"

// ------------------ Setup ------------------
GameObject::GameObject()
{
	facing.x = 1;
	facing.y = 0;
}

// ------------------ Actions ------------------
void GameObject::moveForward(int value, const ASGE::GameTime& time_data)
{
	int normalized_value = value * 100;

	float movement_value = normalized_value * (time_data.delta_time.count() / 1000.f);

	object->xPos(object->xPos() + (movement_value * facing.x));
	object->yPos(object->yPos() + (movement_value * facing.y));

	updateBound();
}

void GameObject::moveDown(float value, const ASGE::GameTime& time_data)
{
	int normalized_value = value * 100;

	float movement_value = normalized_value * (time_data.delta_time.count() / 1000.f);

	object->yPos(object->yPos() + (movement_value));

	updateBound();
}

void GameObject::rotateObject(float value)
{
	angle += value;

	object->rotationInRadians(angle);
}

void GameObject::update(const ASGE::GameTime& time)
{
	moveForward(velocity.x, time);
	moveDown(velocity.y, time);
}


// ------------------ Sub Actions (Private) ------------------
void GameObject::updateBound()
{
	object_bound.top_x = object->xPos();
	object_bound.top_y = object->yPos();

	object_bound.bottom_x = object->xPos() + object->width();
	object_bound.bottom_y = object->yPos() + object->height();
}

// ------------------ Visual Effects ------------------
void GameObject::visualEffect(float value)
{
	object->scale(value);
}

void GameObject::visualEffect(ASGE::Colour value)
{
	object->colour(value);
}

// ------------------ Loading File ------------------
void GameObject::loadObject(ASGE::Renderer* renderer)
{
	object = renderer->createUniqueSprite();
	object->loadTexture("..\\..\\Resources\\Textures\\error.png");
	object->xPos(60);
	object->yPos(350);
	updateBound();
}

void GameObject::loadObject(ASGE::Renderer* renderer, char filename[])
{
	object = renderer->createUniqueSprite();

	if (!object->loadTexture(filename))
	{
		object->loadTexture("..\\..\\Resources\\Textures\\error.png");
	}

	object->xPos(0);
	object->yPos(0);
	updateBound();
}

void GameObject::loadObject(ASGE::Renderer* renderer, std::string filename)
{
	object = renderer->createUniqueSprite();

	if (!object->loadTexture(filename))
	{
		object->loadTexture("..\\..\\Resources\\Textures\\error.png");
	}

	object->xPos(0);
	object->yPos(0);
	updateBound();
}

// ------------------ Position ------------------
void GameObject::positionObject(int x, int y)
{
	object->xPos(x);
	object->yPos(y);
	updateBound();
}

void GameObject::positionObject(vector2 value)
{
	object->xPos(value.x);
	object->yPos(value.y);
	updateBound();
}

void GameObject::moveObjectBy(int x, int y)
{
	object->xPos(object->xPos() + x);
	object->yPos(object->yPos() + y);
	updateBound();
}
