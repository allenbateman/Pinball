#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* big_ball = world->CreateBody(&body);

	int background2_vertex[160] = {
	8, 108,
	8, 330,
	23, 357,
	35, 353,
	25, 326,
	25, 321,
	27, 321,
	50, 378,
	50, 380,
	46, 380,
	43, 381,
	40, 382,
	38, 385,
	36, 389,
	33, 387,
	31, 383,
	29, 380,
	28, 377,
	27, 373,
	27, 369,
	9, 369,
	9, 385,
	10, 397,
	10, 405,
	11, 415,
	14, 427,
	16, 436,
	19, 446,
	21, 452,
	24, 459,
	27, 465,
	30, 471,
	33, 476,
	36, 481,
	39, 486,
	42, 490,
	42, 498,
	10, 530,
	10, 714,
	25, 714,
	25, 677,
	27, 676,
	149, 719,
	0, 719,
	0, 72,
	405, 72,
	405, 720,
	395, 720,
	395, 176,
	394, 165,
	391, 156,
	386, 143,
	379, 129,
	372, 121,
	365, 113,
	359, 107,
	351, 101,
	342, 95,
	334, 91,
	325, 87,
	313, 84,
	305, 82,
	296, 81,
	92, 81,
	82, 82,
	72, 84,
	66, 87,
	57, 91,
	51, 95,
	47, 98,
	42, 102,
	35, 108,
	30, 115,
	26, 124,
	24, 122,
	24, 110,
	22, 105,
	19, 102,
	13, 102,
	9, 105
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
	25, 563
	};

	int background2LD2_vertex[10] =
	{
		44, 567,
	44, 626,
	46, 627,
	48, 626,
	48, 568,
	
	};

	int background2LD3_vertex[20] =
	{
		63, 573,
	63, 629,
	65, 631,
	68, 633,
	75, 635,
	98, 646,
	104, 646,
	107, 642,
	71, 572,
	68, 571,
	
	};

	int background2RD1_vertex[20] =
	{
		352, 564,
	352, 654,
	265, 686,
	264, 677,
	257, 669,
	344, 637,
	347, 633,
	348, 625,
	349, 564,
	351, 562
	};

	int background2RD2_vertex[12] =
	{
		334, 567,
	334, 624,
	332, 627,
	330, 625,
	330, 568,
	332, 566
	};

	int background2RD3_vertex[20] =
	{
		315, 573,
	315, 627,
	312, 632,
	280, 645,
	275, 646,
	272, 644,
	272, 640,
	306, 573,
	310, 570,
	314, 571
	};

	int background2LU_vertex[72] =
	{
		60, 309,
	28, 248,
	25, 240,
	24, 232,
	24, 183,
	25, 175,
	26, 167,
	29, 158,
	32, 151,
	37, 142,
	41, 135,
	48, 129,
	62, 140,
	63, 179,
	95, 199,
	79, 232,
	95, 257,
	100, 262,
	110, 267,
	117, 273,
	117, 287,
	114, 290,
	107, 286,
	103, 279,
	59, 208,
	59, 158,
	40, 147,
	38, 152,
	35, 159,
	33, 166,
	30, 175,
	29, 182,
	29, 239,
	36, 255,
	64, 306,
	63, 309
	};

	int background2LU1_vertex[12] =
	{
		131, 276,
	131, 285,
	134, 287,
	136, 285,
	136, 276,
	134, 274
	};

	int background2LU2_vertex[12] =
	{
		150, 274,
	150, 285,
	152, 287,
	154, 285,
	154, 275,
	152, 272
	};

	int background2RU_vertex[66] =
	{
		211, 327,
	215, 288,
	213, 284,
	210, 281,
	206, 281,
	201, 283,
	198, 287,
	186, 299,
	134, 314,
	130, 312,
	133, 309,
	161, 300,
	164, 298,
	167, 296,
	174, 265,
	173, 259,
	165, 231,
	164, 228,
	166, 227,
	217, 258,
	217, 275,
	221, 278,
	227, 278,
	232, 275,
	232, 250,
	215, 233,
	215, 170,
	198, 154,
	198, 137,
	200, 136,
	263, 198,
	263, 274,
	213, 327
	};

	int background2RU1_vertex[80] =
	{
		263, 153,
	335, 227,
	341, 236,
	344, 242,
	348, 251,
	351, 260,
	354, 268,
	356, 283,
	356, 304,
	355, 312,
	351, 320,
	346, 326,
	327, 332,
	323, 330,
	344, 297,
	346, 290,
	345, 285,
	343, 278,
	339, 270,
	334, 264,
	328, 264,
	271, 326,
	268, 326,
	253, 312,
	254, 309,
	289, 272,
	290, 269,
	278, 258,
	278, 218,
	280, 216,
	309, 246,
	309, 257,
	275, 294,
	275, 301,
	279, 304,
	284, 304,
	324, 261,
	324, 232,
	261, 169,
	261, 153
	};

	int background2RU2_vertex[106] =
	{
		300, 156,
	319, 137,
	325, 143,
	334, 152,
	340, 161,
	349, 178,
	354, 195,
	355, 205,
	372, 205,
	372, 186,
	370, 174,
	367, 164,
	363, 155,
	357, 144,
	349, 136,
	334, 121,
	337, 117,
	344, 123,
	351, 131,
	361, 142,
	367, 153,
	373, 164,
	378, 181,
	380, 192,
	380, 458,
	377, 462,
	370, 465,
	363, 469,
	357, 472,
	352, 475,
	348, 474,
	348, 446,
	351, 443,
	354, 438,
	357, 433,
	361, 427,
	364, 420,
	367, 409,
	368, 402,
	370, 391,
	372, 381,
	372, 273,
	370, 261,
	366, 246,
	359, 232,
	355, 224,
	349, 215,
	345, 208,
	338, 201,
	331, 193,
	325, 186,
	316, 176,
	299, 158
	};

	int background2M_vertex[14] =
	{
		307, 373,
	328, 365,
	330, 367,
	330, 428,
	327, 430,
	306, 378,
	304, 374
	};

	int background2S_vertex[32] =
	{
		380, 479,
	379, 719,
	230, 719,
	351, 676,
	353, 677,
	353, 714,
	368, 714,
	368, 529,
	349, 510,
	349, 505,
	353, 499,
	357, 493,
	361, 488,
	366, 485,
	373, 483,
	377, 481
	};


	//Background colliders
	background2B = App->physics->CreateChain(0, 0, background2_vertex, 160, b2_staticBody);
	background2B2 = App->physics->CreateChain(0, 0, background2LD1_vertex, 30, b2_staticBody);
	background2B3 = App->physics->CreateChain(0, 0, background2LD2_vertex, 10, b2_staticBody);
	background2B4 = App->physics->CreateChain(0, 0, background2LD3_vertex, 20, b2_staticBody);
	background2B5 = App->physics->CreateChain(0, 0, background2RD1_vertex, 20, b2_staticBody);
	background2B6 = App->physics->CreateChain(0, 0, background2RD2_vertex, 12, b2_staticBody);
	background2B7 = App->physics->CreateChain(0, 0, background2RD3_vertex, 20, b2_staticBody);
	background2B8 = App->physics->CreateChain(0, 0, background2LU_vertex, 72, b2_staticBody);
	background2B9 = App->physics->CreateChain(0, 0, background2LU1_vertex, 12, b2_staticBody);
	background2B10 = App->physics->CreateChain(0, 0, background2LU2_vertex, 12, b2_staticBody);
	background2B11 = App->physics->CreateChain(0, 0, background2RU_vertex, 66, b2_staticBody);
	background2B12 = App->physics->CreateChain(0, 0, background2RU1_vertex, 80, b2_staticBody);
	background2B13 = App->physics->CreateChain(0, 0, background2RU2_vertex, 106, b2_staticBody);
	background2B14 = App->physics->CreateChain(0, 0, background2M_vertex, 14, b2_staticBody);
	background2B15 = App->physics->CreateChain(0, 0, background2S_vertex, 32, b2_staticBody);

	
	//b2CircleShape shape;
	//shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	//b2FixtureDef fixture;
	//fixture.shape = &shape;
	//big_ball->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius,b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType bodyType,float density)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	
	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	b->CreateFixture(&fixture);

	//delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

