// aa2d_4View.cpp : implementation of the Caa2d_4View class
// 蚁群动画
// 孟志刚 E_mail: mzg541@126.com
//====================================================================

#include "stdafx.h"
//#include "iostream"
#include "aa2d_4.h"
#include <cstring>
#include <time.h>
//#include <IL/il.h>
//#include "BITMAP.H"
#include "aa2d_4Doc.h"
#include "aa2d_4View.h"
#include "vectorlib.h"
#include "DynamicCamera.h"
#include "glm.h"
//#include "3ds.h"

typedef unsigned long UInt32;

using namespace std;

//float g_frameTime;

static DynamicCamera	*l_pDynCam;
//float					 g_frameTime = 0;
UInt32 sys_Time=0;
static LARGE_INTEGER TimeFreq, TimeStart;

GLMmodel* pmodel = NULL;
GLMmodel* pmodel2 = NULL;
GLMmodel* pmodel3 = NULL;
GLMmodel* pmodel4 = NULL;
GLMmodel* pmodel5 = NULL;
GLMmodel* pmodel6 = NULL;
GLMmodel* pmodelf = NULL;
GLMmodel* pmodels = NULL;

GLuint    listAnt1, listAnt2, listAnt3, listAnt4, listAnt5, listAnt6, listFood, listStone;
GLuint    tex_antt[10];

//CLoad3DS g_load3DS;
//t3DModel g_3DModel;

//UINT g_Texture[MAX_TEXTURES];
//GLenum g_ViewMode=  GL_TRIANGLES;	// = GL_TRIANGLES; //GL_POINTS, GL_LINE_STRIP


double tan_beta = 792.0/566.0*tan(27.0*PI/180);     //beta为视角的一半；
double beta = atan(tan_beta)+50.0*PI/180;
double cos_beta = cos(beta);

double texelsize = 2*0.1*tan_beta/800.0;
//float h = 1.8;
//pixelsize = (h*2*0.1/Distance_Proj)/128.0;
double DistanceProj_threshold = 10.5215;           //when pixelsize/texelsize=1.0
double Distance_threshold = DistanceProj_threshold/cos_beta;

//光源和材质
//GLfloat light_pos[ ] = {0.0, 0.0, 0.0, 1.0};
GLfloat mKa[] = {0.11, 0.06, 0.11, 1.00};
GLfloat mKd[] = {0.43, 0.47, 0.54, 1.00};
GLfloat mKs[] = {0.33, 0.33, 0.52, 1.00};
GLfloat mKe[] = {0.0, 0.0, 0.0, 0.0} ;
GLfloat material_Se[] = {10.0};


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//#define BITMAP_ID2   0x4D44    // the universal bitmap ID
/////////////////////////////////////////////////////////////////////////////
// Caa2d_4View

IMPLEMENT_DYNCREATE(Caa2d_4View, CView)

BEGIN_MESSAGE_MAP(Caa2d_4View, CView)
	//{{AFX_MSG_MAP(Caa2d_4View)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_START, Caa2d_4View::OnFileStart)
	ON_COMMAND(ID_FILE_FOOD, &Caa2d_4View::OnFileFood)
	ON_COMMAND(ID_FILE_CAVE, &Caa2d_4View::OnFileCave)
	ON_COMMAND(ID_FILE_SCARE, &Caa2d_4View::OnFileScare)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_APP_ABOUT, &Caa2d_4View::OnAppAbout)
	ON_COMMAND(ID_EDIT_OPTION, &Caa2d_4View::OnEditOption)
	ON_COMMAND(ID_FILE_OBSTA, &Caa2d_4View::OnFileObsta)
	ON_COMMAND(ID_VIEW_SHOWTREE, &Caa2d_4View::OnViewShowtree)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_CAMERA2, &Caa2d_4View::OnViewCamera2)
	ON_COMMAND(ID_EDIT_SNAP, &Caa2d_4View::OnEditSnap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Caa2d_4View construction/destruction

CString	test;    //场景信息
//anmobj* m_anmobj;
//Caa2d_4View::a=0;
Caa2d_4View::Caa2d_4View()
{
	// TODO: add construction code here
	DayOfYear = 1;
	HourOfDay = 1;
	ant_num = 2000;
	antrun = false;
	showtree = false;
	camera2 = false;
	file_number = 0;
	impostor = true;

	m_xScaling = m_yScaling = m_zScaling = 1.0f;
	
	hFont  = CreateFont(-12,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"Arial");
	hFont0 = CreateFont(-48,0,0,0,800,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"黑体");
	m_Fram = 0;  //
	g_frameTime = 0;
	m_Time = timeGetTime();				//
	tim = 0;                              //刷屏速度
	// Initialize our system time.
	QueryPerformanceFrequency(&TimeFreq);
	QueryPerformanceCounter(&TimeStart);
	Font=new CGLFont() ;
	//BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];					// Holds The Height Map Data (NEW)

	//float scaleValue = 0.15f;								// Scale Value For The Terrain (NEW)
}

Caa2d_4View::~Caa2d_4View()
{
}

BOOL Caa2d_4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//An aa2d_4 Window must be created with the following flags
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	//cs.cx = 640; cs.cy = 480;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// Caa2d_4View drawing

void Caa2d_4View::OnDraw(CDC* pDC)
{
	Caa2d_4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	// Get the system time, in milliseconds.
	m_ElapsedTime = ::timeGetTime(); // get current time
	// Get the current time, and update the time controller.s
	LARGE_INTEGER TimeNow;
	QueryPerformanceCounter(&TimeNow);
	sys_Time = (UInt32)((TimeNow.QuadPart-TimeStart.QuadPart)*1000/TimeFreq.QuadPart);
	static float	lastTime = 0.0f;
	float			fTime = sys_Time/1000.f;
	g_frameTime = fTime-lastTime;
	lastTime = fTime;

	if ( ElapsedTimeinMSSinceLastRender() < 30 )
		return;

	// Clear out the color & depth buffers
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glScalef(m_xScaling,m_yScaling,m_zScaling);   //缩放
		//glScalef(0.001, 0.001, 0.001);
		RenderScene();
		
	glPopMatrix();

	// Tell OpenGL to flush its pipeline
	::glFinish();

	// Now Swap the buffers
	::SwapBuffers( m_pDC->GetSafeHdc() );

	//Perform Post Display Processing
	// Only update the title every 15 redraws (this is about
	// every 1/2 second)
	PostRenderScene();

	// the very last thing we do is to save
	// the elapsed time, this is used with the
	// next elapsed time to calculate the
	// elapsed time since a render and the frame rate
	m_previousElapsedTime = m_ElapsedTime;
}

/////////////////////////////////////////////////////////////////////////////
// Caa2d_4View diagnostics

#ifdef _DEBUG
void Caa2d_4View::AssertValid() const
{
	CView::AssertValid();
}

void Caa2d_4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Caa2d_4Doc* Caa2d_4View::GetDocument() const// non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Caa2d_4Doc)));
	return (Caa2d_4Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Caa2d_4View message handlers

int Caa2d_4View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// initialize our dynamic camera
	l_pDynCam = new DynamicCamera( Vector_(MAP, 0, -MAP) );
	gridmap_=new TMap(predefine_x,predefine_y);

	cnst.LOCURA_ANG = 0.2;
	cnst.RADIO_VISION = 10;
	cnst.RADIO_CHOQUE = 3;
	cnst.HORMVELOCITY = 1;
	cnst.TIEMPO_BESO = 0;
	cnst.GRIDS_SCAN = 2;
	cnst.MEMORIA = 5;
	cnst.RADIO_CHOQUE = 10;
	cnst.TIEMPO_SALIDECUEVA = 800;

	// TODO: Add your specialized creation code here
	GetParentFrame()->GetWindowText( m_WindowTitle );

	// get rid of that " - Untitled" stuff
	GetParentFrame()->ModifyStyle(FWS_ADDTOTITLE,0);

	InitializeOpenGL();

	// Now we initialize the animation code
	m_StartTime = ::timeGetTime(); // get time in ms

	// need a previous time if we start off animated
	m_previousElapsedTime = m_StartTime;	

	//Start Timer
	SetTimer(1, 50, NULL);

	return 0;
}

BOOL Caa2d_4View::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//comment out the original call
	//return CView::OnEraseBkgnd(pDC);
	//Tell Windows not to erase the background
	return TRUE;
}

