#include <Engine/OGLGame.h>

#include "GameState.h"
#include "Win.h"
#include "WorldController.h"
#include "GameObject.h"
#include "Game.h"

WinScreen::WinScreen()
{
	background = std::make_unique<GameObject>();
}

void WinScreen::init(ASGE::Renderer* renderer)
{
	menuSetup(renderer);
	updateButton();

}

void WinScreen::mouseControl(Bounds mouse, const ASGE::ClickEvent* click_event, AngryBirdsGame* main)
{

	for (int i = 0; i < button_item.size(); i++)
	{
		if (world_controller->collision(mouse, button_item[i]->getBound()) == true)
		{
			current_select = i;
			updateButton();
			buttonUse(main);
		}
	}
}

void WinScreen::keyboardControl(const ASGE::KeyEvent* key_event, AngryBirdsGame* main)
{
	if (key_event->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (key_event->key == ASGE::KEYS::KEY_RIGHT || key_event->key == ASGE::KEYS::KEY_DOWN)
		{
			if (current_select < button_item.size() - 1)
			{
				current_select++;
			}
			else
			{
				current_select = 0;
			}
			updateButton();
		}
		else if (key_event->key == ASGE::KEYS::KEY_LEFT || key_event->key == ASGE::KEYS::KEY_UP)
		{
			if (current_select > 0)
			{
				current_select--;
			}
			else
			{
				current_select = button_item.size() - 1;
			}
			updateButton();
		}
		else if (key_event->key == ASGE::KEYS::KEY_ENTER || key_event->key == ASGE::KEYS::KEY_SPACE)
		{
			buttonUse(main);
		}
	}
}

void WinScreen::update(double delta_time)
{

}

void WinScreen::render(ASGE::Renderer* renderer)
{
	renderer->renderSprite(*background->getObject());

	renderer->renderText(std::to_string(current_select), 0, 150);

	// Display Buttons
	for (int i = 0; i < button_item.size(); i++)
	{
		renderer->renderSprite(*button_item[i]->getObject());
	}
}

void WinScreen::menuSetup(ASGE::Renderer* renderer)
{
	background->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\Win.png");

	buttonSetup(renderer);

}

void WinScreen::buttonSetup(ASGE::Renderer* renderer)
{

	// Level 1
	std::unique_ptr<GameObject> new_button = std::make_unique<GameObject>();
	new_button->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\BackButton.png");
	new_button->positionObject(490, 470);
	button_item.push_back(std::move(new_button));

	// Exit
	new_button = std::make_unique<GameObject>();
	new_button->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\ContinueButton.png");
	new_button->positionObject(1100, 470);
	button_item.push_back(std::move(new_button));

}

void WinScreen::updateButton()
{
	for (int i = 0; i < button_item.size(); i++)
	{
		if (current_select == i)
		{
			button_item[i]->visualEffect(ASGE::COLOURS::WHITE);
			button_item[i]->visualEffect(1.01f);
		}
		else
		{
			button_item[i]->visualEffect(ASGE::COLOURS::LIGHTGRAY);
			button_item[i]->visualEffect(1.0f);
		}
	}
}

void WinScreen::buttonUse(AngryBirdsGame* main)
{
	switch (current_select)
	{
	case 0:
		main->setLevelComplete();
		main->setGameState(GAME_STATE::LEVEL_SELECTION);
		break;
	case 1:
		main->setLevelComplete();
		if (main->getCurrentLevel() < 3)
		{
			main->nextLevel();
			main->setGameState(GAME_STATE::GAME);
		}
		else
		{
			main->setGameState(GAME_STATE::LEVEL_SELECTION);
		}
		break;
	}
}