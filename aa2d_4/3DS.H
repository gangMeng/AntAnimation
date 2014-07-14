#ifndef _3DS_H
#define _3DS_H
#include <afxtempl.h>
#include <algorithm>
#include <numeric>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>
#include <gl\gl.h>										
#include <gl\glu.h>										
#include <gl\glaux.h>
#include <crtdbg.h>
#include <cassert>
using namespace std;

//  基本块(Primary Chunk)，位于文件的开始
#define PRIMARY       0x4D4D

//  主块(Main Chunks)
#define OBJECTINFO    0x3D3D				// 网格对象的版本号
#define VERSION       0x0002				// .3ds文件的版本
#define EDITKEYFRAME  0xB000				// 所有关键帧信息的头部

//  对象的次级定义(包括对象的材质和对象）
#define MATERIAL	  0xAFFF				// 保存纹理信息
#define OBJECT		  0x4000				// 保存对象的面、顶点等信息

//  材质的次级定义
#define MATNAME       0xA000				// 保存材质名称
#define MATDIFFUSE    0xA020				// 对象/材质的颜色
#define MATMAP        0xA200				// 新材质的头部
#define MATMAPFILE    0xA300				// 保存纹理的文件名

#define OBJECT_MESH   0x4100				// 新的网格对象

//  OBJECT_MESH的次级定义
#define OBJECT_VERTICES     0x4110			// 对象顶点
#define OBJECT_FACES		0x4120			// 对象的面
#define OBJECT_MATERIAL		0x4130			// 对象的材质
#define OBJECT_UV			0x4140			// 对象的UV纹理坐标

#define MAX_TEXTURES 100					// 最大的纹理数目


/*************************************************************************************/

#define PI 3.1415926535
#define eps 1e-07			//容许误差
#define MAX_N 60 			//PCA迭代次数
#define DIMEM 3				//PCA矩阵维度
#define numOfSampVerts 100	//采样点数
#define ANGLE_DIFF 0.2		//对称点裁剪时候的裁剪点角度阈值
#define CUR_DIFF 0.5		//对称点裁剪时候的裁剪点曲率阈值
#define MS_THRESHOLD 1e-2	//meanshift聚类迭代误差阈值
#define MS_ITERTIMES 10		//meanshift聚类迭代次数
#define MS_H		1		//meanShift窗宽
#define MS_PROP	0.3			//meanshift迭代运算中最近邻顶点比例
#define CLUSTER_PROP 0.05	//最终成功聚类中的顶点比例数
#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }


/******************************以下为3D模型格式定义类**********************************/
// 定义3D点的类，用于保存模型中的顶点
class CVector3f{
public:
	CVector3f();
	CVector3f(float, float, float);
	CVector3f(const CVector3f& v);
	CVector3f& operator=(const CVector3f& v);
	bool operator==(const CVector3f& v);
	
	CVector3f operator+(const CVector3f& lhs){
		return CVector3f(x+lhs.x, y+lhs.y, z+lhs.z);
	}
	CVector3f operator+(float num){
		return CVector3f(x+num, y+num, z+num);
	}
	CVector3f operator-(const CVector3f& lhs){
		return CVector3f(x-lhs.x, y-lhs.y, z-lhs.z);
	}
	CVector3f operator-(float num){
		return CVector3f(x-num, y-num, z-num);
	}
	CVector3f operator*(float num){
		return CVector3f(x*num, y*num, z*num);
	}
	CVector3f operator/(float num){
		return CVector3f(x/num, y/num, z/num);
	}

public:
	float x, y, z;
};

// 定义2D点类，用于保存模型的UV纹理坐标
class CVector2 {
public:
	CVector2();
public:
	float x, y;
};

//  面的结构定义
class tFace{
public:
	tFace();
	tFace(int, int, int);
	tFace(const tFace&);
	tFace& operator=(const tFace& face);
public:
	int vertIndex[3];			// 顶点索引
	int coordIndex[3];			// 纹理坐标索引
};

