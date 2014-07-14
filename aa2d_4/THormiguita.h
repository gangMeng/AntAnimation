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

struct  TConstantes//�����ǡ�����
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
class TObstaculo:public TMapObject//�ϰ���
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
	int			fimg;//no use ԭ��������ͼƬ��������
private:
public:
	friend class TMap;
};
class TComida:public TMapObject //ʳ��
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
	RealType	fCantidad;//��

protected:

private:

public:
	friend class TMap;

};
class TCueva:public TMapObject	//��Ѩ
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
	RealType	fRadio;		//Ӱ�췶Χ���������ڴ˷�Χ���ܲ������Ѩ�Ĵ����뷽λ


	int			foodStore;//add for debug
protected:
private:
public:
	friend class TMap;
};
class THormiguita:public TMapObject //����
{
public:
	THormiguita(TMap *aMap);	//Tmap �����������
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
	queue<Vector>	fPosiMemory;	//��¼���ϵ������㼣
	Vector	fVelocity;			//�ٶ�
	RealType	fVelocityModule;	//��֪�����ٶȵĳ��ȣ�
	hs_state	fEstado;		//״̬
	int		fParadaCount;
	int		fComiendoCount;
	THormiguita	*	fUltimaConQuienChoque;
	THormiguita	*	fBesandomeCon;
	vd_state		fVengoDe;//״̬
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
	Vector	PosicionRecordada();  //���ϵ������㼣
	void	Mover();		//�ƶ�
	void	Locura();		//�����������
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