void Caa2d_4View::OnSize(UINT nType, int cx, int cy) 
{
	//if (height==0)										// Prevent A Divide By Zero By
	//	height=1;										// Making Height Equal One
	
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GLdouble aspect_ratio; // width/height ratio
	
	if ( 0 >= cx || 0 >= cy )
	{
		return;
	}
	resolution.x = MAP*2;
	resolution.y = MAP*2;
	resolution2.x = cx;
	resolution2.y = cy;

	gridmap_->fGridSizeX =(double)resolution.x/predefine_x;
	gridmap_->fGridSizeY =(double)resolution.y/predefine_y;

	//texelsize = 2*0.1*tan_beta/cx;

	// select the full client area
    ::glViewport(0, 0, cx, cy);

	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;

	// select the projection matrix and clear it
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();

	// select the viewing volume
	//gluOrtho2D(0, 500.0*aspect_ratio, 0, 500.0);
	//gluOrtho2D(0, cx, 0, cy);
	gluPerspective							// 设置透视图
		( 54.0f,							// 透视角设置为 45 度
		  aspect_ratio,	                    // 窗口的宽与高比
		  0.1f,								// 视野透视深度:近点1.0f
		  3000.0f							// 视野透视深度:始点0.1f远点1000.0f
		);
	// 这和照象机很类似，第一个参数设置镜头广角度，第二个参数是长宽比，后面是远近剪切。
	// switch back to the modelview matrix and clear it
	//gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2],g_fx[0],g_fx[1],g_fx[2]);
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
}

void Caa2d_4View::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	//Make the RC non-current
	if(::wglMakeCurrent (0,0) == FALSE)
	{
		MessageBox("Could not make RC non-current");
	}
	
	//Delete the rendering context
	if(::wglDeleteContext (m_hRC)==FALSE)
	{
		MessageBox("Could not delete RC");
	}
	delete l_pDynCam;
	delete gridmap_;
	for (unsigned int i=0;i<ants.size();++i)
	{
		THormiguita *t=ants[i];
		delete t;
	}
	for (unsigned int i=0;i<others.size();++i)
	{
		TMapObject *t=others[i];
		delete t;
	}
	//Delete the DC
	if(m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
	m_pDC = NULL;
	//Delete pmodel
	/*if (pmodel) glmDelete(pmodel);
	pmodel = NULL;*/
	if (m_baiscobj)
	{
		delete m_baiscobj;
	}
	m_baiscobj = NULL;

	//Delete the timer
	KillTimer(1);
}

BOOL Caa2d_4View::InitializeOpenGL()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);

	//Failure to Get DC
	if(m_pDC == NULL)
	{
		MessageBox("Error Obtaining DC");
		return FALSE;
	}

	//Failure to set the pixel format
	if(!SetupPixelFormat())
	{
		return FALSE;
	}

	//Create Rendering Context
	m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());

	//Failure to Create Rendering Context
	if(m_hRC == 0)
	{
		MessageBox("Error Creating RC");
		return FALSE;
	}
	
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
	{
		MessageBox("Error making RC Current");
		return FALSE;
	}

	//Specify Black as the clear color
	::glClearColor(0.0f,0.0f,0.0f,0.0f);

	//Specify the back of the buffer as clear depth
	::glClearDepth(1.0f);

	//Enable Depth Testing
	::glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	m_baiscobj = new baiscobj();
	m_baiscobj->light0();
	//texturegen("grass01.bmp",tex_sky);
	//texturegen("piedra01.bmp",tex_obs);
	//m_baiscobj->LoadT16("piedra01.bmp",tex_obs);
	//pngtexture("antWithAlpha.png",tex_ant);
  //pngtexture("antmask2.tga",tex_ant);	 
	//m_baiscobj->LoadT16("ant8.bmp",tex_ant);

	if (!pmodel) {
		pmodel = glmReadOBJ("ant.obj");  //i6test.jpg  texture.bmp
		m_baiscobj->LoadT16("texture.bmp",tex_antt[1]);
		if (!pmodel) exit(0);
		glmScale(pmodel,4.0);
		glmUnitize(pmodel);
		glmFacetNormals(pmodel);
		glmVertexNormals(pmodel, 90.0);
		listAnt1 = glmList(pmodel, GLM_MATERIAL | GLM_TEXTURE, tex_antt[1]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodel);
	}
	if (!pmodel2) {
		pmodel2 = glmReadOBJ("ant2.obj");  //i6test.jpg  texture.bmp
		if (!pmodel2) exit(0);
		m_baiscobj->LoadT16("i2test.bmp",tex_antt[2]);
		glmScale(pmodel2,4.0);
		glmUnitize(pmodel2);
		glmFacetNormals(pmodel2);
		glmVertexNormals(pmodel2, 90.0);
		listAnt2 = glmList(pmodel2, GLM_MATERIAL | GLM_TEXTURE, tex_antt[2]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodel2);
	}
	if (!pmodel3) {
		pmodel3 = glmReadOBJ("ant3.obj");  //i6test.jpg  texture.bmp
		if (!pmodel3) exit(0);
		m_baiscobj->LoadT16("i3test.bmp",tex_antt[3]);
		glmScale(pmodel3,4.0);
		glmUnitize(pmodel3);
		glmFacetNormals(pmodel3);
		glmVertexNormals(pmodel3, 90.0);
		listAnt3 = glmList(pmodel3, GLM_MATERIAL | GLM_TEXTURE, tex_antt[3]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodel3);
	}
	if (!pmodel4) {
		pmodel4 = glmReadOBJ("ant4.obj");  //i6test.jpg  texture.bmp
		if (!pmodel4) exit(0);
		m_baiscobj->LoadT16("i4test.bmp",tex_antt[4]);
		glmScale(pmodel4,4.0);
		glmUnitize(pmodel4);
		glmFacetNormals(pmodel4);
		glmVertexNormals(pmodel4, 90.0);
		listAnt4 = glmList(pmodel4, GLM_MATERIAL | GLM_TEXTURE, tex_antt[4]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodel4);
	}
	if (!pmodel5) {
		pmodel5 = glmReadOBJ("ant5.obj");  //i6test.jpg  texture.bmp
		if (!pmodel5) exit(0);
		m_baiscobj->LoadT16("i5test.bmp",tex_antt[5]);
		glmScale(pmodel5,4.0);
		glmUnitize(pmodel5);
		glmFacetNormals(pmodel5);
		glmVertexNormals(pmodel5, 90.0);
		listAnt5 = glmList(pmodel5, GLM_MATERIAL | GLM_TEXTURE, tex_antt[5]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodel5);
	}
	//if (!pmodel6) {
	//	pmodel6 = glmReadOBJ("ant6.obj", "i6test.bmp", 6);  //i6test.jpg  texture.bmp
	//	if (!pmodel6) exit(0);
	//	glmScale(pmodel6,4.0);
	//	glmUnitize(pmodel6);
	//	glmFacetNormals(pmodel6);
	//	glmVertexNormals(pmodel6, 90.0);
	//	listAnt6 = glmList(pmodel6, GLM_MATERIAL | GLM_TEXTURE);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
	//	glmDelete(pmodel6);
	//}
	if (!pmodelf) {
		pmodelf = glmReadOBJ("cookie2.obj");  //i6test.jpg  texture.bmp
		if (!pmodelf) exit(0);
		m_baiscobj->LoadT16("cookie.bmp",tex_antt[7]);
		glmScale(pmodelf,2.0);
		glmUnitize(pmodelf);
		glmFacetNormals(pmodelf);
		glmVertexNormals(pmodelf, 90.0);
		listFood = glmList(pmodelf, GLM_MATERIAL | GLM_TEXTURE, tex_antt[7]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodelf);
	}
	if (!pmodels) {
		pmodels = glmReadOBJ("cave.obj");  //i6test.jpg  texture.bmp Stone_Forest_1
		if (!pmodels) exit(0);
		m_baiscobj->LoadT16("cave2.bmp",tex_antt[8]);
		glmScale(pmodels,3.0);
		glmUnitize(pmodels);
		glmFacetNormals(pmodels);
		glmVertexNormals(pmodels, 90.0);
		listStone = glmList(pmodels, GLM_SMOOTH | GLM_TEXTURE, tex_antt[8]);  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
		glmDelete(pmodels);
	}
	//if (!pmodel2) {
	//	pmodel2 = glmReadOBJ("cookie.obj", "cookietexture01.bmp");
	//	if (!pmodel2) exit(0);
	//	glmScale(pmodel,5.0);
	//	glmUnitize(pmodel2);
	//	glmFacetNormals(pmodel2);
	//	glmVertexNormals(pmodel2, 90.0);
	//	listFood = glmList(pmodel2, GLM_TEXTURE );  //GLM_TEXTURE GLM_MATERIAL GLM_SMOOTH
	//	glmDelete(pmodel2);

	//}
	//光源的位置和属性
	//light_pos = {m_baiscobj->g_look[0], m_baiscobj->g_look[1], m_baiscobj->g_look[2], 1.0};
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	/*m_3ds = new CLoad3DS();
	m_3ds->Init("cookie.3DS",0);*/
	//g_load3DS.CleanUp();	//解决多次打开文件正确显示
	//g_3DModel.CleanUp();
	/*g_load3DS.Import3DS(&g_3DModel, "cookie.3DS");
	g_load3DS.CreateTexture(g_Texture, "cookie.bmp", 0);*/
	
	return TRUE;
}

