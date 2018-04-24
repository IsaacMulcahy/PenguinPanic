#pragma once
#include <vector>
#include <Engine/OGLGame.h>

#include "Vector2.h"
#include "Cannon.h"
#include "WorldController.h"

class GameObject;

class LevelController
{
public:
	LevelController();
	~LevelController() = default;

	// Camera
	void positionCamera(float x, float y);
	vector2 getCameraPosition() const;
	
	// Update
	void updateObjects(const ASGE::GameTime&);

	// Rendering
	void renderLevel(ASGE::Renderer*, int, int);
	void renderObjects(ASGE::Renderer*, int, int);
	void renderPenguins(ASGE::Renderer*, int, int);
	
	// Getters
	Cannon* getCannon() const;
	
	// Physics
	bool collision(GameObject*);
	bool isGrounded(GameObject*, const ASGE::GameTime&);
	bool passableGround(GameObject*, const ASGE::GameTime&);
	void checkGravity(GameObject*, const ASGE::GameTime&);
	
	// Level Control
	void makeLevel(int, ASGE::Renderer*);
	bool levelLoaded();
	bool levelWon();
	void debugSetLevelWon();

	GameObject* getCurrentPenguin();
	

private:
	void createLevel(ASGE::Renderer*, int level_number);
	void levelOne(ASGE::Renderer*);

	std::unique_ptr<WorldController> world_controller;

	std::unique_ptr<Cannon> cannon;
	std::vector<std::unique_ptr<GameObject>> object_list;
	std::vector<std::unique_ptr<GameObject>> penguin;

	vector2 area_size = vector2(0,0);
	vector2 world_offset = vector2(0, 0);
	vector2 camera_position = vector2(0, 0);

	bool level_completed = false;
	bool level_loaded = false;
};