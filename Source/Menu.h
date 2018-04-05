#pragma once
#include <vector>
#include <Engine/OGLGame.h>
#include "GameObject.h"
#include "Bounds.h"
#include "WorldController.h"

class AngryBirdsGame;

class Menu
{
public:
	Menu();
	~Menu() = default;

	void init(ASGE::Renderer* renderer);
	void mouseControl(Bounds mouse, const ASGE::ClickEvent* click_event, AngryBirdsGame* main);
	void keyboardControl(const ASGE::KeyEvent* , AngryBirdsGame* main);
	void update(double delta_time);
	void render(ASGE::Renderer* renderer);
private:
	void menuSetup(ASGE::Renderer* renderer);
	void buttonSetup(ASGE::Renderer* renderer);

	void updateButton();

	std::unique_ptr<GameObject> background;
	std::unique_ptr<GameObject> game_logo;

	std::unique_ptr<WorldController> world_controller;

	std::vector<GameObject> button_item;

	int current_select = 0;
};