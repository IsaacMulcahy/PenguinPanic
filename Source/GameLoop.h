#pragma once

#include "WorldController.h"
#include "LevelController.h"
#include "GameObject.h"

class AngryBirdsGame;

class GameLoop
{
public:
	GameLoop() = default;
	~GameLoop() = default;

	enum GameAction
	{
		NONE = 0,
		UP,
		DOWN,
		POWER_UP,
		POWER_DOWN,
		SHOOT
	};

	bool init(ASGE::Renderer* render_ptr, int level);
	void render(ASGE::Renderer* render_ptr, int game_width, int game_height);
	void mouseInput(const ASGE::ClickEvent*, double mouse_x, double mouse_y);
	void keyInput(const ASGE::KeyEvent* key_event, AngryBirdsGame* main);
	void processGameActions(const ASGE::GameTime& time_data);
	void update(const ASGE::GameTime& time_data, AngryBirdsGame* main);

	void tidyUp() { lvl_controller = std::make_unique<LevelController>(); };
private:
	void positionPenguinInCannon();

	void levelCheck(AngryBirdsGame* main);

	int lvl_score = 0;
	float power = 10;
	
	std::unique_ptr<ASGE::Sprite> background;
	GameAction current_action;

	// Classes
	std::unique_ptr<WorldController> world_controller;
	std::unique_ptr<LevelController> lvl_controller;
};