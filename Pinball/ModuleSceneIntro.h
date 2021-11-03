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

	p2List<PhysBody*> bumpers;


	PhysBody* sensor;
	PhysBody* background2B;
	PhysBody* background2B2;
	PhysBody* Bumper1;
	PhysBody* Bumper2;
	PhysBody* Bumper3;

	PhysBody* wheelBody;
	PhysBody* wheelAnchor;

	b2Vec2 bumper1Pos = { 0,0 };
	b2Vec2 bumper2Pos = { 0,0 };
	b2Vec2 bumper3Pos = { 0,0 };

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
	SDL_Texture* YellowScoreText = nullptr;
	SDL_Texture* OrangeScoreText = nullptr;

	int OrangeFont;
	int YellowFont;

	SDL_Texture* wheel;
	SDL_Texture* bumper;
	SDL_Texture* bumperOn;
	SDL_Texture* ball;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int BallsNum = 4;

	int score;
	int lives;

	bool win_condition;
	bool game_stop;

	
	int	high_score;
	int	ScoreMultiply = 1;

	void AddScore(int multiply, int Addscore);
	void SetBoolstoFalse();

	void DrawScore();

	SDL_Rect game_menu_rect;
	SDL_Rect game_menu_crop;
	SDL_Texture* game_menu_texture;
};
