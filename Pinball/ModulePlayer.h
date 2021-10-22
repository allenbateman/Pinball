#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "ModulePhysics.h"
class PhysBody;
struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:

	int lives; 
	int score;

	SDL_Texture* plunger;
	SDL_Texture* flipperRight;
	SDL_Texture* flipperLeft;
	SDL_Texture* flipperUp;

	b2Vec2  flipperRPos, flipperLPos, flipperUPos = { 0,0 };
	b2Vec2  flipperRAnchorPos, flipperLAnchorPos, flipperUAnchorPos = { 0,0 };
	b2Vec2 plungerPos, plungerAnchorPos = { 0,0 };

	p2List<PhysBody*> flippers;

	PhysBody* flipperLBody;
	PhysBody* flipperUBody;
	PhysBody* flipperRBody;

	PhysBody* anchorL;
	PhysBody* anchorU;
	PhysBody* anchorR;


	PhysBody* plungerBody;
	PhysBody* plungerAnchor;
	b2Vec2 plungerSize;


	b2DistanceJoint* plungerJoint;

	b2RevoluteJoint* jointL;
	b2RevoluteJoint* jointU;
	b2RevoluteJoint* jointR;


	

};