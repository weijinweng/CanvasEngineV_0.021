#ifndef WORLD_H
#define WORLD_H
#include "Engine.h"
#include <SDL.h>
#include <SDL_thread.h>
#include "GUI.h"
#include "physicsShape.h"

class physicsEngine;
class World;
class Layer;
class Obj;
class Camera;

struct Key
{
	int keyCode;
	bool on;
};

enum ObjType
{
	TERRAIN,
	PERSON,
	BACKGROUND
};


class Obj
{
public:
	bool show;
	ObjType type;
	std::list<Obj*> children;
	World* containerWorld;
	renderData renderable;
	Obj();
	Obj(double x, double y, double w, double h);
	virtual void calibrateRenderable();
	virtual void setWorld(World* world);
	virtual void behave();
	virtual void simulate(int deltaTime);
	virtual void render(std::list<renderData>* data, Camera* camera);
};

class Camera{
public:
	SDL_Rect obj;
	SDL_Texture* cameraTexture;
	std::list<renderData> renderBuffer;
	int flipAngle;
	Camera(){}
	void setScreenSize(int width, int height);
	void render();
	~Camera(){}
};

class collidableObj:public Obj
{
public:
	double height;
	collidableObj();
	collidableObj(double x, double y, double w, double h);
	virtual void setCollider(double x, double y, double w, double h);
	virtual bool onCollide(collidableObj* other);
	virtual void setWorld(World* world);
	virtual void calibratePhysics();
	virtual void move(double x, double y);	
};

class terrain:public collidableObj
{
public:
	virtual bool onCollide(collidableObj* other);
};

class World:public CanvasSystem
{
public:
	SDL_sem* renderSem;
	SDL_sem* eventSem;
	Camera camera;
	std::list<SDL_Event> eventBuffer;
	physicsEngine* physics;
	std::list<Obj*> objs;
	World();
	virtual bool simulate();
	virtual bool initialize();
	void calibrate();
	void getEvents(SDL_Event e);
	void render();
};


#endif