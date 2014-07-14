//
// vector.h
//
// vector header file
//

#ifndef __VECTOR_H__
#define __VECTOR_H__

// includes
//#include "base.h"

class Vector_;

#include "matrix.h"

// structs

// vars

// funcs/classes

class Vector_
{
public:
	float	x, y, z;

	// constructors
	Vector_()								{ Set(0, 0, 0); }
	Vector_(float _x, float _y, float _z)	{ Set(_x, _y, _z); }
	Vector_(Vector_ &v)						{ Set(v.x, v.y, v.z); }

	// operators
	Vector_ operator +(Vector_ &v);	// return VectorAdd(this, v)
	void operator +=(Vector_ &v);	// this = VectorAdd(this, v)

	Vector_ operator -(Vector_ &v);	// return VectorSub(this, v)
	void operator -=(Vector_ &v);	// this = VectorSub(this, v)

	Vector_ operator *(Matrix_ &m);	// return MatrixMulVector(this, m)
	Vector_ operator *(Vector_ &v);	// return VectorMul(this, v)
	Vector_ operator *(float f);		// return VectorScale(this, f)
	void operator *=(Matrix_ &m);	// this = MatrixMulVector(this, m)
	void operator *=(Vector_ &v);	// this = VectorMul(this, v)
	void operator *=(float f);		// this = VectorScale(this, f)

	Vector_ operator ^(Vector_ &v);	// return VectorCross(this, v)
	void operator ^=(Vector_ &v);	// this = VectorCross(this, v)

	Vector_ operator !();			// return VectorNegate(this)

	// functions
	void	Set(float _x, float _y, float _z);
	float	Dot(Vector_ &v);
	float	SelfDot();
	Vector_	Normalize();
	void	SelfNormalize();
	float	Length();
	float	Distance(Vector_ &v);
	Vector_	Lerp(Vector_ &v, float f);
	void	SelfLerp(Vector_ &v, float f);

	float	*GetPtr() { return (float*)this; };
};


#endif //__VECTOR_H__