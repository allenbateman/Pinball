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
	update_status Update();
	bool CleanUp();

public:

	int lives; 
	int score;
	SDL_Texture* plunger;
	SDL_Texture* flipperRight;
	SDL_Texture* flipperLeft;
	SDL_Texture* flipperUp;
	iPoint plungerPos, flipperRPos, flipperLPos, flipperUPos = { 0,0 };
	

};