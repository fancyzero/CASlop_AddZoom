// DataShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SlopeS.h"
#include "DataShow.h"
#include "afxdialogex.h"
#include "SlopeSView.h"


// CDataShow �Ի���

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


// CDataShow ��Ϣ�������


BOOL CDataShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	RECT rt = { 0 };
	::GetWindowRect(m_hWnd, &rt);
	::MoveWindow(m_hWnd, 900, 150, rt.right - rt.left, rt.bottom - rt.top, FALSE);  //x��ʾ�������Ͻ�X�����꣬y��ʾ�������Ͻ�Y��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDataShow::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (tkid - 1 >= 0)
	{
		tkid -= 1;
		((CSlopeSView *)m_pMain)->tkid = tkid;
		((CSlopeSView *)m_pMain)->Write();
	}

}


void CDataShow::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (tkid + 1 <= tks - 1)
	{
		tkid += 1;
		((CSlopeSView *)m_pMain)->tkid = tkid;
		((CSlopeSView *)m_pMain)->Write();
	}
}
