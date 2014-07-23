#include "physicsEngine.h"
#include "physicsShape.h"

physicsShape::physicsShape()
{
}

physicsType physicsShape::getType()
{
	return type;
}

void physicsShape::initialize()
{
}

void physicsShape::draw()
{
}

void physicsShape::computeMass(double density)
{

}

void physicsShape::setOrient(double radians)
{
}

physicsShape* physicsShape::clone()
{
	return NULL;
}

physicsCircle::physicsCircle(double radius)
{
	this->radius = radius;
	type = CIRCLE;
	body = NULL;
}

physicsShape* physicsCircle::clone()
{
	physicsCircle* circle = new physicsCircle(radius);
	return (physicsShape*) circle;
}

void physicsCircle::initialize()
{
	computeMass(1.0);
}

void physicsCircle::setOrient(double radians)
{
}

void physicsCircle::computeMass(double density)
{
	body->mass = PI*radius*radius*density;
	body->imass = (body->mass) ? 1.0f/body->mass : 0.0;
	body->inertia = body->mass * radius * radius;
	body->iInertia = body->inertia ? 1.0/body->inertia : 0.0;
}

void physicsCircle::draw()
{
		const int draw_segs = 24;
		if(body == NULL)
		{

		}
		double orient = body->orient;
		double increment = (2*PI)/24;
		Vector2D vertex[draw_segs+1];
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
		for(int i = 0; i < draw_segs; ++i)
		{
			vertex[i].x = std::cos(orient);
			vertex[i].y = std::sin(orient);
			vertex[i] *= radius;
			vertex[i] += body->position;
			orient += increment;
			if(i)
			{
				SDL_RenderDrawLine(mainRenderer, vertex[i-1].x, 
					vertex[i-1].y, vertex[i].x, vertex[i].y);
			}

		}
		SDL_RenderDrawLine(mainRenderer, vertex[draw_segs-1].x, vertex[draw_segs-1].y, vertex[0].x, vertex[0].y);
		Vector2D radianLine(0, 1);
		double c = std::cos(body->orient);
		double s = std::sin(body->orient);

		radianLine.set(radianLine.x * c - radianLine.y * s, radianLine.x * s + radianLine.y * c);
		radianLine *= radius;
		radianLine = radianLine + body->position;
		SDL_RenderDrawLine(mainRenderer, body->position.x, body->position.y, radianLine.x, radianLine.y);
}

physicsPolygon::physicsPolygon()
{
	type = POLYGON;
	vertexCount = 0;
}

void physicsPolygon::initialize()
{
	computeMass(1.0);
}

physicsShape* physicsPolygon::clone()
{
	physicsPolygon* poly = new physicsPolygon();
	poly->u = u;
	for(int i = 0; i < vertexCount; ++i)
	{
		poly->vertices[i] = vertices[i];
		poly->normals[i] = normals[i];
	}
	poly->vertexCount = vertexCount;
	return poly;
}

void physicsPolygon::computeMass(double density)
{
	Vector2D c(0,0);
	double area = 0.0;
	double I = 0;
	const double k_inv3 = (double)1/(double)3;

	for(int i = 0; i < vertexCount; ++i)
	{
		Vector2D p1(vertices[i]);
		int i2 = i + 1 < vertexCount ? i + 1: 0;
		Vector2D p2(vertices[i2]);

		double D = Cross(p1, p2);
		double triangleArea = 0.5 * D;

		area += triangleArea;

		c+= triangleArea * k_inv3 * (p1 + p2);

		double intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		double inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25 * k_inv3 * D) * (intx2 + inty2);
	}

	c*= ((double)1)/area;

	for(int i = 0; i < vertexCount; ++i)
	{
		vertices[i] -= c;
	}

	body->mass = density * area;
	body->imass = (body->mass) ? 1 / body->mass : 0.0;
	body->inertia = I * density;
	body->iInertia = body->inertia?1/body->inertia:0.0;

}

void physicsPolygon::setOrient(double radians)
{
	u.set(radians);
}

