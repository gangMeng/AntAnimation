// tmVectMat.cpp: implementation of the tmVectMat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aa2d_4.h"
#include "tmVectMat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const Vector NullVector(0, 0, 0);
const Vector AxisXVector(1, 0, 0);
const Vector AxisYVector(0, 1, 0);
const Vector AxisZVector(0, 0, 1);


Vector ZRotate(const Vector &v, double ang) 
{
	Vector res;
	res.x = v.x * cos(ang) - v.y * sin(ang);
	res.y = v.x * sin(ang) + v.y * cos(ang);
	return res;
}

Vector DXCoordSys(const Vector &v)//复制v？
{
	Vector res(v);
	return res;
}



void AddVector(const Vector &v, VectorArray& VectArr)//作用未知
{
	//	int len;
	//	len = VectArr.size();  //? sizeof
	VectArr.push_back(v);
}
/*
procedure AddVector( v:TVector; var VectArr:TVectorArray);
var
len :integer;
begin
len := Length(VectArr);
SetLength( VectArr, len+1);
VectArr[len]:=v;
end;
*/

Vector VectorMul(const Vector &v1,const Vector &v2) 
{//new v;  v.i=v1.i*v2.i;
	Vector res;
	for (int i=0;i<3;++i)
	{
		res.coord[i]=v1.coord[i]*v2.coord[i];
	}
	return res;
}
bool EqualVects(const Vector &v1, const Vector &v2) 
{
	for (int i=0;i<3;++i)
	{
		if(fabs(v1.coord[i]-v2.coord[2])>Threshold)
			return false;
	}
	return true;
}
/*
function EqualVects(const v1, v2:TVector):boolean;
begin
Result:= (v1.x=v2.x) and (v1.y=v2.y) and (v1.z=v2.z);
end;
*/
void MatrixScale(const Vector &v, Matrix& m) 
{
	m.setzero();
	for (unsigned int i=0;i<3;++i)
	{
		m.m[i][i]=v.coord[i];
	}
	m.m[3][3]=1;
// 	m.get(1,1) = v.x;
// 	m.get(2,2) = v.y;
// 	m.get(3,3) = v.z;
// 	m.get(4,4) = 1;
}

Matrix MatrixScale(const Vector &v) {
	Matrix res;
	MatrixScale(v, res);
	return res;
}

double RazonModulos(Vector vect1, Vector vect2) 
{
	return sqrt(vect1.len2()/vect2.len2());
	//	return (double)sqrt(SqModule(vect1)/SqModule(vect2));
}
/*
function RazonModulos(const vect1, vect2 :TVector):single;
begin
Result:=Sqrt(SqModule(vect1)/SqModule(vect2));
end;*/

Vector VectorProdMatrix(const Matrix &mat, const Vector &vSrc) {
	double x, y, z, w;
	x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
	w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;
	Vector res;
	res.x = x/w;
	res.y = y/w;
	res.z = z/w;
	return res;
}
/*
function  VectorProdMatrix(const Mat: TMatrix; const vSrc : TVector):TVector;
{result.x := Matrix._11*vector.x + Matrix._12*vector.y + Matrix._13*vector.z;
result.y := Matrix._21*vector.x + Matrix._22*vector.y + Matrix._23*vector.z;
result.z := Matrix._31*vector.x + Matrix._32*vector.y + Matrix._33*vector.z;}
var
x, y, z, w: Double;
begin
x := vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
y := vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
z := vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
w := vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;
Result.x := x/w;
Result.y := y/w;
Result.z := z/w;
end;*/
void IdentityMatrix(Matrix& matrix)
{
	matrix.setzero();
	//	memset(&matrix, 0,sizeof(Matrix)); 
	matrix._11= 1;
	matrix._22= 1;
	matrix._33= 1;
	matrix._44= 1;
}

/*
vect=-vect;

  
	procedure NegVect(var vect: TVector);
	begin
	vect.x:= -vect.x;
	vect.y:= -vect.y;
	vect.z:= -vect.z;
	end;
*/

