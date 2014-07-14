// aa2d_4Doc.cpp : implementation of the Caa2d_4Doc class
//

#include "stdafx.h"
#include "aa2d_4.h"

#include "aa2d_4Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Caa2d_4Doc

IMPLEMENT_DYNCREATE(Caa2d_4Doc, CDocument)

BEGIN_MESSAGE_MAP(Caa2d_4Doc, CDocument)
END_MESSAGE_MAP()


// Caa2d_4Doc construction/destruction

Caa2d_4Doc::Caa2d_4Doc()
{
	// TODO: add one-time construction code here

}

Caa2d_4Doc::~Caa2d_4Doc()
{
}

BOOL Caa2d_4Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Caa2d_4Doc serialization

void Caa2d_4Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Caa2d_4Doc diagnostics

#ifdef _DEBUG
void Caa2d_4Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Caa2d_4Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Caa2d_4Doc commands
