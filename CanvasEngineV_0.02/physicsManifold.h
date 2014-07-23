#ifndef MANIFOLD_H
#define MANIFOLD_H

class physicsBody;

class physicsManifold{
public:
	physicsManifold(physicsBody* a, physicsBody* b);
	void solve();
	void initialize();
	void applyImpulse();
	void positionalCorrection();
	void infiniteMassCorrection();

	physicsBody* a;
	physicsBody* b;
	
	double penetration;
	Vector2D normal;
	Vector2D contacts[2];
	unsigned int contact_count;
	double e;
	double df;
	double sf;
};

bool CirclevsCircle(physicsManifold* m, physicsBody* a, physicsBody* b);
bool PolygonvsCircle(physicsManifold* m, physicsBody* a, physicsBody* b);
bool CirclevsPolygon(physicsManifold* m, physicsBody* a, physicsBody* b);
bool PolygonvsPolygon(physicsManifold* m, physicsBody* a, physicsBody* b);

#endif