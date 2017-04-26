// Parameter.cpp : 实现文件
//

#include "stdafx.h"
#include "SlopeS.h"
#include "Parameter.h"
#include "afxdialogex.h"
#include "time.h"

// CParameter 对话框

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


// CParameter 消息处理程序


void CParameter::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	COLORREF color = RGB(255, 0, 0);      // 颜色对话框的初始颜色为红色  
	CColorDialog colorDlg(color);         // 构造颜色对话框，传入初始颜色值  

	CRect rectPicture;       // 图片控件的矩形区域坐标   
	CBrush newBrush;         // 创建的新画刷   
	CBrush *pOldBrush;       // 旧画刷的指针   
	CClientDC clientDC(this); // 构造客户区的设备上下文对象 

	if (IDOK == colorDlg.DoModal())       // 显示颜色对话框，并判断是否点击了“确定”   
	{
		color = colorDlg.GetColor();      // 获取颜色对话框中选择的颜色值  
		clr = color;

		// 用选择的颜色创建新画刷   
		newBrush.CreateSolidBrush(color);

		// 获取图片控件矩形区域的屏幕坐标   
		m_picColor.GetWindowRect(&rectPicture);
		// 将图片控件矩形区域的屏幕坐标转换为其父窗口即对话框的客户区坐标   
		ScreenToClient(&rectPicture);

		// 选择新画刷，并保存旧画刷的指针到pOldBrush   
		pOldBrush = clientDC.SelectObject(&newBrush);
		// 以新画刷为图片控件填充颜色   
		clientDC.Rectangle(rectPicture);

		// 恢复旧画刷   
		clientDC.SelectObject(pOldBrush);
		// 删除新画刷   
		newBrush.DeleteObject();
	}
}

void CParameter::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	UpdateData(false);
	CRect rectPicture;
	CBrush newBrush;

	newBrush.CreateSolidBrush(clr);

	m_picColor.GetWindowRect(&rectPicture);
	ScreenToClient(&rectPicture);
	dc.SelectObject(&newBrush);
	dc.Rectangle(rectPicture);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CParameter::OnUpdateData(){
	UpdateData(true);//将控件值传给变量
}