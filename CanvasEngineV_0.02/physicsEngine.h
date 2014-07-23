#ifndef PHYSICS_H
#define PHYSICS_H

#include "Engine.h"
#include "Vector.h"
#include <cmath>
#include <cassert>
#include <algorithm>
#include <list>
#include <SDL.h>
#include <SDL_thread.h>
#include "physicsManifold.h"

class physicsShape;
class physicsBody;
class physicsEngine;
class collidableObj;



class point{
public:
	point(){}
	point(int x, int y){
		this->x = x;
		this->y = y;
	}
	double x,y;
	~point();
};


const double gravityScale = 15.0;
const Vector2D gravitationalForce( 0, 10 * gravityScale);
const double dt = 1.0/240.0;

class rayTrace{
public:
	rayTrace();
	void generateRay(double x1, double y1, double x2, double y2);
};

struct physicsMaterial{
public:
	double staticFriction;
	double dynamicFriction;
	double restitution;
};

class physicsBody{
public:
	double angularVelocity;
	double torque;
	double orient;
	double mass;
	double imass;
	double inertia;
	double iInertia;
	Vector2D rPosition;
	//Position Vectors;
	Vector2D position;
	Vector2D velocity;
	Vector2D force;
	//The collision layer of the physics body 
	//collisions do not occur between objects of different layers
	int collisionLayer;
	//shape of the object
	physicsMaterial material;

	physicsShape* shape;
	physicsBody(physicsShape* shape, double x, double y);
	//Find velocity
	void integrateVelocity(int timepassed);
	//apply impulse
	void applyImpulse(Vector2D imp, Vector2D r);
	//applyforce
	void applyForce(Vector2D f);
	//for terrain/buildings
	void setStatic();
	//Updates the position for World to read off
	void updatePosition();
	//set a body's orientation
	void setOrient(double radians);

	~physicsBody();

};


class physicsEngine:public CanvasSystem
{
public:
	int iterations;
	std::list<physicsManifold> conflicts;
	std::list<physicsBody*> pObjects;
	physicsEngine();
	~physicsEngine();
	virtual bool initialize();
	virtual void addObject(physicsBody* Object);
	virtual bool simulate();
	virtual void step();
	static void integrateForce(physicsBody* b, double timepassed);
	static void integrateVelocity( physicsBody *b, double timepassed);
	virtual void erase(physicsBody* body);
	virtual void render();

};

#endif