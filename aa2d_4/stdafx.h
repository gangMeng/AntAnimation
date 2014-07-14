// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C0EA25C7_76D1_11D4_B8B1_D0636FC10000__INCLUDED_)
#define AFX_STDAFX_H__C0EA25C7_76D1_11D4_B8B1_D0636FC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <BITMAP.H>
//OpenGL Headers/
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include <gl\glaux.h>
//#include <GL/glext.h>

#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")	// OpenGL32连接库
#pragma comment( lib, "glu32.lib")		// GLu32连接库
#pragma comment( lib, "glaux.lib")		// GLaux连接库

#include <vector>
#include <list>
#include <cmath>
using namespace std;

const double double_max=1.7976000000000001e+308;
const double epsion=1e-5;
const double PI=2*asin(1.0);
const int predefine_x=51;
const int predefine_y=38;
//float g_frameTime;

#define MAP_W       24       // size of map along x-axis 32 
#define MAP_SCALE   24.0f     // the scale of the terrain map
#define MAP			MAP_W*MAP_SCALE/2
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define RAND_COORD(x)   ((float)rand()/RAND_MAX * (x))
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
#define		MAP_SIZE	  1024							// Size Of Our .RAW Height Map (NEW)
#define		STEP_SIZE	  16							// Width And Height Of Each Quad (NEW)
#define		HEIGHT_RATIO  1.5f							// Ratio That The Y Is Scaled According To The X And Z (NEW)
//#define sqr( X )	( (X) * (X) )	                   	// Square the X

inline double sqr(const double a)//平方
{
	return a*a;
}


#include <mmsystem.h>		// for MM timers (you'll need WINMM.LIB)
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C0EA25C7_76D1_11D4_B8B1_D0636FC10000__INCLUDED_)
