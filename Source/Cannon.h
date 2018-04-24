#pragma once
#include "GameObject.h"

class Cannon
{
public:
	Cannon() = default;
	~Cannon() = default;

	enum CANNON_STATE
	{
		UNLOADED = 0,
		LOADED
	};

	void setState(CANNON_STATE updated_state) { state = updated_state; };
	CANNON_STATE getState() const { return state; };
	float getAngle() const { return base->getAngle(); };
	
	void setupObject(ASGE::Renderer* renderer);
	void positionObject(int, int);
	void rotateObject(float);
	void render(ASGE::Renderer* renderer);
private:
	CANNON_STATE state = UNLOADED;

	std::unique_ptr<GameObject> base;
	std::unique_ptr<GameObject> loaded;
};