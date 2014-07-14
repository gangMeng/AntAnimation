// OpDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "aa2d_4.h"
#include "OpDialog.h"


// COpDialog 对话框

IMPLEMENT_DYNAMIC(COpDialog, CDialog)

COpDialog::COpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COpDialog::IDD, pParent)
	, m_count(0)
	, m_madness(0)
	, m_speed(0)
{

}

COpDialog::~COpDialog()
{
}

void COpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_count);
	DDX_Text(pDX, IDC_EDIT2, m_madness);
	DDX_Text(pDX, IDC_EDIT3, m_speed);
}


BEGIN_MESSAGE_MAP(COpDialog, CDialog)
END_MESSAGE_MAP()


// COpDialog 消息处理程序