//Setup Pixel Format
/////////////////////////////////////////////////////////////////////////////
BOOL Caa2d_4View::SetupPixelFormat()
{
  static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |		    // support OpenGL
        PFD_DOUBLEBUFFER,				// double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        16,                             // 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    
    int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if ( m_nPixelFormat == 0 )
	{
       return FALSE;
	}

    if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
    {
       return FALSE;
	}
    
   /* m_baiscobj=new baiscobj();
	m_baiscobj->light0();*/
    return TRUE;
}

void Caa2d_4View::RenderScene ()
{

	if (camera2)
	{
		l_pDynCam->Update();
	}
	else
		m_baiscobj->DisplayScene();
	//l_pDynCam->Update();
	//m_baiscobj->DisplayScene();							//摄像机
	m_baiscobj->CreateSkyBox(3,6,3,6);		     //显示天
	m_baiscobj->DrawSand();						//显示随机山势

	if(showtree) 
	{
		srand(100);//产生树的固定随机数种子
	    for(int i=0;i<50;i++)						//树的数量			
	    {float x= RAND_COORD((MAP_W-1)*MAP_SCALE);	//位置x
	     float z= RAND_COORD((MAP_W-1)*MAP_SCALE);	//位置z
	     float size=4.0f+rand()%4;					//大小2-4随机
	     float h=-size/10;							//深浅
	     int   cactus=rand()%4+11;					//树形随机4种   
	     m_baiscobj->ShowTree(x,z,size,h,cactus);	//显示树
	    }
	}

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);                   

	glDepthFunc(GL_LESS);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); 
	
	for (unsigned int i=0;i<others.size();++i)
	{
		TMapObject *t=others[i];
		if(t->type==type_food)                    //食物
		{
			float y;                                        // 根据地势取模型应在高度
			y = m_baiscobj->GetHeight(t->actual_pos.x, -(t->actual_pos.y));
			

			//glPopAttrib();//恢复前一属性
			//srand(200);                        //产生树的固定随机数种子
			//float size=4.0f+rand()%4;					//大小2-4随机
			//size *= 0.25;
			//float h=-size/10;							//深浅
			//int   cactus=rand()%4+11;					//树形随机4种   
			//m_baiscobj->ShowTree(t->actual_pos.x,t->actual_pos.y,size,h,11);	//显示食物
			glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
			glPushMatrix();
			//glColor3f(0.0, 1.0, 0.0 );
			glTranslatef(t->actual_pos.x, y+1.2, -(t->actual_pos.y));
			//m_3ds->show3ds(0, t->actual_pos.x, y, t->actual_pos.y, 2.0);
			//glutWireSphere(2.0, 10, 8);
			glCallList(listFood);
			//g_3DModel.Show3DModel(g_ViewMode);
			
			glPopMatrix();
			glPopAttrib();//恢复前一属性
		}
		else if(t->type==type_cave)              //巢
		{
			float y;                                        // 根据地势取模型应在高度
			y = m_baiscobj->GetHeight(t->actual_pos.x, -(t->actual_pos.y));

			//srand(400);                        //产生树的固定随机数种子
			//float size=4.0f+rand()%4;					//大小2-4随机
			//size *= 0.25;
			//float h=-size/10;							//深浅
			//int   cactus=rand()%4+11;					//树形随机4种   
			//m_baiscobj->ShowTree(t->actual_pos.x,(t->actual_pos.y),size,h,12);	//显示巢
			glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
			glPushMatrix();
			//glColor3f(0.0, 0.0, 1.0 );
			glTranslatef(t->actual_pos.x, y+1, -(t->actual_pos.y));
			glCallList(listStone);
			//glutWireSphere(3.0, 20, 16);
			
			glPopMatrix();
			glPopAttrib();//恢复前一属性
		}
		else if(t->type==type_obstacle)          //障碍
		{
			float y;                                        // 根据地势取模型应在高度
			y = m_baiscobj->GetHeight(t->actual_pos.x, -(t->actual_pos.y));

			//srand(500);                        //产生树的固定随机数种子
			//float size = 0.4f+rand()%4*0.1f;					//大小2-4随机
			//float h = -size/10;							//深浅
			//int   cactus=rand()%9+15;					//障碍形状随机   
			//m_baiscobj->ShowTree(t->actual_pos.x,(t->actual_pos.y),size,h,cactus);	//显示障碍
			glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
			glPushMatrix();
			glColor3f(1.0, 0.0, 0.0 );
			glTranslatef(t->actual_pos.x, y, -(t->actual_pos.y));

			glutWireCone(1.5, 3, 20, 16);

			glPopMatrix();
			glPopAttrib();//恢复前一属性
		}
	}
	//glColor3f(0.3,0.3,0.3);
	//glPointSize(1);
	if(antrun)
	{
		//antrun=antrun;
		//test
		
		glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
	    glPushMatrix();
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5f);*/
	//	glColor4f(1,1,1,0.5);
		//glColor4f(0,0,0,0);
		/*glBindTexture(GL_TEXTURE_2D, m_baiscobj->tex_ant);
		glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable ( GL_TEXTURE_2D );*/

		//glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);  //_AND_BACK
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
		//glMaterialfv(GL_FRONT, GL_SHININESS, material_Se);
		
		float mat[16];// get the modelview matrix
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		Vector X(mat[0], mat[4], mat[8]);// get the right and up vectors
		Vector Z(mat[1], mat[5], mat[9]); 
		for (unsigned int i=0;i<ants.size();++i)
		{
			//		glPushMatrix();
			//		TRACE("%f\t%f\n",ants[i]->actual_pos.x/(double)resolution.x,ants[i]->actual_pos.y/(double)resolution.y);
			//	  glTranslatef((ants[i]->actual_pos.x/(double)resolution.x)*4-2,(ants[i]->actual_pos.y/(double)resolution.y)*4-2,-5);
			// 绘制蚂蚁
			//float y;                                        // 根据地势取模型应在高度
			//y = m_baiscobj->GetHeight(ants[i]->actual_pos.x, -(ants[i]->actual_pos.y));
			//y = 0.1f;
			//impostor = true;
			
			Vector pos(ants[i]->actual_pos.x,0.0,-(ants[i]->actual_pos.y));   //蚂蚁的位置 
			pos.y = m_baiscobj->GetHeight(ants[i]->actual_pos.x, -(ants[i]->actual_pos.y)) + 1.3 ;//树根深浅 + 1.2
			Vector vel(ants[i]->fVelocity.x, 0.0, (ants[i]->fVelocity.y));  //蚂蚁的速度

			Vector eye(m_baiscobj->g_eye[0],m_baiscobj->g_eye[1],m_baiscobj->g_eye[2]);
			Vector dis(eye-pos);
			double dis_proj = dis * Z;
			double cos_alpha = dis_proj/dis.len();
			double dis2 = dis.len();
			//if (cos_alpha>cos_beta) 
			//{
			//	if ((dis_proj<DistanceProj_threshold)&&(dis.len()<Distance_threshold))
			//	{
			//		impostor = false;   //使用mesh
			//	}
			//}

			if (!impostor)
			{
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_DEPTH_TEST);                   

				glDepthFunc(GL_LESS);

				glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

				glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); 
				//glmUnitize(pmodel, pos.x, pos.y+10, pos.z);
				/*if (!ants[i]->Rotated)
				{
					glPushMatrix();
					glRotatef(180.0, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					ants[i]->Rotated = true;
				}*/
				if (dis2 <= 60.0)
				{
					glPushMatrix();
					glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(pos.x, pos.y, pos.z);
					//glPopMatrix();
					//glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
					glCallList(listAnt1);
					//glmDraw(pmodel, GLM_NONE);
					glPopMatrix();
				}
				if (dis2 > 60.0 && dis2 <= 120.0)
				{
					glPushMatrix();
					glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(pos.x, pos.y, pos.z);
					//glPopMatrix();
					//glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
					glCallList(listAnt2);
					//glmDraw(pmodel, GLM_NONE);
					glPopMatrix();
				}
				if (dis2 > 120.0 && dis2 <= 180.0)
				{
					glPushMatrix();
					glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(pos.x, pos.y, pos.z);
					//glPopMatrix();
					//glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
					glCallList(listAnt3);
					//glmDraw(pmodel, GLM_NONE);
					glPopMatrix();
				}
				if (dis2 > 180.0 && dis2 <= 240.0)
				{
					glPushMatrix();
					glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(pos.x, pos.y, pos.z);
					//glPopMatrix();
					//glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
					glCallList(listAnt4);
					//glmDraw(pmodel, GLM_NONE);
					glPopMatrix();
				}
				if (dis2 > 240.0)
				{
					glPushMatrix();
					glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(pos.x, pos.y, pos.z);
					//glPopMatrix();
					//glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
					glCallList(listAnt5);
					//glmDraw(pmodel, GLM_NONE);
					glPopMatrix();
				}
				if (dis2 > 300.0)
				{
					glPushMatrix();
					glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(pos.x, pos.y, pos.z);
					//glPopMatrix();
					//glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
					glCallList(listAnt6);
					//glmDraw(pmodel, GLM_NONE);
					glPopMatrix();
				}
				//glPushMatrix();
				//glRotatef(ants[i]->rot_ang*180/PI, 0.0f, 1.0f, 0.0f);
				//glPopMatrix();
				//glPushMatrix();
				//glTranslatef(pos.x, pos.y, pos.z);
				////glPopMatrix();
				////glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);  //GLM_NONE
				//glCallList(listAnt);
				////glmDraw(pmodel, GLM_NONE);
				//glPopMatrix();
			}
			
			if (impostor) //impostor 表示
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.5f);
				glBindTexture(GL_TEXTURE_2D, m_baiscobj->tex_ant);
				glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
				glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glEnable ( GL_TEXTURE_2D );
				//Billboard绘制蚂蚁
				Vector vel(ants[i]->fVelocity.x, 0.0, (ants[i]->fVelocity.y));  //蚂蚁的速度
				Vector tmppos, p1, p2, p3, p4;
				tmppos = -vel;
				tmppos.normalize();
				p1 = pos+(X+Z)*-1.2;  
				//p1 = pos;
				p2 = tmppos*2.4 + p1;
				p3 = p2; 
				p3.y += 2.4;
				p4 = p1;
				p4.y += 2.4;
				if((ants[i]->fVengoDe==vdComida)&&(ants[i]->fEstado!=hsComiendo)) {
					glBindTexture(GL_TEXTURE_2D, m_baiscobj->tex_ant2);
					glBegin(GL_QUADS);//显示全部蚂蚁，随运动方向旋转
					glTexCoord2f(0.0,0.0);glVertex3fv(p1.coord);//左下点
					glTexCoord2f(1.0,0.0);glVertex3fv(p2.coord);//右下点
					glTexCoord2f(1.0,1.0);glVertex3fv(p3.coord);//右上点
					glTexCoord2f(0.0,1.0);glVertex3fv(p4.coord);//左上点
					glEnd();
				}
				else {
					glBindTexture(GL_TEXTURE_2D, m_baiscobj->tex_ant);
					glBegin(GL_QUADS);//显示全部蚂蚁，随运动方向旋转
					glTexCoord2f(0.0,0.0);glVertex3fv(p1.coord);//左下点
					glTexCoord2f(1.0,0.0);glVertex3fv(p2.coord);//右下点
					glTexCoord2f(1.0,1.0);glVertex3fv(p3.coord);//右上点
					glTexCoord2f(0.0,1.0);glVertex3fv(p4.coord);//左上点
					glEnd();
				}
				//Billboard绘制结束
				glDisable(GL_BLEND);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_TEXTURE_2D);
			}
			
			//old code
			//glBegin(GL_QUADS);//显示全部蚂蚁
			//glTexCoord2f(0.0,0.0);glVertex3fv((pos+(X+Z)*-1.2).coord);//左下点
			//glTexCoord2f(1.0,0.0);glVertex3fv((pos+(X-Z)* 1.2).coord);//右下点
			//glTexCoord2f(1.0,1.0);glVertex3fv((pos+(X+Z)* 1.2).coord);//右上点
			//glTexCoord2f(0.0,1.0);glVertex3fv((pos+(Z-X)* 1.2).coord);//左上点
			//glEnd();
			
			//QueryPerformanceCounter(&Time1);
			//蚂蚁运动时随速度方向旋转
			
			
						
			/*TRACE("P1: %f\t%f\n", p1.x, p1.z);
			TRACE("P2: %f\t%f\n", p2.x, p2.z);
			TRACE("P3: %f\t%f\n", p3.x, p3.z);	
			TRACE("P4: %f\t%f\n", p4.x, p4.z);*/

		}


		
		glPopMatrix();
	    glPopAttrib();//恢复前一属性
	}
	/*glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(-2,-2,-5);
	glVertex3d(-2,2,-5);
	glVertex3d(2,2,-5);
	glVertex3d(2,-2,-5);
	glVertex3d(-2,-2,-5);
	glEnd();*/

    text();//显示文字
	//SwapBuffers(m_pDC->GetSafeHdc());								 // 切换缓冲区
}

