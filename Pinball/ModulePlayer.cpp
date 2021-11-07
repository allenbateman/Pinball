#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "SDL_mixer/include/SDL_mixer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	plunger		 = App->textures->Load("pinball/PinballAssets/PinballSprites/plunger.png");
	flipperRight = App->textures->Load("pinball/PinballAssets/PinballSprites/flipper2.png");
	flipperLeft	 = App->textures->Load("pinball/PinballAssets/PinballSprites/flipper.png");
	flipperUp	 = App->textures->Load("pinball/PinballAssets/PinballSprites/flipper.png");

	plungerPos = {382 ,664 };
	plungerAnchorPos = {380 ,715 };
	flipperLPos = {119,680 };
	flipperRPos = { 260,680 };
	flipperUPos = { 45,388 };

	flipperRAnchorPos = { 260, 680 };
	flipperLAnchorPos = { 119, 680 };
	flipperUAnchorPos = { 45,  388 };

	plungerSize = { 13,5 };
	plungerForce = { 0,10 };
	
	startTime = SDL_GetTicks();
	counterMusic = 0;
	repetition = 1;

	masterAudioOn = true;
	SfxOn = true;
	MusicOn = true;

	sfx_bumper = App->audio->LoadFx("pinball/pinballAssets/pinballAudio/bumper1.wav");
	sfx_fliper = App->audio->LoadFx("pinball/pinballAssets/pinballAudio/flip.wav");
	sfx_fliper2 = App->audio->LoadFx("pinball/pinballAssets/pinballAudio/flip2.wav");

	flipperVelocity = 20;

	int flipperL_vertex[18] = {
		3, 17,
		57, 32,
		62, 30,
		63, 23,
		59, 19,
		10, 0,
		3, 2,
		0, 9,
		2, 16
	};

	int flipperR_vertex[16] = {
		53, 0,
		2, 22,
		0, 27,
		3, 33,
		8, 33,
		61, 17,
		64, 9,
		54, 0
	};

	//flipper bodies
	flipperLBody = App->physics->CreateChain(flipperLPos.x, flipperLPos.y, flipperL_vertex, 18, b2_dynamicBody);
	flipperUBody = App->physics->CreateChain(flipperUPos.x, flipperUPos.y, flipperL_vertex, 18, b2_dynamicBody);
	flipperRBody = App->physics->CreateChain(flipperRPos.x-64, flipperRPos.y-34, flipperR_vertex, 16, b2_dynamicBody);
	

	//anchor flipper bodies
	anchorL = App->physics->CreateCircle(flipperLAnchorPos.x, flipperLAnchorPos.y, 3, b2_kinematicBody);
	anchorU = App->physics->CreateCircle( flipperUAnchorPos.x,  flipperUAnchorPos.y, 3, b2_kinematicBody);
	anchorR = App->physics->CreateCircle(flipperRAnchorPos.x, flipperRAnchorPos.y, 3, b2_kinematicBody);

	//flipper joints
	b2Vec2 localCenterA;//local center of the fixed body
	b2Vec2 localCenterB;//local center of the flipper

	localCenterA.Set(0, 0);
	localCenterB.Set(9, 9);
	jointL = App->physics->RevoluteJoint(anchorL, localCenterA, flipperLBody, localCenterB, false, true, 0, -45, 45);
	jointU = App->physics->RevoluteJoint(anchorU, localCenterA, flipperUBody, localCenterB, false, true, 0, -45, 45);

	localCenterB.Set(54, 9);
	jointR = App->physics->RevoluteJoint(anchorR, localCenterA, flipperRBody, localCenterB, false, true, 0, -45, 45);


	//pungler

		//set up local centers
	b2Vec2 plungerlocalCenterBody;
	b2Vec2 plungerlocalCenterAnchor;

	plungerlocalCenterAnchor.x = plungerAnchorPos.x + plungerSize.x / 2;
	plungerlocalCenterAnchor.y = plungerAnchorPos.y + plungerSize.y / 2;

	plungerlocalCenterBody.x = plungerPos.x + plungerSize.x / 2;
	plungerlocalCenterBody.y = plungerPos.y + plungerSize.y / 2;

	b2Filter b;
	b.categoryBits = 1;
	b.maskBits = 1 | -1;

	plungerBody = App->physics->CreateRectangle(plungerlocalCenterBody.x, plungerlocalCenterBody.y, plungerSize.x, plungerSize.y, b2_dynamicBody);
	plungerBody->body->GetFixtureList()->SetFilterData(b);
	plungerAnchor = App->physics->CreateRectangle(plungerlocalCenterAnchor.x, plungerlocalCenterAnchor.y, plungerSize.x, plungerSize.y, b2_staticBody);
	plungerJoint = App->physics->DistanceJoint(plungerAnchor, plungerlocalCenterAnchor, plungerBody, plungerlocalCenterBody,true, 2 ,8.5f,0);
	plungerBody->body->SetFixedRotation(true);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(plunger);
	App->textures->Unload(flipperRight);
	App->textures->Unload(flipperLeft);
	App->textures->Unload(flipperUp);
	return true;
}

