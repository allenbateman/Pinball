#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

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
	flipperLPos = {119,680 };
	flipperRPos = { 260,680 };
	flipperUPos = { 32,377 };

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

	//anchor bodies
	anchorL = App->physics->CreateCircle(119, 680, 3, b2_kinematicBody);
	anchorU = App->physics->CreateCircle(44,  387, 3, b2_kinematicBody);
	anchorR = App->physics->CreateCircle(260, 680, 3, b2_kinematicBody);

	b2Vec2 localCenterA;//local center of the fixed body
	b2Vec2 localCenterB;//local center of the flipper

	localCenterA.Set(0,0);
	localCenterB.Set(9, 9);
	jointL = App->physics->RevoluteJoint(anchorL, localCenterA, flipperLBody, localCenterB,false,true,0,-45,45);
	jointU = App->physics->RevoluteJoint(anchorU, localCenterA, flipperUBody, localCenterB,false,true,0,-45,45);

	localCenterB.Set(54, 9);
	jointR = App->physics->RevoluteJoint(anchorR, localCenterA, flipperRBody, localCenterB,false,true,0,-45,45);	

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
	
		//Left
		if(jointL->GetJointAngle()>= jointL->GetLowerLimit())
			flipperLBody->body->SetAngularVelocity(-30);
		else
			flipperLBody->body->SetAngularVelocity(0);

		//Upper
		if (jointU->GetJointAngle() >= jointU->GetLowerLimit())
			flipperUBody->body->SetAngularVelocity(-30);
		else
			flipperUBody->body->SetAngularVelocity(0);

	}
	else {

		//left
		if (jointL->GetJointAngle() < 0)
			flipperLBody->body->SetAngularVelocity(30);
		else
			flipperLBody->body->SetAngularVelocity(0);
		
		//uppper
		if (jointU->GetJointAngle() < 0)
			flipperUBody->body->SetAngularVelocity(30);
		else
			flipperUBody->body->SetAngularVelocity(0);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		//Right
		if (jointR->GetJointAngle() <= jointR->GetUpperLimit())
			flipperRBody->body->SetAngularVelocity(30);
		else
			flipperRBody->body->SetAngularVelocity(0);
	}
	else {
		//right
		if (jointR->GetJointAngle() > 0)
			flipperRBody->body->SetAngularVelocity(-30);
		else
			flipperRBody->body->SetAngularVelocity(0);
	}



	b2Vec2 localCenter;//local center of the flipper
	localCenter.Set(9, 9);
	App->renderer->Blit(plunger, plungerPos.x, plungerPos.y);
	
	//left
	App->renderer->Blit(flipperLeft, flipperLPos.x - localCenter.x, flipperLPos.y - localCenter.y, SDL_FLIP_NONE, 0, 1, 1, jointL->GetJointAngle() * RADTODEG, localCenter.x, localCenter.y);
	//upper
	App->renderer->Blit(flipperUp, flipperUPos.x - localCenter.x, flipperUPos.y - localCenter.y, SDL_FLIP_NONE, 0, 1, 1, jointU->GetJointAngle() * RADTODEG, localCenter.x, localCenter.x);
	//right
	localCenter.Set(54, 9);
	App->renderer->Blit(flipperRight, flipperRPos.x - localCenter.x, flipperRPos.y - localCenter.y, SDL_FLIP_NONE, 0, 1, 1, jointR->GetJointAngle() * RADTODEG, localCenter.x, localCenter.y);
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{




	return UPDATE_CONTINUE;
}



