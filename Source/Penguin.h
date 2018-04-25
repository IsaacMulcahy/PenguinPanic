#pragma once
#include "GameObject.h"

class Penguin : public GameObject
{
public:
	Penguin() { penguin = std::make_unique<GameObject>(); };
	~Penguin() = default;

	void setFired(bool value) { fired = value; };
	bool getFired() { return fired; };

private:
	std::unique_ptr<GameObject> penguin;
	bool fired = false;

};