void physicsPolygon::draw()
	{
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
		Vector2D vertexes[MAXPOLY];
		for(int i = 0; i < vertexCount; ++i)
		{
			vertexes[i] = body->position + u * vertices[i];
			if(i)
			{
				SDL_RenderDrawLine(mainRenderer, vertexes[i-1].x, vertexes[i-1].y, vertexes[i].x, vertexes[i].y);
			}
		}
		SDL_RenderDrawLine(mainRenderer, vertexes[vertexCount-1].x, vertexes[vertexCount-1].y, vertexes[0].x, vertexes[0].y);
	}

void physicsPolygon::setBox(double hw, double hh)
{
		vertexCount = 4;
		vertices[0].set(-hw, -hh);
		vertices[1].set( hw, -hh);
		vertices[2].set( hw, hh);
		vertices[3].set( -hw, hh);
		normals[0].set( 0, -1);
		normals[1].set( 1, 0);
		normals[2].set( 0, 1);
		normals[3].set( -1, 0);
}

void physicsPolygon::set(Vector2D* vertexes, int count)
{

	for( int i = 0; i < count; i++)
	{
		vertices[i] = vertexes[i];
	}
	vertexCount = count;
	for(int i = 0; i < vertexCount; ++i)
	{
		int i2 = i + 1 < vertexCount ? i + 1:0;
		Vector2D face = vertices[i2] - vertices[i];

		if(!(face.lengthSquared() > EPSILON*EPSILON))
		{
			printf("error in face.lengthSquared");
		}

		normals[i] = Vector2D(face.y, -face.x);
		normals[i].normalize();
	}
		/*
		if(count > 2 && count <= MAXPOLY)
		{
			count = std::min(count, MAXPOLY);

			int rightMost = 0;
			double highestXcoord = vertices[0].x;
			for(int i = 1; i < count; ++i)
			{
				double x = vertices[i].x;
				if(x > highestXcoord)
				{
					highestXcoord = x;
					rightMost = i;
				}

				else if( x == highestXcoord)
					if(vertices[i].y < vertices[rightMost].y)
						rightMost = i;
			}
			int hull[MAXPOLY];
			int outCount = 0;
			int indexHull = rightMost;

			for(;;)
			{
				hull[outCount] = indexHull;

				int nextHullindex = 0;
				for(int i = 1; i < count; ++i)
				{
					if(nextHullindex == indexHull)
					{
						nextHullindex = i;
						continue;
					}

					Vector2D e1 = vertices[nextHullindex] - vertices[hull[outCount]];
					Vector2D e2 = vertices[i] - vertices[hull[outCount]];
					double c = Cross(e1, e2);
					if(c < 0)
						nextHullindex = i;

					if(c == 0 && e2.lengthSquared() > e1.lengthSquared())
						nextHullindex = i;
				}

				++outCount;
				indexHull = nextHullindex;

				if(nextHullindex == rightMost)
				{
					vertexCount = outCount;
					printf("works\n");
					break;
				}
			}

			for(int i = 0; i < vertexCount; ++i)
			{
				int i2 = i + 1 < vertexCount ? i + 1:0;
				Vector2D face = vertices[i2] - vertices[i];

				if(!(face.lengthSquared() > EPSILON*EPSILON))
				{
					printf("error in face.lengthSquared");
				}

				normals[i] = Vector2D(face.y, -face.x);
				normals[i].normalize();
			}

		}*/
}

Vector2D physicsPolygon::GetSupport(Vector2D direction)
	{
		double bestProjection = -FLT_MAX;
		Vector2D bestVertex;

		for(int i = 0; i < vertexCount; ++i)
		{
			Vector2D v = vertices[i];
			double projection = dot(v, direction);

			if(projection > bestProjection)
			{
				bestVertex = v;
				bestProjection = projection;
			}
		}

		return bestVertex;
	}

physicsManifold::physicsManifold(physicsBody* a, physicsBody* b)
{
	this->a = a;
	this->b = b;
	contact_count = 0;
	penetration = 0;
	e = 0;
	sf = 0;
	df = 0;
}