/*

  vect=v1+v2;
  
	
	  procedure AddVects(v1, v2: TVector; var vect: TVector);
	  begin
	  vect.x:= v1.x+ v2.x;
	  vect.y:= v1.y+ v2.y;
	  vect.z:= v1.z+ v2.z;
	  end;
*/


/*

  vect=v1-v2;
  
	
	  procedure SubVects(v1, v2: TVector; var vect: TVector);
	  begin
	  vect.x:= v1.x- v2.x;
	  vect.y:= v1.y- v2.y;
	  vect.z:= v1.z- v2.z;
	  end;
*/

/*
v1+v2

  
	function AddVects(const v1, v2: TVector): TVector;
	begin
	Result.x:= v1.x+ v2.x;
	Result.y:= v1.y+ v2.y;
	Result.z:= v1.z+ v2.z;
	end;
	
	  
		v1+=v2;
		
		  
			procedure AddVect(var v1:TVector; const v2: TVector);
			begin
			v1.x:= v1.x+ v2.x;
			v1.y:= v1.y+ v2.y;
			v1.z:= v1.z+ v2.z;
			end;
			
			  
				v1-v2;
				
				  function SubVects(const v1, v2: TVector): TVector;
				  begin
				  Result.x:= v1.x- v2.x;
				  Result.y:= v1.y- v2.y;
				  Result.z:= v1.z- v2.z;
				  end;
				  
					
					  v1-=v2;
					  
						procedure SubVect(var v1:TVector; const v2: TVector);
						begin
						v1.x:= v1.x- v2.x;
						v1.y:= v1.y- v2.y;
						v1.z:= v1.z- v2.z;
end;*/
void CrossProd(const Vector &iniV,const Vector &endV, Vector& prod) 
{
	prod.cross_product(iniV,endV);
}
/*
procedure CrossProd(const iniV, endV: TVector; var prod: TVector);
begin
prod.x:= iniV.y* endV.z- iniV.z* endV.y;
prod.y:= iniV.z* endV.x- iniV.x* endV.z;
prod.z:= iniV.x* endV.y- iniV.y* endV.x;
end;*/

Vector CrossProd(const Vector &iniV,const Vector& endV) 
{
	Vector res;
	res.cross_product(iniV,endV);
	return res;
}
/*
function CrossProd(const iniV, endV: TVector): TVector;
begin
result.x:= iniV.y* endV.z- iniV.z* endV.y;
result.y:= iniV.z* endV.x- iniV.x* endV.z;
result.z:= iniV.x* endV.y- iniV.y* endV.x;
end;*/


/*
v1*v2;

  function ScalarProd(const v1, v2: TVector): RealType;
  begin
  result:= v1.x* v2.x+ v1.y* v2.y+ v1.z* v2.z;
  end;
*/

/*
vect*=number;

  procedure RealProd( var vect: TVector; number: RealType);
  begin
  vect.x:= number* vect.x;
  vect.y:= number* vect.y;
  vect.z:= number* vect.z;
  end;
  
	function RealProdFunc( const vect:TVector; number: single ):TVector;
	begin
	Result.x := number * vect.x;
	Result.y := number * vect.y;
	Result.z := number * vect.z;
end;*/

/*
return vect.len2();
function SqModule(const vect: TVector): RealType;
begin
result:= sqr(vect.x)+ sqr(vect.y)+ sqr(vect.z);
end;

  
	return vect.len()
	
	  function Module(const vect: TVector): RealType;
	  begin
	  result:= sqrt(sqr(vect.x)+ sqr(vect.y)+ sqr(vect.z));
	  end;
	  
		vect.normalize();
		
		  procedure Normalize(var vect: TVector);
		  begin
		  RealProd(vect, 1/Module(vect));
end;*/

RealType CosAng(Vector v1, Vector v2)
{//v1与v2的夹角的cos值
	return (v1*v2)/(v1.len()*v2.len());
}

