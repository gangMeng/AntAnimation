// GLFont.cpp: implementation of the CGLFont class.
// ѧ�������Ϸϵ�д���
// ������ E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"

#include "GLFont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//unsigned int Base;
//////////////////////////////////////////////////////////////////////

CGLFont::CGLFont()
{
}
CGLFont::~CGLFont()
{
}
void CGLFont::entext( float x,float y, LPCTSTR str, HFONT hFont,
					  float r, float g, float b) //ƽ���ַ���ʾ����֧�ֺ��֡�
{ 	HDC hdc = wglGetCurrentDC();        //��ȡ���ó���
	SelectObject(hdc, hFont);           //��������
	unsigned int Base = glGenLists(96); //������СΪ96��ʾ�б�,����ַ�λͼ
	wglUseFontBitmaps(hdc, 32, 96,Base);//����λͼ����
	glDisable(GL_TEXTURE_2D);           //��ʹ����ͼ
	glDisable(GL_LIGHTING);             //��ʹ�ù���
	glPushAttrib(GL_LIST_BIT);          //������ʾ�б�λ����
	glColor3f(r,g,b);                   //��ɫ
	glRasterPos2f(x/100, y/100);        //ƽ�涨λ
		glListBase(Base - 32);          //����ʾ�б�ʼ
		glCallLists(strlen(str), GL_UNSIGNED_BYTE, str); //������ʾ�б�
	glPopAttrib();                      //�ָ���ʾ�б�����
	glEnable(GL_LIGHTING);              //ʹ����ͼ
	glEnable(GL_TEXTURE_2D);            //ʹ�ù���
	glDeleteLists(Base, 96);            //ɾ����ʾ�б�
}

