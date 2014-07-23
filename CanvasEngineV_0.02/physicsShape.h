#ifndef PHYSICSSHAPE_H
#define PHYSICSSHAPE_H
#include "physicsEngine.h"
#include <SDL.h>

extern SDL_Renderer* mainRenderer;
const int MAXPOLY = 20;
class Vector2D;

enum physicsType{
	CIRCLE,
	POLYGON,
	ANY
};

class physicsShape{
public:
	physicsType type;
	physicsBody* body;

	double radius;

	physicsShape();
	physicsType getType();
	virtual physicsShape* clone();
	virtual void initialize();
	virtual void draw();
	virtual void computeMass(double density);
	virtual void setOrient(double radians);

	mat22 u;
};

class physicsCircle:public physicsShape{
public:
	physicsCircle(double radius);

	void initialize();
	physicsShape* clone();
	void computeMass(double density);
	void setOrient(double radians);
	void draw();
};



class physicsPolygon:public physicsShape
{
public:
	int vertexCount;
	Vector2D vertices[MAXPOLY];
	Vector2D normals[MAXPOLY];
	physicsPolygon();
	
	physicsShape* clone();
	void initialize();
	void computeMass(double density);
	void setOrient(double radian);
	void draw();
	void setBox(double hw, double hh);
	void set(Vector2D* vertexes, int count);
	Vector2D GetSupport(Vector2D direction);
};

#endif PHYSICSSHAPE_H