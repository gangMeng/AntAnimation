// tmVectMat.h: interface for the tmVectMat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMVECTMAT_H__F0A2CA76_C9FB_418D_9D04_49AF17A5CC62__INCLUDED_)
#define AFX_TMVECTMAT_H__F0A2CA76_C9FB_418D_9D04_49AF17A5CC62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct D3DVECTOR
{
	D3DVECTOR(const float a,const float b,const float c)
	{
		x=a;y=b;z=c;
	}
	D3DVECTOR()
	{
		x=y=z=0;
	}
 	double operator * (const D3DVECTOR &a)   //a.b
 	{
 		double t=x*a.x+y*a.y+z*a.z;
 		return t;
 	}
 	double len()
 	{		return sqrt(*this*(*this));	}
 	double len2()
 	{		return (*this*(*this));	}
 	void set(const double &a,const double &b,const double &c)
	{	x=a;y=b;z=c;	}
 	void operator +=(const D3DVECTOR &a)
 	{
 		x+=a.x;
 		y+=a.y;
		z+=a.z;
 	}
	void operator -=(const D3DVECTOR &a)
	{
		coord[0]-=a.coord[0];
		coord[1]-=a.coord[1];
		coord[2]-=a.coord[2];
	}
	void operator *=(const D3DVECTOR &b)  //a*=b//a=a×b
	{
		D3DVECTOR a(*this);
		coord[0]=a.coord[1]*b.coord[2]-b.coord[1]*a.coord[2];
		coord[1]=a.coord[2]*b.coord[0]-b.coord[2]*a.coord[0];
		coord[2]=a.coord[0]*b.coord[1]-b.coord[0]*a.coord[1];
	}

	void operator +=(const double &a)
	{
		coord[0]+=a;
		coord[1]+=a;
		coord[2]+=a;
	}
	void operator -=(const double &a)
	{
		coord[0]-=a;
		coord[1]-=a;
		coord[2]-=a;
	}
 	void operator *=(const double &a)
 	{
 		coord[0]*=a;
 		coord[1]*=a;
		coord[2]*=a;
 	}
 	void operator /=(const double &a)
 	{
 		coord[0]/=a;
 		coord[1]/=a;
 		coord[2]/=a;
 	}

	D3DVECTOR operator + (const D3DVECTOR &a)
	{
		D3DVECTOR c=*this;
		c+=a;
		return c;
	}
	D3DVECTOR operator - (const D3DVECTOR &a)
	{
		D3DVECTOR c=*this;
		c-=a;
		return c;
	}

	D3DVECTOR operator * (const double a)
	{
		D3DVECTOR c=*this;
		c*=a;
		return c;
	}
	D3DVECTOR operator / (const double a)
	{
		D3DVECTOR c=*this;
		c/=a;
		return c;
	}
// 	bool operator == (const Vector &a)
// 	{
// 		for (int i=0;i<3;++i)
// 		{
// 			if (fabs(coord[i]-a.coord[i])>1e-3)
// 			{
// 				return false;
// 			}
// 		}
// 		return true;
// 	}
// /*
// 	D3DVECTOR operator * (const D3DVECTOR &b)
// 	{
// 		D3DVECTOR c;
// 		c.coord[0]=coord[1]*b.coord[2]-b.coord[1]*coord[2];
// 		c.coord[1]=coord[2]*b.coord[0]-b.coord[2]*coord[0];
// 		c.coord[3]=coord[0]*b.coord[1]-b.coord[0]*coord[1];
// 		return c;
// 
// 	}
// */
	D3DVECTOR operator - ()
	{
		D3DVECTOR c=*this;
		c.coord[0]=-c.coord[0];
		c.coord[1]=-c.coord[1];
		c.coord[2]=-c.coord[2];
		return c;
	}
// 
 	D3DVECTOR &cross_product(const D3DVECTOR &a,const D3DVECTOR &b)//a×b
 	{
 		x=a.y*b.z-b.y*a.z;
 		y=a.z*b.x-a.x*b.z;
 		z=a.x*b.y-b.x*a.y;
 		return *this;
 	}
 	void normalize()
 	{
 		double sq(len());
 		if(sq!=0)
 			*this/=sq;
 	}
	union
	{
		struct  
		{
			float x,y,z;
		};
		float coord[3];
	};
//	float x, y, z; 
} D3DVECTOR;

