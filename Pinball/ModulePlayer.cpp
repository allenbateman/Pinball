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

	plunger = App->textures->Load("pinball/PinballAssets/PinballSprites/plunger.png");
	flipperRight = App->textures->Load("pinball/PinballAssets/PinballSprites/flipper.png");
	flipperLeft = App->textures->Load("pinball/PinballAssets/PinballSprites/flipper.png");
	flipperUp = App->textures->Load("pinball/PinballAssets/PinballSprites/flipper.png");

	plungerPos = {382 ,664 };
	flipperLPos = { 105,668 };
	flipperRPos = { 205,668 };
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
	//flippers.add(App->physics->CreateChain(flipperLPos.x, flipperLPos.y, flipperL_vertex, 18, b2_kinematicBody));
	flipperLBody = App->physics->CreateChain(flipperLPos.x, flipperLPos.y, flipperL_vertex, 18, b2_kinematicBody);
	//flippers.add(App->physics->CreateChain(flipperUPos.x, flipperUPos.y, flipperL_vertex, 18, b2_kinematicBody));
	flipperUBody = App->physics->CreateChain(flipperUPos.x, flipperUPos.y, flipperL_vertex, 18, b2_kinematicBody);
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
	//flippers.add(App->physics->CreateChain(flipperRPos.x, flipperRPos.y, flipperR_vertex, 16, b2_kinematicBody));
	flipperRBody = App->physics->CreateChain(flipperRPos.x, flipperRPos.y, flipperR_vertex, 16, b2_kinematicBody);
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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		

		flipperLBody->body->SetTransform(flipperLBody->body->GetPosition(),-45);
		flipperUBody->body->SetTransform(flipperUBody->body->GetPosition(),-45);
		
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		flipperLBody->body->SetTransform(flipperLBody->body->GetPosition(), 0);
		flipperUBody->body->SetTransform(flipperUBody->body->GetPosition(), 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipperRBody->body->SetTransform(flipperRBody->body->GetPosition(), 45);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		flipperRBody->body->SetTransform(flipperRBody->body->GetPosition(), 0);
	}

	App->renderer->Blit(plunger, plungerPos.x, plungerPos.y);
	App->renderer->Blit(flipperRight, flipperRPos.x, flipperRPos.y, SDL_FLIP_HORIZONTAL);
	App->renderer->Blit(flipperLeft, flipperLPos.x, flipperLPos.y,SDL_FLIP_NONE);
	App->renderer->Blit(flipperUp, flipperUPos.x, flipperUPos.y);

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{




	return UPDATE_CONTINUE;
}



