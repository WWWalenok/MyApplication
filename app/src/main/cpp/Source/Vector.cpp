#include "../Include/Vector.hpp"
#include <cmath>

using namespace std;

#define Normalize3_M(Vector)\
{ \
	float r = Vector[0] * Vector[0] + Vector[1] * Vector[1] + Vector[2] * Vector[2];\
	union {float f; uint32_t i; } conv = { r };\
	conv.i = 0x5f3759df - (conv.i >> 1);\
	conv.f *= 1.5F - r * 0.5F * conv.f * conv.f;\
	Vector[0] *= conv.f;\
	Vector[1] *= conv.f;\
	Vector[2] *= conv.f;\
} \

#define Normalize3(Vector)\
{ \
	float r = Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z;\
	union {float f; uint32_t i; } conv = { r };\
	conv.i = 0x5f3759df - (conv.i >> 1);\
	conv.f *= 1.5F - r * 0.5F * conv.f * conv.f;\
	Vector.x *= conv.f;\
	Vector.y *= conv.f;\
	Vector.z *= conv.f;\
} \

#define Normalize2_M(Vector)\
{ \
	float r = Vector[0] * Vector[0] + Vector[1] * Vector[1];\
	union {float f; uint32_t i; } conv = { r };\
	conv.i = 0x5f3759df - (conv.i >> 1);\
	conv.f *= 1.5F - r * 0.5F * conv.f * conv.f;\
	Vector[0] *= conv.f;\
	Vector[1] *= conv.f;\
} \


#define Normalize2(Vector)\
{ \
	float r = Vector.x * Vector.x + Vector.y * Vector.y;\
	union {float f; uint32_t i; } conv = { r };\
	conv.i = 0x5f3759df - (conv.i >> 1);\
	conv.f *= 1.5F - r * 0.5F * conv.f * conv.f;\
	Vector.x *= conv.f;\
	Vector.y *= conv.f;\
} \

float Q_rsqrt(float number)
{
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;
	union {
		float f;
		uint32_t i;
	} conv = { number }; // member 'f' set to value of 'number'.
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= threehalfs - x2 * conv.f * conv.f;
	return conv.f;
}

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3::Vector3(double* val)
{
	if (val != nullptr)
	{
		x = val[0];
		y = val[1];
		z = val[2];
	}
	else
	{
		x = 0;
		y = 0;
		z = 0;
	}
}

Vector3 Vector3::operator +(const Vector3& a) { return Vector3(a.x + x, a.y + y, a.z + z); }

Vector3 Vector3::operator -(const Vector3& a) { return Vector3(x - a.x, y - a.y, z - a.z); }

Vector3 Vector3::operator *(double a) { return Vector3(x * a, y * a, z * a); }

double Vector3::operator *(const Vector3& a) { return a.x * x + a.y * y + a.z * z; }

Vector3 Vector3::operator /(const Vector3& a) { return Vector3(y * a.z - z * a.y, -x * a.z + z * a.x, x * a.y - y * a.x); }

Vector3 Vector3::operator /(double a) { return Vector3(x / a, y / a, z / a); }

double Vector3::GetLenght()
{
	return 1 / Q_rsqrt(x * x + y * y + z * z);
}

void Vector3::SetLenght(double d)
{
	d = d * Q_rsqrt(x * x + y * y + z * z);
	x *= d;
	y *= d;
	z *= d;
}


Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(double _x, double _y)
{
	x = _x;
	y = _y;
}

Vector2 Vector2::operator +(const Vector2& a) { return Vector2(a.x + x, a.y + y); }

Vector2 Vector2::operator -(const Vector2& a) { return Vector2(a.x - x, a.y - y); }

Vector2 Vector2::operator *(double a) { return Vector2(x * a, y * a); }

double Vector2::operator *(const Vector2& a) { return a.x * x + a.y * y; }

double Vector2::operator /(const Vector2& a) { return x * a.y - y * a.x; }

Vector2 Vector2::operator /(double a) { return Vector2(x / a, y / a); }

Vector2 Vector2::operator !() { return Vector2(y, -x); }

double Vector2::GetLenght()
{
	return 1 / Q_rsqrt(x * x + y * y);
}

void Vector2::SetLenght(double d)
{
	d = d * Q_rsqrt(x * x + y * y);
	x *= d;
	y *= d;
}

Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Quaternion::Quaternion(double _x, double _y, double _z, double _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Quaternion Quaternion::operator +(const Quaternion& a) { return Quaternion(a.x + x, a.y + y, a.z + z, a.w + w); }

Quaternion Quaternion::operator -(const Quaternion& a) { return Quaternion(a.x - x, a.y - y, a.z - z, a.w - w); }

Quaternion Quaternion::operator *(double a) { return Quaternion(x * a, y * a, z * a, w * a); }

Quaternion Quaternion::operator *(const Quaternion& a) { return Quaternion(w * a.w - (x * a.x + y * a.y + z * a.z), w * a.x + x * a.w + y * a.z - z * a.y, w * a.y + y * a.w + z * a.x - x * a.z, w * a.z + z * a.w + x * a.y - y * a.x); }

Quaternion Quaternion::operator *(const Vector3& a) { return Quaternion(-(x * a.x + y * a.y + z * a.z), w * a.x + y * a.z - z * a.y, w * a.y + z * a.x - x * a.z, w * a.z + x * a.y - y * a.x); }

//Quaternion Quaternion::operator /(const Quaternion &a) { return *this * ~a; }

Vector3 Quaternion::operator &(const Vector3& a) { Quaternion q = (*this * a) * ~*this; return Vector3(q.x, q.y, q.z); }

Quaternion Quaternion::operator /(double a) { return Quaternion(x / a, y / a, z / a, w / a); }

Quaternion Quaternion::operator !() { return Quaternion(-x, -y, -z, w); }

Quaternion Quaternion::operator ~() { double d = 1 / (x * x + y * y + z * z + w * w); return Quaternion(-x * d, -y * d, -z * d, w * d); }

double Quaternion::GetLenght()
{

	return sqrt(x * x + y * y + z * +w * w);

}

void Quaternion::SetLenght(double d)
{
	d = d / GetLenght();
	x *= d;
	y *= d;
	z *= d;
	w *= d;
}
