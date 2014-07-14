//
// matrix.h
//
// matrix header file
//

#ifndef __MATRIX_H__
#define __MATRIX_H__

// includes
//#include "base.h"

class Matrix_;

#include "vector.h"

// defs
enum {_XX = 0, _XY, _XZ, _YX, _YY, _YZ, _ZX, _ZY, _ZZ};

// structs

// vars

// funcs/classes

class Matrix_
{
public:
	float	p[9];

	// constructors
	Matrix_()								{ memset(p,0,sizeof(float)*9); }					// nullify matrix
	Matrix_(	float xx, float xy, float xz,
			float yx, float yy, float yz,
			float zx, float zy, float zz)	{ Set(xx, xy, xz,  yx, yy, yz,  zx, zy, zz); }		// construct matrix from 9 floats
	Matrix_(float ax, float ay, float az)	{ SetAng(ax, ay, az); };							// construct matrix form 3 angles
	Matrix_(Matrix_ &m)						{ memcpy(p, m.p, sizeof(float)*9); }				// construct matrix from another matrix


	// operators
	Matrix_ operator *(Matrix_ &m);	// return MatrixMulMatrix(this, m)
	Vector_ operator *(Vector_ &v);	// return MatrixMulVector(this, v)
	Matrix_ operator *(float f);		// return MatrixScale(this, f)
	void operator *=(Matrix_ &m);	// this = MatrixMulMatrix(this, m)
	void operator *=(float f);		// this = MatrixScale(this, f)

	Matrix_ operator !();			// return MatrixInverse(this)

	// functions
	void Set(	float xx, float xy, float xz,
				float yx, float yy, float yz,
				float zx, float zy, float zz);	// set teh matrix's values directly
	void SetAng(float ax, float ay, float az);	// specify a xyz rotation matrix
	void Identity();							// set this matrix to identity
	void Invert();								// invert this matrix
};



#endif //__MATRIX_H__
