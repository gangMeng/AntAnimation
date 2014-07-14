// aa2d_4Doc.h : interface of the Caa2d_4Doc class
//


#pragma once


class Caa2d_4Doc : public CDocument
{
protected: // create from serialization only
	Caa2d_4Doc();
	DECLARE_DYNCREATE(Caa2d_4Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~Caa2d_4Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


