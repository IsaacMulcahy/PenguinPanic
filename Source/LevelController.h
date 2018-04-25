#pragma once
#include <vector>
#include <Engine/OGLGame.h>

#include "Vector2.h"
#include "Cannon.h"
#include "Penguin.h"
#include "WorldController.h"

class GameObject;

class LevelController
{
public:
	LevelController();
	~LevelController() = default;
	
	// Update
	void updateObjects(const ASGE::GameTime&);

	// Rendering
	void renderLevel(ASGE::Renderer*, int, int);
	void renderObjects(ASGE::Renderer*, int, int);
	void renderPenguins(ASGE::Renderer*, int, int);
	void renderNarwhales(ASGE::Renderer*, int, int);
	
	// Getters
	Cannon* getCannon() const;
	int currentSubScore() const { return current_score; };
	
	// Physics
	bool collision(GameObject*);
	bool isGrounded(GameObject*, const ASGE::GameTime&);
	void checkGravity(GameObject*, const ASGE::GameTime&);
	void checkNarwhales(GameObject*);
	
	// Level Control
	void makeLevel(int, ASGE::Renderer*);
	bool levelWon();

	Penguin* getCurrentPenguin();
	int penguinCount();

private:
	void createLevel(ASGE::Renderer*, int level_number);
	void levelOne(ASGE::Renderer*);
	void levelTwo(ASGE::Renderer*);
	void levelThree(ASGE::Renderer*);

	std::unique_ptr<WorldController> world_controller;

	std::unique_ptr<Cannon> cannon;
	std::vector<std::unique_ptr<GameObject>> object_list;
	std::vector<std::unique_ptr<Penguin>> penguin;
	std::vector<std::unique_ptr<GameObject>> narwhale;

	int current_score = 0;
	bool level_loaded = false;
};