/*
function CosAng(const v1, v2: TVector): RealType;
begin
result:= ScalarProd(v1, v2)/(Module(v1)* Module(v2));
end;
*/
RealType SqCosAng(const Vector &v1, const Vector &v2) {
	double a=CosAng(v1,v2);
	return a*a;
	//	return (CosAng(v1,v2));
	//	return (RealType)sqr(ScalarProd(v1, v2))/(SqModule(v1)* SqModule(v2));
}

/*
CosAng的平方
function SqCosAng(const v1, v2: TVector): RealType;
begin
result:= sqr(ScalarProd(v1, v2))/(SqModule(v1)* SqModule(v2));
end;
*/

RealType AbsSin(Vector v1,Vector v2) //////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	Vector cross_prod;
	cross_prod.cross_product(v1,v2);
	//	CrossProd(v1, v2, cross_prod);
	return cross_prod.len()/(v1.len()*v2.len());
	//	return Module(cross_prod)/Module(v1)* Module(v2);
}
/*
function AbsSin(const v1, v2: TVector): RealType;
var
cross_prod: TVector;
begin
CrossProd(v1, v2, cross_prod);
result:= Module(cross_prod)/Module(v1)* Module(v2);
end;
*/
Matrix MatrixProd(const Matrix &matrix1,const Matrix &matrix2)
{
	Matrix result;
	result._11= matrix1._12* matrix2._21+ matrix1._12* matrix2._21+
		matrix1._13* matrix1._31+ matrix1._14* matrix2._41;
	result._12= matrix1._12* matrix2._22+ matrix1._12* matrix2._22+
		matrix1._13* matrix2._32+ matrix1._14* matrix2._42;
	result._13= matrix1._12* matrix2._23+ matrix1._12* matrix2._23+
		matrix1._13* matrix2._33+ matrix1._14* matrix2._43;
	result._14= matrix1._12* matrix2._24+ matrix1._12* matrix2._24+
		matrix1._13* matrix2._34+ matrix1._14* matrix2._44;
	
	result._21= matrix1._22* matrix2._21+ matrix1._22* matrix2._21+
		matrix1._23* matrix2._31+ matrix1._24* matrix2._41;
	result._22= matrix1._22* matrix2._22+ matrix1._22* matrix2._22+
		matrix1._23* matrix2._32+ matrix1._24* matrix2._42;
	result._23= matrix1._22* matrix2._23+ matrix1._22* matrix2._23+
		matrix1._23* matrix2._33+ matrix1._24* matrix2._43;
	result._24= matrix1._22* matrix2._24+ matrix1._22* matrix2._24+
		matrix1._23* matrix2._34+ matrix1._24* matrix2._44;
	
	result._31= matrix1._32* matrix2._21+ matrix1._32* matrix2._21+
		matrix1._33* matrix2._31+ matrix1._34* matrix2._41;
	result._32= matrix1._32* matrix2._22+ matrix1._32* matrix2._22+
		matrix1._33* matrix2._32+ matrix1._34* matrix2._42;
	result._33= matrix1._32* matrix2._23+ matrix1._32* matrix2._23+
		matrix1._33* matrix2._33+ matrix1._34* matrix2._43;
	result._34= matrix1._32* matrix2._24+ matrix1._32* matrix2._24+
		matrix1._33* matrix2._34+ matrix1._34* matrix2._44;
	
	result._41= matrix1._42* matrix2._21+ matrix1._42* matrix2._21+
		matrix1._43* matrix2._31+ matrix1._44* matrix2._41;
	result._42= matrix1._42* matrix2._22+ matrix1._42* matrix2._22+
		matrix1._43* matrix2._32+ matrix1._44* matrix2._42;
	result._43= matrix1._42* matrix2._23+ matrix1._42* matrix2._23+
		matrix1._43* matrix2._33+ matrix1._44* matrix2._43;
	result._44= matrix1._42* matrix2._24+ matrix1._42* matrix2._24+
		matrix1._43* matrix2._34+ matrix1._44* matrix2._44;
	return result;
}

