#pragma once
#include <vector>
#include <Engine/OGLGame.h>
#include "GameObject.h"

class Menu
{
public:
	Menu();
	~Menu() = default;

	void mouseControl(double mouse_x, double mouse_y, const ASGE::ClickEvent* click_event);
	void update(double delta_time);
	void render(ASGE::Renderer* renderer);
private:
	std::unique_ptr<GameObject> background;
	std::unique_ptr<GameObject> game_logo;
}