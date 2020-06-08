// This is a class for vector management and calculation.
// It achieves some basic operations between vectors like '+', '-', '*', '/', '+=', etc. 
// And also some basic functions like 'normalize','clamp','dot','cross', etc.

#pragma once
#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <iostream>
#include <cmath>
#include <algorithm>


class Vector3f
{
public:
	float x, y, z;
	Vector3f() : x(0), y(0), z(0) {}
	Vector3f(float xx) : x(xx), y(xx), z(xx) {}
	Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

	Vector3f operator + (const Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
	Vector3f operator - (const Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }
	Vector3f operator - () const { return Vector3f(-x, -y, -z); }
	Vector3f operator * (const Vector3f& v) const { return Vector3f(x*v.x, y*v.y, z*v.z); }
	Vector3f operator * (const float& r) const { return Vector3f(x * r, y * r, z * r); }
	Vector3f operator / (const float& r) const { return Vector3f(x / r, y / r, z / r); }
	friend Vector3f operator * (const float& r, const Vector3f& v)
	{
		return Vector3f(v.x * r, v.y * r, v.z * r);
	}
	Vector3f operator += (const Vector3f& v) { *this = *this + v; return *this; }

	float dot(const Vector3f& v) const { return x * v.x + y * v.y + z * v.z; }
	Vector3f cross(const Vector3f& v) const { return Vector3f(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x); }

	float norm() const { return sqrt(x*x + y * y + z * z); }
	float norm2() const { return x * x + y * y + z * z; }
	void normalize()  
	{
		*this = *this / this->norm();
	}
};

Vector3f normalize(Vector3f v)
{
	return v / v.norm();
}

Vector3f clamp(Vector3f v)
{
	return Vector3f(std::min(v.x, 1.0f), std::min(v.y, 1.0f), std::min(v.z, 1.0f));
}

Vector3f clamp0(Vector3f v)
{
	return Vector3f(std::max(v.x, 0.0f), std::max(v.y, 0.0f), std::max(v.z, 0.0f));
}
Vector3f projection(Vector3f a, Vector3f b)
{
	b.normalize();
	return a.dot(b)*b;
}

#endif // !MY_VECTOR_H