/*
procedure MatrixProd(matrix1, matrix2: TMatrix; var result: TMatrix);
begin
result._11:= matrix1._12* matrix2._21+ matrix1._12* matrix2._21+
matrix1._13* matrix1._31+ matrix1._14* matrix2._41;
result._12:= matrix1._12* matrix2._22+ matrix1._12* matrix2._22+
matrix1._13* matrix2._32+ matrix1._14* matrix2._42;
result._13:= matrix1._12* matrix2._23+ matrix1._12* matrix2._23+
matrix1._13* matrix2._33+ matrix1._14* matrix2._43;
result._14:= matrix1._12* matrix2._24+ matrix1._12* matrix2._24+
matrix1._13* matrix2._34+ matrix1._14* matrix2._44;

  result._21:= matrix1._22* matrix2._21+ matrix1._22* matrix2._21+
  matrix1._23* matrix2._31+ matrix1._24* matrix2._41;
  result._22:= matrix1._22* matrix2._22+ matrix1._22* matrix2._22+
  matrix1._23* matrix2._32+ matrix1._24* matrix2._42;
  result._23:= matrix1._22* matrix2._23+ matrix1._22* matrix2._23+
  matrix1._23* matrix2._33+ matrix1._24* matrix2._43;
  result._24:= matrix1._22* matrix2._24+ matrix1._22* matrix2._24+
  matrix1._23* matrix2._34+ matrix1._24* matrix2._44;
  
	result._31:= matrix1._32* matrix2._21+ matrix1._32* matrix2._21+
	matrix1._33* matrix2._31+ matrix1._34* matrix2._41;
	result._32:= matrix1._32* matrix2._22+ matrix1._32* matrix2._22+
	matrix1._33* matrix2._32+ matrix1._34* matrix2._42;
	result._33:= matrix1._32* matrix2._23+ matrix1._32* matrix2._23+
	matrix1._33* matrix2._33+ matrix1._34* matrix2._43;
	result._34:= matrix1._32* matrix2._24+ matrix1._32* matrix2._24+
	matrix1._33* matrix2._34+ matrix1._34* matrix2._44;
	
	  result._41:= matrix1._42* matrix2._21+ matrix1._42* matrix2._21+
	  matrix1._43* matrix2._31+ matrix1._44* matrix2._41;
	  result._42:= matrix1._42* matrix2._22+ matrix1._42* matrix2._22+
	  matrix1._43* matrix2._32+ matrix1._44* matrix2._42;
	  result._43:= matrix1._42* matrix2._23+ matrix1._42* matrix2._23+
	  matrix1._43* matrix2._33+ matrix1._44* matrix2._43;
	  result._44:= matrix1._42* matrix2._24+ matrix1._42* matrix2._24+
	  matrix1._43* matrix2._34+ matrix1._44* matrix2._44;
	  end;
	  
*/
/*
vector(x,y,z);

  function MakeVector(const x,y,z:single):TVector;
  begin
  Result.x:=x;
  Result.y:=y;
  Result.z:=z;
end;*/

Vector MakeVector(const double ang, const double module)
{
	Vector res;
	res.x = module*cos(ang);
	res.y = module*sin(ang);
	res.z = 0;
	return res;
}

/*
function MakeVector(const ang:single; module:real):TVector;overload;
begin
Result.x:=module*cos(ang);
Result.y:=module*sin(ang);
Result.z:=0;
end;*/

Vector Proyectar( Vector v, Vector Target)
{//将v投影至target
	double Cosine;
	double ResultModule;
	Vector res;
	Cosine = CosAng(v, Target);
	ResultModule =v.len() * Cosine;
	if(fabs(ResultModule)<Threshold)
	{
		return res;
	} 
	else 
	{
		Target.normalize();
		Target*=ResultModule;
		//		RealProd(Target, ResultModule);
		return Target;
	}
	return res;
}

/*
function  Proyectar(const v:TVector; Target :TVector):TVector;
{
Halla la proyeccion de V sobre otro vector Target
}
var
Cosine  :single;
ResultModule :single;
begin
Cosine:=CosAng( v, Target);
ResultModule:=Module(v)*Cosine;
if ResultModule=0 then
begin
Result.x:=0;
Result.y:=0;
Result.z:=0;
end  else
begin
Normalize(Target);
RealProd(Target, ResultModule);
Result:=Target;
end;
end;*/

