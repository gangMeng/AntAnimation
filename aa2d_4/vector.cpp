//
// vector.cpp
//
//
//
#include "stdafx.h"
#include "vector.h"

// vars

// functions

Vector_ Vector_::operator +(Vector_ &v)	// return VectorAdd(this, v)
{
	Vector_ temp(x+v.x, y+v.y, z+v.z);
	return temp;
}
void Vector_::operator +=(Vector_ &v)		// this = VectorAdd(this, v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector_ Vector_::operator -(Vector_ &v)	// return VectorSub(this, v)
{
	Vector_ temp(x-v.x, y-v.y, z-v.z);

	return temp;
}
void Vector_::operator -=(Vector_ &v)		// this = VectorSub(this, v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector_ Vector_::operator *(Matrix_ &m)	// return MatrixMulVector(this, m)
{
	return (m * (*this));
}
Vector_ Vector_::operator *(Vector_ &v)	// return VectorScale(this, v)
{
	Vector_ temp(x*v.x, y*v.y, z*v.z);
	return temp;
}
Vector_ Vector_::operator *(float f)		// return VectorScale(this, f)
{
	Vector_ temp(x*f, y*f, z*f);
	return temp;
}
void Vector_::operator *=(Matrix_ &m)		// this = MatrixMulVector(this, m)
{
	*this = *this * m;
}
void Vector_::operator *=(Vector_ &v)		// this = VectorScale(this, v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}
void Vector_::operator *=(float f)		// this = VectorScale(this, v)
{
	x *= f;
	y *= f;
	z *= f;
}

Vector_ Vector_::operator ^(Vector_ &v)	// return VectorCross(this, v)
{
	Vector_ temp;

	temp.x = y*v.z - z*v.y;
	temp.y = z*v.x - x*v.z;
	temp.z = x*v.y - y*v.x;

	return temp;
}
void Vector_::operator ^=(Vector_ &v)		// this = VectorCross(this, v)
{
	Set(y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x);
}

Vector_ Vector_::operator !()
{
	Vector_ temp(-x,-y,-z);
	return temp;
}

void Vector_::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float Vector_::Dot(Vector_ &v)
{
	return x*v.x + y*v.y + z*v.z;
}

float Vector_::SelfDot()
{
	return x*x + y*y + z*z;
}

Vector_ Vector_::Normalize()
{
	Vector_	temp;

	temp = *this * (1.f / sqrtf(SelfDot()));

	return temp;
}


void Vector_::SelfNormalize()
{
	*this *= 1.f / sqrtf(SelfDot());
}

float Vector_::Length()
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector_::Distance(Vector_ &v)
{
	return (*this - v).Length();
}

Vector_ Vector_::Lerp(Vector_ &v, float f)
{
	Vector_ temp;

	temp = (*this * f) + (v * (1.f-f));

	return temp;
}

void Vector_::SelfLerp(Vector_ &v, float f)
{
	*this = (*this * f) + (v * (1.f-f));
}







///////////////

void vmake(Vector_ *a, float x, float y, float z)
{
	a->x = x;
	a->y = y;
	a->z = z;
}

//---

void vadd(Vector_ *a, Vector_ *b, Vector_ *c)
{
	c->x = a->x+b->x;
	c->y = a->y+b->y;
	c->z = a->z+b->z;
}

void vselfadd(Vector_ *a, Vector_ *b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

//---

void vsub(Vector_ *a, Vector_ *b, Vector_ *c)
{
	c->x = a->x-b->x;
	c->y = a->y-b->y;
	c->z = a->z-b->z;
}

void vselfsub(Vector_ *a, Vector_ *b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
}

//---

void vmul(Vector_ *a, Vector_ *b, Vector_ *c)
{
	c->x = a->x*b->x;
	c->y = a->y*b->y;
	c->z = a->z*b->z;
}

void vselfmul(Vector_ *a, Vector_ *b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
}

//---

void vscale(Vector_ *a, float d, Vector_ *c)
{
	c->x = a->x * d;
	c->y = a->y * d;
	c->z = a->z * d;
}

void vselfscale(Vector_ *a, float d)
{
	a->x *= d;
	a->y *= d;
	a->z *= d;
}

//---

void vneg(Vector_ *a, Vector_ *c)
{
	c->x = -a->x;
	c->y = -a->y;
	c->z = -a->z;
}

void vselfneg(Vector_ *a)
{
	a->x *= -1.f;
	a->y *= -1.f;
	a->z *= -1.f;
}

//---

float vdot(Vector_ *a, Vector_ *b)
{
	return( a->x*b->x + a->y*b->y + a->z*b->z );
}

float vselfdot(Vector_ *a)
{
	return ( sqr(a->x) + sqr(a->y) + sqr(a->z) );
}

//---

void vcross(Vector_ *a, Vector_ *b, Vector_ *c)
{
	c->x = a->y*b->z - a->z*b->y;
	c->y = a->z*b->x - a->x*b->z;
	c->z = a->x*b->y - a->y*b->x;
}

//---

void vnorm(Vector_ *a)
{
	float Length, C;

	Length = sqr(a->x) + sqr(a->y) + sqr(a->z);

	if (Length != 0.f)
	{
		C = 1.f / (float)sqrt(Length);
		a->x *= C;
		a->y *= C;
		a->z *= C;
	}
}

//---

float vlen(Vector_ *a)
{
	return (float)sqrt( sqr(a->x) + sqr(a->y) + sqr(a->z) );
}

//---

float vdist(Vector_ *a, Vector_ *b)
{
	float x;
	
	x = sqr(b->x-a->x) + sqr(b->y-a->y) + sqr(b->z-a->z);

	if (x != 0.f)
		return (float)sqrt( x );
	else return 0.f;
}

//---

void vselflerp(Vector_ *a, Vector_ *b, float f)
{
	float	t;
	
	t = 1.f-f;

	a->x = a->x*f + b->x*t;
	a->y = a->y*f + b->y*t;
	a->z = a->z*f + b->z*t;
}

void vlerp(Vector_ *a, Vector_ *b, float f, Vector_ *c)
{
	float	t;
	
	t = 1.f-f;

	c->x = a->x*f + b->x*t;
	c->y = a->y*f + b->y*t;
	c->z = a->z*f + b->z*t;
}