// THormiguita.h: interface for the THormiguita class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THORMIGUITA_H__A85F0A4E_F4A0_4BB5_8A99_6CDD6C1DCBDF__INCLUDED_)
#define AFX_THORMIGUITA_H__A85F0A4E_F4A0_4BB5_8A99_6CDD6C1DCBDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "tmVectMat.h"
#include "GridMap.h"
#include <queue>
using namespace std;

struct  TConstantes//常量们。。。
{
	RealType	LOCURA_ANG			;
	RealType	RADIO_VISION		;
	RealType	RADIO_CHOQUE		;
	RealType	HORMVELOCITY		;
	int			TIEMPO_BESO			;
	int			GRIDS_SCAN			;
	int			MEMORIA				;
	int			TIEMPO_SALIDECUEVA	;
};

extern TConstantes cnst;

inline double drand()
{
	return (double)rand()/RAND_MAX;
}
inline int random(int a)
{
	return drand()*(a+1);
}
// const RealType LOCURA_ANG = 0.3;
// const RealType RADIO_VISION = 30;
// const RealType RADIO_CHOQUE = 5;
// const RealType HORMVELOCITY  = 1;
// const RealType TIEMPO_BESO = 3;
// const RealType GRIDS_SCAN =2;
// const RealType MEMORIA = 5;
// const RealType TIEMPO_SALIDECUEVA = 800;

//estados
enum hs_state{hsNormal,	hsParada,	hsComiendo	};
enum vd_state{vdNada,	vdComida,	vdCueva		};


// TDrawing	*piedras;
// TDrawing 	sombra;		
// TCoordsArray *masks;
typedef	Vector* PVector;
class TObstaculo:public TMapObject//障碍物
{
public:
	TObstaculo();
	virtual ~TObstaculo()
	{
		;
	}
	void SetPosi(const RealType x,const RealType y)
	{
		actual_pos.x=x;actual_pos.y=y;
	}
	void Draw(/*TDrawing aDrawing*/);
	RealType	fRadio;
protected:
	int			fimg;//no use 原程序中是图片或者纹理
private:
public:
	friend class TMap;
};
class TComida:public TMapObject //食物
{
public:
	TComida()
	{
		type = type_food;
		fCantidad = 2;  //drand()*10+5;
	}
	void SetPosi(const RealType x,const RealType y)
	{
		actual_pos.x=x; actual_pos.y=y; actual_pos.z=0;
		
	}
	virtual ~TComida()
	{
		;
	}
	void Draw(/*TDrawing aDrawing*/);
	RealType	fCantidad;//量

protected:

private:

public:
	friend class TMap;

};
class TCueva:public TMapObject	//巢穴
{
public:
	TCueva()
	{
//		TMapObject();
		type = type_cave;
		fRadio = 3.0;  //0.5
		

		foodStore=0;
	}
	virtual ~TCueva()
	{
		;
	}
	void SetPosi(const RealType x,const RealType y)
	{
		actual_pos.x=x; actual_pos.y=y; actual_pos.z=0;
	}
	void Draw(/*TDrawing aDrawing*/)
	{
/*
		int	x,y,r;
// 		aDrawing.Canvas.Brush.Color=clGray;
// 		aDrawing.Canvas.Pen.Color=clGray;
		x=round(fPosi.x);
		y=round(fPosi.y);
		r=round(fRadio);
// 		aDrawing.Canvas.Ellipse(x-r, y-r, x+r, y+r);
// 		aDrawing.Canvas.Brush.Color:=clBlack;
// 		aDrawing.Canvas.Ellipse(x-r+3, y-r+3, x+r-2, y+r-2);*/

	}
	RealType	fRadio;		//影响范围，即蚂蚁在此范围内能察觉到巢穴的存在与方位


	int			foodStore;//add for debug
protected:
private:
public:
	friend class TMap;
};
class THormiguita:public TMapObject //蚂蚁
{
public:
	THormiguita(TMap *aMap);	//Tmap 是网格管理者
	virtual ~THormiguita()
	{
		;
	}
	void SetPosi(const RealType x,const RealType y)
	{
		actual_pos.x=x; actual_pos.y=y; actual_pos.z=0;
		fMap->PutObj(this,x,y);
	}
	virtual void Draw(/*TDrawing aDrawing*/);

	TMap	*fMap;
	queue<Vector>	fPosiMemory;	//记录蚂蚁的昔日足迹
	Vector	fVelocity;			//速度
	RealType	fVelocityModule;	//不知道。速度的长度？
	hs_state	fEstado;		//状态
	int		fParadaCount;
	int		fComiendoCount;
	THormiguita	*	fUltimaConQuienChoque;
	THormiguita	*	fBesandomeCon;
	vd_state		fVengoDe;//状态
	int		fTiempoQueVengoDe;
	int		fMenorTiempoQueMeDigeron;
	bool	fCheckit;
	Vector	fCheckitColor;
	RealType org_ang, chg_ang;
	RealType rot_ang;
	bool    angChanged;
	bool    first;
	bool Rotated;
protected:
private:

protected:
	Vector	PosicionRecordada();  //蚂蚁的昔日足迹
	void	Mover();		//移动
	void	Locura();		//赋予随机方向
	void	Interaccion();

	void	InteracOtraHormiga(THormiguita *obj);
	void	InteracObstaculo(TObstaculo		*obs);
	void	InteracComida(TComida	*obs);
	void	InteracCueva(TCueva	*obs);
public:
	void Run();

	void CheckIt(); //unknow use
//	Vector posi();
//	Vector Velocity();
	bool IsCheckIt();
	friend class TMap;

};
		   

#endif // !defined(AFX_THORMIGUITA_H__A85F0A4E_F4A0_4BB5_8A99_6CDD6C1DCBDF__INCLUDED_)
