// GridMap.cpp: implementation of the GridMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void TGridList::Add(TMapObject *obj)
{
	fList.insert(obj);
//	obj.fIndex=fList.size();
//	fList.push_back(&obj);
  //obj.fIndex = fList.Add(obj);
}

void TGridList::Delete(TMapObject *obj)
{
	if (fList.find(obj)!=fList.end())
		fList.erase(obj);
//   int i;
//   TMapObject o;
//   fList.Delete(obj.fIndex);
//   for(i=obj.fIndex; i<=fList.Count-1; i++) {
//     o = (TMapObject)TObject(fList.Item[i]);
//     o.fIndex--;
//   }
}


TMap::TMap(int aWidth, int aHeight) 
{
	fMap=NULL;
	Resize(aWidth, aHeight);

	
	fWidth = aWidth;
	fHeight = aHeight;
}

TMap::~TMap() 
{
	if(fMap)
		delete[] fMap;
}

void TMap::Draw() {
//   int j, i, k;
//   TMapObject *o;
// //   fscr.PutImage2(0, 0, fFondo);
//   for(j=0; j<=fHeight-1; j++)
//     for(i=0; i<=fWidth-1; i++)
//       for(k=0; k<=fMap[j,i].fList.Count-1; k++) {
// 	o = fMap[j*fHeight+i].fList[k];
// 	o->Draw(fscr);
	/* Debug
	fScr.Canvas.Font.Color == clGreen;
        fScr.Canvas.Brush.Style == bsClear;
        fScr.Canvas.TextOut(round(i*fGridSizeX),
                            round(j*fGridSizeY), IntToStr(fMap[j,i].fList.Count)); */
//      }
}

void TMap::ObjectsNear(TMapObject* obj, int Grids, list<TMapObject*> &List)
{
	int i, j;
	for(i=obj->grid_pos.x-Grids; i<=obj->grid_pos.x+Grids; i++) 
	{
		if((i>=0)&&(i<fWidth)) 
		{
			for(j=obj->grid_pos.y-Grids; j<=obj->grid_pos.y+Grids; j++) 
			{
				if((j>=0)&&(j<fHeight)) 
				{//将fMap[j*fHeight+i].fList中的元素放入List
					set<TMapObject *>::iterator it_s,it_end;
					it_s=fMap[i*fHeight+j].fList.begin();
					it_end=fMap[i*fHeight+j].fList.end();
					while (it_s!=it_end)
					{
						List.push_back(*it_s);
						++it_s;
					}
				}
			}
		}
	}
}

void TMap::ObjectsNear(RealType ax, RealType ay, int Grids, list<TMapObject*> &List)
{
  int i, j;
  int x, y;
  x = int(ax/fGridSizeX);
  y = int(ay/fGridSizeY);
  for(i=x-Grids; i<=x+Grids; i++)
  {
	  if((i>=0)&&(i<fWidth))
	  {
		  for(j=y-Grids; j<=y+Grids; j++)
		  {
			  
			  if((j>=0)&&(j<fHeight))
			  {

				  {//将fMap[j*fHeight+i].fList中的元素放入List
					  set<TMapObject *>::iterator it_s,it_end;
					  it_s=fMap[i*fHeight+j].fList.begin();
					  it_end=fMap[i*fHeight+j].fList.end();
					  while (it_s!=it_end)
					  {
						  List.push_back(*it_s);
						  ++it_s;
					  }
				  }
			  }
		  }
		  
	}
  }
}

bool TMap::Puedo(RealType x, RealType y) 
{
	return (x<(MAP_W-1)*MAP_SCALE)&&(x>0)&&(y<(MAP_W-1)*MAP_SCALE)&&(y>0);
	//  return (x>20)&&(x<fFondo.Width-20)&&(y>20)&&(y<fFondo.Height-30);
	//return true;
}
void TMap::PutObj(TMapObject *obj,RealType x,RealType y) 
{
  TPoint NewPos;
  NewPos.x = int(x/fGridSizeX);///////////////////////////////////////////////////////use tunce instead of int()
  NewPos.y = int(y/fGridSizeY);

	if (NewPos.x<0)
		NewPos.x=0;
	else if(NewPos.x>=fWidth)
		NewPos.x=fWidth-1;
	if(NewPos.y<0)
		NewPos.y=0;
	else if(NewPos.y>=fHeight)
		NewPos.y=fHeight-1;

  if((NewPos.x!=obj->grid_pos.x)||(NewPos.y!=obj->grid_pos.y))
  {
	  if(obj->grid_pos.x!=-1)
		  fMap[obj->grid_pos.x*fHeight+ obj->grid_pos.y].Delete(obj);
	  fMap[NewPos.x*fHeight+ NewPos.y].Add(obj);
	  obj->grid_pos.x = NewPos.x;
	  obj->grid_pos.y = NewPos.y;
  }
}
bool TMap::haveobj(RealType x,RealType y)
{
	return 0;
/*
	TPoint NewPos;
	NewPos.x = int(x/fGridSizeX);
	NewPos.y = int(y/fGridSizeY);
	if((NewPos.x!=obj->fActualPos.x)||(NewPos.y!=obj->fActualPos.y))
	{
		fMap[NewPos.y*fHeight+ NewPos.x].
		obj->fActualPos.x = NewPos.x;
		obj->fActualPos.y = NewPos.y;
	}
*/

}
void TMap::Resize(int NewWidth, int NewHeight)
{
 	if (fMap)
 	{
 		delete[] fMap;
 	}
	fMap=new TGridList[NewWidth*NewHeight];
	
	fGridSizeX=1;
	fGridSizeY=1;
	
	fWidth = NewWidth;
	fHeight = NewHeight;

	
}

void TMap::RestoreBackGround() {

}

void TMap::SetBackGround(string aFileName) 
{
//   TPicture tmpb;
//   fFondo = new TDrawing();
//   tmpb = new TPicture();
//   tmpb.LoadFromFile(aFileName);
//   fFondo.Bitmap.Width = tmpb.Width;
//   fFondo.Bitmap.Height = tmpb.Height;
//   fFondo.Bitmap.canvas.Draw(0, 0, tmpb.Graphic);
//   tmpb.free;
//   fFondo.RefreshScanlines;
// 
//   fScr = new TDrawing();
//   fScr.Bitmap.Width = fFondo.Width;
//   fScr.Bitmap.Height = fFondo.Height;
//   fScr.RefreshScanlines();
// 
//   fGridSizeX = fFondo.Width/fWidth;
//   fGridSizeY = fFondo.Height/fHeight;
}

void TMap::Show(HDC aDC)
{
//   fScr.DC = aDC;
//   fScr.Show;
}

void TMap::DeleteObject(TMapObject *obj)
{
	fMap[obj->grid_pos.x*fHeight+ obj->grid_pos.y].Delete(obj);
}


  
void TMap::getobjs(int x,int y,vector<pos_and_type> &pos_and_types)
{
	if (x<0||x>=fWidth||y<0||y>=fHeight)
		return;
	TGridList *s=&fMap[x*fHeight+y];
	set<TMapObject *>::iterator it_b(s->fList.begin()),it_end(s->fList.end());
	while(it_b!=it_end)
	{
		TMapObject *obj=*it_b;
		pos_and_types.push_back(pos_and_type(obj->actual_pos,obj->type));
		it_b++;
	}
}


GridMap::GridMap()
{

}

GridMap::~GridMap()
{

}
