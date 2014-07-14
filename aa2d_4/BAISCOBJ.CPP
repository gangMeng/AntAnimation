// 蚁群动画
// 孟志刚 E_mail: mzg541@126.com
//====================================================================
#include "stdafx.h"
#include "baiscobj.h"
#include "vectorlib.h"
//#include "anmobj.h"
//#include "bitmap.h"
//extern GLfloat	r;       //飞机盘旋角度
float  gao = 1.8f;
extern CString	test;    //场景信息
//extern anmobj* m_anmobj;
#define GL_CLAMP_TO_EDGE 0x812F
#define BITMAP_ID   0x4D42    // the universal bitmap ID

GLfloat light_Ka[ ] = {0.0, 0.0, 0.0, 1.0};
GLfloat light_Kd[ ] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_Ks[ ] = {1.0, 1.0, 1.0, 1.0};


//////////////////////////////////////////////////////////////////////
baiscobj::baiscobj()
{	g_eye[0]= MAP;//
	g_eye[2]=-MAP;//
	g_Angle=0;//
	g_elev=-0;//
	count = 0;
////////////////////////////////////////////////////////////////////////
	char	appdir[256];
	GetCurrentDirectory(256,appdir);
	CString dir=appdir;
	//if(dir.Right(8)!="运行程序")
	//SetCurrentDirectory("../运行程序");
//	SetCurrentDirectory("h:/");
////////////////////////////////////////////////////////////////////////
	g_imageData = LoadBit("Terrain1.bmp",&g_bit);	//调等高地形图
	LoadT8("sand2.bmp",	 g_cactus[0]);	//地面帖图
	LoadT8("2RBack.bmp",g_cactus[2]);	//天空贴图后
	LoadT8("2Front.bmp",g_cactus[3]);	//天空贴图前
	LoadT8("2Top.bmp",	 g_cactus[4]);	//天空贴图顶
	LoadT8("2Left.bmp", g_cactus[5]);	//天空贴图左
	LoadT8("2Right.bmp",g_cactus[6]);	//天空贴图右
	isBlack = false;
	LoadT16("ant_food2.bmp",g_cactus[11]);    //树1帖图
	LoadT16("ant_cave4.bmp",g_cactus[12]);    //树2帖图
	LoadT16("CACTUS2.BMP",g_cactus[13]);    //树3帖图
	LoadT16("CACTUS3.BMP",g_cactus[14]);    //树4帖图
	LoadT16("piedra01.bmp",g_cactus[15]);   //障碍1贴图
	LoadT16("piedra02.bmp",g_cactus[16]);   //障碍1贴图
	LoadT16("piedra03.bmp",g_cactus[17]);   //障碍1贴图
	LoadT16("piedra04.bmp",g_cactus[18]);   //障碍1贴图
	LoadT16("piedra05.bmp",g_cactus[19]);   //障碍1贴图
	LoadT16("piedra06.bmp",g_cactus[20]);   //障碍1贴图
	LoadT16("piedra07.bmp",g_cactus[21]);   //障碍1贴图
	LoadT16("piedra08.bmp",g_cactus[22]);   //障碍1贴图
	LoadT16("piedra09.bmp",g_cactus[23]);   //障碍9贴图
	InitTerrain(0.8);//初始化地面
	//m_3ds=new CLoad3DS();
 	//load3dobj("data/3ds/","航天发射台.3DS",0);
 	//load3dobj("data/3ds/","直升机0.3ds",1);
 	//load3dobj("data/3ds/","飞机1.3ds",2);

	//LoadT16("images/grab0.BMP", m_anmobj->impostors[0]);    //
	//LoadT16("images/grab1.BMP",m_anmobj->impostors[1]);    //
 //   LoadT16("images/grab2.BMP",m_anmobj->impostors[2]);    //
 //   LoadT16("images/grab3.BMP",m_anmobj->impostors[3]);    //
	//LoadT16("images/grab4.BMP",m_anmobj->impostors[4]);    //
	//LoadT16("images/grab5.BMP",m_anmobj->impostors[5]);    //
 //   LoadT16("images/grab6.BMP",m_anmobj->impostors[6]);    //

// 
// 	LoadT16("data/images/grab0_NRM.BMP", m_anmobj->normalmaps[0]);    //
// 	LoadT16("data/images/grab1_NRM.BMP",m_anmobj->normalmaps[1]);    //
// 	LoadT16("data/images/grab2_NRM.BMP",m_anmobj->normalmaps[2]);    //
// 	LoadT16("data/images/grab3_NRM.BMP",m_anmobj->normalmaps[3]);    //
// 	LoadT16("data/images/grab4_NRM.BMP",m_anmobj->normalmaps[4]);    //
// 	LoadT16("data/images/grab5_NRM.BMP",m_anmobj->normalmaps[5]);    //
// 	LoadT16("data/images/grab6_NRM.BMP",m_anmobj->normalmaps[6]);    //


	
	//m_anmobj->model[0]=NULL;	m_anmobj->model[1]=NULL;
	//m_anmobj->texture[0]=NULL;	m_anmobj->texture[1]=NULL;
	//m_anmobj->getobj("data/模型/士兵0/");					//士兵模型士兵0
	//for(int i=0;i<RNSHU;i++)
	//{m_anmobj->man[i].qd[0]=MAP*1.5+rand()%200;//MAP+80+rand()%20;	//起点manqdx
	// m_anmobj->man[i].qd[1]=MAP-100+rand()%200;//MAP+i*0.5;//-10+i*1;//
	// m_anmobj->man[i].zd[0]=MAP*0.5+rand()%30;//MAP+20-rand()%5;	//终点manzdx
	// m_anmobj->man[i].zd[1]=MAP-100+rand()%200;//MAP+i*0.5;//-10+i*1;//
	// m_anmobj->man[i].dz=1;				//初始动作，跑。
	//}
	
	//LoadT16("piedra01.bmp",tex_obs);
	//isBlack = true;
	LoadT16("ant_new.bmp",tex_ant);
	LoadT16("ant_new3.bmp",tex_ant2);
	glEnable(GL_TEXTURE_2D);
}
baiscobj::~baiscobj()
{	for(int i=0;i<16;i++) glDeleteTextures(1, &g_cactus[i]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
//场景===========================================================
void baiscobj::light0()
{	//GLfloat light_position[] = {1.0,5.0,1.0,1.0};
    //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	GLfloat light_position[] = {g_look[0], g_look[1], g_look[2], 1.0};
	GLfloat dir[] = {g_look[0]-g_eye[0], g_look[1]-g_eye[1], g_look[1]-g_eye[1]};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 30);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}
BOOL baiscobj::DisplayScene()//摄像漫游
{ float speed=0.5f;						//步长
  float x=g_eye[0],y=g_eye[1],z=g_eye[2];
  if (KEY_DOWN(VK_SHIFT))  speed   =speed*2;//按SHIFT时的加速true
  if (KEY_DOWN(VK_LEFT))   g_Angle-=speed*2;//左转,方位角-
  if (KEY_DOWN(VK_RIGHT))  g_Angle+=speed*2;//右转,方位角+
  rad_xz = float (3.13149* g_Angle/180.0f);	//计算左右旋转角度
  if (KEY_DOWN(33))		   g_elev +=speed;	//Page UP  键
  if (KEY_DOWN(34))		   g_elev -=speed;	//Page Down键
  if (g_elev<-360)		   g_elev  =-360;	//仰俯角
  if (g_elev> 360)		   g_elev  = 360;	//仰俯角
  if (KEY_DOWN(VK_UP))						//前进
  { g_eye[2]+=(float)sin(rad_xz)*speed;			//视点的x分量
    g_eye[0]+=(float)cos(rad_xz)*speed;			//视点的Z分量
  }
  if (KEY_DOWN(VK_DOWN))					//后退
  { g_eye[2]-=(float)sin(rad_xz)*speed;			//视点的x分量
    g_eye[0]-=(float)cos(rad_xz)*speed;			//视点的Z分量
  }
  
 //控制到摄像机不离开地面
  if(g_eye[0]<  MAP_SCALE)			g_eye[0]=  MAP_SCALE;
  if(g_eye[0]> (MAP_W-2)*MAP_SCALE)	g_eye[0]= (MAP_W-2)*MAP_SCALE;
  if(g_eye[2]<-(MAP_W-2)*MAP_SCALE)	g_eye[2]=-(MAP_W-2)*MAP_SCALE;
  if(g_eye[2]> -MAP_SCALE)			g_eye[2]= -MAP_SCALE;
  if (KEY_DOWN(VK_HOME))    gao += speed;
  if (KEY_DOWN(VK_END))     gao -= speed;
  g_eye[1] =GetHeight((float)g_eye[0],(float)g_eye[2])+gao;//设置摄像机对地位置高
  //摄像机的方向
  g_look[0] = (float)(g_eye[0] +100*cos(rad_xz));	//目标点X分量
  g_look[2] = (float)(g_eye[2] +100*sin(rad_xz));	//目标点Z分量
  g_look[1] = g_eye[1] +g_elev;				//目标点Y分量
  vector3_t viewdir(g_eye[0]-g_look[0], g_eye[1]-g_look[1], g_eye[2]-g_look[2]);
  viewdir.Normalize();
  //viewdir = -viewdir;
  vector3_t viewpos(g_eye[0], g_eye[1], g_eye[2]), targepos(g_look[0], g_look[1], g_look[2]);

  if (KEY_DOWN('A')) {
	 count--;
  }
  if (KEY_DOWN('Z')) {
	  count++;
  }
  viewpos = viewpos + viewdir*count;
  //建立modelview矩阵方向
  gluLookAt(viewpos.x, viewpos.y, viewpos.z,	//视点
			targepos.x, targepos.y, targepos.z,	//目标点
			0.0,1.0,0						//视点方向
		   );
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
  glGetDoublev(GL_PROJECTION_MATRIX, projection); 
  glGetIntegerv(GL_VIEWPORT, viewport);
  ////////////////////////////////////////////////////////////////
  int r0=abs((int)g_Angle);
  test.Format("[方位=%03d X=%3.0f y=%3.0f 高=%2.1f 俯仰角=%2.0f]",
		  r0%360,g_eye[0],-g_eye[2],g_eye[1],g_elev); 
  //r+=1.0f;if(r>360) r=0;
  return TRUE;
}
//==========================================================================
void baiscobj::InitTerrain(float h)//建立地域数组
{ int index = 0;
  int Vertex;
  for (int z = 0; z < MAP_W; z++)
   for (int x = 0; x < MAP_W; x++)
    { Vertex = z * MAP_W + x;
      g_terrain [Vertex][0] = float(x)*MAP_SCALE;	//地域数据X分量
//      g_terrain [Vertex][1] = (float)(g_imageData[(z*MAP_W+x)*3]/3);//Y分量，山的高度
	  g_terrain [Vertex][1] = h + FRAND * h;		//Y分量，随机产生山的高度
      g_terrain [Vertex][2] = -float(z)*MAP_SCALE;	//地域数据Z分量
      g_texcoord[Vertex][0] = (float) x;			//索引数组x
     g_texcoord[Vertex][1] = (float) z;				//索引数组z 
     g_index [index++] = Vertex;					//顶点数组1维
     g_index [index++] = Vertex+ MAP_W;
   }
  glEnableClientState(GL_VERTEX_ARRAY);				//允许使用地域数组
  glVertexPointer    (3,GL_FLOAT,0,g_terrain);		//装入地域数据
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);		//允许使用索引数组
  glTexCoordPointer  (2,GL_FLOAT,0,g_texcoord);		//装入索引数组
}
void baiscobj::DrawSand()//显示随机山势
{ glBindTexture(GL_TEXTURE_2D, g_cactus[0]);//地贴图
  glTexEnvf    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大采用线性滤波
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  for (int z = 0; z < MAP_W-1; z++)
    glDrawElements(GL_TRIANGLE_STRIP,MAP_W*2,GL_UNSIGNED_INT,&g_index[z*MAP_W*2]);  //GL_TRIANGLE_FAN  GL_TRIANGLE_STRIP
	//显示由顶点数组指定的面。
}
float baiscobj::GetHeight(float x, float z)// 取地面高度
{ 	float CameraX = x/MAP_SCALE;              //计算在那一块
	float CameraZ =-z/MAP_SCALE;              //计算在那一块
	int Col0 = int(CameraX);                  //块的列号
	int Row0 = int(CameraZ);                  //块的行号
	int Col1 = Col0 + 1;                      //相邻列
	int Row1 = Row0 + 1;                      //相邻行
	if (Col1 > MAP_W)	Col1 = 0;             //相邻列大于地块数，取首列
	if (Row1 > MAP_W)	Row1 = 0;             //相邻行大于地块数，取首行
	float h00=g_terrain[Col0 + Row0*MAP_W][1];//获取块四角的高度
	float h01=g_terrain[Col1 + Row0*MAP_W][1];
	float h11=g_terrain[Col1 + Row1*MAP_W][1];
	float h10=g_terrain[Col0 + Row1*MAP_W][1];
	float tx =CameraX - int(CameraX);         //求块内X偏移位置
	float ty =CameraZ - int(CameraZ);         //求块内Z偏移位置
	float txty = tx * ty;                     //以下为双线性插值(内插)计算
	return h00*(1.0f-ty-tx+txty) 
			+ h01*(tx-txty)
			+ h11*txty
			+ h10*(ty-txty);                  //返回插值计算值，为所求点的高度。
}
void baiscobj::CreateSkyBox(int a,int wi,int he,int le)//显示天空
{	float width =MAP*wi;		// 天空盒宽
	float height=MAP*he;		// 天空盒高
	float length=MAP*le;		// 天空盒长
	float x = MAP  -width /2;	// 天空的位置x
	float y = MAP/a-height/2;	// 天空的位置y
	float z = -MAP -length/2;	// 天空的位置z
///////////////////////////////////////////////////////////////////////////////
	texture(g_cactus[2]);		// 设置BACK贴图左
	glBegin(GL_QUADS);			// 多组独立填充四边形	
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();
	texture(g_cactus[3]);		// 设置FRONT贴图右
	glBegin(GL_QUADS);			// 多组独立填充四边形
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();

	texture(g_cactus[4]);		// 设置TOP贴图顶
	glBegin(GL_QUADS);			// 多组独立填充四边形		
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);
	glEnd();
	texture(g_cactus[5]);		// 设置LEFT贴图前
	glBegin(GL_QUADS);			// 多组独立填充四边形
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();
	texture(g_cactus[6]);		// 设置RIGHT贴图后
	glBegin(GL_QUADS);			// 多组独立填充四边形		
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
	glEnd();
}
void baiscobj::texture(UINT textur)//设置贴图滤波
{	glBindTexture  (GL_TEXTURE_2D, textur);// 设置贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); //缩小采用线性滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大采用线性滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}
//组合图形=================================================================
//void baiscobj::picter(float x,float y,float z)//组合图形
//{y=GetHeight(x,z);
// glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
// glDisable(GL_TEXTURE_2D);
// glPushMatrix();//平台==============================
//	glTranslatef(x,y+0.5f,z);		//平台的定位
//	glColor3f(0.0f,1.0f,0.2f);		//绿色
//	auxSolidCube(1);				//方台(边长)
//	glTranslatef(0.0f,0.8f,0.0f);	//架的位置调整,上升0.8
//	glColor3f(0.0f,0.0f,1.0f);		//蓝色
//	auxSolidBox(.2f,1.3f,.2f);		//长方架(宽、高、长)
// glPopMatrix();
// glPushMatrix();//雷达==============================
//	glTranslatef(x,y+2.5f,z);		//雷达的定位1
//	glRotatef(r-90,0.0,1.0,0.0);	//雷达旋转2
//	//=======================================
//	glColor3f(1.0f,1.0f,1.0f);		//白色
//	glRotatef(45, 1.0, 0.0, 0.0);	//盘的角度调整，仰30度
//	auxWireCone(1.5,0.6f);			//线园锥盘(底半径、高)
//	//=======================================
//	glRotatef(180, 1.0, 0.0, 0.0);	//杆的角度调整,反方向转
//	glTranslatef(0.0f,0.0f,-0.7f);  //杆的位置调整,缩进一点
//	auxWireCone(0.2f,2.0f);			//园锥杆(底半径、高)
//	glColor3f(FRAND,0,0);			//随机红色
//	glTranslatef(0.0f,0.0f,2.0f);	//杆的位置调整,缩进一点
//	auxSolidSphere(0.1f);			//园(半径)
// glPopMatrix();
// glPushMatrix();//glPopMatrix();
//	glTranslatef(x,y+30.0f,z);//火箭的定位
//	glRotatef(r, 0.0, 1.0, 0.0);	//火箭的旋转
//	glTranslatef(x/5,0,0);//火箭的定位
//	//=============================================
//	glColor3f(1.0f,0.0f,0.0f);		//红色
//	glRotatef(180, 0.0, 1.0, 0.0);	//角度调整,与雷达平行，箭头朝前
//	auxSolidCone(.2,0.6);			//园锥(底半径、高)
//	//=============================================
//	glColor3f(1.0f,1.0f,1.0f);		//白色
//	glRotatef(90, 1.0, 0.0, 0.0);	//角度调整,与火箭头对接
//	glTranslatef(0.0f,-1.0f,0);		//位置调整,与火箭头对接
//	auxSolidCylinder(.2f,1);		//园柱(半径、高)
//	glRotatef(-270, 1.0, 0.0, 0.0);
//	glColor3f(FRAND+.6f,0.2f,0.0f);	//随机色
//	glTranslatef(0.0f,-0.0f,-0.2f); //位置调整,缩进一点
//	auxSolidCone(.2,1.5);			//园锥(底半径、高)
// glPopMatrix();
// glEnable(GL_TEXTURE_2D);
// glPopAttrib();//恢复前一属性
// r+=1.0f;if(r>360) r=0;
//}
bool baiscobj::LoadT8(char *filename, GLuint &texture)//调8位贴图
{	AUX_RGBImageRec *pImage = NULL;
	pImage = auxDIBImageLoad(filename);	 // 装入位图			
	if(pImage == NULL)	return false;	 // 位图没装入返回错误
	glGenTextures(1, &texture);			 // 生成贴图(纹理)
	glBindTexture    (GL_TEXTURE_2D,texture);// 捆绑贴图(纹理)
	gluBuild2DMipmaps(GL_TEXTURE_2D,4,   //
					  pImage->sizeX,     // 图形宽
					  pImage->sizeY,     // 图形高
					  GL_RGB, GL_UNSIGNED_BYTE,
					  pImage->data       // 图形数据
					 );
	free(pImage->data);                  // 释放位图数据占据的内存资源
	free(pImage);	
	return true;                         // 返回成功
}
void baiscobj::LoadT16(char *filename, GLuint &texture)//调帖图
{ glGenTextures(1, &texture);            //获取1个未使用的贴图名称
  glBindTexture(GL_TEXTURE_2D, texture); //选择要绑定的贴图(纹理)
  BITMAPINFOHEADER bitHeader;			 //定义位图结构
  unsigned char *buffer;                 //定义位图指针
  buffer=LoadBitmapFileWithAlpha(filename,&bitHeader); //调入位图


  glTexImage2D(GL_TEXTURE_2D,
	  0, GL_RGBA,bitHeader.biWidth,bitHeader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,buffer);

  //gluBuild2DMipmaps	( GL_TEXTURE_2D,     // 创建一个 2D贴图(纹理)
		//			  4,                 // 使用3种颜色(RGB)+ 颜色深度
		//			  bitHeader.biWidth, // 图像宽
		//			  bitHeader.biHeight,// 图像高
		//			  GL_RGBA,           // 红绿蓝顺序
		//			  GL_UNSIGNED_BYTE,  // 图像的数据类型是字节
		//			  buffer             // 指定贴图(纹理)数据
		//			);                   // 创建贴图(纹理)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); //缩小采用三线性滤波
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大采用线性滤波
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  free(buffer);                                // 释放位图数据
}
unsigned char * baiscobj::LoadBit(char *filename, BITMAPINFOHEADER *bitmap)
{ FILE *filePtr;                        // 定义文件句柄
  BITMAPFILEHEADER  Header;   // 定义位图指针
  unsigned char    *Image;        // 位图数据区
  unsigned int      imageIdx = 0;       // 图形索引
  unsigned char     tempRGB;            // 交换变量
  filePtr = fopen(filename, "rb");// 读文件
  if (filePtr == NULL)    return NULL;// 读文件出错返回
  fread(&Header, sizeof(BITMAPFILEHEADER), 1, filePtr);
  if (Header.bfType != BITMAP_ID)
  { fclose(filePtr);
    return NULL;
  }
  fread(bitmap, sizeof(BITMAPINFOHEADER), 1, filePtr);
  fseek(filePtr, Header.bfOffBits, SEEK_SET);
  Image = (unsigned char*)malloc(bitmap->biSizeImage);
  if (!Image)
  { free(Image);
    fclose(filePtr);
    return NULL;
  }
  fread(Image, 1, bitmap->biSizeImage, filePtr);//将图形数据读入
  if (Image == NULL)
  { fclose(filePtr);
    return NULL;
  }
  for (imageIdx = 0; imageIdx < bitmap->biSizeImage; imageIdx+=3)
  { tempRGB = Image[imageIdx];
    Image[imageIdx] = Image[imageIdx + 2];
    Image[imageIdx + 2] = tempRGB;
  }
  fclose(filePtr);
  return Image;
}
/////////////////////////////////////////////////////////////////////////////
void baiscobj::ShowTree(float x,float z,float h,float s,int cactus)//树
{ glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);
  float mat[16];// get the modelview matrix
  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
  vector3_t X(mat[0], mat[4], mat[8]);// get the right and up vectors
  vector3_t Z(mat[1], mat[5], mat[9]); 
  glBindTexture(GL_TEXTURE_2D, g_cactus[cactus]);//选择树
  vector3_t pos(x,0.0,-z);//树的位置 
  pos.y = GetHeight(x, -z) + h + s;//树根深浅
  glBegin(GL_QUADS);//显示全部树
	 glTexCoord2f(0.0,0.0);glVertex3fv((pos+(X+Z)*-h).v);//左下点
	 glTexCoord2f(1.0,0.0);glVertex3fv((pos+(X-Z)* h).v);//右下点
	 glTexCoord2f(1.0,1.0);glVertex3fv((pos+(X+Z)* h).v);//右上点
	 glTexCoord2f(0.0,1.0);glVertex3fv((pos+(Z-X)* h).v);//左上点
  glEnd();
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
}

