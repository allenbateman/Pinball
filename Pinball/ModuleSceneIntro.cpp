#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "SDL_mixer/include/SDL_mixer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
	lives = 3; //Si quereis poner m?s vidas pues aqui teneis los datillos
	balls_left = 3;

	game_menu_rect = { (SCREEN_WIDTH / 2) - 100, (SCREEN_HEIGHT / 2) - 64, 200, 128 };
	game_menu_crop = { 0, 0, 200, 128 };
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	background1 = App->textures->Load("pinball/PinballAssets/PinballSprites/layer0.png");
	background2 = App->textures->Load("pinball/PinballAssets/PinballSprites/layer1.png");
	background3 = App->textures->Load("pinball/PinballAssets/PinballSprites/layer2.png");
	background4 = App->textures->Load("pinball/PinballAssets/PinballSprites/PintballResilayer3.png");
	ball = App->textures->Load("pinball/PinballAssets/PinballSprites/pinball.png");
	wheel = App->textures->Load("pinball/PinballAssets/PinballSprites/wheel0.png");
	bumper = App->textures->Load("pinball/PinballAssets/PinballSprites/bumperoff.png");
	bumperOn = App->textures->Load("pinball/PinballAssets/PinballSprites/bumperon.png");	
	game_menu_texture = App->textures->Load("pinball/PinballAssets/PinballSprites/popupmenu.png"); //Game OverMenu

	//Audio
	startTime = SDL_GetTicks();
	counterMusic = 0;
	repetition = 1;

	masterAudioOn = true;
	SfxOn = true;
	MusicOn = true;

	sfx_bumper = App->audio->LoadFx("pinball/pinballAssets/pinballAudio/bumper1.wav");

	//Fonts
	OrangeFont = App->fonts->Load("Assets/Sprites/OrangeNumsNew.png", "0123456789", 1);
	YellowFont = App->fonts->Load("Assets/Sprites/YellowNumsNew.png", "0123456789", 1);

	int wheel0_vertex[32] = {
	2, 0,
	15, 22,
	21, 22,
	24, 27,
	51, 27,
	51, 30,
	25, 30,
	22, 34,
	16, 35,
	2, 57,
	0, 56,
	13, 34,
	11, 29,
	13, 24,
	0, 1,
	1, 0
	};
	wheelPos = { 147,123 };
	wheelBody = App->physics->CreateChain(wheelPos.x, wheelPos.y, wheel0_vertex, 32, b2_dynamicBody);
	wheelAnchor = App->physics->CreateCircle(wheelPos.x, wheelPos.y, 3, b2_kinematicBody);
	wheelJoint = App->physics->RevoluteJoint(wheelAnchor, { -1,-1 }, wheelBody, { 17.5,28 });

	bumper1Pos = { 96,164 };
	bumper2Pos = { 113,225 };
	bumper3Pos = { 180,180 };

	bumpers.add(App->physics->CreateCircle(bumper1Pos.x, bumper1Pos.y, 13, b2_kinematicBody));
	bumpers.getLast()->data->listener = this;
	bumpers.getLast()->data->type = ColliderType::BUMPER;
	bumpers.add(App->physics->CreateCircle(bumper2Pos.x, bumper2Pos.y, 13, b2_kinematicBody));
	bumpers.getLast()->data->listener = this;
	bumpers.getLast()->data->type = ColliderType::BUMPER;
	bumpers.add(App->physics->CreateCircle(bumper3Pos.x, bumper3Pos.y, 13, b2_kinematicBody));
	bumpers.getLast()->data->listener = this;
	bumpers.getLast()->data->type = ColliderType::BUMPER;


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background1);
	App->textures->Unload(background2);
	App->textures->Unload(background3);
	App->textures->Unload(background4);

	App->textures->Unload(YellowScoreText);
	App->textures->Unload(OrangeScoreText);

	if (game_stop == 0) App->textures->Unload(game_menu_texture);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	b2Vec2 backgroundPosition;
	int x = 0, y = 0;
	backgroundPosition.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	//background drawing
	App->renderer->Blit(background1, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background2, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background3, backgroundPosition.x, backgroundPosition.y);
	//App->renderer->Blit(background4, backgroundPosition.x, backgroundPosition.y);
	
	
	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		// create ball in pring pos
		circles.add(App->physics->CreateCircle(387, 655,5, b2_dynamicBody));
		circles.getLast()->data->listener = this;
		circles.getLast()->data->type = ColliderType::BALL;
	}
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		// create ball in mouse pos
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(),5, b2_dynamicBody));
		circles.getLast()->data->listener = this;
		circles.getLast()->data->type = ColliderType::BALL;
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	//wheelBody->body->SetAngularVelocity(-5);
	App->renderer->Blit(wheel, wheelPos.x - 17.5, wheelPos.y - 28, SDL_FLIP_NONE, NULL, 1.0f, 1, -wheelJoint->GetJointAngle() * RADTODEG, 17.5, 28);


	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (y > SCREEN_HEIGHT)
		{
			lives--;
			circles.del(c);
			LOG("%i/3 LIFES LOST", lives);
			break;
		}
		else {
			App->renderer->Blit(ball, x, y, SDL_FLIP_NONE, NULL, 1.0f, c->data->GetRotation());
			
		}	
		c = c->next;
		
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, SDL_FLIP_NONE, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = bumpers.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(bumper, x, y, SDL_FLIP_NONE, NULL, 1.0f, 1.0f,c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	


	//Aqu? est? el game loop (lo de la win/lose condition)

	if (score == 1000000) //si quereis cambiar la score para ganar adelante
	{
		win_condition = 1;
		game_stop = 1;
	}

	if ((((lives == 0) || (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)) && (!game_stop))) // Esto ser? la losing condition, pero de momento no puedo hacerla bien
	{
		game_stop = 1;
		LOG("**********GAME HAS STOPPED**********")
	}

	if (game_stop)
	{
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			game_stop = 0;
			LOG("**********GAME HAS RESUMED**********");
			if (lives == 0) lives = 3;
		}		
		//Falta ense?ar la score
		//Hace falta implementar que cuando pulses R se reinicie todo (la score, las vidas, la posicion de las bolas, etc)

		App->renderer->Blit(game_menu_texture, game_menu_rect.x, game_menu_rect.y, SDL_FLIP_NONE, &game_menu_crop);
	}
	
	
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	//LOG("LA BOLA EST? COLISIONANDO");	
	
	//Estos colliders lo detectan todo como bumper o como ball aunque no lo sean por algun motivo.  
	/*	
	if (bodyA->type == ColliderType::BUMPER)
	{
		LOG("COLLISION BUMPER A");
		score += 1000;
		LOG("SCORE = %i", score);
	}	

	if (bodyB->type == ColliderType::BUMPER)
	{
		LOG("COLLISION BUMPER B");
		score += 1000;
		LOG("SCORE = %i", score);
	}	

	if (bodyA->type == ColliderType::BALL)
	{
		LOG("COLLISION BALL A");
		score += 1000000;
		LOG("SCORE = %i", score);
	}
	if (bodyB->type == ColliderType::BALL)
	{
		LOG("COLLISION BALL B");
		score += 1000000;
		LOG("SCORE = %i", score);
	}	
	*/
	
	//LA BOLA colisona con un BUMPER de esos redonditos y azules (uw?)
	LOG("COLISIONANDO CON %i, %i", METERS_TO_PIXELS(bodyB->body->GetPosition().x), METERS_TO_PIXELS(bodyB->body->GetPosition().y));
	if ((METERS_TO_PIXELS((bodyB->body->GetPosition()) == bumper1Pos)) ||
		(METERS_TO_PIXELS((bodyB->body->GetPosition()) == bumper2Pos)) ||
		(METERS_TO_PIXELS((bodyB->body->GetPosition()) == bumper3Pos)))
	{

		score += 10000;
		LOG("SCORE = %i", score);
	}

	
	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
	*/
}

void ModuleSceneIntro::AddScore(int multiply, int Addpoints)
{
	score += multiply * Addpoints;
}

void ModuleSceneIntro::DrawScore()
{

	char char_score[10];
	sprintf_s(char_score, "%.5d", score);
	App->fonts->Blit(650, 500, OrangeFont, char_score);

	char char_hscore[10];
	sprintf_s(char_hscore, "%.5d", high_score);
	App->fonts->Blit(650, 435, YellowFont, char_hscore);

	char BallNumbers[10];
	sprintf_s(BallNumbers, "%.1d", BallsNum);
	App->fonts->Blit(650, 560, YellowFont, BallNumbers);

}