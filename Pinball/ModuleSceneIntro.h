#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"


class PhysBody;
struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> chains;

	PhysBody* sensor;
	PhysBody* background2B;
	PhysBody* background2B2;

	PhysBody* wheelBody;
	PhysBody* wheelAnchor;
	b2Vec2 wheelPos = { 0,0 };
	b2RevoluteJoint* wheelJoint;
	
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background1;
	SDL_Texture* background2;
	SDL_Texture* background3;
	SDL_Texture* background4;
	SDL_Texture* background2LD1;

	SDL_Texture* wheel;
	SDL_Texture* ball;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int score;
	int lives;

	bool win_condition;
	bool game_stop;
};