/////////////////////////////////////////////////////////////////////////////
//void baiscobj::ShowObs(float x,float z,float h,float s,int cactus)//障碍
//{ glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glEnable(GL_ALPHA_TEST);
//  glAlphaFunc(GL_GREATER, 0);
//  float mat[16];// get the modelview matrix
//  glGetFloatv(GL_MODELVIEW_MATRIX, mat);
//  vector3_t X(mat[0], mat[4], mat[8]);// get the right and up vectors
//  vector3_t Z(mat[1], mat[5], mat[9]); 
//  glBindTexture(GL_TEXTURE_2D, g_cactus[cactus]);//选择树
//  vector3_t pos(x,0.0,-z);//树的位置 
//  pos.y = GetHeight(x, -z) + h + s;//树根深浅
//  glBegin(GL_QUADS);//显示障碍
//    glTexCoord2f(0.0,0.0);glVertex3fv((pos+(X+Z)*-h).v);//左下点
//    glTexCoord2f(1.0,0.0);glVertex3fv((pos+(X-Z)* h).v);//右下点
//    glTexCoord2f(1.0,1.0);glVertex3fv((pos+(X+Z)* h).v);//右上点
//    glTexCoord2f(0.0,1.0);glVertex3fv((pos+(Z-X)* h).v);//左上点
//  glEnd();
//  glDisable(GL_ALPHA);
//  glDisable(GL_BLEND);
//}