Matrix GenEjeRotationMatrix(const Vector &eje, 
							const double CosAngle, 
							const double SinAngle) 
{
	Vector v(eje);
	v.normalize();
	Matrix res;
	res.setzero();
	//	memset(&res,  0,sizeof(Matrix));
	res.get(1,1) = ( v.x * v.x ) * ( 1.0 - CosAngle ) + CosAngle;
	res.get(1,2) = ( v.x * v.y ) * ( 1.0 - CosAngle ) - (v.z * SinAngle);
	res.get(1,3) = ( v.x * v.z ) * ( 1.0 - CosAngle ) + (v.y * SinAngle);
	
	res.get(2,1) = ( v.y * v.x ) * ( 1.0 - CosAngle ) + (v.z * SinAngle);
	res.get(2,2) = ( v.y * v.y ) * ( 1.0 - CosAngle ) + CosAngle ;
	res.get(2,3) = ( v.y * v.z ) * ( 1.0 - CosAngle ) - (v.x * SinAngle);
	res.get(3,1) = ( v.z * v.x ) * ( 1.0 - CosAngle ) - (v.y * SinAngle);
	res.get(3,2) = ( v.z * v.y ) * ( 1.0 - CosAngle ) + (v.x * SinAngle);
	res.get(3,3) = ( v.z * v.z ) * ( 1.0 - CosAngle ) + CosAngle;
	res.get(4,4) = 1.0;
	return res;
}

/*
function GenEjeRotationMatrix(const eje:TVector; const
CosAngle, SinAngle : single):TMatrix;
var
v          : TVector;
begin
//Yaw
v:=eje;
Normalize(v);
FillChar(Result,sizeof(TMatrix),0);
Result._11 := ( v.x * v.x ) * ( 1.0 - CosAngle ) + CosAngle;
Result._12 := ( v.x * v.y ) * ( 1.0 - CosAngle ) - (v.z * SinAngle);
Result._13 := ( v.x * v.z ) * ( 1.0 - CosAngle ) + (v.y * SinAngle);

  Result._21 := ( v.y * v.x ) * ( 1.0 - CosAngle ) + (v.z * SinAngle);
  Result._22 := ( v.y * v.y ) * ( 1.0 - CosAngle ) + CosAngle ;
  Result._23 := ( v.y * v.z ) * ( 1.0 - CosAngle ) - (v.x * SinAngle);
  Result._31 := ( v.z * v.x ) * ( 1.0 - CosAngle ) - (v.y * SinAngle);
  Result._32 := ( v.z * v.y ) * ( 1.0 - CosAngle ) + (v.x * SinAngle);
  Result._33 := ( v.z * v.z ) * ( 1.0 - CosAngle ) + CosAngle;
  Result._44 := 1.0;
end;*/

//生成绕向量eje旋转Angle的矩阵
Matrix GenEjeRotationMatrix(const Vector &eje, double Angle) {
	return GenEjeRotationMatrix( eje, cos(Angle), sin(Angle));
}

void MatrixDesp(const Vector &v, Matrix& m) {
	m.setzero();
	m.set(4,1,v.x);
	m.set(4,2,v.y);
	m.set(4,3,v.z);
	m.set(1,1,1);
	m.set(2,2,1);
	m.set(3,3,1);
	m.set(4,4,1);
}

/*
procedure MatrixDesp(const v:TVector; var m:TMatrix);
begin
m._12 := 0;
m._13 := 0;
m._14 := 0;
m._21 := 0;
m._23 := 0;
m._24 := 0;
m._31 := 0;
m._32 := 0;
m._34 := 0;
m._41 := v.x;
m._42 := v.y;
m._43 := v.z;
m._11 := 1;
m._22 := 1;
m._33 := 1;
m._44 := 1;
end;*/

