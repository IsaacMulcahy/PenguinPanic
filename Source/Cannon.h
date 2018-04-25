#pragma once
#include "GameObject.h"

enum CANNON_STATE
{
	UNLOADED = 0,
	LOADED,
	LOADED_LIGHT,
	LOADED_HEAVY
};

class Cannon
{
public:
	Cannon() = delete;
	Cannon(ASGE::Renderer* renderer);
	~Cannon() = default;

	void setState(CANNON_STATE updated_state) { last_state = state; state = updated_state; };
	void setPrevState() { state = last_state; };
	CANNON_STATE getState() const { return state; };
	float getAngle() const { return base->getAngle(); };
	void setupAngle(float value) { base->setupAngle(value); };

	vector2 getObjectPosition();
	void positionObject(int, int);
	void rotateObject(float);
	void render(ASGE::Renderer* renderer);
private:
	CANNON_STATE state = LOADED;
	CANNON_STATE last_state = UNLOADED;

	std::unique_ptr<GameObject> base;
	std::unique_ptr<GameObject> loaded;
	std::unique_ptr<GameObject> loaded_light;
	std::unique_ptr<GameObject> loaded_heavy;
};