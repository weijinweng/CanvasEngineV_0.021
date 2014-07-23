#ifndef VECTOR_H
#define VECTOR_H
#include <cmath>
#include <cassert>
#include <algorithm>

const double PI = 3.141592741;
const double EPSILON = 0.0001;

class Vector2D{
public:
	Vector2D();
	Vector2D(double x, double y);
	double x,y;
	static double dot(Vector2D x1, Vector2D x2);
	static Vector2D scale(Vector2D vector, double scale);
	Vector2D operator- (Vector2D other) const;
	Vector2D operator+ (Vector2D other) const;
	Vector2D operator+ (double s) const;
	void operator+= (Vector2D other);
	void operator-= (Vector2D other);
	Vector2D operator* (double other)
	{
		Vector2D vector;
		vector.x = other * x;
		vector.y = other * y;
		return vector;
	}
	Vector2D operator/ (double other)
	{
		Vector2D vector;
		vector.x = x/other;
		vector.y = y/other;
		return vector;
	}
	void operator*= (double other)
	{
		x *= other;
		y *= other;
	}
	void operator/= (double other)
	{
		x /= other;
		y /= other;
	}
	Vector2D operator- (void) const
	{
		return Vector2D(-x, -y);
	}
	double lengthSquared();
	double lengthRooted();
	void normalize()
	{
		double length = lengthRooted();

		if(length > EPSILON)
		{
			double invLength = 1/length;
			x *= invLength;
			y *= invLength;
		}
	}
	void set(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
};

inline Vector2D operator* (double other, Vector2D vec)
{
	Vector2D vector;
	vector.x = vec.x * other;
	vector.y = vec.y * other;
	return vector;
}

inline Vector2D operator/ (double other, Vector2D vec)
{
	Vector2D vector;
	vector.x = vec.x/other;
	vector.y = vec.y/other;
	return vector;
}

class mat22{
public:
	double m00, m01, m10, m11;
	mat22()
	{}
	mat22(double a, double b, double c, double d)
	{
		m00 = a;
		m01 = b;
		m10 = c;
		m11 = d;
	}

	void set(double radians)
	{
		double c = std::cos(radians);
		double s = std::sin(radians);

		m00 = c; m01 = -s;
		m10 = s; m11 = c;
	}

	mat22 abs()
	{
		return mat22(std::abs(m00), std::abs(m01), std::abs(m10), std::abs(m11));
	}

	Vector2D axisX()
	{
		return Vector2D(m00, m10);
	}

	Vector2D axisY()
	{
		return Vector2D(m01, m11);
	}

	mat22 transpose()
	{
		return mat22(m00, m10, m01, m11);
	}

	const Vector2D operator*(const Vector2D rhs)
	{
		return Vector2D(m00 * rhs.x + m01 * rhs.y, m10 * rhs.x + m11 * rhs.y);
	}

	const mat22 operator*(const mat22 rhs)
	{
		return mat22(
			m00 * rhs.m00 + m01 * rhs.m10,
			m00 * rhs.m01 + m01 * rhs.m11,
			m10 * rhs.m00 + m11 * rhs.m10,
			m10 * rhs.m01 + m11 * rhs.m11);
	}
};

Vector2D min(Vector2D a, Vector2D b);

Vector2D max(Vector2D a, Vector2D b);

double dot(Vector2D a, Vector2D b);

double distSquared(Vector2D a, Vector2D b);

Vector2D Cross(Vector2D vec, double a);

Vector2D Cross(double a, Vector2D vec);

double Cross(Vector2D a, Vector2D b);

bool Equal(double a, double b);
double sqr(double a);

double clamp(double a, double max, double min);
int round(int a);

double random(double l, double h);

bool biasGreaterThan(double a, double b);

#endif