#include "physicsEngine.h"
#include <math.h>
#include "physicsShape.h"
#include "Vector.h"
#include "physicsManifold.h"


Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Vector2D::Vector2D(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Vector2D::lengthSquared()
{
	return x*x + y*y;
}

double Vector2D::lengthRooted()
{
	return sqrt(lengthSquared());
}

Vector2D Vector2D::scale(Vector2D vec, double scale)
{
	vec.x *= scale;
	vec.y *= scale;
	return vec;
}

Vector2D Vector2D::operator+ (Vector2D other) const
{
	Vector2D vector;
	vector.x = x + other.x;
	vector.y = y + other.y;
	return vector;
}

Vector2D Vector2D::operator+ (double s) const
{
	return Vector2D(x + s, y + s);
}

Vector2D Vector2D::operator- (Vector2D other) const
{
	Vector2D vector;
	vector.x = x - other.x;
	vector.y = y - other.y;
	return vector;
}

void Vector2D::operator+= (Vector2D other)
{
	x += other.x;
	y += other.y;
}

void Vector2D::operator-= (Vector2D other)
{
	x -= other.x;
	y -= other.y;
}

double Vector2D::dot(Vector2D x1, Vector2D x2)
{
	return x1.x*x2.x + x1.y*x2.y;
}

Vector2D min(Vector2D a, Vector2D b)
{
	return Vector2D(std::min(a.x, b.x), std::min(a.y, b.y));
}

Vector2D max(Vector2D a, Vector2D b)
{
	return Vector2D(std::max(a.x,b.x), std::max(a.y, b.y));
}

double dot(Vector2D a, Vector2D b)
{
	return a.x * b.x + a.y * b.y;
}

double distSquared(Vector2D a, Vector2D b)
{
	Vector2D c = a - b;
	return dot(c, c);
}

Vector2D Cross(Vector2D vec, double a)
{
	return Vector2D(a*vec.y, -a* vec.x);
}

Vector2D Cross(double a, Vector2D vec)
{
	return Vector2D(-a * vec.y, a * vec.x);
}

double Cross(Vector2D a, Vector2D b)
{
	return a.x * b.y - a.y * b.x;
}

bool Equal(double a, double b)
{
	return abs(a-b) <= EPSILON;
}

double sqr(double a)
{
	return a*a;
}

double clamp(double a, double max, double min)
{
	if(a < min) return min;
	if(a > max) return max;
	return a;
}

int round(int a)
{
	return (int)(a + 0.5);
}

double random(double l, double h)
{
	double a = (double)rand();
	a /= RAND_MAX;
	a = (h-l) * a + l;
	return a;
}

bool biasGreaterThan(double a, double b)
{
	const double k_relativeBias = 0.95;
	const double k_absoluteBias = 0.01;
	return a >= b * k_relativeBias + a * k_absoluteBias;
}


physicsBody::physicsBody(physicsShape* shape, double x, double y)
{
	this->shape = shape->clone();
	this->shape->body = this;
	position.set(x,y);
	velocity.set(0,0);
	angularVelocity = 0;
	torque = 0;
	orient = 0;
	this->shape->setOrient(orient);
	force.set(0,0);
	material.staticFriction = 0.5;
	material.dynamicFriction = 0.3;
	material.restitution = 0.2;
	this->shape->initialize();
}

void physicsBody::setOrient(double radians)
{
	orient = radians;
	shape->setOrient(radians);
}

void physicsBody::applyForce(Vector2D force)
{
	this->force += force;
}

void physicsBody::applyImpulse(Vector2D impulse, Vector2D contactVector)
{
	velocity += imass*impulse;
	angularVelocity += iInertia * Cross(contactVector, impulse);
}

void physicsBody::setStatic()
{
	iInertia = 0;
	inertia = 0;
	mass = 0;
	imass = 0;
}

physicsBody::~physicsBody()
{
}

physicsEngine::physicsEngine()
{
	iterations = 0;
}

bool physicsEngine::initialize()
{
	return true;
}

bool physicsEngine::simulate()
{
	timer.getDeltaTime();
	static double accumulator = 0;

	accumulator += ((double)timer.deltaTime/1000);

	accumulator = clamp(0, 0.01, accumulator);
	while(accumulator >= dt)
	{
		step();
		accumulator -= dt;
		iterations++;
	}

	return true;
}

physicsEngine::~physicsEngine()
{
}

void physicsEngine::erase(physicsBody* body)
{
}

void physicsEngine::addObject(physicsBody* body)
{
	pObjects.push_back(body);
}

void physicsEngine::integrateForce(physicsBody* body, double timepassed)
{
	if(body->imass == 0)
		return;

	body->velocity += (body->force * body->imass + gravitationalForce) * timepassed/2;
	body->angularVelocity += body->torque * body->iInertia * timepassed/2;

}

void physicsEngine::integrateVelocity(physicsBody* body, double timepassed)
{
	body->position += body->velocity*dt;
	body->orient += body->angularVelocity*dt;
	body->setOrient(body->orient);
	integrateForce(body, timepassed);
}

void physicsEngine::step()
{
	conflicts.clear();
	for(std::list<physicsBody*>::iterator it = pObjects.begin(), et = pObjects.end(); it != et; it++)
	{
		physicsBody* A = (*it);

		for(std::list<physicsBody*>::iterator jt = it; jt != et; jt++)
		{
			if(jt!=it)
			{
				physicsBody* B = (*jt);
				if(A->imass == 0 && B->imass == 0)
					continue;
				physicsManifold m(A,B);
				m.solve();
				if(m.contact_count)
					conflicts.push_back(m);
			}
		}
	}
	for(std::list<physicsBody*>::iterator it = pObjects.begin(), et = pObjects.end(); it != et; it++)
	{
		integrateForce((*it), dt);
	}

	for(std::list<physicsManifold>::iterator it = conflicts.begin(), et = conflicts.end(); it != et; it++)
	{
		(*it).initialize();
	}
	for(int i = 0; i < iterations; i++)
	{
		for(std::list<physicsManifold>::iterator it = conflicts.begin(), et = conflicts.end(); it != et; it++)
		{
			(*it).applyImpulse();
		}
	}
	iterations = 0;
	for(std::list<physicsBody*>::iterator it = pObjects.begin(), et = pObjects.end(); it != et; it++)
	{
		integrateVelocity((*it), dt);
	}

	for(std::list<physicsManifold>::iterator it = conflicts.begin(), et = conflicts.end(); it != et; it++)
	{
		(*it).positionalCorrection();
	}

	for(std::list<physicsBody*>::iterator it = pObjects.begin(), et = pObjects.end(); it != et; it++)
	{
		physicsBody* b = (*it);
		b->force.set(0,0);
		b->torque = 0;
	}

}

void physicsEngine::render()
{
	for(std::list<physicsBody*>::iterator it = pObjects.begin(), et = pObjects.end(); it != et; it++)
	{
		(*it)->shape->draw();
	}
	SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 255);
	for(std::list<physicsManifold>::iterator it = conflicts.begin(), et = conflicts.end(); it != et; it++)
	{
		int iterator = (*it).contact_count;
		Vector2D n = (*it).normal;
		for(int i = 0; i < iterator; i++)
		{
			Vector2D c = (*it).contacts[i];
			Vector2D d = c + n * 20;
			SDL_RenderDrawLine(mainRenderer, c.x, c.y, d.x, d.y);
		}
	}
}