//  材质信息结构体
class tMaterialInfo{
public:
	tMaterialInfo();
public:
	char  strName[255];			// 纹理名称
	char  strFile[255];			// 如果存在纹理映射，则表示纹理文件名称
	BYTE  color[3];				// 对象的RGB颜色
	int   texureId;				// 纹理ID
	float uTile;				// u 重复
	float vTile;				// v 重复
	float uOffset;			    // u 纹理偏移
	float vOffset;				// v 纹理偏移
} ;

//  对象信息结构体
class t3DObject{
public:
	t3DObject();
	~t3DObject();
	void CleanUp();						//??
	t3DObject(const t3DObject& obj);	//?????????
	t3DObject& operator=(const t3DObject& obj);	//?????????
public:
	int  numOfVerts;			// 模型中顶点的数目
	int  numOfFaces;			// 模型中面的数目
	int  numTexVertex;			// 模型中纹理坐标的数目
	int  materialID;			// 纹理ID
	bool bHasTexture;			// 是否具有纹理映射
	char strName[255];			// 对象的名称
	CVector3f  *pVerts;			// 对象的顶点
	tFace *pFaces;				// 对象的面信息

	CVector3f  *pNormals;		// 顶点法向量
	CVector3f  *pFaceNormals;	//面法向量
	CVector2  *pTexVerts;		// 纹理UV坐标
};

//  模型信息结构体
class t3DModel{
public:
	t3DModel();
	~t3DModel();
	void Show3DModel(GLenum mode);
	void CleanUp();
public:
	int numOfObjects;					// 模型中对象的数目
	int numOfMaterials;					// 模型中材质的数目
	vector<tMaterialInfo> pMaterials;	// 材质链表信息
	vector<t3DObject> pObject;			// 模型中对象链表信息
};

/*
//采样后的顶点
class CVector3fEx{
public:
	CVector3fEx(){k1=k2=curvature=0.0;}
public:
	CVector3f vertex;
	vector<CVector3f>nearVert;
	CVector3f vertNormal;
	float k1;
	float k2;
	float curvature;
};

//对称变换得到的特征空间
class SymmetryMap{
public:
	SymmetryMap();
	SymmetryMap(float d, float ss, float rrx, float rry, float rrz, float ttx, float tty, float ttz);
	SymmetryMap(const SymmetryMap&);
	SymmetryMap& operator=(const SymmetryMap& sm);
	SymmetryMap operator+(const SymmetryMap& rhs);
	SymmetryMap operator*(float num);
	SymmetryMap operator/(float num);
	void SetZero();
	void Set(const SymmetryMap&);
	
public:
	int density;
	float s;
	float rx;
	float ry;
	float rz;
	float tx;
	float ty;
	float tz;
};
*/

/***************************以下为3DS文件读取相关类********************************/
// 保存块信息的结构
class tChunk{
public:
	tChunk();
public:
	unsigned short int ID;					// 块的ID		
	unsigned int length;					// 块的长度
	unsigned int bytesRead;					// 需要读的块数据的字节数
};

// CLoad3DS类处理所有的装入代码
class CLoad3DS
{
public:
	CLoad3DS();								// 初始化数据成员
	~CLoad3DS();
	// 装入3ds文件到模型结构中
	bool Import3DS(t3DModel *pModel, CString strFileName);
	void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);
	void Show3DS(GLenum mode);
	void CleanUp();

public:
	bool ImporToDabase(t3DModel *pModel, CString strFileName);

public:
	CString driveName,pathName;
	CString modelName;
	long m_vertNum;
	long m_faceNum;
	t3DObject *Objects[100];

	FILE *m_FilePointer;
	tChunk *m_CurrentChunk;
	tChunk *m_TempChunk;

	//CVector3f *pNormals;
	
	//void ColModelNum(t3DObject *pObject, tChunk *pPreviousChunk);
	bool ColModelNum(t3DModel *pModel,CString strFileName,long& faceNum,long&vertNum);
	bool ColVertsCoordinate(t3DModel *pModel,CString strFileName,float** vertsCoordinate);
	bool ColNormalsCoordinate(t3DModel *pModel,CString strFileName,float** nromalCoordinate);
	bool  ColModel(t3DModel *pModel,CString strFileName);
	bool ColMidleVert(t3DModel *pModel,CString strFileName,float& midVertz);

