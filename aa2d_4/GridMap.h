// GridMap.h: interface for the GridMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDMAP_H__57CA5E85_879E_4340_B075_FC5AB78912AB__INCLUDED_)
#define AFX_GRIDMAP_H__57CA5E85_879E_4340_B075_FC5AB78912AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>
#include <set>
#include <list>
#include "tmVectMat.h"
using namespace std;
struct TPoint
{
	TPoint() {}
	TPoint(int _x, int _y) : x(_x), y(_y) {}
	int  x;
	int  y;
	
};
typedef int TDrawing;
enum objtype{type_ant,type_food,type_cave,type_obstacle};
class TMapObject 
{
public:
	TMapObject()
	{
		type=type_ant;
		grid_pos.x=-1;grid_pos.y=-1;
	}
//    map();
    virtual void Draw()=0;
    objtype type;
    TPoint grid_pos;
	Vector actual_pos;
	virtual ~TMapObject()
	{
		;
	}
	
protected:
};
class TGridList //单个网格
{
public:
    TGridList(){};
    void Add	(TMapObject *obj);//加物体
    void Delete	(TMapObject *obj);//删物体
    set<TMapObject*> fList;		//实际存储物体的容器
protected:
};
struct pos_and_type
{
	pos_and_type(Vector poss,objtype typee)
	{
		pos=poss;type=typee;
	}
	Vector pos;
	objtype type;
};
class TMap //网格管理
{
public:
    TMap(int aWidth, int aHeight);
	TMap()
	{
		fMap=NULL;
		fWidth=fHeight=0;
	}
    ~TMap();

	bool haveobj(RealType x,RealType y);//废弃不用
    void SetBackGround(string aFileName);//废弃不用
    void Draw();//废弃不用
    void Show(HDC aDC);//废弃不用
    void PutObj(TMapObject* obj, RealType x, RealType y);//放入物体于x,y
    bool Puedo(RealType x, RealType y);//是否超出边界
    void ObjectsNear(TMapObject *obj, int Grids, list<TMapObject*> &List);//将临近obj的Grids范围内的物体找出，指针放入List中
    void DeleteObject(TMapObject *obj);//删除物体
    int get(TDrawing Fondo);//no use here
	void ObjectsNear(RealType ax, RealType ay, int Grids, list<TMapObject*> &List);//将临近x,y的Grids范围内的物体找出，指针放入List中
    void Resize(int NewWidth, int NewHeight);//重新调整网格大小


	void getobjs(int x,int y,vector<pos_and_type> &pos_and_types);
	
    RealType fGridSizeX, fGridSizeY;//no use
	
	int fWidth, fHeight;//网格大小
protected:
    TGridList* fMap;//网格数组
    //int fWidth, fHeight;
    TDrawing fFondo, fScr;//no use
    void RestoreBackGround();//no use
};

class GridMap  //no use
{
public:
	GridMap();
	virtual ~GridMap();

};

#endif // !defined(AFX_GRIDMAP_H__57CA5E85_879E_4340_B075_FC5AB78912AB__INCLUDED_)