void Proyectar2D(const Vector &v, int x0, int y0, int* xR, int* yR, double escala, byte vista) 
{
	switch(vista) 
	{
    case 0:
		*xR = x0 + round(v.x*escala);
		*yR = y0 + -round(v.z*escala);
		break;
    case 1:
		*xR = x0 + round(v.x*escala);
		*yR = y0 + -round(v.y*escala);
		break;
    case 2:
		*xR = x0 + round(v.z*escala);
		*yR = y0 + -round(v.y*escala);
		break;
    case 3:
		*xR = x0 + round(v.x*escala  + v.z*escala*0.4 );
		*yR = y0 + -round(v.y*escala + v.z*escala*0.4 );
		break;
    default:
		break;
	}
}

/*
{proyecta un vector 3D en una vista 2D para dibujar en el canvas}
procedure Proyectar2D(v:TVector; x0, y0:integer; var xR, yR:integer; escala:single; vista:byte);
begin
case vista of
0:begin {superior}
xR:=x0+ round(v.x*escala);
yR:=y0+ -Round(v.z*escala);
end;
1:begin {frontal}
xR:=x0+ round(v.x*escala);
yR:=y0+ -Round(v.y*escala);
end;
2:begin {lateral}
xR:=x0+ round(v.z*escala);
yR:=y0+ -Round(v.y*escala);
end;
3:begin {izom閠rica}
xR:=x0+ round(v.x*escala  + v.z*escala*0.4 );
yR:=y0+ -Round(v.y*escala + v.z*escala*0.4 );
end;
end;
end;*/


Matrix MatrixMultiply(const Matrix &a,const Matrix &b) 
{
	int i, j, k;
	Matrix res;
	res.setzero();
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			for(k=0; k<4; k++)
			{
				res.m[i][j]+=a.m[i][k]*b.m[k][j];
			}
		}
	}
	//	pQ[4*i+j] = pQ[4*i+j] + pA[4*i+k] * pB[4*k+j];
	return res;
}
/*
function MatrixMultiply(const a, b: TMatrix):TMatrix;
type
PArrayD3DValue = ^TArrayD3DValue;
TArrayD3DValue = array[0..15] of Single;
var
pA, pB, pQ: PArrayD3DValue;
i, j, k: Integer;
begin
FillChar(Result, SizeOf(Result), 0); //Esto es mierda

  pA := @a;
  pB := @b;
  pQ := @Result;
  for i:=0 to 3 do
  for j:=0 to 3 do
  for k:=0 to 3 do
  pQ[4*i+j] := pQ[4*i+j] + pA[4*i+k] * pB[4*k+j];
end;*/


