// EditPoint.cpp : 实现文件
//

#include "stdafx.h"
#include "SlopeS.h"
#include "EditPoint.h"
#include "afxdialogex.h"


// CEditPoint 对话框

IMPLEMENT_DYNAMIC(CEditPoint, CDialogEx)

CEditPoint::CEditPoint(CWnd* pParent /*=NULL*/)
: CDialogEx(CEditPoint::IDD, pParent)
{

}

CEditPoint::~CEditPoint()
{
}

void CEditPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_EDIT, m_EditPoint);
}


BEGIN_MESSAGE_MAP(CEditPoint, CDialogEx)
	ON_EN_CHANGE(IDC_POINT_EDIT, &CEditPoint::OnEnChangePointEdit)
END_MESSAGE_MAP()


// CEditPoint 消息处理程序


void CEditPoint::OnEnChangePointEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