void baiscobj::ShowTree0(float x,float z,float h,float s,int cactus)//树
{//普通平面做的树。
  glPushMatrix();//
  float y = GetHeight(x,-z) + h + s;//树根深浅
  glTranslatef(x,y, -z);//位置
  glRotatef(180, 1.0, 0.0, 0.0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0);
	glBindTexture(GL_TEXTURE_2D, g_cactus[cactus]);//选择树
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//
	glBegin(GL_QUADS);// 开始绘制		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-h, h, 0.0f);		// 右上点
		glTexCoord2f(0.0f, 0.0f); glVertex3f( h, h, 0.0f);		// 右上点
		glTexCoord2f(0.0f, 1.0f); glVertex3f( h,-h, 0.0f);		// 右下点
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-h,-h, 0.0f);		// 左下点
	glEnd();
  glDisable(GL_ALPHA);
  glDisable(GL_BLEND);
  glPopMatrix();
}


////////////////////////////////////////////////////////////////////
//void baiscobj::load3dobj(char* dir,char* cn,int a)
//{	char	appdir[256];
//	GetCurrentDirectory(256,appdir);
//	SetCurrentDirectory(dir);
//	m_3ds->Init(cn,a);
//	SetCurrentDirectory(appdir);
//
//}
//void baiscobj::Scene(int obj,float x,float h,float z,float r,int re,float size)//显示对象
//{	               //序号obj,位置  x,高度  h,位置  z,半径  r,角度re,大小  size
//	 glPushMatrix();//glPopMatrix();
//	 int y=GetHeight(x,z)+h;
//	 glTranslatef(x,y,z);	//飞机定位
//	 glRotatef(re, 0.0, 1.0, 0.0);	//飞机的旋转
//	 if(obj>0) glRotatef(-20, 1.0, 0.0, 0.0);	//飞机的斜角
//	 m_3ds->show3ds(obj,0,0.0f,r,size);//显示3ds模型飞机运行半径的定位
//	 glPopMatrix();
//}

