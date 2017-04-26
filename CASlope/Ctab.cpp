// Ctab.cpp : 实现文件
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Ctab.h"
#include "afxdialogex.h"
#include "Parameter.h"

// Ctab 对话框

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


// Ctab 消息处理程序


BOOL Ctab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect tabRect;   // 标签控件客户区的位置和大小   
	m_tab.SetMinTabWidth(40);
	if (pTuCeng > TUCENG_COUNT - 5)pTuCeng = 5;
	for (int i = 0; i < TUCENG_COUNT; i++){
		CString s;
		s.Format(_T("土层%d"), i + 1);
		m_tab.InsertItem(i, s);         // 插入第i个标签  
		m_tuceng[i].Create(IDD_DIALOG1, &m_tab);    // 创建第i个标签页
		m_tuceng[i].OnPaint();
	}
	m_tab.GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;


	for (int i = 0; i < TUCENG_COUNT; i++){
		if (i == pFocus){
			// 根据调整好的tabRect放置子对话框，并设置为显示
			m_tuceng[i].SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		}
		else
			// 根据调整好的tabRect放置子对话框，并设置为隐藏
			m_tuceng[i].SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void Ctab::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)//切换标签
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect   

	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
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
	// TODO:  在此添加控件通知处理程序代码
	for (int i = 0; i < TUCENG_COUNT; i++)
	{
		m_tuceng[i].OnUpdateData();//将控件值传给变量
	}
	CString Msg, Title;
	Msg = "设定完成后在模型位置右键选择对应参数";
	Title = "提示信息";
	::MessageBox(GetSafeHwnd(), Msg, Title, MB_OK | MB_ICONINFORMATION);
	CDialogEx::OnOK();

}