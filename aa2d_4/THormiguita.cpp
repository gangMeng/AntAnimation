// THormiguita.cpp: implementation of the THormiguita class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aa2d_4.h"
#include "THormiguita.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define Threshold2 0.001

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "tmVectMat.h"
#include <queue>
using namespace std;

TConstantes cnst;

void THormiguita::Interaccion()
{
	list<TMapObject*> list_;
	fMap->ObjectsNear(this,cnst.GRIDS_SCAN,list_);
	while (!list_.empty())
	{
		TMapObject *obj=list_.back();	
		list_.pop_back();
		switch(obj->type)
		{
		case type_ant:
			InteracOtraHormiga((THormiguita *)obj);
			break;
		case type_cave:
			InteracCueva((TCueva *)obj);
			break;
		case type_food:
			InteracComida((TComida *)obj);
		    break;
		case type_obstacle:
			InteracObstaculo((TObstaculo *)obj);
		    break;
		default:
		    break;
		}
	}
}
THormiguita::THormiguita(TMap *aMap)
{
	type=type_ant;
	RealType ang;
	PVector ptrVector;
	actual_pos.x = aMap->fWidth/2;
	actual_pos.y = aMap->fHeight/2;
//	fPosi(0,0,0);
	fMap=aMap;
	ang=drand()*2*PI;
	/*org_ang = ang;
	first = true;
	chg_ang = org_ang;*/
	fVelocity.x=sin(ang);
	fVelocity.y=cos(ang);
	fEstado=hsNormal;
	fParadaCount = 10 + random(200);
	fUltimaConQuienChoque=NULL;
	fVengoDe=vdCueva;
	fTiempoQueVengoDe=1000000;
	fMenorTiempoQueMeDigeron=1000000;

	Rotated = false;
	rot_ang = ang;
						 
	for (int i=0;i<cnst.MEMORIA-1;++i)
	{
//		ptrVector=new Vector(1,1,0);
		fPosiMemory.push(Vector(1,1,0));
	}
	fCheckit = false;
	fCheckitColor.set(255/255.0,(100+random(155))/255.0,(200+random(55))/255.0);
//	fCheckitColor = ARGB(255, 100+ random(155),  255, 200+random(55));
}
void THormiguita::Draw(/*TDrawing aDrawing*/)
{			   
/*
	int	x,y;
	Vector v;
	//sombra
	x=round(fPosi.x);
	y=round(fPosi.y);
//	aDrawing.PutPixel(x+1,y,$808080);
	
	x=round(fPosi.x+fVelocity.x);
	y=round(fPosi.y+fVelocity.y);
//	aDrawing.PutPixel(x+1,y,$808080);
	
	x=round(fPosi.x+fVelocity.x*2);
	y=round(fPosi.y+fVelocity.y*2);
//	aDrawing.PutPixel(x+1,y,$808080);
 // {sombra comida}
  if ((fVengoDe==vdComida)&&
	  (fEstado!=hsComiendo)) 
  {
	  x=round(fPosi.x+fVelocity.x*4);
	  y=round(fPosi.y+fVelocity.y*4);
//	  aDrawing.FillRect(x+1,y+1,x+2,y+2,$808080);
  }
  //cuerpo
  x=round(fPosi.x);
  y=round(fPosi.y);
//  aDrawing.PutPixel(x,y,0);
  
  x=round(fPosi.x+fVelocity.x);
  y=round(fPosi.y+fVelocity.y);
  //aDrawing.PutPixel(x,y,$606060);
  
  x=round(fPosi.x+fVelocity.x*2);
  y=round(fPosi.y+fVelocity.y*2);
  //aDrawing.PutPixel(x,y,$606060);
  
 // {comida}
  if( (fVengoDe==vdComida)&&
	  (fEstado!=hsComiendo) )
  {
	  x=round(fPosi.x+fVelocity.x*4);
	  y=round(fPosi.y+fVelocity.y*4);
// 	  aDrawing.FillRect(x,y,x+1,y+1,$ffffff);
  }
  
 // {checkit}
  if( fCheckit)
  {
	  x=round(fPosi.x);
	  y=round(fPosi.y);
// 	  aDrawing.Rectangle(x-5,y-5,x+5,y+5, fCheckitColor);

  }*/

  
//   //debug
//   {if (fVengoDe=vdCueva) then
// 	  begin
// x=round(fPosi.x+fVelocity.x*4);
// y=round(fPosi.y+fVelocity.y*4);
//   aDrawing.PutPixel(x,y,$ff);
//   end;  }
//   
//   {colita memoria}
//   {v:=TVector( fPosiMemory.Peek^);
// x:=round(v.x);
// y:=round(v.y);
//   aDrawing.PutPixel(x,y,$FF); }
//   end;
}
void THormiguita::Locura()
{
	RealType ang,tmpang;
	Matrix mat;
//  {todo: bye bye matrix hacerlo con ZRotation }
	  //locura -  rotar de forma aleatoria la direccion ligeramente
	ang = drand()*cnst.LOCURA_ANG-cnst.LOCURA_ANG/2;
	rot_ang = ang;
	
	mat=GenEjeRotationMatrix(Vector(0,0,1), ang);
	fVelocity=VectorProdMatrix(mat, fVelocity);
		  //parada loca
	if(random(200)==1)
	{
		fEstado=hsParada;
		fParadaCount=random(50);
	}
}
void THormiguita::Mover()
{
	Vector newpos;
	RealType ang;
	Matrix mat;
	RealType lang;
	bool bug;
	lang = 0;
	//step:=MakeVector(10,10,10);
	newpos=actual_pos+fVelocity*0.2;  //fVelocity*2
	 if( fMap->Puedo(newpos.x, newpos.y) )
	 {
		 actual_pos=newpos;
	 }
	 else
	 {
//	   {todo:cacharroso repeat}
	   do 
	   {
		   lang+=0.1;
		   ang=(drand()-0.5)*lang;
		   rot_ang = ang;
		   /*if (fabs(ang - org_ang)<Threshold2) {
	           angChanged = false;
			   chg_ang = 0.0;
		   }
		   else {
			   chg_ang = ang - org_ang;
			   org_ang = ang;
			   angChanged = true;
		   }*/		   
		   mat=GenEjeRotationMatrix(Vector(0,0,1), ang);
		   fVelocity=VectorProdMatrix(mat, fVelocity);
		   //step:=fVelocity;
		   //RealProd(step, cnst.HORMVELOCITY);
		   newpos=actual_pos+fVelocity*0.2;   //fVelocity*2

		   bug= (fVelocity.len()<Threshold);
			 //bug= (fVelocity.len()==0);
		   if(bug)
		   {
			   fVelocity.set(1,0,0);
			   lang=0.1;
		   }
 		   /*if(lang>1)
 		   {
 			   fVelocity.x=fMap->fGridSizeX*fMap->fWidth/2-actual_pos.x;
 			   fVelocity.y=fMap->fGridSizeY*fMap->fHeight/2-actual_pos.y;
 			   fVelocity.z=5;
 			   fVelocity.normalize();
 				actual_pos+=fVelocity*2;
 			   lang=0.1;
 			   break;
 		   }*/
	   } while(!(   fMap->Puedo(newpos.x, newpos.y))
					|| bug );
     //ang:=Random*6.28;
     //fVelocity.x:=1*sin(ang);
     //fVelocity.y:=1*cos(ang);
	 }
}
void THormiguita::Run()	 
{
	PVector ptrVector;
	switch(fEstado)
	{
	case hsNormal:
		fVelocity*=cnst.HORMVELOCITY;
//		RealProd(fVelocity,cnst.HORMVELOCITY);
		Mover();
		fVelocity*=1/cnst.HORMVELOCITY;
//		RealProd(fVelocity,1/cnst.HORMVELOCITY);
		Locura();
		Interaccion();
		break;
	case hsParada:
		if (--fParadaCount<=0)//信息素？ 为0时蚂蚁开始彷徨？
		{
			fEstado=hsNormal;
			fBesandomeCon=NULL;
		}		
	    break;
	case hsComiendo:
		if (--fComiendoCount<=0)
		{
			fEstado=hsNormal;
			fVengoDe=vdComida;
			fTiempoQueVengoDe=0;
		}
	    break;
	default:
	    break;
	}
	//fMap->DeleteObject(this);
  fMap->PutObj(this, actual_pos.x, actual_pos.y);//putobj内置了deleteobj。
  ++fTiempoQueVengoDe;
  ++fMenorTiempoQueMeDigeron;
  if (fTiempoQueVengoDe%3==0)//走3步记录一次蚂蚁的昔日足迹？
  {
	  if(!fPosiMemory.empty())
	  {
		  
//		  ptrVector=fPosiMemory.front();
		  fPosiMemory.pop();
	  }

//	  ptrVector=fPosiMemory.pop();
	  fPosiMemory.push(actual_pos);
  }
}
void THormiguita::InteracObstaculo(TObstaculo *obs)
{
	RealType dist;
	Vector trayect;
	Vector newvel;

	trayect=obs->actual_pos-actual_pos;
	dist=trayect.len();
//	 {si choca por distancia}如果它是可见的距离
	if(dist<obs->fRadio&&
		dist>0&&
		CosAng(fVelocity,trayect)>0)
	{		 //redirigir

		newvel.cross_product(trayect,Vector(0,0,1));
//		newvel=CrossProd(trayect,Vector(0,0,1));
		fVelocity=Proyectar(fVelocity, newvel);
        if (fVelocity.len()<Threshold)
			fVelocity=newvel;
        fVelocity.normalize();
	}
}	
void THormiguita::InteracOtraHormiga(THormiguita *obj)
{	  
	RealType dist;
	Vector trayect;
	Vector newvel;
	
	trayect=obj->actual_pos-actual_pos;
	dist=trayect.len();
//	 {si es visible por distancia}
	 if( (dist<cnst.RADIO_VISION)&&
		 (dist>=0) )
	 {
		//{no tienen la misma informacion} 没有相同的信息
		if(fVengoDe!=obj->fVengoDe)
		{

	//	 {EL BESITO} {distancia peque馻 y de frente}
			if (true)
			{
				if ((obj!=fUltimaConQuienChoque)&&
					((obj->fBesandomeCon==NULL) || (obj->fBesandomeCon==this)))
				{
					
					if ((dist<cnst.RADIO_CHOQUE) &&
						(CosAng(fVelocity, trayect)>0.5) )
					{
						
						fEstado=hsParada;
						fParadaCount=cnst.TIEMPO_BESO;
						fBesandomeCon=obj;
						fUltimaConQuienChoque=obj;
					}
					//	  {CONVERSATION}
					if( ((fVengoDe==vdComida) && (obj->fVengoDe==vdCueva)) ||
						((fVengoDe==vdCueva) && (obj->fVengoDe==vdComida)))
					{
						if (obj->fTiempoQueVengoDe<fMenorTiempoQueMeDigeron)
						{
							fMenorTiempoQueMeDigeron=obj->fTiempoQueVengoDe;
							newvel=obj->PosicionRecordada()-PosicionRecordada();
							if (newvel.len()>0)
								newvel.normalize();
							else
								newvel=fVelocity;
							fVelocity=newvel;
						}
					}
				}
			}
			else//if true
			{
// 				if( RANDOM(6)==1)
// 				{
// 					trayect.normalize();
// 					trayect*=VELOCITY;
// 					fVelocity=trayect;
// 				}
			}
		}
	 }
}
void THormiguita::InteracComida(TComida *obs)
{
	RealType dist;
	Vector trayect;
	Vector newvel;
	
	trayect=obs->actual_pos-actual_pos;
	dist=trayect.len();
	if(fVengoDe!=vdComida)
	{
		if ((dist < obs->fCantidad*2) && (dist>0) )
		{
			// {si la veo me dirijo a ella} 如果我可以看到它
			trayect*=1/dist;
			fVelocity=trayect;
			
			if (dist < obs->fCantidad)
			{
				// {si llego cojo comidita}
				if((obs->fCantidad-=0.0001)<=0)
				{
					fMap->DeleteObject(obs);
				}
				trayect=PosicionRecordada()-actual_pos;
				if(trayect.len()>0)
				{
					trayect.normalize();
				}
				else
				{
					fVelocity=-fVelocity;
					trayect=fVelocity;
				}
				
				fVelocity=trayect;
				fEstado=hsComiendo;
				fComiendoCount=50+random(20);
				fVengoDe=vdComida;
				fTiempoQueVengoDe=0;
				fMenorTiempoQueMeDigeron=1000000;
			}
		}
	}
	else
	{
		if (dist < obs->fCantidad)
		{
			//pas?por la comidita!!!
			
			fTiempoQueVengoDe=0;
		}
	}
} 
void THormiguita::InteracCueva(TCueva *obs)
{
	RealType dist;
	Vector trayect;
	Vector newvel;
	
	trayect=obs->actual_pos-actual_pos;
	dist=trayect.len();
	if( fVengoDe!=vdCueva)
	{
		if ((dist < obs->fRadio*2) && (dist>0)) 
		{
			//{si la veo me dirijo a ella} 如果我可以看到它
			trayect.normalize();

			fVelocity=trayect;
		  if (dist < obs->fRadio)
		  {
			  // {si llego me meto un ratico } 如果我把一段时间
			  trayect=-trayect;
			  fVelocity=trayect;
			  fEstado=hsParada;
			  fParadaCount=50+random(20);
			  fVengoDe=vdCueva;
			  fTiempoQueVengoDe=0;
			  fMenorTiempoQueMeDigeron=1000000;

			  /*if(fComiendoCount)
				  obs->foodStore+=1;*/
//			  fComiendoCount=0;//加上的，食物量
		  }
		}
	}
	else
	{
		if (dist < obs->fRadio)
		{
			// {si llego me me actualizo - pas? por la cuevita!! }
			fTiempoQueVengoDe=0;
		}
/*
			(*
				if fVengoDe<>vdCueva then
									 begin
trayect:=SubVects(obs.fPosi, fPosi);
dist:=Module(trayect);
	 if (dist < Obs.fRadio*2) and (dist>0) then {si la veo me dirijo a ella}
	 begin
		 Normalize(trayect);
fVelocity:=Trayect;
		  if (dist < Obs.fRadio) then {si llego me meto un ratico }
		  begin
			  NegVect(Trayect);
fVelocity:=Trayect;
fEstado:=hsParada;
fParadaCount:=50+random(20);
fVengoDe:=vdCueva;
fTiempoQueVengoDe:=0;
fMenorTiempoQueMedigeron:=100000;
						 end;
						 end;
						 end;    *)*/
	}
}
Vector THormiguita::PosicionRecordada()
{
	return fPosiMemory.front();
}
void THormiguita::CheckIt()
{
	fCheckit=!fCheckit;
}
TObstaculo::TObstaculo()
{
	type = type_obstacle;	
	fRadio = 1.5;
//	fimg=random(length(Piedras));
}
void TObstaculo::Draw(/*TDrawing aDrawing*/)
{
/*
	int x,y,r;
	 // aDrawing.Canvas.Brush.Color:=clGray;
	// aDrawing.Canvas.Pen.Color:=clGray;
	x=round(fPosi.x);
	y=round(fPosi.y);
	r=round(fRadio);
	// aDrawing.Canvas.Ellipse(x-r, y-r, x+r, y+r); }
	// Piedras[fimg].PutImage(x,y, aDrawing);//, Masks[fimg]);
	aDrawing.PutImageMasked(x-r+5,y-r+5, Sombra, masks[fimg],x-r+5,y-r+5);
	aDrawing.PutImageMasked(x-r,y-r, Piedras[fimg], masks[fimg]);
*/
} 
  
