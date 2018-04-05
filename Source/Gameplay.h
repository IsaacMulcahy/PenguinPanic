#pragma once

#include "WorldController.h"
#include "LevelController.h"

class Gameplay
{
public:
	Gameplay();
	~Gameplay();

	enum GameAction
	{
		NONE = 0,
		FACE_LEFT,
		FACE_RIGHT,
		GO_DOWN,
		JUMP
	};

	bool init(ASGE::Renderer*, int);
	void render(int, int);
	void mouseInput(const ASGE::ClickEvent*, double, double);
	void keyInput(int, int);
	void processGameActions(const ASGE::GameTime& time_data);
	void update(const ASGE::GameTime& time_data);
private:
	int score;
	
	ASGE::Sprite* background;
	GameAction current_action;

	// Classes
	std::unique_ptr<WorldController> world_controller;
	std::unique_ptr<LevelController> lvl_controller;
	ASGE::Renderer* renderer;
};