private:
	// 读一个字符串
	int GetString(char *);
	// 读下一个块
	void ReadChunk(tChunk *);
	// 读下一个块
	void ProcessNextChunk(t3DModel *pModel, tChunk *);
	// 读下一个对象块
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);
	// 读下一个材质块
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);
	// 读对象颜色的RGB值
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);
	// 读对象的顶点
	void ReadVertices(t3DObject *pObject, tChunk *);
	// 读对象的面信息
	void ReadVertexIndices(t3DObject *pObject, tChunk *);
	// 读对象的纹理坐标
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);
	// 读赋予对象的材质名称
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);
	// 计算对象顶点的法向量
	void ComputeNormals(t3DModel *pModel);

};

/************************************************************************/
/* 计算法线等函数                                                                    
/************************************************************************/
//计算矢量长度
float Mag(CVector3f v);

//计算三角形面积，参数为三条边矢量
float TrangleArea(const CVector3f& v1,  const CVector3f& v2, const CVector3f& v3);


// 下面的函数求两点决定的矢量
CVector3f Vector2(const CVector3f& vPoint1, const CVector3f& vPoint2);

// 下面的函数两个矢量相加
CVector3f AddVector(CVector3f& vVector1, CVector3f& vVector2);
// 下面的函数两个矢量相减
CVector3f SubVector(CVector3f& vVector1, CVector3f& vVector2);
// 下面的函数将一个矢量除以一个常数
CVector3f DivideByNum(CVector3f& v, float num);
CVector3f MultiplyByNum(CVector3f& v, float num);
// 下面的函数得到两个矢量的点积
float Dot(CVector3f& v1, CVector3f& v2);
// 下面的函数处理矢量的缩放
CVector3f DivideVectorByScaler(CVector3f& vVector1, float Scaler);
// 下面的函数返回两个矢量的叉积
CVector3f Cross(CVector3f& vVector1, CVector3f& vVector2);
//计算两个矢量之间的夹角[0-180）
float AngleBetweenVector(CVector3f& v1, CVector3f& v2);
//向量取反
CVector3f MinusVector(CVector3f& v);  

// 下面的函数规范化矢量
CVector3f Normalize(CVector3f& vNormal);

/************************************************************************/
//特征提取函数
/************************************************************************/
/*
//雅可比法求矩阵特征值和特征向量
bool JacobiEigen(float a[][DIMEM], float u[][DIMEM]);

//使用PCA方式归一化坐标
bool CoordinateNormalization(t3DModel* pModel);

//顶点采样
bool VertexSampling(t3DModel* pModel, CVector3fEx* pVectEx, int numOfVerts);

//计算顶点处的曲率, pVertex为采样后的顶点集合,均已存好邻接点
void CalculateCurvature(CVector3fEx* pVertex, int pointNum);

//对称点裁剪，对称变换，pVertex是计算好主曲率k1和k2的采样点
bool SymDetection(vector<SymmetryMap> &vSymMap, CVector3fEx* pVertex, int pointNum);

//MeahShift聚类
bool MeanShiftCluster(vector<SymmetryMap> &vSymMap, int num);

//高斯核函数
float GaussFunc(float x);

//两个对称点之间的距离
float Dist(const SymmetryMap& sm1, const SymmetryMap& sm2);
float Dist2(const SymmetryMap& sm1, const SymmetryMap& sm2);
//快排
void swap(float dist[], int v[], int k, int j);
void QuickSort(float dist[], int v[], int left, int right);

//特征空间操作函数
SymmetryMap SMMultiply(const SymmetryMap& sm, float num);

SymmetryMap SMDivede(const SymmetryMap& sm, float num);


class DistNode{
public:
	DistNode():dist(0.0),nearMem(-1){}
	DistNode(const DistNode& src){
		this->dist = src.dist;
		this->nearMem = src.nearMem;
	}
	DistNode& operator=(const DistNode& rhs){
		if(this==&rhs){
			return *this;
		}
		else{
			this->dist = rhs.dist;
			this->nearMem = rhs.nearMem;
			return *this;
		}
	}
	bool operator<(const DistNode& rhs){
		return this->dist < rhs.dist;
	}
	void set(float d, int m){
		this->dist = d;
		this->nearMem = m;
	}

public:
	float dist;
	int nearMem;
};

*/


#endif