Matrix GenOsaTransformMat(Vector DestPTLeft,	Vector DestPTRight, 
						  Vector DestUp,		Vector SrcPTLeft,
						  Vector SrcPTRight,	Vector SrcUp)
{
	Vector v, v2, eje, ejex(1,0,0);
	double Cosine, factor;
	Matrix Mat;
	Matrix res;
	// {Desplaza a origen}
	v = -SrcPTRight;
	//NegVect(v);
	MatrixDesp(v, res);
	v2 = (DestPTLeft-DestPTRight);
	v = (SrcPTLeft-SrcPTRight);
	//Rota 1.a
	Cosine = CosAng(v, ejex);
	eje = CrossProd(ejex, v);
	Mat = GenEjeRotationMatrix(eje, Cosine, sqrt(1-sqr(Cosine)));
	res = MatrixMultiply(res, Mat);
	SrcUp = VectorProdMatrix(Mat, SrcUp);
	//escalar
	factor = sqrt(SqModule(v2) / SqModule(v));
	MatrixScale(Vector(factor, factor, factor), Mat);
	res = MatrixMultiply(res, Mat);
	//Rota 1.b
	Cosine = CosAng(v2, ejex);
	eje = CrossProd( v2, ejex);
	Mat = GenEjeRotationMatrix(eje, Cosine, sqrt( 1-sqr(Cosine) ) );
	res = MatrixMultiply(res, Mat);
	
	SrcUp = VectorProdMatrix(Mat, SrcUp);
	//Rota 2
	Cosine = CosAng(SrcUp, DestUp);
	eje = CrossProd(DestUp, SrcUp);
	eje = Proyectar(eje, v2);
	
	Mat = GenEjeRotationMatrix(eje, Cosine, sqrt( 1-sqr(Cosine) ) );
	res = MatrixMultiply(res, Mat);
	//posicion
	MatrixDesp(DestPTRight, Mat);
	res = MatrixMultiply(res, Mat);
	
	return res;
}
/*
function GenOsaTransformMat(const DestPTLeft, DestPTRight,  DestUp: TVector;
SrcPTLeft,  SrcPTRight,   SrcUp: TVector): TMatrix;
var
v       :TVector;
Cosine  :single;
v2      :TVector;
eje     :TVector;
factor  :single;
ejex    :TVector;
{matrices}
Mat :TMatrix;
begin
ejex:=MakeVector(1,0,0);
{Desplaza a origen}
v:=SrcPTRight;
NegVect(v);
MatrixDesp(v, Result);

  v2:=SubVects(DestPTLeft, DestPTRight);
  v:=SubVects(SrcPTLeft, SrcPTRight);
  {Rota 1.a}
  Cosine:=CosAng(v, ejex);
  eje:=CrossProd(ejex, v);
  Mat:=GenEjeRotationMatrix(eje, Cosine, Sqrt( 1-Sqr(Cosine) ) );
  Result:=MatrixMultiply(Result, Mat);
  SrcUp:=VectorProdMatrix(Mat, SrcUp);
  
	{escalar}
	factor:=sqrt(sqModule(v2) / sqModule(v));
	   MatrixScale(MakeVector(factor, factor, factor), Mat);
	   Result:=MatrixMultiply(Result, mat);
	   
		 {Rota 1.b}
		 Cosine:=CosAng(v2, ejex);
		 eje:=CrossProd( v2, ejex);
		 Mat:=GenEjeRotationMatrix(eje, Cosine, Sqrt( 1-Sqr(Cosine) ) );
		 Result:=MatrixMultiply(Result, Mat);
		 
		   SrcUp:=VectorProdMatrix(Mat, SrcUp);
		   {Rota 2}
		   Cosine:=CosAng(SrcUp, DestUp);
		   eje:=CrossProd(DestUp, SrcUp);
		   eje:=Proyectar(eje, v2);
		   
			 Mat:=GenEjeRotationMatrix(eje, Cosine, Sqrt( 1-Sqr(Cosine) ) );
			 Result:=MatrixMultiply(Result, Mat);
			 {posicion}
			 MatrixDesp(DestPTRight, Mat);
			 Result:=MatrixMultiply(Result, Mat);
			 {  Result:=MatrixMultiply(Mat1, Mat2);
			 Result:=MatrixMultiply(Result, Mat3);
			 Result:=MatrixMultiply(Result, Mat4);
			 Result:=MatrixMultiply(Result, Mat5);
			 Result:=MatrixMultiply(Result, Mat6); }
			 end;
*/

bool IsModuleZero(Vector vect) {
	return vect.len2()<fabs(Threshold);
	//	return (vect.x==0)&&(vect.y==0)&&(vect.z==0);
}

/*
function IsModuleZero(const vect:TVector):boolean;
begin
Result:=(vect.x=0) and (vect.y=0) and (vect.z=0);
end;
*/

bool Matchvects(Vector v1, Vector v2, double Threshold) {
	return (v2-v1).len()<fabs(Threshold);
}

/*
function MatchVects(const v1, v2:TVector; Threshold:single):boolean;
begin
Result := Module(SubVects(v2, v1))<abs(Threshold);
end;*/

bool SqMatchVects(Vector v1,  Vector v2, double Threshold) {
	return (SqModule(v2-v1))<fabs(Threshold);
}
/*
function SqMatchVects(const v1, v2:TVector; Threshold:single):boolean;
begin
Result := SqModule(SubVects(v2, v1))<abs(Threshold);
end;*/

RealType SqModule(Vector vect) 
{//长度的平方
	return vect.len2();
}


