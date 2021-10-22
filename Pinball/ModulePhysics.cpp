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

	int background2_vertex[124] = {
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

	/*int background2LD1_vertex[30] = {
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

	int background2LD2_vertex[12] =
	{
		44, 567,
	44, 626,
	46, 627,
	48, 626,
	48, 568,
	44, 567
	};

	int background2LD3_vertex[22] =
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
	63, 573
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
	};*/

	//Background colliders
	background2B = App->physics->CreateChain(0, 360, background2_vertex, 124, b2_kinematicBody);
	/*background2B2 = App->physics->CreateChain(0, 360, background2LD1_vertex, 30, b2_kinematicBody);
	background2B3 = App->physics->CreateChain(0, 360, background2LD2_vertex, 12, b2_kinematicBody);
	background2B4 = App->physics->CreateChain(0, 360, background2LD3_vertex, 22, b2_kinematicBody);
	background2B5 = App->physics->CreateChain(0, 360, background2RD1_vertex, 20, b2_kinematicBody);
	background2B6 = App->physics->CreateChain(0, 360, background2RD2_vertex, 12, b2_kinematicBody);
	background2B7 = App->physics->CreateChain(0, 360, background2RD3_vertex, 20, b2_kinematicBody);*/

	
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

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType bodyType)
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

	jointdef.referenceAngle = referenceAngle;
	jointdef.enableLimit = enableLimit;
	jointdef.lowerAngle = lowerAngle* DEGTORAD;
	jointdef.upperAngle = upperAngle * DEGTORAD;

	//LOG("JOINT-> anchor A.x:%i A.y:%i", METERS_TO_PIXELS(jointdef.localAnchorA.x), METERS_TO_PIXELS(jointdef.localAnchorA.y));
	//LOG("JOINT-> anchor B.x:%i B.y:%i", METERS_TO_PIXELS(jointdef.localAnchorB.x), METERS_TO_PIXELS(jointdef.localAnchorB.y));

	b2RevoluteJoint* joint = (b2RevoluteJoint*)world->CreateJoint(&jointdef);
	return joint;
}
b2DistanceJoint* ModulePhysics::DistanceJoint(PhysBody* bodyA, b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB, float distance)
{
	b2DistanceJointDef jointDef;

	jointDef.bodyA = bodyA->body;
	jointDef.localAnchorA.Set(PIXEL_TO_METERS(localCenterA.x), PIXEL_TO_METERS(localCenterA.y));
	jointDef.bodyB = bodyB->body;
	jointDef.localAnchorB.Set(PIXEL_TO_METERS(localCenterB.x), PIXEL_TO_METERS(localCenterB.y));

	jointDef.collideConnected = false;

	jointDef.length = distance;


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