// Ctab.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Ctab.h"
#include "afxdialogex.h"
#include "Parameter.h"

// Ctab �Ի���

IMPLEMENT_DYNAMIC(Ctab, CDialogEx)

Ctab::Ctab(CWnd* pParent /*=NULL*/)
: CDialogEx(Ctab::IDD, pParent)
{

}

Ctab::~Ctab()
{
}

void Ctab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(Ctab, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &Ctab::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &Ctab::OnBnClickedOk)
END_MESSAGE_MAP()


// Ctab ��Ϣ�������


BOOL Ctab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect tabRect;   // ��ǩ�ؼ��ͻ�����λ�úʹ�С   
	m_tab.SetMinTabWidth(40);
	if (pTuCeng > TUCENG_COUNT - 5)pTuCeng = 5;
	for (int i = 0; i < TUCENG_COUNT; i++){
		CString s;
		s.Format(_T("����%d"), i + 1);
		m_tab.InsertItem(i, s);         // �����i����ǩ  
		m_tuceng[i].Create(IDD_DIALOG1, &m_tab);    // ������i����ǩҳ
		m_tuceng[i].OnPaint();
	}
	m_tab.GetClientRect(&tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect   
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ   
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;


	for (int i = 0; i < TUCENG_COUNT; i++){
		if (i == pFocus){
			// ���ݵ����õ�tabRect�����ӶԻ��򣬲�����Ϊ��ʾ
			m_tuceng[i].SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		}
		else
			// ���ݵ����õ�tabRect�����ӶԻ��򣬲�����Ϊ����
			m_tuceng[i].SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void Ctab::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)//�л���ǩ
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CRect tabRect;    // ��ǩ�ؼ��ͻ�����Rect   

	// ��ȡ��ǩ�ؼ��ͻ���Rect����������������ʺϷ��ñ�ǩҳ   
	m_tab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	for (int i = 0; i < TUCENG_COUNT; i++)
	{
		if (m_tab.GetCurSel() == i){
			m_tuceng[i].SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			m_tuceng[i].OnUpdateData();
		}
		else
			m_tuceng[i].SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	}
}

void Ctab::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < TUCENG_COUNT; i++)
	{
		m_tuceng[i].OnUpdateData();//���ؼ�ֵ��������
	}
	CString Msg, Title;
	Msg = "�趨��ɺ���ģ��λ���Ҽ�ѡ���Ӧ����";
	Title = "��ʾ��Ϣ";
	::MessageBox(GetSafeHwnd(), Msg, Title, MB_OK | MB_ICONINFORMATION);
	CDialogEx::OnOK();

}