void physicsManifold::solve()
{
	physicsType aType = a->shape->getType();
	physicsType bType = b->shape->getType();

	if(aType == CIRCLE && bType == CIRCLE)
	{
		CirclevsCircle(this,a,b);
	}
	if(aType == POLYGON && bType == POLYGON)
	{
		PolygonvsPolygon(this, a,b);
	}
	if(aType == POLYGON && bType == CIRCLE)
	{
		PolygonvsCircle(this, a,b);
	}
	if(aType == CIRCLE && bType == POLYGON)
	{
		CirclevsPolygon(this, a,b);
	}
}

bool CirclevsCircle(physicsManifold* m, physicsBody* a, physicsBody* b)
{
	physicsCircle* aCircle = (physicsCircle*) a;
	physicsCircle* bCircle = (physicsCircle*) b;

	Vector2D normal = b->position - a->position;

	double dist_sqr = normal.lengthSquared();
	double radius = aCircle->radius + bCircle->radius;

	if(dist_sqr >= radius*radius)
	{
		m->contact_count = 0;
		return false;
	}

	double distance = std::sqrt(dist_sqr);

	m->contact_count = 1;
	
	if(distance == 0)
	{
		m->penetration = aCircle->radius;
		m->normal = Vector2D(1,0);
		m->contacts[0] = a->position;
	}
	else {
		m->penetration = radius - distance;
		m->normal = normal/distance;
		m->contacts[0] = m->normal * aCircle->radius + a->position;
	}

	return true;
}

bool CirclevsPolygon(physicsManifold* m, physicsBody* a, physicsBody* b)
{
	physicsCircle* A = (physicsCircle*) a->shape;
	physicsPolygon* B = (physicsPolygon*) b->shape;

	m->contact_count =0;

	Vector2D center = a->position;
	center = B->u.transpose() * (center - b->position);

	double separation = -FLT_MAX;
	int faceNormal = 0;
	for(int i = 0; i < B->vertexCount; ++i)
	{
		double s = dot(B->normals[i], center - B->vertices[i]);

		if(s > A->radius)
		{
			return false;
		}
		if(s > separation)
		{
			separation = s;
			faceNormal = i;
		}
	}

	Vector2D v1 = B->vertices[faceNormal];
	int i2 = (faceNormal + 1 < B->vertexCount )? faceNormal + 1 :0;
	Vector2D v2 = B->vertices[i2];
	
	if(separation < EPSILON)
	{

		m->contact_count =1;
		m->normal = -(B->u*B->normals[faceNormal]);
		m->contacts[0] = m->normal * A->radius = a->position;
		m->penetration = A->radius;
		return true;
		
	}

	double dot1 = dot(center - v1, v2 - v1);
	double dot2 = dot(center - v2, v1 - v2);
	m->penetration = A->radius - separation;

	if(dot1 <= 0)
	{
		if(distSquared(center, v1) > A->radius * A->radius)
		{
			return false;
		}
		m->contact_count = 1;
		Vector2D n = v1 - center;
		n = B->u * n;
		n.normalize();
		m->normal = n;
		v1 = B->u* v1 + b->position;
		m->contacts[0] = v1;
		
	}

	else if(dot2 <= 0)
	{
		if(distSquared(center, v2) > A->radius* A->radius)
			return false;
		m->contact_count = 1;
		Vector2D n = v2 - center;
		v2 = B->u*v2 + b->position;
		m->contacts[0] = v2;
		n = B->u*n;
		n.normalize();
		m->normal = n;

	}

	else
	{
		Vector2D n = B->normals[faceNormal];
		if(dot(center-v1, n ) > A->radius)
			return false;
		n = B->u*n;
		m->normal = -n;
		m->contacts[0] = m->normal* A->radius + a->position;
		m->contact_count =1;
	}
	return true;
}

