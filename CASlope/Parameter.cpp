// Parameter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Parameter.h"
#include "afxdialogex.h"
#include "time.h"

// CParameter �Ի���

IMPLEMENT_DYNAMIC(CParameter, CDialogEx)

CParameter::CParameter(CWnd* pParent /*=NULL*/)
: CDialogEx(CParameter::IDD, pParent)
, name(_T(""))
, gama(0)
, c(0)
, phi(0)
{
	srand((unsigned)time(&t));
}

CParameter::~CParameter()
{
}

void CParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, name);
	DDX_Text(pDX, IDC_EDIT2, gama);
	DDX_Text(pDX, IDC_EDIT3, c);
	DDX_Text(pDX, IDC_EDIT4, phi);
	DDX_Control(pDX, IDC_COLOR_SHOW_STATIC, m_picColor);
}


BEGIN_MESSAGE_MAP(CParameter, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CParameter::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CParameter ��Ϣ�������


void CParameter::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	COLORREF color = RGB(255, 0, 0);      // ��ɫ�Ի���ĳ�ʼ��ɫΪ��ɫ  
	CColorDialog colorDlg(color);         // ������ɫ�Ի��򣬴����ʼ��ɫֵ  

	CRect rectPicture;       // ͼƬ�ؼ��ľ�����������   
	CBrush newBrush;         // �������»�ˢ   
	CBrush *pOldBrush;       // �ɻ�ˢ��ָ��   
	CClientDC clientDC(this); // ����ͻ������豸�����Ķ��� 

	if (IDOK == colorDlg.DoModal())       // ��ʾ��ɫ�Ի��򣬲��ж��Ƿ����ˡ�ȷ����   
	{
		color = colorDlg.GetColor();      // ��ȡ��ɫ�Ի�����ѡ�����ɫֵ  
		clr = color;

		// ��ѡ�����ɫ�����»�ˢ   
		newBrush.CreateSolidBrush(color);

		// ��ȡͼƬ�ؼ������������Ļ����   
		m_picColor.GetWindowRect(&rectPicture);
		// ��ͼƬ�ؼ������������Ļ����ת��Ϊ�丸���ڼ��Ի���Ŀͻ�������   
		ScreenToClient(&rectPicture);

		// ѡ���»�ˢ��������ɻ�ˢ��ָ�뵽pOldBrush   
		pOldBrush = clientDC.SelectObject(&newBrush);
		// ���»�ˢΪͼƬ�ؼ������ɫ   
		clientDC.Rectangle(rectPicture);

		// �ָ��ɻ�ˢ   
		clientDC.SelectObject(pOldBrush);
		// ɾ���»�ˢ   
		newBrush.DeleteObject();
	}
}

void CParameter::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	UpdateData(false);
	CRect rectPicture;
	CBrush newBrush;

	newBrush.CreateSolidBrush(clr);

	m_picColor.GetWindowRect(&rectPicture);
	ScreenToClient(&rectPicture);
	dc.SelectObject(&newBrush);
	dc.Rectangle(rectPicture);
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CParameter::OnUpdateData(){
	UpdateData(true);//���ؼ�ֵ��������
}