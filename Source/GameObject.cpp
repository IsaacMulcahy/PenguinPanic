#include <Engine/Sprite.h>
#include <vector>

#include "GameObject.h"

int GameObject::new_id = 1;

GameObject::GameObject() : id(new_id++)
{
	facing.x = 1;
	facing.y = 0;
}

void GameObject::loadObject(ASGE::Renderer* renderer)
{
	object = renderer->createRawSprite();
	object->loadTexture("..\\..\\Resources\\Textures\\error.png");
	object->xPos(60);
	object->yPos(350);
	updateBound();
}

void GameObject::loadObject(ASGE::Renderer* renderer, char filename[])
{
	object = renderer->createRawSprite();

	if (!object->loadTexture(filename))
	{
		object->loadTexture("..\\..\\Resources\\Textures\\error.png");
	}

	object->xPos(60);
	object->yPos(350);
	updateBound();
}

void GameObject::loadObject(ASGE::Renderer* renderer, std::string filename)
{
	object = renderer->createRawSprite();

	if (!object->loadTexture(filename))
	{
		object->loadTexture("..\\..\\Resources\\Textures\\error.png");
	}

	object->xPos(60);
	object->yPos(350);
	updateBound();
}

bool GameObject::passableCheck() const
{

	if (passable == true) {
		return true;
	}
	else
	{
		return false;
	}
}

int GameObject::getID() const
{
	return id;
}

GameObject::OBJECT_TYPE GameObject::getType() const
{
	return type;
}

GameObject::vector2 GameObject::getFacing() const
{
	return facing;
}

ASGE::Sprite* GameObject::getObject() const
{
	return object;
}

GameObject::bounds GameObject::getBound() const
{
	return object_bound;
}

float GameObject::getJumpStrength() const
{
	return jump_stength;
}

float GameObject::getVelocity() const
{
	return velocity;
}

bool GameObject::getGrounded() const
{
	return is_grounded;
}

void GameObject::setType(OBJECT_TYPE new_type)
{
	type = new_type;
}

void GameObject::setPassable(bool value)
{
	passable = value;
}

void GameObject::setJumpStrength(float value)
{
	jump_stength = value;
}

void GameObject::setVelocity(float value)
{
	velocity = value;
}

void GameObject::setGrounded(bool value)
{
	is_grounded = value;
}

void GameObject::setupAnimation(int sizeX, int sizeY)
{
	height_of_frame = sizeY;
	width_of_frame = sizeX;

	object->height(sizeY);
	object->width(sizeX);
}

void GameObject::positionObject(int x, int y)
{
	object->xPos(x);
	object->yPos(y);
	updateBound();
}

void GameObject::moveObjectBy(int x, int y)
{
	object->xPos(object->xPos() + x);
	object->yPos(object->yPos() + y);
	updateBound();
}

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

void GameObject::changeFacing(GameObject::vector2 change)
{
	facing.x = change.x;
	facing.y = change.y;
}

void GameObject::updateBound()
{
	object_bound.top_x = object->xPos();
	object_bound.top_y = object->yPos();

	object_bound.bottom_x = object->xPos() + object->width();
	object_bound.bottom_y = object->yPos() + object->height();
}

void GameObject::animateObject(GameObject::ANIMATION_STATE change_to_state, int set_frame)
{

}

void GameObject::increaseAnimationFrame()
{

}