update_status ModulePlayer::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (!App->scene_intro->game_stop) //Para que cuando se pare el juego las palas y toda la pesca no se mueva
	{
		
		int counter = 0;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		counter += 1;
		if (counter % 2)
		{
			plungerForce.y += counter;
			plungerBody->body->ApplyForce(plungerForce, { 0, 0 }, true);
		}
		if (counter > 100)
		{
			counter = 100;
		}

	}
	else {
		plungerForce.y = 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_STATE::KEY_DOWN)
	{
		if (masterAudioOn)
			if (SfxOn)
				App->audio->PlayFx(sfx_fliper2);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
	
		//Left
		if(jointL->GetJointAngle()>= jointL->GetLowerLimit())
			flipperLBody->body->SetAngularVelocity(-flipperVelocity);
		else
			flipperLBody->body->SetAngularVelocity(0);

		//Upper
		if (jointU->GetJointAngle() >= jointU->GetLowerLimit())
			flipperUBody->body->SetAngularVelocity(-flipperVelocity);
		else
			flipperUBody->body->SetAngularVelocity(0);

	}
	else {

		//left
		if (jointL->GetJointAngle() < 0)
			flipperLBody->body->SetAngularVelocity(flipperVelocity);
		else
			flipperLBody->body->SetAngularVelocity(0);
		
		//uppper
		if (jointU->GetJointAngle() < 0)
			flipperUBody->body->SetAngularVelocity(flipperVelocity);
		else
			flipperUBody->body->SetAngularVelocity(0);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_STATE::KEY_DOWN)
	{
		if (masterAudioOn)
			if (SfxOn)
				App->audio->PlayFx(sfx_fliper);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		//Right
		if (jointR->GetJointAngle() <= jointR->GetUpperLimit())
			flipperRBody->body->SetAngularVelocity(flipperVelocity);
		else
			flipperRBody->body->SetAngularVelocity(0);
	}
	
	else {
		//right
		if (jointR->GetJointAngle() > 0)
			flipperRBody->body->SetAngularVelocity(-flipperVelocity);
		else
			flipperRBody->body->SetAngularVelocity(0);
	
	} 
	}
	

	b2Vec2 localCenter;//local center of the flipper
	localCenter.Set(9, 9);
	
	
	//left
	App->renderer->Blit(flipperLeft, flipperLPos.x - localCenter.x, flipperLPos.y - localCenter.y, SDL_FLIP_NONE, 0, 1, 1, jointL->GetJointAngle() * RADTODEG, localCenter.x, localCenter.y);
	//upper
	App->renderer->Blit(flipperUp, flipperUPos.x - localCenter.x, flipperUPos.y - localCenter.y, SDL_FLIP_NONE, 0, 1, 1, jointU->GetJointAngle() * RADTODEG, localCenter.x, localCenter.x);
	//right
	localCenter.Set(54, 9);
	App->renderer->Blit(flipperRight, flipperRPos.x - localCenter.x, flipperRPos.y - localCenter.y, SDL_FLIP_NONE, 0, 1, 1, jointR->GetJointAngle() * RADTODEG, localCenter.x, localCenter.y);
	
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 11;
	r.h = 56;
	r.h += plungerBody->body->GetPosition().y;
	App->renderer->Blit(plunger, plungerPos.x, plungerPos.y,SDL_FLIP_NONE, &r);
	App->renderer->DrawLine(METERS_TO_PIXELS(plungerBody->body->GetPosition().x), METERS_TO_PIXELS(plungerBody->body->GetPosition().y),
		METERS_TO_PIXELS(plungerAnchor->body->GetPosition().x), METERS_TO_PIXELS(plungerAnchor->body->GetPosition().y),
		0, 0, 255, 255);
	
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{




	return UPDATE_CONTINUE;
}