bool PolygonvsCircle(physicsManifold* m, physicsBody* a, physicsBody* b)
{
	bool returnValue = CirclevsPolygon(m, b, a);
	m->normal = -m->normal;
	return returnValue;
}
double AxisOfLeastPenetration(int* faceIndex, physicsPolygon* a, physicsPolygon* b)
{
	double bestDistance = -FLT_MAX;
	int bestIndex;

	for(int i = 0; i < a->vertexCount; ++i)
	{
		Vector2D n = a->normals[i];
		Vector2D nw = a->u*n;

		mat22 buT = b->u.transpose();
		n = buT * nw;

		Vector2D s = b->GetSupport(-n);

		Vector2D v = a->vertices[i];
		v = a->u*v + a->body->position;
		v -= b->body->position;
		v = buT * v;

		double d = dot(n, s - v);

		if(d > bestDistance)
		{
			bestDistance = d;
			bestIndex = i;
		}
	}

	*faceIndex = bestIndex;
	return bestDistance;
}

void FindIncidentFace(Vector2D* v, physicsPolygon* refPoly, physicsPolygon* incPoly, int referenceIndex)
{
	Vector2D referenceNormal = refPoly->normals[referenceIndex];

	referenceNormal = refPoly->u*referenceNormal;
	referenceNormal = incPoly->u.transpose()* referenceNormal;

	int incidentFace = 0;
	double minDot = FLT_MAX;
	for(int i = 0; i < incPoly->vertexCount; ++i)
	{
		double fDot = dot(referenceNormal, incPoly->normals[i]);
		if(fDot < minDot)
		{
			minDot = fDot;
			incidentFace = i;
		}
	}

	v[0] = incPoly->u * incPoly->vertices[incidentFace] + incPoly->body->position;
	incidentFace = incidentFace + 1 >= incPoly->vertexCount ? 0:incidentFace +1;
	v[1] = incPoly->u*incPoly->vertices[incidentFace] + incPoly->body->position;
}

int Clip(Vector2D n, double c, Vector2D* face)
{
	int sp = 0;
	Vector2D out[2] = {
		 face[0],
		 face[1]
	};
	
	double d1 = dot(n, face[0]) -c;
	double d2 = dot(n, face[1]) -c;

	if(d1 <= 0)
	{
			out[sp++] = face[0];
	}
	if(d2 <= 0)
	{
		out[sp++] = face[1];
	}

	if(d1 * d2 < 0)
	{
		double alpha = d1 / ( d1-d2);
		out[sp] = face[0] + alpha * (face[1] - face[0]);
		++sp;
	}
	face[0] = out[0];
	face[1] = out[1];

	assert(sp!=3);
	
	return sp;
}

