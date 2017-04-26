// Cjdsd.cpp : 实现文件
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Cjdsd.h"
#include "afxdialogex.h"


// Cjdsd 对话框

IMPLEMENT_DYNAMIC(Cjdsd, CDialogEx)

Cjdsd::Cjdsd(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cjdsd::IDD, pParent)
	, bianchang(10)
	, banjing(5)
	, tiaokuaishu(5)
{

}

Cjdsd::~Cjdsd()
{
}

void Cjdsd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, bianchang);
	DDX_Text(pDX, IDC_EDIT2, banjing);
	DDX_Text(pDX, IDC_EDIT3, tiaokuaishu);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
}


BEGIN_MESSAGE_MAP(Cjdsd, CDialogEx)
	ON_BN_CLICKED(IDOK, &Cjdsd::OnBnClickedOk)
END_MESSAGE_MAP()


// Cjdsd 消息处理程序


BOOL Cjdsd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_check1.SetCheck(check1);
	m_check2.SetCheck(1);
	m_check3.SetCheck(1);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void Cjdsd::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_check1.GetCheck())
		check1 = 1;
	else
		check1 = 0;

	UpdateData(true);
	CDialogEx::OnOK();
}
