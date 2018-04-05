#pragma once
#include <vector>
#include <Engine/OGLGame.h>

#include "Vector2.h"
#include "GameObject.h"
#include "WorldController.h"
#include "GameObjectBlueprint.h"

class LevelController
{
public:
	LevelController();
	~LevelController();

	void makeLevel(int, ASGE::Renderer*);
	void positionCamera(float x, float y);
	vector2 getCameraPosition() const;
	void renderLevel(ASGE::Renderer*, int, int);
	void renderBackgrounds(ASGE::Renderer*, int, int);
	void renderObjects(ASGE::Renderer*, int, int);
	void renderPlayer(ASGE::Renderer*) const;
	GameObject* getPlayer() const;
	bool collision(GameObject*);
	bool isGrounded(GameObject*, const ASGE::GameTime&);
	bool passableGround(GameObject*, const ASGE::GameTime&);
	void checkGravity(GameObject*, const ASGE::GameTime&);
	bool levelLoaded();
	bool levelWon();
	void debugSetLevelWon();
	

private:
	void createLevel(ASGE::Renderer*);
	void makeObjectFromBlueprint(GameObjectBlueprint, vector2, ASGE::Renderer*);
	void createPlayer(ASGE::Renderer*);

	std::unique_ptr<WorldController> world_controller;
	std::vector<GameObject> blueprint;

	std::vector<GameObject> background_list[1];
	std::vector<GameObject> object_list[1];
	GameObject* player;

	vector2 area_size = vector2(0,0);
	vector2 world_offset = vector2(0, 0);
	vector2 camera_position = vector2(0, 0);

	bool level_completed = false;
	bool level_loaded = false;
};