/*****************************************************************************
LoadBitmapFile()

Returns a pointer to the bitmap image of the bitmap specified by filename.
Also returns the bitmap header information. No support for 8-bit bitmaps.
*****************************************************************************/
unsigned char * baiscobj::LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;                        // the file pointer
	BITMAPFILEHEADER  bitmapFileHeader;   // bitmap file header
	unsigned char    *bitmapImage;        // bitmap image data
	unsigned int      imageIdx = 0;       // image index counter
	unsigned char     tempRGB;            // swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
} // end LoadBitmapFile()

/*****************************************************************************
LoadBitmapFileWithAlpha

Loads a bitmap file normally, and then adds an alpha component to use for
blending
*****************************************************************************/
unsigned char * baiscobj::LoadBitmapFileWithAlpha(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	unsigned char *bitmapImage = LoadBitmapFile(filename, bitmapInfoHeader);
	unsigned char *bitmapWithAlpha = (unsigned char *)malloc(bitmapInfoHeader->biSizeImage * 4 / 3);

	if (bitmapImage == NULL || bitmapWithAlpha == NULL)
		return NULL;

	// loop through the bitmap data
	for (unsigned int src = 0, dst = 0; src < bitmapInfoHeader->biSizeImage; src +=3, dst +=4)
	{
		// if the pixel is black, set the alpha to 0. Otherwise, set it to 255.
		if (bitmapImage[src] == 0 && bitmapImage[src+1] == 0 && bitmapImage[src+2] == 0)
			bitmapWithAlpha[dst+3] = 0;
		else
			bitmapWithAlpha[dst+3] = 0xFF;

		// copy pixel data over
		/*bitmapWithAlpha[dst] = bitmapImage[src];
		bitmapWithAlpha[dst+1] = bitmapImage[src+1];
		bitmapWithAlpha[dst+2] = bitmapImage[src+2];*/
		if (isBlack)
		{
			bitmapWithAlpha[dst] = 0;
			bitmapWithAlpha[dst+1] = 0;
			bitmapWithAlpha[dst+2] = 0;
		}
		else
		{
			bitmapWithAlpha[dst] = bitmapImage[src];
			bitmapWithAlpha[dst+1] = bitmapImage[src+1];
			bitmapWithAlpha[dst+2] = bitmapImage[src+2];
		}
	}

	free(bitmapImage);
	//delete bitmapImage;

	return bitmapWithAlpha;
} // end LoadBitmapFileWithAlpha()