// Method.cpp : 实现文件
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Method.h"
#include "afxdialogex.h"


// CMethod 对话框

IMPLEMENT_DYNAMIC(CMethod, CDialogEx)

CMethod::CMethod(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMethod::IDD, pParent)
	, mode(1)
	, ks(1e5)
	, kn(1e5)
{
}

CMethod::~CMethod()
{
}

void CMethod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB_COMBO, m_comboWeb);
	DDX_Text(pDX, IDC_EDITKs, ks);
	DDX_Text(pDX, IDC_EDITKn, kn);
}


BEGIN_MESSAGE_MAP(CMethod, CDialogEx)
	//ON_LBN_SELCHANGE(IDC_MSD_LIST, &CMethod::OnLbnSelchangeMsdList)
	ON_BN_CLICKED(IDOK, &CMethod::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_WEB_COMBO, &CMethod::OnCbnSelchangeWebCombo)
	ON_BN_CLICKED(IDCANCEL, &CMethod::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMethod 消息处理程序


BOOL CMethod::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comboWeb.AddString(_T("细胞机条分法"));
	m_comboWeb.SetCurSel(mode - 1);
	SetDlgItemText(IDC_SELECT_EDIT, _T("你选择的计算模型是：细胞机条分法"));
	SetDlgItemText(IDC_SELECT_EDIT2, _T("采用数值计算方法为：细胞机条分法"));
	GetDlgItem(IDC_SELECT_EDIT2)->ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	return TRUE; 
}




void CMethod::OnCbnSelchangeWebCombo()
{
	int m = m_comboWeb.GetCurSel();
	mode = m + 1;

	CString strText;
	m_comboWeb.GetLBText(m, strText);

	SetDlgItemText(IDC_SELECT_EDIT, _T("你选择的计算模型是：") + strText);

	switch (m)
	{
	case 0:
		SetDlgItemText(IDC_SELECT_EDIT2, _T("采用数值计算方法为：细胞机条分法"));
		GetDlgItem(IDC_SELECT_EDIT2)->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

}


void CMethod::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}

void CMethod::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