typedef struct _D3DMATRIX 
{
    union
	{
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
	void setzero()
	{
		for (unsigned int i=0;i<4;++i)
		{
			for (unsigned int j=0;j<4;++j)
			{
				m[i][j]=0;
			}
		}
	}
	float &get(int i,int j)
	{
		return m[i-1][j-1];
	}
	void set(int i,int j,float c)
	{
		m[i-1][j-1]=c;
	}

} D3DMATRIX;

typedef D3DVECTOR Vector;
typedef D3DMATRIX Matrix;

// class Matrix
// {
// public:
// 	double datas[16];
// 	void set(int i,int j,double num)
// 	{
// 		--i;--j;
// 		datas[4*i+j]=num;
// 	}
// 	double &get(int i,int j)
// 	{
// 		--i;--j;
// 		return datas[4*i+j];
// 	}
// 	void setzero()
// 	{
// 		memset(datas,0,sizeof(double)*16);
// 	}
// protected:
// private:
// };
typedef Vector * pVector;
typedef unsigned char byte;
// typedef struct {
// 	double _11, _12, _13, _14;
// 	double _21, _22, _23, _24;
// 	double _31, _32, _33, _34;
// 	double _41, _42, _43, _44;
// }Matrix;
typedef Matrix *pMatrix ;

typedef double RealType;
typedef vector<Vector> VectorArray;

//Vector MakeVector(const double x, const double y, const double z);
Vector MakeVector(const double ang, double module);

//void NegVect(Vector& vect);
//void AddVects(Vector v1, Vector v2, Vector& vect);
//void SubVects(Vector v1, Vector v2, Vector& vect);
//void AddVects(Vector& v1, const Vector v2);
//Vector AddVects(const Vector v1, const Vector v2);
//void SubVect(Vector& v1, const Vector v2);
//Vector SubVects(const Vector v1, const Vector v2);

//void CrossProd(const Vector iniV, const Vector endV, Vector& prod);
//Vector CrossProd(const Vector iniV, const Vector endV);
//RealType ScalarProd(const Vector v1, const Vector v2);
Vector VectorMul(const Vector v1, const Vector v2);
//void RealProd(Vector& vect, RealType number);
Vector RealProdFunc(const Vector vect, double number);

RealType SqModule(const Vector vect);//长度的平方
// RealType Module(const Vector vect);
bool IsModuleZero(const Vector vect);
double RazonModulos(const Vector vect1, const Vector vect2);
// void Normalize(Vector& vect);
bool EqualVects(const Vector v1, const Vector v2);

Vector ZRotate(const Vector v, double ang);
RealType CosAng(const Vector v1, const Vector v2);
RealType SqCosAng(const Vector v1, const Vector v2);
RealType AbsSin(const Vector v1, const Vector v2);
void Proyectar2D(Vector v, int x0, int y0, int* xR, int* yR, double escala, byte vista);
bool Matchvects(const Vector v1, const Vector v2, double Threshold);
bool SqMatchVects(const Vector v1, const Vector v2, double Threshold);
Vector DXCoordSys(const Vector v);

Matrix GenEjeRotationMatrix(const Vector &eje, const double CosAngle, const double SinAngle);
Matrix GenEjeRotationMatrix(const Vector &eje, double Angle);
Matrix GenOsaTransformMat(const Vector DestPTLeft, const Vector DestPTRight,  const Vector DestUp, Vector SrcPTLeft,        Vector SrcPTRight, Vector SrcUp);

void MatrixDesp(const Vector &v, Matrix& var);
void IdentityMatrix(Matrix& matrix);
void MatrixScale(const Vector &v, Matrix& m);
Matrix MatrixScale(const Vector &v);
Matrix MakeMatrix(const Vector v1, const Vector v2, const Vector v3, const Vector pos);
bool MatrixInvert(const Matrix a, Matrix& q);
pVector MatrixPosPtr(const Matrix a);

void AddVector(Vector v, VectorArray VectArr);

//inline double sqr(const double a)//平方
//{
//	return a*a;
//}

inline int round(const double a)//取最接近的整数
{
	if(a>0)
		return (int)(a+0.5);
	else
		return (int)(a-0.5);
}
const double Threshold=0.0001;
Vector VectorProdMatrix(const Matrix &mat, const Vector &vSrc);
Vector Proyectar( Vector v, Vector Target);



class tmVectMat  
{
public:
	tmVectMat();
	virtual ~tmVectMat();

};

#endif // !defined(AFX_TMVECTMAT_H__F0A2CA76_C9FB_418D_9D04_49AF17A5CC62__INCLUDED_)
