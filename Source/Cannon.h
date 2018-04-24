#pragma once
#include "GameObject.h"

enum CANNON_STATE
{
	UNLOADED = 0,
	LOADED
};

class Cannon
{
public:
	Cannon() = delete;
	Cannon(ASGE::Renderer* renderer);
	~Cannon() = default;

	void setState(CANNON_STATE updated_state) { state = updated_state; };
	CANNON_STATE getState() const { return state; };
	float getAngle() const { return base->getAngle(); };

	vector2 getObjectPosition();
	void positionObject(int, int);
	void rotateObject(float);
	void render(ASGE::Renderer* renderer);
private:
	CANNON_STATE state = LOADED;

	std::unique_ptr<GameObject> base;
	std::unique_ptr<GameObject> loaded;
};