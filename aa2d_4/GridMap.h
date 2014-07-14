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
class TGridList //��������
{
public:
    TGridList(){};
    void Add	(TMapObject *obj);//������
    void Delete	(TMapObject *obj);//ɾ����
    set<TMapObject*> fList;		//ʵ�ʴ洢���������
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
class TMap //�������
{
public:
    TMap(int aWidth, int aHeight);
	TMap()
	{
		fMap=NULL;
		fWidth=fHeight=0;
	}
    ~TMap();

	bool haveobj(RealType x,RealType y);//��������
    void SetBackGround(string aFileName);//��������
    void Draw();//��������
    void Show(HDC aDC);//��������
    void PutObj(TMapObject* obj, RealType x, RealType y);//����������x,y
    bool Puedo(RealType x, RealType y);//�Ƿ񳬳��߽�
    void ObjectsNear(TMapObject *obj, int Grids, list<TMapObject*> &List);//���ٽ�obj��Grids��Χ�ڵ������ҳ���ָ�����List��
    void DeleteObject(TMapObject *obj);//ɾ������
    int get(TDrawing Fondo);//no use here
	void ObjectsNear(RealType ax, RealType ay, int Grids, list<TMapObject*> &List);//���ٽ�x,y��Grids��Χ�ڵ������ҳ���ָ�����List��
    void Resize(int NewWidth, int NewHeight);//���µ��������С


	void getobjs(int x,int y,vector<pos_and_type> &pos_and_types);
	
    RealType fGridSizeX, fGridSizeY;//no use
	
	int fWidth, fHeight;//�����С
protected:
    TGridList* fMap;//��������
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
