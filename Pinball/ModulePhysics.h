#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


enum class ColliderType {
	BUMPER,
	WHEEL,
	BALL
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	int id;
	b2Body* body;
	Module* listener;
	ColliderType type;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius,b2BodyType type);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType bodyType, float density =1);
	b2Joint* JointBodies(PhysBody* bodyA, PhysBody* bodyB, b2JointType type);
	b2RevoluteJoint* RevoluteJoint(PhysBody* bodyA, b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB,bool collide = false, bool enableLimit = false, float referenceAngle = 0, float lowerAngle =-30000, float upperAngle = 30000);
	b2PrismaticJoint* PrismaticJoint(PhysBody* bodyA, b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB, float distance);
	b2DistanceJoint* DistanceJoint(PhysBody* bodyA, b2Vec2 localCenterA, PhysBody* bodyB, b2Vec2 localCenterB, bool collide, float distance, float frequency, float damping);

	PhysBody* background2B;
	PhysBody* background2B2;
	PhysBody* background2B3;
	PhysBody* background2B4;
	PhysBody* background2B5;
	PhysBody* background2B6;
	PhysBody* background2B7;
	PhysBody* background2B8;
	PhysBody* background2B9;
	PhysBody* background2B10;
	PhysBody* background2B11;
	PhysBody* background2B12;
	PhysBody* background2B13;
	PhysBody* background2B14;
	PhysBody* background2B15;

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};