/*
  { TComida }
  
  constructor TComida.Create;
  begin
	  inherited create;
fCantidad:=random*10+5;
		  end;
		  
*/
void TComida::Draw(/*TDrawing aDrawing*/)
{
/*
	int x,y,r;
	
	aDrawing.Canvas.Brush.Color=clWhite;
	aDrawing.Canvas.Pen.Color=clWhite;
	x=round(fPosi.x);
	y=round(fPosi.y);
	r=round(fCantidad);
	aDrawing.Canvas.Ellipse(x-r, y-r, x+r, y+r);
*/
		  
} 
/*
  
  { TCueva }
  

  
  
  var
s     :string;
len   :integer;
tmpb  :TPicture;
	   begin
		   //parametros
		   with cnst do
	   begin
LOCURA_ANG := 0.2;
RADIO_VISION := 30;
RADIO_CHOQUE := 5;
HORMVELOCITY  := 1;
TIEMPO_BESO := 0;
GRIDS_SCAN :=2;
MEMORIA := 5;
TIEMPO_SALIDECUEVA := 800;
					end;
					//piedras
s:='piedra01.bmp';
  
  repeat
len:=Length(piedras);
  Setlength(piedras, len+1);
  SetLength(masks, len+1);
  Piedras[len]:=TDrawing.Create;
  Piedras[len].Bitmap.LoadFromFile(s);
  Piedras[len].RefreshScanlines;
  Masks[len]:=Piedras[len].GetMaskCoords(0);
  inc(s[8]);
  until not FileExists(s);
Sombra:=TDrawing.Create;
tmpb := TPicture.Create;
	  tmpb.LoadFromFile('nbackgroundDark.jpg');
	  Sombra.Bitmap.width :=  tmpb.Width;
	  Sombra.Bitmap.Height := tmpb.Height;
	  Sombra.Bitmap.canvas.Draw(0,0, tmpb.Graphic);
	  tmpb.Free;
	  Sombra.RefreshScanlines;
	  end*/

/*tmpang = ang - org_ang;
if (fabs(tmpang)<Threshold2) {
angChanged = false;
chg_ang = 0.0;
}		
else {
chg_ang = tmpang;
org_ang = ang;
angChanged = true;
}*/	

