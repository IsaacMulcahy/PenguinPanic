#pragma once
#include <string>
#include <Engine/OGLGame.h>

#include "GameObject.h"
#include "Menu.h"
#include "Rect.h"


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
	
	enum GAME_STATE
	{
		MENU = 0,
		GAME = 1
	};

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