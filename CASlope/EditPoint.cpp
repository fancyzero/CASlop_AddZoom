// EditPoint.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SlopeS.h"
#include "EditPoint.h"
#include "afxdialogex.h"


// CEditPoint �Ի���

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


// CEditPoint ��Ϣ�������


void CEditPoint::OnEnChangePointEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
