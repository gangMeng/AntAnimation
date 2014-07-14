// aa2d_4View.h : interface of the Caa2d_4View class
// author:孟志刚  mzg541@126.com
#if !defined(AFX_aa2d_4VIEW_H__C0EA25CD_76D1_11D4_B8B1_D0636FC10000__INCLUDED_)
#define AFX_aa2d_4VIEW_H__C0EA25CD_76D1_11D4_B8B1_D0636FC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <IL/il.h>
#include "baiscobj.h"
#include "GridMap.h"
#include "THormiguita.h"
#include "OpDialog.h"
#include "GLFont.h"
#include "DynamicCamera.h"

static int file_number;                             //截图文件名编号

enum m_state{mFood,	mCave, mScare, mObsta};


class Caa2d_4View : public CView
{
protected: // create from serialization only
	Caa2d_4View();
	DECLARE_DYNCREATE(Caa2d_4View)

// Attributes
public:
	Caa2d_4Doc* GetDocument() const;
	void Asustar(double x, double y);

	TMap *gridmap_;
	CPoint resolution, resolution2;
	bool antrun;
    bool showtree;
	bool camera2;
	bool impostor;

	//static int a;

	baiscobj* m_baiscobj;
	GLuint texture_groud,tex_sky, tex_obs, tex_ant, tex_ant2  ;
	int		m_Time,m_Fram,tim;
	HFONT	hFont,hFont0;//字体
	CGLFont* Font;

	//GLdouble	g_eye[3]={50,0,100};		//眼睛位置
	//GLdouble	g_look[3]={50,50,0};		//视线方向
	//GLdouble	g_fx[3]={0,1,1};
	//static DynamicCamera	*l_pDynCam;
	/*GLint       viewport2[4]; 
	GLdouble    modelview2[16]; 
	GLdouble    projection2[16];*/

	int ant_num;
	//float	g_frameTime;

	vector<THormiguita *> ants;
	vector<TMapObject *> others;
	
	HGLRC m_hRC;	//Rendering Context
	CDC* m_pDC;		//Device Context

	//For elapsed timing calculations
	DWORD m_StartTime, m_ElapsedTime, m_previousElapsedTime;
	GLfloat m_xScaling, m_yScaling, m_zScaling;

	CString m_WindowTitle;	//Window Title

	int DayOfYear;
	int HourOfDay;
    m_state mouseval;
// Operations
public:

// Overrides
public:
	BOOL InitializeOpenGL();	//Initialize OpenGL
	BOOL SetupPixelFormat();	//Set up the Pixel Format
	void RenderScene();			//Render the Scene
	void PostRenderScene();	//Post Render Operations

	Vector screen2world(int x, int y);
	void SnapScreen(LPCSTR filename,int x, int y, int width, int height);

	//void showant(int i, float y);
	bool texturegen(char *filename,GLuint &Texid);
	bool pngtexture(char *filename,GLuint &Texid);
	void	text();		 //显示文字

	//Timing Member Functions
	int FramesPerSecond();	//Calculate the FPS
	DWORD ElapsedTimeinMSSinceLastStartup()
	{return(m_ElapsedTime - m_StartTime);}
	DWORD ElapsedTimeinMSSinceLastRender()
	{return(m_ElapsedTime - m_previousElapsedTime);}

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	/*virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);*/

	
// Implementation
public:
	virtual ~Caa2d_4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(Caa2d_4View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileStart();
public:
	afx_msg void OnFileFood();
public:
	afx_msg void OnFileCave();
public:
	afx_msg void OnFileScare();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnAppAbout();
public:
	afx_msg void OnEditOption();
public:
	afx_msg void OnFileObsta();
	afx_msg void OnViewShowtree();
public:
	afx_msg void OnFileStart32771();
public:
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnViewCamera2();
public:
	afx_msg void OnEditSnap();
};

#ifndef _DEBUG  // debug version in aa2d_4View.cpp
inline Caa2d_4Doc* Caa2d_4View::GetDocument() const
   { return reinterpret_cast<Caa2d_4Doc*>(m_pDocument); }
#endif

extern float g_frameTime;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_aa2d_4VIEW_H__C0EA25CD_76D1_11D4_B8B1_D0636FC10000__INCLUDED_)
