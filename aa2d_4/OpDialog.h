#if !defined(AFX_OPDIALOG_H__C0EA25C7_76D1_11D4_B8B1_D0331FC10000__INCLUDED_)
#define AFX_OPDIALOG_H__C0EA25C7_76D1_11D4_B8B1_D0331FC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// COpDialog 对话框

class COpDialog : public CDialog
{
	DECLARE_DYNAMIC(COpDialog)

public:
	COpDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_count;
public:
	double m_madness;
public:
	double m_speed;
};

#endif // !defined(AFX_OPDIALOG_H__C0EA25CD_76D1_11D4_B8B1_D0331FC10000__INCLUDED_)
