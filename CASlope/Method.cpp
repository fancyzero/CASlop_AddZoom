// Method.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Method.h"
#include "afxdialogex.h"


// CMethod �Ի���

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


// CMethod ��Ϣ�������


BOOL CMethod::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comboWeb.AddString(_T("ϸ�������ַ�"));
	m_comboWeb.SetCurSel(mode - 1);
	SetDlgItemText(IDC_SELECT_EDIT, _T("��ѡ��ļ���ģ���ǣ�ϸ�������ַ�"));
	SetDlgItemText(IDC_SELECT_EDIT2, _T("������ֵ���㷽��Ϊ��ϸ�������ַ�"));
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

	SetDlgItemText(IDC_SELECT_EDIT, _T("��ѡ��ļ���ģ���ǣ�") + strText);

	switch (m)
	{
	case 0:
		SetDlgItemText(IDC_SELECT_EDIT2, _T("������ֵ���㷽��Ϊ��ϸ�������ַ�"));
		GetDlgItem(IDC_SELECT_EDIT2)->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

}


void CMethod::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialogEx::OnOK();
}

void CMethod::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