b2Joint* ModulePhysics::JointBodies(PhysBody* bodyA, PhysBody* bodyB, b2JointType type)
{
	b2JointDef jointdef;
	jointdef.type = type;
	jointdef.bodyA = bodyA->body;
	jointdef.bodyB = bodyB->body;
	jointdef.collideConnected = false;

	b2Joint* joint = (b2Joint *) world->CreateJoint(&jointdef);
	return joint;
}
//revolute Joint
b2RevoluteJoint* ModulePhysics::RevoluteJoint(PhysBody* bodyA,b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB,bool collide, bool enableLimit, float referenceAngle,float lowerAngle,float upperAngle)
{
	b2RevoluteJointDef jointdef;

	jointdef.bodyA = bodyA->body;
	jointdef.localAnchorA.Set(PIXEL_TO_METERS(localCenterA.x), PIXEL_TO_METERS(localCenterA.y));

	jointdef.bodyB = bodyB->body;
	jointdef.localAnchorB.Set(PIXEL_TO_METERS(localCenterB.x), PIXEL_TO_METERS(localCenterB.y));

	jointdef.collideConnected = collide;

	jointdef.enableLimit = enableLimit;
	jointdef.referenceAngle = referenceAngle;
	jointdef.lowerAngle = lowerAngle* DEGTORAD;
	jointdef.upperAngle = upperAngle * DEGTORAD;

	b2RevoluteJoint* joint = (b2RevoluteJoint*)world->CreateJoint(&jointdef);
	return joint;
}
b2PrismaticJoint* ModulePhysics::PrismaticJoint(PhysBody* bodyA, b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB, float distance)
{
	b2PrismaticJointDef jointDef;

	jointDef.bodyA = bodyA->body;
	jointDef.localAnchorA.Set(PIXEL_TO_METERS(localCenterA.x), PIXEL_TO_METERS(localCenterA.y));

	jointDef.bodyB = bodyB->body;
	jointDef.localAnchorB.Set(PIXEL_TO_METERS(localCenterB.x), PIXEL_TO_METERS(localCenterB.y));

	jointDef.localAxisA.Set(0, -1);
	jointDef.enableLimit = true;
	jointDef.lowerTranslation = PIXEL_TO_METERS(localCenterB.y);
	jointDef.upperTranslation = PIXEL_TO_METERS(localCenterA.y);
	jointDef.collideConnected = true;

	b2PrismaticJoint* joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);

	return nullptr;
}
b2DistanceJoint* ModulePhysics::DistanceJoint(PhysBody* bodyA, b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB, bool collide, float distance, float frequency, float damping)
{
	b2DistanceJointDef jointDef;

	jointDef.bodyA = bodyA->body;
	jointDef.localAnchorA.Set(PIXEL_TO_METERS(localCenterA.x), PIXEL_TO_METERS(localCenterA.y));

	jointDef.bodyB = bodyB->body;
	jointDef.localAnchorB.Set(PIXEL_TO_METERS(localCenterB.x), PIXEL_TO_METERS(localCenterB.y));

	jointDef.length = distance;
	jointDef.frequencyHz = frequency;
	jointDef.dampingRatio = damping;
	jointDef.collideConnected = collide;
	

	b2DistanceJoint* joint = (b2DistanceJoint*)world->CreateJoint(&jointDef);

	return nullptr;
}
// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}