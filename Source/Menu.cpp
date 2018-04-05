#include <Engine/OGLGame.h>

#include "GameState.h"

#include "Menu.h"
#include "WorldController.h"
#include "GameObject.h"
#include "Game.h"

Menu::Menu()
{
	background = std::make_unique<GameObject>();
	game_logo = std::make_unique<GameObject>();
	world_controller = std::make_unique<WorldController>();
}

void Menu::init(ASGE::Renderer* renderer)
{
	menuSetup(renderer);
	
}

void Menu::mouseControl(Bounds mouse, const ASGE::ClickEvent* click_event, AngryBirdsGame* main)
{

	for (int i = 0; i < button_item.size(); i++)
	{
		if (world_controller->collision(mouse, button_item[i].getBound()) == true)
		{
			current_select = i;
			updateButton();
		}
	}

	switch (current_select)
	{
		case 0:
			main->setGameState(GAME_STATE::GAME);
		break;
	}
}

void Menu::keyboardControl(const ASGE::KeyEvent* key_event, AngryBirdsGame* main)
{
	if (key_event->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (key_event->key == ASGE::KEYS::KEY_RIGHT)
		{
			if (current_select < button_item.size()-1)
			{
				current_select++;
			}
			else
			{
				current_select = 0;
			}
			updateButton();
		} 
		else if (key_event->key == ASGE::KEYS::KEY_LEFT)
		{
			if (current_select > 0)
			{
				current_select--;
			}
			else
			{
				current_select = button_item.size();
			}
			updateButton();
		}
	}
}

void Menu::update(double delta_time)
{

}

void Menu::render(ASGE::Renderer* renderer)
{
	renderer->renderSprite(*background->getObject());

	renderer->renderText(std::to_string(current_select),0,150);

	// Display Buttons
	for (int i = 0; i < button_item.size(); i++)
	{
		renderer->renderSprite(*button_item[i].getObject());
	}
}

void Menu::menuSetup(ASGE::Renderer* renderer)
{
	background->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\menu.jpg");

	buttonSetup(renderer);

}

void Menu::buttonSetup(ASGE::Renderer* renderer)
{
	int current_button = button_item.size();

	// Play Button
	button_item.push_back(GameObject());
	button_item.at(current_button).loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\Play Button.png");
	button_item.at(current_button).positionObject(375, 870);

	current_button++;

	// Quit Button
	button_item.push_back(GameObject());
	button_item.at(current_button).loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\Exit Button.png");
	button_item.at(current_button).positionObject(1300, 870);
}

void Menu::updateButton()
{
	for (int i = 0; i < button_item.size(); i++)
	{
		if (current_select == i)
		{
			button_item[i].visualEffect(ASGE::COLOURS::WHITE);
			button_item[i].visualEffect(1.3f);
		}
		else
		{
			button_item[i].visualEffect(ASGE::COLOURS::LIGHTGRAY);
			button_item[i].visualEffect(1.0f);
		}
	}
}
