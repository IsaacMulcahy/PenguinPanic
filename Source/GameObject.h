#pragma once
#include <vector>

#include <Engine/OGLGame.h>
#include "Vector2.h"
#include "Bounds.h"
#include "ObjectType.h"
#include "AnimationState.h"

class GameObject
{
public:
	GameObject();
	~GameObject() = default;



	// ------------------ Getters ------------------
	bool passableCheck() const { return passable; };
	OBJECT_TYPE getType() const { return type; };
	vector2 getFacing() const { return facing; }
	ASGE::Sprite* getObject() const { return object; };
	Bounds getBound() const { return object_bound; };
	float getVelocity() const { return velocity; };
	bool getGrounded() const { return is_grounded; };

	// ------------------ Setters ------------------
	void setType(OBJECT_TYPE new_type) { type = new_type; };
	void setPassable(bool value) { passable = value; };
	void changeFacing(vector2 value) { facing = value; };
	void setGrounded(bool value) { is_grounded = value; };
	void setVelocity(float value) { velocity = value; };

	// ------------------ Actions ------------------
	void moveForward(int, const ASGE::GameTime&);
	void moveDown(float, const ASGE::GameTime&);
	
	// ------------------ Visual Effects ------------------
	void visualEffect(float value);
	void visualEffect(ASGE::Colour value);

	// ------------------ Setup ------------------
	void setupAnimation(int, int);

	// ------------------ Loading File ------------------
	void loadObject(ASGE::Renderer*);
	void loadObject(ASGE::Renderer*, char[]);
	void loadObject(ASGE::Renderer*, std::string);

	// ------------------ Position ------------------
	void positionObject(int, int);
	void moveObjectBy(int, int);

	// ------------------ Animation ------------------
	void animateObject(ANIMATION_STATE, int);
	void increaseAnimationFrame();

private:
	// ------------------ Sub Actions ------------------
	void updateBound();

protected:
	bool passable = false;
	ASGE::Sprite* object = nullptr;
	OBJECT_TYPE type = OBJECT_TYPE::SOLID;
	vector2 facing = vector2(0, 0);
	Bounds object_bound;

	float velocity;
	float jump_stength;
	bool is_grounded;

	float height_of_frame;
	float width_of_frame;
	ANIMATION_STATE animation_type;
	int frame;

};