//////////////////////////////////////////////////////////////////////////////
// FramesPerSecond
// fetch frame rate calculations
int Caa2d_4View::FramesPerSecond( void )
{
	double eTime = ElapsedTimeinMSSinceLastRender();

	/*if ( 0 == (int)eTime )
		return 0;*/

	return (int)(1000/eTime);
}

//////////////////////////////////////////////////////////////////////////////
// PostRenderScene
// perform post display processing
//
// The default PostRenderScene places the framerate in the
// view's title. Replace this with your own title if you like.
void Caa2d_4View::PostRenderScene( void )
{
	// Only update the title every 15 redraws (this is about
	// every 1/2 second)
	/*static int updateFrame = 15;

	if (16 > ++updateFrame )
		return;

	updateFrame = 0;*/

	char string[256];
	_snprintf( string, 200, "%s ( %d Frames/sec )",
		(const char*)m_WindowTitle, FramesPerSecond() );

	GetParentFrame()->SetWindowText( string );

}

void Caa2d_4View::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
// 	if(DayOfYear < 365)
// 		DayOfYear++;
// 	else
// 		DayOfYear = 1;
// 
// 	if(HourOfDay < 365)
// 		HourOfDay++;
// 	else
// 		HourOfDay = 1;
	/*clock_t start, finish;
	double duration;
	start = 0;*/
	if(antrun)
	{
	DayOfYear=DayOfYear;
	
	/*start = clock();*/

	for (unsigned int i=0;i<ants.size();++i)
	{ 		
		ants[i]->Run();	
	}
	}

	InvalidateRect(NULL, FALSE);

	OnDraw(m_pDC);
	//finish = clock();
	//duration = (double)(finish - start) / CLOCKS_PER_SEC; //
    //TRACE( "time is %f\n", duration);
	CView::OnTimer(nIDEvent);
}

