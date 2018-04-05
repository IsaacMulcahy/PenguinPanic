#pragma once
#include <string>
#include <Engine/OGLGame.h>
#include "GameState.h"

#include "GameObject.h"

class Menu;

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

	// State
	GAME_STATE game_state = GAME_STATE::MENU;
};