bool PolygonvsPolygon(physicsManifold* m, physicsBody* a, physicsBody* b)
{
	physicsPolygon* A = (physicsPolygon*) a->shape;
	physicsPolygon* B = (physicsPolygon*) b->shape;
	m->contact_count = 0;

	int faceA;
	double penetrationA = AxisOfLeastPenetration(&faceA, A, B);
	if(penetrationA >= 0)
	{
		return false;
	}

	int faceB;
	double penetrationB = AxisOfLeastPenetration(&faceB, B, A);
	if(penetrationB >= 0)
		return false;

	int referenceIndex;
	bool flip;
	
	physicsPolygon* refPoly;
	physicsPolygon* incPoly;

	if(biasGreaterThan(penetrationA, penetrationB))
	{
		refPoly = A;
		incPoly = B;
		referenceIndex = faceA;
		flip = false;
	}

	else
	{
		refPoly = B;
		incPoly = A;
		referenceIndex = faceB;
		flip = true;
	}

	Vector2D incidentFace[2];
	FindIncidentFace(incidentFace, refPoly, incPoly, referenceIndex);

	Vector2D v1 = refPoly->vertices[referenceIndex];
	referenceIndex = referenceIndex + 1 == refPoly->vertexCount ? 0 : referenceIndex + 1;
	Vector2D v2 = refPoly->vertices[referenceIndex];

	v1 = refPoly->u*v1 + refPoly->body->position;
	v2 = refPoly->u*v2 + refPoly->body->position;

	Vector2D sidePlaneNormal = (v2-v1);
	sidePlaneNormal.normalize();

	Vector2D refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

	double refC = dot(refFaceNormal, v1);
	double negSide = -dot(sidePlaneNormal, v1);
	double posSide = dot(sidePlaneNormal, v2);

	if(Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
		return false;

	if(Clip( sidePlaneNormal, posSide, incidentFace) < 2)
	{
		return false;
	}

	m->normal = flip?-refFaceNormal:refFaceNormal;

	int cp = 0;
	double separation = dot(refFaceNormal, incidentFace[0]) - refC;
	if(separation <= 0.0)
	{
		m->contacts[cp] = incidentFace[0];
		m->penetration = -separation;
		++cp;
	}
	else m->penetration = 0;

	separation = dot(refFaceNormal, incidentFace[1] ) - refC;
	if(separation <= 0.0)
	{
		m->contacts[cp] = incidentFace[1];

		m->penetration += -separation;
		++cp;

		m->penetration /= (double)cp;
	}

	m->contact_count = cp;
}

void physicsManifold::initialize()
{
	e = std::min(a->material.restitution, b->material.restitution);

	sf = std::sqrt(a->material.staticFriction*b->material.staticFriction);
	df = std::sqrt(a->material.dynamicFriction*b->material.dynamicFriction);

	for( int i = 0; i < contact_count; ++i)
	{
		Vector2D ra = contacts[i] - a->position;
		Vector2D rb = contacts[i] - b->position;

		Vector2D rv = b->velocity + Cross(b->angularVelocity, rb) - a->velocity - Cross( a->angularVelocity, ra);

		if(rv.lengthSquared() < (dt*gravitationalForce).lengthSquared() + EPSILON)
			e = 0;
	}
}

void physicsManifold::applyImpulse()
{
	if(Equal(a->imass + b->imass, 0))
	{

		infiniteMassCorrection();
		return;
	}

	for(int i = 0; i < contact_count; ++i)
	{
		Vector2D ra = contacts[i] - a->position;
		Vector2D rb = contacts[i] - b->position;

		Vector2D rv = b->velocity + Cross(b->angularVelocity, rb) - a->velocity - Cross( a->angularVelocity, ra);

		double contactVel = dot(rv, normal);

		if(contactVel > 0)
			return;

		double raCrossN = Cross(ra, normal);
		double rbCrossN = Cross(rb, normal);
		double invMassSum = a->imass + b->imass + sqr(raCrossN) * a->iInertia + sqr(rbCrossN) * b->iInertia;

		double j = -(1 + e) * contactVel;
		j/=invMassSum;
		j/= (double)contact_count;

		Vector2D impulse = normal*j;
		a->applyImpulse(-impulse, ra);
		b->applyImpulse(impulse, rb);

		rv = b->velocity + Cross(b->angularVelocity, rb) -
			a->velocity - Cross(a->angularVelocity, ra);

		Vector2D t = rv - (normal* dot(rv, normal));
		t.normalize();
		
		double jt = -dot(rv, t);
		jt/=invMassSum;
		jt/=(double) contact_count;

		if(Equal(jt, 0))
			return;

		Vector2D tangentImpulse;
		if(std::abs(jt) < j*sf)
			tangentImpulse = t*jt;
		else tangentImpulse = t*-j*df;

		a->applyImpulse( -tangentImpulse, ra);
		b->applyImpulse( tangentImpulse, rb);
	}
}

void physicsManifold::positionalCorrection()
{
	const double k_slop = 0.05;
	const double percent = 0.4;
	Vector2D correction = (std::max(penetration - k_slop, 0.0 ) / (a->imass + b->imass)) * normal * percent;
	a->position -= correction * a->imass;
	b->position += correction * b->imass;
}

void physicsManifold::infiniteMassCorrection()
{
	a->velocity.set(0, 0);
	b->velocity.set(0, 0);
}