void Caa2d_4View::OnFileStart()
{
	antrun = true;
    //m_baiscobj->LoadT16("ant8.bmp",tex_ant);
	for (int i=0;i<ant_num;++i)
	{
		THormiguita * th=new THormiguita(gridmap_);

		double ppx = RAND_COORD((MAP_W-1)*MAP_SCALE);
		double ppy = RAND_COORD((MAP_W-1)*MAP_SCALE);

		th->SetPosi(ppx,ppy);
		//		gridmap_->PutObj(th,ppx,ppy);
		ants.push_back(th);
		/*if (i==10)
		{
			TRACE( "Org and Changed rotation angle is %lf,%lf\n", ants[i]->org_ang,ants[i]->chg_ang);
		}*/
	}
	//TComida * st=new TComida();
	//double ppx = resolution.x*drand();
	//double ppy = resolution.y*drand();
	//st->SetPosi(ppx,ppy);
	//gridmap_->PutObj(st,ppx,ppy);
 // others.push_back(st);
	//TCueva * cur=new TCueva();
	//ppx = resolution.x*drand();
	//ppy = resolution.y*drand();
	//cur->SetPosi(ppx,ppy);
	//gridmap_->PutObj(cur,ppx,ppy);
 // others.push_back(cur);
	//antrun=true;
}
void Caa2d_4View::OnFileFood()
{
	// TODO: 在此添加命令处理程序代码
  mouseval = mFood;
}

void Caa2d_4View::OnFileCave()
{
	// TODO: 在此添加命令处理程序代码
  mouseval = mCave;
}

void Caa2d_4View::OnFileScare()
{
	// TODO: 在此添加命令处理程序代码
  mouseval = mScare;
}


void Caa2d_4View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*if((point.x<14.25)||(point.x>914.25))
		return;*/
	/*CString s;
	s.Format("%ld",point.x);*/
	//MessageBox(s, NULL, MB_OK);
//	double m=(resolution.x-resolution.y)/2.0;
//	double x=(point.x-m)/resolution.y;
////	x*=0.5;
//	x=(x)*(resolution.x/2.0-m)*3.0;//+6.5;
//	
//	point.y=resolution.y-point.y;
//	TRACE( "x is %f\n", x);
	
	switch (mouseval)
	{
	case mFood:
	{
		TComida * st = new TComida();
		Vector v;
		v = screen2world(point.x, point.y);
		TRACE( "Food Screen coord is %d,%d\n", point.x, point.y);
		TRACE( "Food World coord is %f,%f,%f\n", v.x, v.y, v.z);
		st->SetPosi(v.x, -(v.z));
		gridmap_->PutObj(st, v.x, -(v.z));
		others.push_back(st);
		break;
	}
	case mCave:
	{
		TCueva * cur = new TCueva();
		Vector v;
		v = screen2world(point.x, point.y);
		TRACE( "Cave Screen coord is %d,%d\n", point.x, point.y);
		TRACE( "Cave World coord is %f,%f,%f\n", v.x, v.y, v.z);
		cur->SetPosi(v.x, -(v.z));
		gridmap_->PutObj(cur, v.x, -(v.z));
		others.push_back(cur);
		//for (int i=0;i<ant_num;++i)
		//{
		//	THormiguita * th=new THormiguita(gridmap_);

		//	/*double r = 5*drand();
		//	double ang = drand()*2*PI;
		//	double ppx = v.x + r*cos(ang);
		//	double ppy = -(v.z) + r*sin(ang);*/
		//	double ppx = RAND_COORD(MAP);
		//	double ppy = RAND_COORD(MAP);

		//	th->SetPosi(ppx,ppy);
		//	//		gridmap_->PutObj(th,ppx,ppy);
		//	ants.push_back(th);
		//	/*if (i==10)
		//	{
		//	TRACE( "Org and Changed rotation angle is %lf,%lf\n", ants[i]->org_ang,ants[i]->chg_ang);
		//	}*/
		//}
		break;
	}
	case mObsta:
	{
		TObstaculo * obs=new TObstaculo();
		Vector v;
		v = screen2world(point.x, point.y);
		obs->SetPosi(v.x, -(v.z));
		gridmap_->PutObj(obs, v.x, -(v.z));
		others.push_back(obs);
		break;
	}
	case mScare:
		Asustar(point.x, point.y);
		break;
	default:
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}

