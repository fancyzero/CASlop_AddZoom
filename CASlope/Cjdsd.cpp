// Cjdsd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Cjdsd.h"
#include "afxdialogex.h"


// Cjdsd �Ի���

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


// Cjdsd ��Ϣ�������


BOOL Cjdsd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_check1.SetCheck(check1);
	m_check2.SetCheck(1);
	m_check3.SetCheck(1);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void Cjdsd::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_check1.GetCheck())
		check1 = 1;
	else
		check1 = 0;

	UpdateData(true);
	CDialogEx::OnOK();
}
