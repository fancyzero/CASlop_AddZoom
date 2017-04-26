// DataShow.cpp : 实现文件
//

#include "stdafx.h"
#include "SlopeS.h"
#include "DataShow.h"
#include "afxdialogex.h"
#include "SlopeSView.h"


// CDataShow 对话框

IMPLEMENT_DYNAMIC(CDataShow, CDialogEx)

CDataShow::CDataShow(CWnd* pParent /*=NULL*/)
: CDialogEx(CDataShow::IDD, pParent)
{

}

CDataShow::~CDataShow()
{
}

void CDataShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_text);
}


BEGIN_MESSAGE_MAP(CDataShow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDataShow::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDataShow::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDataShow 消息处理程序


BOOL CDataShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RECT rt = { 0 };
	::GetWindowRect(m_hWnd, &rt);
	::MoveWindow(m_hWnd, 900, 150, rt.right - rt.left, rt.bottom - rt.top, FALSE);  //x表示窗口左上角X轴坐标，y表示窗口左上角Y轴

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDataShow::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (tkid - 1 >= 0)
	{
		tkid -= 1;
		((CSlopeSView *)m_pMain)->tkid = tkid;
		((CSlopeSView *)m_pMain)->Write();
	}

}


void CDataShow::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (tkid + 1 <= tks - 1)
	{
		tkid += 1;
		((CSlopeSView *)m_pMain)->tkid = tkid;
		((CSlopeSView *)m_pMain)->Write();
	}
}