void Caa2d_4View::Asustar(double x, double y)
{
	x/=gridmap_->fGridSizeX;
	y/=gridmap_->fGridSizeY;

	TRACE( "x = %d and y = %d\n", x, y);
	list<TMapObject*> List;
	Vector p(x,y,0), newvel;
	//THormiguita h;
	//int i;
	gridmap_->ObjectsNear(x, y, 3, List);
	//	p = MakeVector(x,y,0);
	list<TMapObject*>::const_iterator pos;
	for(pos=List.begin(); pos!=List.end(); ++pos) {
		if((*pos)->type == type_ant) {
			//h = *pos;
			newvel = (*pos)->actual_pos - p;
			if(newvel.len() != 0) {
				newvel.normalize();
				((THormiguita *)(*pos))->fVelocity = newvel;
			}
		}
	}

} 
void Caa2d_4View::OnAppAbout()
{
	// TODO: 在此添加命令处理程序代码
}

void Caa2d_4View::OnEditOption()
{
	// TODO: 在此添加命令处理程序代码
	COpDialog dlg;
	int ant_newnum;
	dlg.m_count=ants.size();
	dlg.m_madness=cnst.LOCURA_ANG;
	dlg.m_speed=cnst.HORMVELOCITY;
	if (dlg.DoModal()==IDOK)
	{
		ant_newnum=dlg.m_count;
		cnst.LOCURA_ANG=dlg.m_madness;
		cnst.HORMVELOCITY=dlg.m_speed;
    if(ant_newnum<ants.size())
		{
			while (ants.size()>ant_newnum)
			{
				THormiguita *t=ants.back();
				gridmap_->DeleteObject(t);
				delete t;
				ants.pop_back();
			}
// 			for(int i=ant_newnum;i<ant_num;i++)
// 			{
// 				gridmap_->DeleteObject(ants[i]);
//         THormiguita *t=ants[i];
// 				delete t;
// 			}
			ant_num=ant_newnum;
		} else
		{
			for(int i=ants.size();i<ant_newnum;i++)
			{
				THormiguita * th=new THormiguita(gridmap_);

				double ppx = MAP*1.5+rand()%200;
				double ppy = MAP-100+rand()%200;

				th->SetPosi(ppx,ppy);
				//		gridmap_->PutObj(th,ppx,ppy);
				ants.push_back(th);

			}
      ant_num=ant_newnum;
		}
	}
}

void Caa2d_4View::OnFileObsta()
{
	// TODO: 在此添加命令处理程序代码
	mouseval = mObsta;
}

bool Caa2d_4View::texturegen(char *filename,GLuint &Texid)
{
	AUX_RGBImageRec * TextureImage;
	if(!filename)
		return false;
	TextureImage=auxDIBImageLoad(filename);	
	glGenTextures(1,&Texid);
	glBindTexture(GL_TEXTURE_2D,Texid);	// 生成纹理
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	if (TextureImage)					// 如果图像存在
	{
		if (TextureImage->data)			// 如果图像存在
		{
			free(TextureImage->data);	// 释放纹理图像内存
		}
		free(TextureImage);				// 释放纹理结构
	}
	return true;			
}

bool Caa2d_4View::pngtexture(char *filename,GLuint &Texid)
{
	//glEnable(GL_BLEND);
	//ilInit(); /* Initialization of DevIL */
	//ilGenImages(1, &texid); /* Generation of one image name */
	//ilBindImage(texid); /* Binding of image name */
	//success = ilLoadImage(filename);
	//if (success) /* If no error occured: */
	//{
	//	success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
	//																											unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
	//	glGenTextures(1, &Texid); /* Texture name generation */
 //   glBindTexture(GL_TEXTURE_2D, Texid); /* Binding of texture name */
	//	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
	//		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
	//		ilGetData());                                      // Texture specification 
	//}
	///*ilEnable(IL_FILE_OVERWRITE);
	//ilSaveImage("anttest2.png");*/ 
	//ilDeleteImages(1, &texid); 

	return true;
}

//void Caa2d_4View::LoadT16(char *filename, GLuint &texture)//调帖图
//{ glGenTextures(1, &texture);            //获取1个未使用的贴图名称
//glBindTexture(GL_TEXTURE_2D, texture); //选择要绑定的贴图(纹理)
//BITMAPINFOHEADER bitHeader;			 //定义位图结构
//unsigned char *buffer;                 //定义位图指针
//buffer=LoadBitmapFileWithAlpha(filename,&bitHeader); //调入位图
//
//
//glTexImage2D(GL_TEXTURE_2D,
//						 0, GL_RGBA,bitHeader.biWidth,bitHeader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,buffer);
//
//////gluBuild2DMipmaps	( GL_TEXTURE_2D,     // 创建一个 2D贴图(纹理)
//////			  4,                 // 使用3种颜色(RGB)+ 颜色深度
//////			  bitHeader.biWidth, // 图像宽
//////			  bitHeader.biHeight,// 图像高
//////			  GL_RGBA,           // 红绿蓝顺序
//////			  GL_UNSIGNED_BYTE,  // 图像的数据类型是字节
//////			  buffer             // 指定贴图(纹理)数据
//////			);                   // 创建贴图(纹理)
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); //缩小采用三线性滤波
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大采用线性滤波
////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//free(buffer);                                // 释放位图数据
//}


void Caa2d_4View::OnViewShowtree()
{
	// TODO: Add your command handler code here
	showtree = !showtree;
}

void Caa2d_4View::text()//显示文字
{	DWORD Dura =(timeGetTime()-m_Time)/1000;
	if(Dura>0) tim=m_Fram/Dura;
	m_Fram++;
	char str[128];
	sprintf(str, "刷屏: %2d 帧/秒 %s",tim,test);
	Font->settext(350,550,str,hFont,1,1,1.0f);
	//Font->settext(419,258,"+",hFont,1,0,0);
}

//for(int i=0;i<RNSHU;i++)
//{m_anmobj->man[i].qd[0]=MAP*1.5+rand()%200;//MAP+80+rand()%20;	//起点manqdx
//m_anmobj->man[i].qd[1]=MAP-100+rand()%200;//MAP+i*0.5;//-10+i*1;//
//m_anmobj->man[i].zd[0]=MAP*0.5+rand()%30;//MAP+20-rand()%5;	//终点manzdx
//m_anmobj->man[i].zd[1]=MAP-100+rand()%200;//MAP+i*0.5;//-10+i*1;//
//m_anmobj->man[i].dz=1;				//初始动作，跑。
//}

