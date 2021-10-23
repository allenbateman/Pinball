#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
	lives = 3; //Si quereis poner más vidas pues aqui teneis los datillos
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
	background4 = App->textures->Load("pinball/PinballAssets/PinballSprites/layer3.png");
	ball = App->textures->Load("pinball/PinballAssets/PinballSprites/pinball.png");

	/*int background2_vertex[124] = {
	397, 355,
	396, 356,
	395, -186,
	392, -202,
	387, -211,
	385, -218,
	380, -227,
	374, -236,
	369, -243,
	362, -251,
	354, -257,
	344, -265,
	335, -268,
	325, -274,
	315, -277,
	299, -279,
	91, -280,
	80, -277,
	68, -274,
	54, -269,
	46, -263,
	38, -257,
	34, -251,
	27, -242,
	25, -237,
	25, -249,
	22, -256,
	16, -259,
	11, -257,
	7, -252,
	8, -33,
	22, -3,
	34, -8,
	23, -34,
	25, -40,
	50, 18,
	40, 21,
	33, 28,
	29, 20,
	27, 13,
	26, 7,
	8, 8,
	10, 27,
	10, 33,
	10, 44,
	11, 55,
	13, 66,
	15, 73,
	18, 81,
	21, 90,
	24, 100,
	29, 110,
	32, 116,
	38, 125,
	41, 130,
	42, 137,
	10, 172,
	10, 353,
	23, 353,
	25, 316,
	147, 358,
	1, 358
	};

	int background2LD1_vertex[30] = {
	25, 562,
	25, 650,
	32, 657,
	37, 657,
	42, 658,
	113, 687,
	112, 683,
	113, 676,
	116, 672,
	121, 670,
	34, 637,
	30, 632,
	28, 627,
	29, 564,
	25, 562
	};
	//Background colliders
	background2B = App->physics->CreateChain(0, 360, background2_vertex, 124, b2_kinematicBody);
	background2B2 = App->physics->CreateChain(0, 360, background2LD1_vertex, 30, b2_kinematicBody);
	*/
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

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

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	b2Vec2 backgroundPosition;
	int x = 0, y = 0;
	backgroundPosition.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	App->renderer->Blit(background1, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background2, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background3, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background4, backgroundPosition.x, backgroundPosition.y);
	
	//if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	ray_on = !ray_on;
	//	ray.x = App->input->GetMouseX();
	//	ray.y = App->input->GetMouseY();
	//}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(),6, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, SDL_FLIP_NONE, NULL, 1.0f, c->data->GetRotation());
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

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, SDL_FLIP_NONE, NULL, 1.0f, c->data->GetRotation());
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
	
	//Aquí está el game loop (lo de la win/lose condition)

	if (score == 1000000) //si quereis cambiar la score para ganar adelante
	{
		win_condition = 1;
		game_stop = 1;
	}

	if ((lives == 0) || (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)) // Esto será la losing condition, pero de momento no puedo hacerla bien
	{
		game_stop = 1;
	}

	if (game_stop == 1)
	{
		//Las Paletas tienen que dejar de funcionar
		//enseñar score		
	}
		
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

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
	}*/
}
