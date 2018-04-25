#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

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
	ASGE::Sprite* getObject() const { return object.get(); };
	Bounds getBound() const { return object_bound; };
	vector2 getVelocity() const { return velocity; };
	bool getGrounded() const { return is_grounded; };
	float getAngle() const { return angle; };
	bool getPhysicsEnabled() const { return apply_physics; };
	vector2 getObjectPosition() const { vector2 current_pos = vector2(0, 0); current_pos.x = object->xPos(); current_pos.y = object->yPos(); return current_pos;};
	bool getVisable() const { return visiable; };

	// ------------------ Setters ------------------
	void setType(OBJECT_TYPE new_type) { type = new_type; };
	void setPassable(bool value) { passable = value; };
	void changeFacing(vector2 value) { facing = value; };
	void setGrounded(bool value) { is_grounded = value; };
	void setVelocity(vector2 value) { velocity = value; };
	void setVelocity(int x, int y) { velocity.x = x; velocity.y = y; };
	void setPhysics(bool value) { apply_physics = value; };
	void setupAngle(float value) { angle = value; };
	void setVisability(bool value) { visiable = value; };

	// ------------------ Actions ------------------
	void moveForward(int, const ASGE::GameTime&);
	void moveDown(float, const ASGE::GameTime&);
	void rotateObject(float value);
	void update(const ASGE::GameTime&);
	
	// ------------------ Visual Effects ------------------
	void visualEffect(float value);
	void visualEffect(ASGE::Colour value);

	// ------------------ Loading File ------------------
	void loadObject(ASGE::Renderer*);
	void loadObject(ASGE::Renderer*, char[]);
	void loadObject(ASGE::Renderer*, std::string);

	// ------------------ Position ------------------
	void positionObject(int, int);
	void positionObject(vector2);
	void moveObjectBy(int, int);
private:
	// ------------------ Sub Actions ------------------
	void updateBound();

protected:
	bool passable = false;
	std::unique_ptr<ASGE::Sprite> object;
	OBJECT_TYPE type = OBJECT_TYPE::SOLID;
	vector2 facing = vector2(0, 0);
	Bounds object_bound;

	float angle = 0;

	vector2 velocity = vector2(0,0);
	bool is_grounded = false;
	bool apply_physics = false;
	bool visiable = true;

	ANIMATION_STATE animation_type = ANIMATION_STATE::IDLE;


};