//void Caa2d_4View::ShowAnt(float x,float z,float h,float s,int cactus)//蚂蚁
//{ glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glEnable(GL_ALPHA_TEST);
//glAlphaFunc(GL_GREATER, 0);
//float mat[16];// get the modelview matrix
//glGetFloatv(GL_MODELVIEW_MATRIX, mat);
//vector3_t X(mat[0], mat[4], mat[8]);// get the right and up vectors
//vector3_t Z(mat[1], mat[5], mat[9]); 
//glBindTexture(GL_TEXTURE_2D, g_cactus[cactus]);//选择树
//vector3_t pos(x,0.0,-z);//蚂蚁的位置 
//pos.y = GetHeight(x, -z) + h + s;//树根深浅
//glBegin(GL_QUADS);//显示全部树
//glTexCoord2f(0.0,0.0);glVertex3fv((pos+(X+Z)*-h).v);//左下点
//glTexCoord2f(1.0,0.0);glVertex3fv((pos+(X-Z)* h).v);//右下点
//glTexCoord2f(1.0,1.0);glVertex3fv((pos+(X+Z)* h).v);//右上点
//glTexCoord2f(0.0,1.0);glVertex3fv((pos+(Z-X)* h).v);//左上点
//glEnd();
//glDisable(GL_ALPHA);
//glDisable(GL_BLEND);
//}

//BOOL Caa2d_4View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// TODO: Add your message handler code here and/or call default
//	if(nFlags&MK_MBUTTON) 
//	{ 
//		if(zDelta>0) 
//		{ 
//			m_xScaling += 0.1f; 
//			m_yScaling += 0.1f; 
//			m_zScaling += 0.1f; 
//
//		} 
//		else 
//		{ 
//			m_xScaling -= 0.1f; 
//			m_yScaling -= 0.1f; 
//			m_zScaling -= 0.1f; 
//		} 
//		InvalidateRect(NULL,FALSE); 
//	} 
//
//	return CView::OnMouseWheel(nFlags, zDelta, pt);
//}

Vector Caa2d_4View::screen2world(int x, int y)
{
	/*GLint viewport[4]; 
	GLdouble modelview[16]; 
	GLdouble projection[16]; */
	GLfloat winX, winY, winZ; 
	GLdouble posX, posY, posZ; 

	glMatrixMode(GL_MODELVIEW_MATRIX); 
	glLoadIdentity(); 
	//gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2],0.0,1.0,0.0);

	/*glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
	glGetDoublev(GL_PROJECTION_MATRIX, projection); 
	glGetIntegerv(GL_VIEWPORT, viewport); */

	winX = (float)x;
	if (camera2)
	{
		winY = (float)l_pDynCam->viewport[3] - (float)y;
	}
	else
		winY = (float)m_baiscobj->viewport[3] - (float)y;
	//winY = (float)m_baiscobj->viewport[3] - (float)y;
	glReadPixels(int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 

	if (camera2)
	{
		gluUnProject(winX, winY, winZ, l_pDynCam->modelview, l_pDynCam->projection, l_pDynCam->viewport, &posX, &posY, &posZ);
	}
	else
		gluUnProject(winX, winY, winZ, m_baiscobj->modelview, m_baiscobj->projection, m_baiscobj->viewport, &posX, &posY, &posZ);
	gluUnProject(winX, winY, winZ, m_baiscobj->modelview, m_baiscobj->projection, m_baiscobj->viewport, &posX, &posY, &posZ); 

	Vector v(posX, posY, posZ);
	return v;
}
void Caa2d_4View::OnViewCamera2()
{
	// TODO: Add your command handler code here
	camera2 = !camera2;
}

void Caa2d_4View::SnapScreen(LPCSTR filename,int x, int y, int width, int height) 
{ 
	//CAcModuleResourceOverride res; 
	wglMakeCurrent(m_pDC->GetSafeHdc(),m_hRC); 

	/*if (pDlgPrintSet->m_radiu == 0) 
	{ 

	GLint viewport[4];	
	glGetIntegerv( GL_VIEWPORT, viewport ); 
	width  = viewport[2]; 
	height = viewport[3];	
	width -= width%4;	
	}	*/

	GLubyte * bmpBuffer = NULL; 
	bmpBuffer = (GLubyte *)malloc(width*height*3*sizeof(GLubyte)); 
	if (!bmpBuffer) 
		//return FALSE;	
		;
	glReadPixels((GLint)x, (GLint)y, (GLint)width, (GLint)height, 
		GL_BGR_EXT, GL_UNSIGNED_BYTE, bmpBuffer); 
	//wglMakeCurrent(m_pDC->GetSafeHdc(),NULL);	


	FILE *filePtr; 
	fopen_s(&filePtr, filename, "wb"); 
	if (!filePtr) 
		//return FALSE; 
		;

	BITMAPFILEHEADER bitmapFileHeader; 
	BITMAPINFOHEADER bitmapInfoHeader; 

	bitmapFileHeader.bfType = 0x4D42;	 //"BM" 
	bitmapFileHeader.bfSize = width*height*3; 
	bitmapFileHeader.bfReserved1 = 0; 
	bitmapFileHeader.bfReserved2 = 0; 
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER); 
	bitmapInfoHeader.biWidth = width; 
	bitmapInfoHeader.biHeight = height; 
	bitmapInfoHeader.biPlanes = 1; 
	bitmapInfoHeader.biBitCount = 24; 
	bitmapInfoHeader.biCompression = BI_RGB; 
	bitmapInfoHeader.biSizeImage = 0; 
	bitmapInfoHeader.biXPelsPerMeter = 0; 
	bitmapInfoHeader.biYPelsPerMeter = 0; 
	bitmapInfoHeader.biClrUsed = 0; 
	bitmapInfoHeader.biClrImportant = 0; 

	fwrite(&bitmapFileHeader, sizeof(bitmapFileHeader), 1, filePtr); 
	fwrite(&bitmapInfoHeader, sizeof(bitmapInfoHeader), 1, filePtr); 
	fwrite(bmpBuffer, width*height*3, 1, filePtr); 
	fclose(filePtr);	
	free(bmpBuffer); 

	//return TRUE; 
}

void Caa2d_4View::OnEditSnap()
{
	// TODO: Add your command handler code here
	char temp[10];
	sprintf(temp, "%d", file_number);
	char * prefix = "result";
	char * postfix = ".bmp";
	char file_name[20] = "";
	strcat(file_name, prefix); strcat(file_name, temp); strcat(file_name, postfix);
	SnapScreen(file_name, 0, 0, resolution2.x, resolution2.y);
	file_number++;
}

//不成功的旋转代码
//if (ants[i]->angChanged)
//{
//	//glLoadIdentity();
//	glTranslated(pos.x, pos.y, pos.z);
//	glRotated(ants[i]->chg_ang, 0.0, 1.0, 0.0);
//	glTranslated(-pos.x, -pos.y, -pos.z);
//}
/*Matrix mat;
mat=GenEjeRotationMatrix(Vector(0,1,0), ants[i]->rot_ang);
float m[16] = {mat._11, mat._12, mat._13, mat._14, mat._21, mat._22, mat._23, mat._24, mat._31, mat._32, mat._33, mat._34, mat._41, mat._42, mat._43, mat._44};
float m2[16];
glPushMatrix();
glGetFloatv(GL_MODELVIEW_MATRIX, m2);
glLoadMatrixf(m2);
glMultMatrixf(m);*/
//if (ants[i]->angChanged)
//{
//	//glLoadIdentity();
//	glTranslated(-pos.x, -pos.y, -pos.z);
//	glRotated(ants[i]->chg_ang, 0.0, 1.0, 0.0);
//	glTranslated(pos.x, pos.y, pos.z);
//}

