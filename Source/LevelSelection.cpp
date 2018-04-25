#include <Engine/OGLGame.h>

#include "GameState.h"
#include "LevelSelection.h"
#include "WorldController.h"
#include "GameObject.h"
#include "Game.h"

LevelSelection::LevelSelection()
{
	background = std::make_unique<GameObject>();
	game_logo = std::make_unique<GameObject>();
}

void LevelSelection::init(ASGE::Renderer* renderer, AngryBirdsGame* main)
{
	menuSetup(renderer);
	updateButton(main);

}

void LevelSelection::mouseControl(Bounds mouse, const ASGE::ClickEvent* click_event, AngryBirdsGame* main)
{

	for (int i = 0; i < button_item.size(); i++)
	{
		if (world_controller->collision(mouse, button_item[i]->getBound()) == true)
		{
			current_select = i;
			updateButton(main);
			buttonUse(main);
		}
	}
}

void LevelSelection::keyboardControl(const ASGE::KeyEvent* key_event, AngryBirdsGame* main)
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
			updateButton(main);
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
			updateButton(main);
		}
		else if (key_event->key == ASGE::KEYS::KEY_ENTER || key_event->key == ASGE::KEYS::KEY_SPACE)
		{
			buttonUse(main);
		}
	}
}

void LevelSelection::update(double delta_time)
{

}

void LevelSelection::render(ASGE::Renderer* renderer)
{
	renderer->renderSprite(*background->getObject());

	renderer->renderText("Score: " + std::to_string(score), 850, 800, 2.0f, ASGE::COLOURS::WHITESMOKE);

	// Display Buttons
	for (int i = 0; i < button_item.size(); i++)
	{
		renderer->renderSprite(*button_item[i]->getObject());
	}
}

void LevelSelection::menuSetup(ASGE::Renderer* renderer)
{
	background->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\LevelSelectScreen.png");

	buttonSetup(renderer);

}

void LevelSelection::buttonSetup(ASGE::Renderer* renderer)
{

	// Level 1
	std::unique_ptr<GameObject> new_button = std::make_unique<GameObject>();
	new_button->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\lvl1.png");
	new_button->positionObject(260, 300);
	button_item.push_back(std::move(new_button));

	// Level 2
	new_button = std::make_unique<GameObject>();
	new_button->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\lvl2.png");
	new_button->positionObject(760, 300);
	button_item.push_back(std::move(new_button));

	// Level 3
	new_button = std::make_unique<GameObject>();
	new_button->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\lvl3.png");
	new_button->positionObject(1260, 300);
	button_item.push_back(std::move(new_button));

	// Exit
	new_button = std::make_unique<GameObject>();
	new_button->loadObject(renderer, "..\\..\\Resources\\Textures\\Menu\\BackButton.png");
	new_button->positionObject(850, 600);
	button_item.push_back(std::move(new_button));

}

void LevelSelection::updateButton(AngryBirdsGame* main)
{
	score = main->getScore();

	for (int i = 0; i < button_item.size(); i++)
	{
		if (current_select == i)
		{
			if (current_select == 3)
			{
				button_item[i]->visualEffect(ASGE::COLOURS::WHITE);
				button_item[i]->visualEffect(1.01f);
			}
			else
			{
				if (main->levelUnlocked(current_select) == true)
				{
					button_item[i]->visualEffect(ASGE::COLOURS::WHITE);
					button_item[i]->visualEffect(1.01f);
				}
				else
				{
					button_item[i]->visualEffect(ASGE::COLOURS::ORANGERED);
					button_item[i]->visualEffect(1.01f);
				}
			}
		}
		else
		{
			button_item[i]->visualEffect(ASGE::COLOURS::LIGHTGRAY);
			button_item[i]->visualEffect(1.0f);
		}
	}
}

void LevelSelection::buttonUse(AngryBirdsGame* main)
{
	switch (current_select)
	{
	case 0:
		if (main->levelUnlocked(current_select) == true)
		{
			main->setLevel(current_select + 1);
			main->setLevelSetup(false);
			main->setGameState(GAME_STATE::GAME);
		}
		break;
	case 1:
		if (main->levelUnlocked(current_select) == true)
		{
			main->setLevel(current_select + 1);
			main->setLevelSetup(false);
			main->setGameState(GAME_STATE::GAME);
		}
		break;
	case 2:
		if (main->levelUnlocked(current_select) == true)
		{
			main->setLevel(current_select + 1);
			main->setLevelSetup(false);
			main->setGameState(GAME_STATE::GAME);
		}
		break;
	case 3:
		main->setGameState(GAME_STATE::MENU);
		break;
	}
}