//////////////////////////////////////////////////////////////////
void CGLFont::c3dtext(LPCTSTR str,HFONT hFont,float z)// 3D������ʾ,֧�ֺ��֡�
{	glDisable(GL_TEXTURE_2D);     // �ر���ͼ
	glDisable(GL_LIGHTING);       // �رչ���
	Printfc3d("���庺��",hFont,z);// ����3D������ʾ
	glEnable(GL_LIGHTING);        // ʹ�ù���
	glEnable(GL_TEXTURE_2D);      // ʹ����ͼ
}
void CGLFont::Printfc3d(CString strText,HFONT hFont,float z)
{	HDC hdc = wglGetCurrentDC(); //�豸����
	HFONT hOldFont=(HFONT)::SelectObject(hdc,hFont);//������ѡ�볡��
	UCHAR * pChar=(UCHAR*)strText.GetBuffer(strText.GetLength());//�����ַ�������
	int   nListNum;                                 //��ʾ�б�
	DWORD dwChar;                                   //�ַ�ָ��
	GLYPHMETRICSFLOAT pgmf[1];                      //���������ַ�������Ϣ
	glPushMatrix();                                 //ѹ���ջ
	for(int i = 0; i < strText.GetLength(); i++)
	{ if(IsDBCSLeadByte((BYTE)pChar[i]))            //�Ƿ�˫�ֽ�(����)
		{ dwChar=(DWORD)((pChar[i]<<8)|pChar[i+1]); //ȡ��ǰ�ַ���˫�ֽ�ת��
		  i++;
		}
	  else	dwChar = pChar[i];                      //ȡ��ǰ�ַ�
	  nListNum = glGenLists(1);                     //�����б�
	  wglUseFontOutlines( hdc,						//ӵ�������HDC
						  dwChar,					//ת��Ϊ��ʾ�б�ĵ�һ���ַ�
						  1,						//ת��Ϊ��ʾ�б���ַ���
						  nListNum,					//��ʾ�б�Ŀ�ʼ
						  0.0f, 
						  z,						//Z�Ḻ����ĺ��
						  WGL_FONT_POLYGONS,		//�������巽ʽ
						  pgmf						//ָ������Ϣ������,Ϊcount��
						);
	  glCallList(nListNum);                         //������ʾ�б�
	  glDeleteLists(nListNum, 1);                   //ɾ���б�
	}	
	glPopMatrix();                                  //������ջ
	strText.ReleaseBuffer();                        //����ַ���
	::SelectObject(hdc,hOldFont);                   //�ָ�����
}
////////////////////////////////////////////////////////////////////////
void CGLFont:: settext (float x,float y,CString str,HFONT Font,float r,float g,float b)
  //ƽ���ַ���ʾ��������ȫ���������
{   glLoadIdentity();
	glPushAttrib(GL_CURRENT_BIT);//����������ɫ����
	glDisable(GL_TEXTURE_2D);                       //�ر���ͼ
	glDisable(GL_LIGHTING);                         //�رչ���
	glColor3f(r,g,b);                               //��ɫ
	  glTranslatef(-(420-x)/800,(260-y)/600,-1.0f); //��λ
	  Printftext (0,0, str,Font);                   //OpenGLƽ�溺��
	glEnable(GL_LIGHTING);                          //������Ч
	glEnable(GL_TEXTURE_2D);                        //��ͼ��Ч
	glPopAttrib();//�ָ�ǰһ����
}
void CGLFont:: Printftext (int x, int y, LPCTSTR lpszText,HFONT hFont)
{ CBitmap bitmap;									//�豸���λͼ����
  BITMAP bm;                                        //λͼ�ṹ����
  SIZE size;                                        //λͼ�ߴ�
  HDC MDC = ::CreateCompatibleDC(0);				//�ݴ��豸����
  SelectObject(MDC,hFont);							//ѡ��������
  ::GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);//��ȡ�ַ�λͼ��С
  bitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);//������MDC��ص�ɫλͼ
  HBITMAP oldBmp=(HBITMAP)SelectObject(MDC,bitmap); //�ַ�λͼ��MDC����
  SetBkColor  (MDC, RGB(0,     0,   0));			//��ɫ����ɫ
  SetTextColor(MDC, RGB(255, 255, 255));			//��ɫ����ɫ
  TextOut(MDC, 0, 0, lpszText, strlen(lpszText));	//������ֵ��ݴ�MDC
  bitmap.GetBitmap(&bm);							//������λͼ���ݽṹ
  size.cx = (bm.bmWidth + 31) & (~31);				//��Ե����
  int bufsize =size.cy * size.cx;					//ͼ�����ݳ���
  struct {  BITMAPINFOHEADER bih;
			RGBQUAD col[2];
 		 }bic;                                      //���嵥ɫλͼ�ṹ
  BITMAPINFO *binf = (BITMAPINFO *)&bic;            //��ȡλͼ�ṹ��Ϣ
  binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);//
  binf->bmiHeader.biWidth    = bm.bmWidth;			//ͼ�ο�
  binf->bmiHeader.biHeight   = bm.bmHeight;			//ͼ�θ�
  binf->bmiHeader.biPlanes   = 1;                   //
  binf->bmiHeader.biBitCount = 1;                   //��ɫ
  binf->bmiHeader.biCompression = BI_RGB;           //��ɫ��ʽ
  binf->bmiHeader.biSizeImage   = bufsize;          //ͼ�����ݳ���
  UCHAR* Bits = new UCHAR[bufsize];					//����ͼ�����ݿ����
  ::GetDIBits(MDC,bitmap,0,bm.bmHeight,Bits,binf,DIB_RGB_COLORS); 
                                                    //ȡ�豸�޹����ݵ�Bits
  glPixelStorei(GL_UNPACK_ALIGNMENT ,1);			//�������ش洢
  glRasterPos2i(x,y);                               //ƽ�涨λ
  glBitmap(size.cx,size.cy,0,0,0,0,Bits);           //ƽ��λͼ��ʾ
  delete Bits;                                      //ɾ��Bits
  SelectObject(MDC, oldBmp);                        //�ָ�λͼ����
  ::DeleteDC(MDC);                                  //ɾ���豸����
}