//老的绘制蚂蚁代码
//showant(i,y+1.4f);                     // 显示蚂蚁模型
//glBegin(GL_QUADS);                      //GL_QUADS 
//glTexCoord2d(0, 0); glVertex3d(ants[i]->actual_pos.x+1.0, y, -(ants[i]->actual_pos.y-1.5)); 
//glTexCoord2d(1, 0); glVertex3d(ants[i]->actual_pos.x+1.0, y, -(ants[i]->actual_pos.y+1.5)); 
//glTexCoord2d(1, 1); glVertex3d(ants[i]->actual_pos.x-1.0, y, -(ants[i]->actual_pos.y-1.5)); 
//glTexCoord2d(0, 1); glVertex3d(ants[i]->actual_pos.x-1.0, y, -(ants[i]->actual_pos.y+1.5)); 
//glEnd(); 

//老的绘制障碍物代码
//glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
//glColor3f(1.0f,0.0f,0.0f);
//glPushMatrix();
//glTranslatef(t->actual_pos.x, 0.0f, -(t->actual_pos.y));
//glutSolidSphere(0.1f, 13, 26);
//glEnable(GL_BLEND);
//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

//glEnable(GL_ALPHA_TEST);
//glAlphaFunc(GL_GREATER, 0.5f);
//glBindTexture(GL_TEXTURE_2D, m_baiscobj->tex_obs ); 
//// 绘制障碍物
//glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
////glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
//glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//glEnable ( GL_TEXTURE_2D );
//glBegin(GL_POLYGON); 
//glTexCoord2d(0, 0); glVertex3d(-10.0,-10.0,0.0); 
//glTexCoord2d(1, 0); glVertex3d(-10.0,10.0,-0.0); 
//glTexCoord2d(1, 1); glVertex3d(10.0,10.0,-0.0); 
//glTexCoord2d(0, 1); glVertex3d(10.0,-10.0,-0.0); 
//glEnd(); 
//glDisable(GL_TEXTURE_2D);
//glDisable(GL_BLEND);
//glDisable(GL_ALPHA_TEST);
//glDisable(GL_TEXTURE_2D);
//glPopMatrix();
//glPopAttrib();//恢复前一属性

//老的绘制食物和巢代码
//glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
//glColor3f(0.0f ,1.0f, 0.2f);         //绿色食物
/*glPushMatrix();
glTranslatef(t->actual_pos.x, 0.0f, -(t->actual_pos.y));
glutSolidSphere(0.1f, 13, 26);
glPopMatrix();*/
//glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
//glColor3f(0.0f, 0.0f, 1.0f);
/*glPushMatrix();
glTranslatef(t->actual_pos.x, 0.0f, -(t->actual_pos.y));
glutSolidSphere(0.1f, 13, 26);
glPopMatrix();*/
//glPopAttrib();//恢复前一属性

//	glDepthMask(0); 
// 设置贴图 
//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//glBindTexture(GL_TEXTURE_2D, tex_sky ); 
//// 绘制背景 sky
//glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
//glEnable ( GL_TEXTURE_2D );
//
//glBegin(GL_POLYGON); 
//glTexCoord2d(0, 0); glVertex3d(0.0,0.0,-0.05); 
//glTexCoord2d(1, 0); glVertex3d(0.0,resolution.y,-0.05); 
//glTexCoord2d(1, 1); glVertex3d(resolution.x,resolution.y,-0.05); 
//glTexCoord2d(0, 1); glVertex3d(resolution.x,0.0,-0.05); 
//glEnd(); 
//glDisable(GL_TEXTURE_2D);

//old code!绘制蚂蚁
//glPopMatrix();			
//glColor3f(0.5,0.5,0.5);
//glBegin(GL_POINTS);
//glVertex2d((ants[i]->actual_pos.x+1),resolution.y-(ants[i]->actual_pos.y));
//glVertex2d((ants[i]->actual_pos.x+1+ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+ants[i]->fVelocity.y));
//glVertex2d((ants[i]->actual_pos.x+1+2*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+2*ants[i]->fVelocity.y));
//glEnd();
////glColor3f(1,0,0);
////if (fVengoDe=vdComida)and not(fEstado=hsComiendo) 
//if((ants[i]->fVengoDe==vdComida)&&(ants[i]->fEstado!=hsComiendo))
//{
//	glBegin(GL_POLYGON);
//	glVertex2d((ants[i]->actual_pos.x+1+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+1+2*ants[i]->fVelocity.y));
//	glVertex2d((ants[i]->actual_pos.x+2+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+1+2*ants[i]->fVelocity.y));
//	glVertex2d((ants[i]->actual_pos.x+2+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+2+2*ants[i]->fVelocity.y));
//	glVertex2d((ants[i]->actual_pos.x+1+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+2+2*ants[i]->fVelocity.y));
//	glEnd();
//}
//glColor3f(0,0,0);
//glBegin(GL_POINTS);
//glVertex2d((ants[i]->actual_pos.x),resolution.y-(ants[i]->actual_pos.y));
//glEnd();
//glColor3f(0.34,0.34,0.34);
//glBegin(GL_POINTS);
//glVertex2d((ants[i]->actual_pos.x+ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+ants[i]->fVelocity.y));
//glVertex2d((ants[i]->actual_pos.x+2*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+2*ants[i]->fVelocity.y));
//glEnd();
//if((ants[i]->fVengoDe==vdComida)&&(ants[i]->fEstado!=hsComiendo))
//{
//	glColor3f(1,1,1);
//	glBegin(GL_POLYGON);
//	glVertex2d((ants[i]->actual_pos.x+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+2*ants[i]->fVelocity.y));
//	glVertex2d((ants[i]->actual_pos.x+1+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+2*ants[i]->fVelocity.y));
//	glVertex2d((ants[i]->actual_pos.x+1+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+1+2*ants[i]->fVelocity.y));
//	glVertex2d((ants[i]->actual_pos.x+4*ants[i]->fVelocity.x),resolution.y-(ants[i]->actual_pos.y+1+2*ants[i]->fVelocity.y));
//	glEnd();
//}

//		glutWireSphere(0.1f,20,20);
//		glPopMatrix();

//测量时间的代码
/*LARGE_INTEGER Time1, Time2;
float elapTime, fTime;	*/		
//DWORD mTime, tmpTime, elapTime1, elapTime2, elapTime3, elapTime4;	
//mTime = timeGetTime();
//QueryPerformanceCounter(&Time1);
/*tmpTime = timeGetTime();
elapTime2 = tmpTime - mTime;
mTime = tmpTime;*/
/*tmpTime = timeGetTime();
elapTime4 = tmpTime - mTime;*/
/*QueryPerformanceCounter(&Time2);
fTime  = ((Time2.QuadPart-Time1.QuadPart)/(float)TimeFreq.QuadPart);
static float	lastTime = 0.0f;			
elapTime = fTime-lastTime;
lastTime = fTime;*/
//mTime = tmpTime;
//TRACE("旋转用时: %d\n",elapTime1);