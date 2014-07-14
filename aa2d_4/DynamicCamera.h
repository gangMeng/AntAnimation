//
// DynamicCamera.h
//
// DynamicCamera header file
//

#ifndef __DYNAMICCAMERA_H__
#define __DYNAMICCAMERA_H__

// includes
//#include "base.h"

#include "vector.h"
#include "matrix.h"
//#include "aa2d_4View.h"

// defs

// structs

// vars

// funcs/classes
class DynamicCamera
{
public:
	DynamicCamera(Vector_ &vecSrc);
	DynamicCamera();
	~DynamicCamera();

	GLint       viewport[4]; 
	GLdouble    modelview[16]; 
	GLdouble    projection[16];

	void Update();

private:
	Vector_	camSrc, camVel, camAng;
	float	angSpeed, moveSpeed;
	float	camFov, camRoll;
	Matrix_	viewMat;
};

extern float g_frameTime;

#endif //__DYNAMICCAMERA_H__