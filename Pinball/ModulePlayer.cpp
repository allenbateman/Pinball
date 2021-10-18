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

// Update: draw background
update_status ModulePlayer::Update()
{

	App->renderer->Blit(plunger, plungerPos.x, plungerPos.y);
	App->renderer->Blit(flipperRight, flipperRPos.x, flipperRPos.y, SDL_FLIP_HORIZONTAL);
	App->renderer->Blit(flipperLeft, flipperLPos.x, flipperLPos.y);
	App->renderer->Blit(flipperUp, flipperUPos.x, flipperUPos.y);

	return UPDATE_CONTINUE;
}



