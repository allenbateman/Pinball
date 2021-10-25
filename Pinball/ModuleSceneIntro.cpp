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
	wheel = App->textures->Load("pinball/PinballAssets/PinballSprites/wheel0.png");

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
	wheelBody = App->physics->CreateChain(wheelPos.x, wheelPos.y, wheel0_vertex, 32, b2_dynamicBody,0);
	wheelAnchor = App->physics->CreateCircle(wheelPos.x, wheelPos.y, 3, b2_kinematicBody);
	wheelJoint = App->physics->RevoluteJoint(wheelAnchor, { -1,-1 }, wheelBody, { 17.5,28 });
	

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

	//background drawing
	App->renderer->Blit(background1, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background2, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background3, backgroundPosition.x, backgroundPosition.y);
	App->renderer->Blit(background4, backgroundPosition.x, backgroundPosition.y);
	
	
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
	//	circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(),6, b2_dynamicBody));
		circles.add(App->physics->CreateCircle(387, 655,6, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	//wheelBody->body->SetAngularVelocity(5);
	App->renderer->Blit(wheel, wheelPos.x-17.5, wheelPos.y-28, SDL_FLIP_NONE, NULL, 1.0f, wheelJoint->GetJointAngle() * RADTODEG, 17.5,28 );


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
