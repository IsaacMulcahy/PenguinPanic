#pragma once

#include <Engine/OGLGame.h>


class GameObject
{
public:
	GameObject();
	~GameObject() = default;

	struct vector2
	{
		int x;
		int y;
	};

	struct vector2f
	{
		float x;
		float y;
	};

	enum OBJECT_TYPE
	{
		SOLID = 0,
		BREAKABLE = 1,
		MISSION_ITEM = 2
	};

	enum ANIMATION_STATE
	{
		IDLE = 0,
		MOVEMENT,
		ATTACK
	};

	struct bounds
	{
		float top_x;
		float top_y;
		float bottom_x;
		float bottom_y;
	};

	// Getting Object
	bool passableCheck() const;
	int getID() const;
	OBJECT_TYPE getType() const;
	vector2 getFacing() const;
	ASGE::Sprite* getObject() const;
	bounds getBound() const;
	float getJumpStrength() const;
	float getVelocity() const;
	bool getGrounded() const;

	// Setting Object
	void setType(OBJECT_TYPE);
	void setPassable(bool);
	void changeFacing(vector2);
	void updateBound();
	void setJumpStrength(float);
	void setGrounded(bool);
	void setVelocity(float);
	void setupAnimation(int, int);

	// Actions 
	void positionObject(int, int);
	void moveObjectBy(int, int);
	void moveForward(int, const ASGE::GameTime&);
	void moveDown(float, const ASGE::GameTime&);
	void loadObject(ASGE::Renderer*);
	void loadObject(ASGE::Renderer*, char[]);
	void loadObject(ASGE::Renderer*, std::string);
	void animateObject(ANIMATION_STATE, int);
	void increaseAnimationFrame();
protected:
	bool passable = false;
	ASGE::Sprite* object = nullptr;
	OBJECT_TYPE type = OBJECT_TYPE::SOLID;
	vector2 facing;
	bounds object_bound;

	float velocity;
	float jump_stength;
	bool is_grounded;

	int height_of_frame;
	int width_of_frame;
	ANIMATION_STATE animation_type;
	int frame;

	// ID
	static int new_id;
	const int id;

};