Matrix MakeMatrix(Vector v1, Vector v2, Vector v3,Vector pos) {
	Matrix Result;
	Result.setzero();
	Vector *v[4]={&v1,&v2,&v3,&pos};
	for (unsigned int i=0;i<4;++i)
	{
		for (unsigned int j=0;j<3;++j)
		{
			Result.m[i][j]=v[i]->coord[j];
		}
	}
	Result.set(4,4,1);
	
	return Result;
}

/*
function  MakeMatrix(const v1,v2,v3,pos :TVector ):TMatrix;
begin
Result._11:=v1.x;
Result._12:=v1.y;
Result._13:=v1.z;
Result._14:=0;
Result._21:=v2.x;
Result._22:=v2.y;
Result._23:=v2.z;
Result._24:=0;
Result._31:=v3.x;
Result._32:=v3.y;
Result._33:=v3.z;
Result._34:=0;
Result._41:=pos.x;
Result._42:=pos.y;
Result._43:=pos.z;
Result._44:=1;
end;*/



bool MatrixInvert(const Matrix &a, Matrix& q) {
	double DetInv;
	if ((fabs(a._44 - 1) > 0.001)||
		(fabs(a._14) > 0.001) ||
		(fabs(a._24) > 0.001) ||
		(fabs(a._34) > 0.001))
		return false;
	DetInv = 1 /( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
		a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
		a._13 * ( a._21 * a._32 - a._22 * a._31 ) );
	q._11 =  DetInv * ( a._22 * a._33 - a._23 * a._32 );
	q._12 = -DetInv * ( a._12 * a._33 - a._13 * a._32 );
	q._13 =  DetInv * ( a._12 * a._23 - a._13 * a._22 );
	q._14 = 0;
	
	q._21 = -DetInv * ( a._21 * a._33 - a._23 * a._31 );
	q._22 =  DetInv * ( a._11 * a._33 - a._13 * a._31 );
	q._23 = -DetInv * ( a._11 * a._23 - a._13 * a._21 );
	q._24 = 0;
	
	q._31 =  DetInv * ( a._21 * a._32 - a._22 * a._31 );
	q._32 = -DetInv * ( a._11 * a._32 - a._12 * a._31 );
	q._33 =  DetInv * ( a._11 * a._22 - a._12 * a._21 );
	q._34 = 0;
	
	q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
	q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
	q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
	q._44 = 1;
	
	return true;
}


/*
function MatrixInvert(const a: TMatrix; var q: TMatrix ): boolean;
var
DetInv: Single;
begin
Result := false;
if (abs(a._44 - 1) > 0.001) or
(abs(a._14) > 0.001) or
(abs(a._24) > 0.001) or
(abs(a._34) > 0.001) then Exit;
DetInv := 1 /( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
			a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
			a._13 * ( a._21 * a._32 - a._22 * a._31 ) );
		
		q._11 :=  DetInv * ( a._22 * a._33 - a._23 * a._32 );
		q._12 := -DetInv * ( a._12 * a._33 - a._13 * a._32 );
		q._13 :=  DetInv * ( a._12 * a._23 - a._13 * a._22 );
		q._14 := 0;
		
		q._21 := -DetInv * ( a._21 * a._33 - a._23 * a._31 );
		q._22 :=  DetInv * ( a._11 * a._33 - a._13 * a._31 );
		q._23 := -DetInv * ( a._11 * a._23 - a._13 * a._21 );
		q._24 := 0;
		
		q._31 :=  DetInv * ( a._21 * a._32 - a._22 * a._31 );
		q._32 := -DetInv * ( a._11 * a._32 - a._12 * a._31 );
		q._33 :=  DetInv * ( a._11 * a._22 - a._12 * a._21 );
		q._34 := 0;
		
		q._41 := -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
		q._42 := -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
		q._43 := -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
		q._44 := 1;
		
Result := true;
end;*/


pVector MatrixPosPtr(Matrix &a) ////not good...!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	pVector pv;
	pv = (pVector)&(a._41);
	return pv;
}
/*
function MatrixPosPtr(const a: TMatrix ):PVector;
begin
Result := PVector( @a._41 );
end;

end.*/

tmVectMat::tmVectMat()
{

}

tmVectMat::~tmVectMat()
{

}
