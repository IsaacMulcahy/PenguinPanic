#pragma once
#include <string>
#include <Engine/OGLGame.h>
#include "GameState.h"

#include "GameObject.h"
#include "GameLoop.h"

class Menu;
class LevelSelection;
class WinScreen;
class LoseScreen;

/**
*  An OpenGL Game based on ASGE.
*/
class AngryBirdsGame :
	public ASGE::OGLGame
{
public:
	AngryBirdsGame();
	~AngryBirdsGame();
	virtual bool init() override;

	void setGameState(GAME_STATE value) { game_state = value; };
	void setLevel(int level) { game_level = level; level_setup = false; };
	void nextLevel() { game_level++; level_setup = false; }
	void setLevelSetup(bool value) { level_setup = value; };
	void setLevelComplete() { level_completed[game_level-1] = true; };
	bool levelUnlocked(int);
	int getCurrentLevel() { return game_level; };
	void addScore(int new_score) { level_score[game_level-1] = new_score; };
	int getScore() { return score; };

private:
	void keyHandler(const ASGE::SharedEventData data);
	void clickHandler(const ASGE::SharedEventData data);
	void setupResolution();

	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	//Add your GameObjects
	std::unique_ptr<Menu> menu;
	std::unique_ptr<LevelSelection> level_selection;
	std::unique_ptr<GameLoop> core_game;
	std::unique_ptr<WinScreen> win_screen;
	std::unique_ptr<LoseScreen> lose_screen;

	static const int numOfLevels = 3;
	std::vector<bool> level_completed;
	std::vector<int> level_score;

	// State
	GAME_STATE game_state = GAME_STATE::MENU;
	bool level_setup = false;
	int game_level = 0;
	int score = 0;
};