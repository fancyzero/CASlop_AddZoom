
// SlopeSView.cpp : CSlopeSView 类的实现

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SlopeS.h"
#endif

#include "SlopeSDoc.h"
#include "SlopeSView.h"
#include "MainFrm.h"
#include "math.h"
#include "Ctab.h"
#include "Cjdsd.h"
#include "Method.h"
#include "Parameter.h"
#include "EditPoint.h"
#include <algorithm>
#include <vector> 
#include "stdafx.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <ctime>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSlopeSView

IMPLEMENT_DYNCREATE(CSlopeSView, CView)

BEGIN_MESSAGE_MAP(CSlopeSView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_NEW_EB, &CSlopeSView::OnNewEb)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	ON_WM_ERASEBKGND()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_NEW_M, &CSlopeSView::OnNewM)
	ON_UPDATE_COMMAND_UI(ID_NEW_EB, &CSlopeSView::OnUpdateNewEb)
	ON_UPDATE_COMMAND_UI(ID_NEW_M, &CSlopeSView::OnUpdateNewM)
	ON_UPDATE_COMMAND_UI(ID_ADD_L, &CSlopeSView::OnUpdateAddL)
	ON_UPDATE_COMMAND_UI(ID_COMT, &CSlopeSView::OnUpdateComt)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ADD_ML, &CSlopeSView::OnAddMl)
	ON_COMMAND(ID_DELETE, &CSlopeSView::OnDelete)
	ON_COMMAND(ID_PARAMETER, &CSlopeSView::OnParameter)
	ON_COMMAND(ID_CENG1, &CSlopeSView::OnCeng1)
	ON_COMMAND(ID_CENG2, &CSlopeSView::OnCeng2)
	ON_COMMAND(ID_CENG3, &CSlopeSView::OnCeng3)
	ON_COMMAND(ID_CENG4, &CSlopeSView::OnCeng4)
	ON_COMMAND(ID_CENG5, &CSlopeSView::OnCeng5)
	ON_COMMAND(ID_CENG6, &CSlopeSView::OnCeng6)
	ON_COMMAND(ID_CENG7, &CSlopeSView::OnCeng7)
	ON_COMMAND(ID_CENG8, &CSlopeSView::OnCeng8)
	ON_COMMAND(ID_CENG9, &CSlopeSView::OnCeng9)
	ON_COMMAND(ID_CENG10, &CSlopeSView::OnCeng10)
	ON_COMMAND(ID_ADD_L, &CSlopeSView::OnAddL)
	ON_COMMAND(ID_SEARCH, &CSlopeSView::OnSearch)
	ON_COMMAND(ID_COMT, &CSlopeSView::OnComt)
	ON_COMMAND(ID_US_M, &CSlopeSView::OnUsM)


	//ON_COMMAND(ID_lianxi, &CSlopeSView::Onlianxi)
	ON_COMMAND(ID_Show, &CSlopeSView::OnShow)
END_MESSAGE_MAP()

// CSlopeSView 构造/析构

CSlopeSView::CSlopeSView()
{
	// TODO:  在此处添加构造代码
	m_scale = 1.0f;
	m_translateX = 320;
	m_translateY = 512;
	m_off_x = 0;
	m_off_y = 0;
	FirstFocus = true;
	m_nDrawType = 0;
	m_nLineStyle = PS_SOLID;
	m_nLineWidth = 1;
	m_delete_id = -1;
	m_crt_p = 0; m_tid = 0; m_tcnt = 0;
	m_clrp = RGB(255, 0, 0);//红
	m_clrl = RGB(0, 0, 0);//黑
	bihe = false; m_drawonelinef = false;
	memset(m_bDraw, false, sizeof(bool) * 20);
	m_bDraw[0] = true;//控制按钮enable
	xypoint = 0; linepoint = 0;
	xybz = false;
	Linebz = false;
	// 将数组m_nzValues的元素都初始化为0   
	memset(m_nzValues, 0, sizeof(CPoint)* POINT_COUNT);
	memset(m_pcrt, 0, sizeof(int)* POINT_COUNT);
	memset(m_fjxpoint, 0, sizeof(CPoint)* POINT_COUNT*POINT_COUNT);
	tc_step = 0;
	memset(m_tfinish, false, sizeof(bool)* POINT_COUNT);
	pTuCeng = 0;
	pFocus = 0;
	for (int i = 0; i < 10; i++){
		CString s;
		s.Format(_T("土层%d"), i + 1);
		tuceng[i].gama = 18.0;
		tuceng[i].c = 15.0;
		tuceng[i].phi = 20.0;
		tuceng[i].name = s;
		if (i == 0)
			tuceng[i].clr = (int)(RGB(252, 243, 169));
		else
			tuceng[i].clr = (int)(RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));//生成0到255的随机数
	}
	tuceng[0].gama = 20; tuceng[1].gama = 20;
	tuceng[0].c = 10; tuceng[1].c = 20;
	tuceng[0].phi = 12.0; tuceng[1].phi = 18.0;
	jd_fg = 10; jd_yh = 5; tks = 10; minkid = 0;
	toptcid = 0;
	mode = 1;
	ks=100000;
	kn=90000;
	rgbcolor[0] = 0x000000FF; rgbcolor[1] = 0x00002AFF; rgbcolor[2] = 0x000055FF; rgbcolor[3] = 0x00007FFF; rgbcolor[4] = 0x0000AAFF; rgbcolor[5] = 0x0000D4FF;
	rgbcolor[6] = 0x0000FFFE; rgbcolor[7] = 0x0000FFD4; rgbcolor[8] = 0x0000FFA9; rgbcolor[9] = 0x0000FF7F; rgbcolor[10] = 0x0000FF55; rgbcolor[11] = 0x0000FF2A;
	rgbcolor[12] = 0x0000FF00; rgbcolor[13] = 0x002AFF00; rgbcolor[14] = 0x0054FF00; rgbcolor[15] = 0x007FFF00; rgbcolor[16] = 0x00A9FF00; rgbcolor[17] = 0x00D4FF00;
	rgbcolor[18] = 0x00FFFF00; rgbcolor[19] = 0x00FFD400; rgbcolor[20] = 0x00FFA900; rgbcolor[21] = 0x00FF7F00; rgbcolor[22] = 0x00FF5400; rgbcolor[23] = 0x00FF0000;
	fgid = 0; tkid = 0;
	m_DataDlg = NULL;
	bishop = 0.01; bph = 0.01;
	check1 = 0;

}

CSlopeSView::~CSlopeSView()
{
}

BOOL CSlopeSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}





// CSlopeSView 绘制


#ifdef _DEBUG
void CSlopeSView::AssertValid() const
{
	CView::AssertValid();
}

void CSlopeSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSlopeSDoc* CSlopeSView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSlopeSDoc)));
	return (CSlopeSDoc*)m_pDocument;
}
#endif //_DEBUG


class CScalableDC 
{
public:
	float m_Scale;
	float m_off_x;
	float m_off_y;
	CDC& m_DC;
	CScalableDC(CDC& pdc)
		:m_DC(pdc)
	{
		m_Scale = 1.0f;
		m_off_x = 0;
		m_off_y = 0;
	}

	CPoint MoveTo(MyPoint point)
	{
		point.x = (point.x + m_off_x) *m_Scale;
		point.y = (point.y + m_off_y)* m_Scale;
		return m_DC.MoveTo(point);
	}
	BOOL LineTo(MyPoint point)
	{
		point.x = (point.x + m_off_x) *m_Scale;
		point.y = (point.y + m_off_y) * m_Scale;
		return m_DC.LineTo(point);
	}
	void FillSolidRect(LPCRECT lpRect, COLORREF clr)
	{
		RECT rc = *lpRect;
		rc.left = (rc.left +m_off_x)*m_Scale;
		rc.right = (rc.right + m_off_x)*m_Scale;
		rc.top = (rc.top + m_off_y)*m_Scale;
		rc.bottom = (rc.bottom + m_off_y)*m_Scale;
		m_DC.FillSolidRect(&rc, clr);
	}
	BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType)
	{
		return m_DC.ExtFloodFill((x + m_off_x)*m_Scale, (y + m_off_y)*m_Scale, crColor, nFillType);
	}

	BOOL Rectangle(LPCRECT lpRect)
	{
		RECT rc = *lpRect;
		rc.left = (rc.left + m_off_x)*m_Scale;
		rc.right = (rc.right + m_off_x)*m_Scale;
		rc.top = (rc.top + m_off_y)*m_Scale;
		rc.bottom = (rc.bottom + m_off_y)*m_Scale;

		return m_DC.Rectangle(&rc);
	}
	BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount)
	{
		return m_DC.TextOutW((x + m_off_x)*m_Scale, (y + m_off_y)*m_Scale, lpszString, nCount);
	}

	BOOL Ellipse(int x1, int y1, int x2, int y2)
	{
		return m_DC.Ellipse((x1 + m_off_x)*m_Scale, (y1 + m_off_y)*m_Scale, (x2 + m_off_x)*m_Scale, (y2 + m_off_y)*m_Scale);
	}

	BOOL Ellipse(LPCRECT lpRect)
	{
		RECT rc = *lpRect;
		rc.left = (rc.left + m_off_x)*m_Scale;
		rc.right = (rc.right + m_off_x)*m_Scale;
		rc.top = (rc.top + m_off_y)*m_Scale;
		rc.bottom = (rc.bottom + m_off_y)*m_Scale;
		return m_DC.Ellipse(&rc);
	}
};


CPoint CSlopeSView::TransformPoint(const CPoint& p)
{
	CPoint ret;
	ret.x = (p.x + m_off_x) * m_scale;
	ret.y = (p.y + m_off_y) * m_scale;
	return ret;
}



BOOL  ScalableArc(CSlopeSView* view, HDC hdc, _In_ int x1, _In_ int y1, _In_ int x2, _In_ int y2, _In_ int x3, _In_ int y3, _In_ int x4, _In_ int y4)
{
	float s = view->m_scale;
	float ox = view->m_off_x;
	float oy = view->m_off_y;
	return Arc(hdc, (x1 + ox) *s, y1*s + oy*s, x2*s + ox*s, y2*s + oy*s, x3*s + ox*s, y3*s + oy*s, x4*s + ox*s, y4*s + oy*s);
}
// CSlopeSView 事件处理函数
void CSlopeSView::OnDraw(CDC* pDC)//重画机制（双缓冲）
{
	CSlopeSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMemDC dcMem(*pDC, this);
	CDC& d = dcMem.GetDC();
	CScalableDC MemDC(d);
	MemDC.m_Scale = m_scale;
	MemDC.m_off_x = m_off_x;
	MemDC.m_off_y = m_off_y;

	
	CRect   rect;
	GetClientRect(&rect);
	MemDC.m_DC.FillSolidRect(&rect, RGB(255, 255, 255));//将背景填白pDC->GetBkColor()
	//画笔
	CPen penp(m_nLineStyle, m_nLineWidth, m_clrp);
	CPen penbh(m_nLineStyle, 4, RGB(128, 128, 128));
	CPen penfinish(m_nLineStyle, 2, RGB(211, 208, 207));
	CPen penl(m_nLineStyle, m_nLineWidth, m_clrl);
	CPen pencu(m_nLineStyle, 1, RGB(0, 0, 255));
	//画刷
	CBrush brush(tuceng[0].clr);
	//画图
	//CPoint m_nzValues_cp[POINT_COUNT];
	//
	//memcpy(m_nzValues_cp, m_nzValues, sizeof m_nzValues);
	//for (int i = 0; i < m_crt_p; i++)
	//{
	//	m_nzValues[i] = TransformPoint(m_nzValues[i]);
	//}

	for (int i = 0; i < m_crt_p; i++)//重画边界线
	{
		CPoint point = m_nzValues[i];
		MemDC.MoveTo(point);
		MemDC.m_DC.SelectObject(&penp);
		if (bihe)MemDC.m_DC.SelectObject(&penl);
		int cross_size = (5 / m_scale) ;
		if (cross_size <= 0)
			cross_size = 1;
		MemDC.LineTo(MyPoint(point.x + cross_size, point.y)); //*绘制点
		MemDC.LineTo(MyPoint(point.x - cross_size, point.y));
		MemDC.MoveTo(MyPoint(point));
		MemDC.LineTo(MyPoint(point.x, point.y + cross_size));
		MemDC.LineTo(MyPoint(point.x, point.y - cross_size));
		MemDC.MoveTo(MyPoint(point));
		MemDC.m_DC.SelectObject(&penl);
		if (bihe)MemDC.m_DC.SelectObject(&penbh);
		if (i<m_crt_p - 1)MemDC.LineTo(m_nzValues[i + 1]);//*绘制线
	}

	//填充黄色
	if (bihe) 
	{
		MemDC.m_DC.SelectObject(brush);
		CPoint m_nzValues_t[POINT_COUNT];
		
		memcpy(m_nzValues_t, m_nzValues, sizeof m_nzValues);
		for (int i = 0; i < m_crt_p; i++)
		{
			m_nzValues_t[i] = TransformPoint(m_nzValues_t[i]);
		}
		HRGN rg = CreatePolygonRgn(m_nzValues_t, m_crt_p, WINDING);
		FillRgn(MemDC.m_DC, rg, brush);
		DeleteObject(rg);
	}

	MemDC.m_DC.SetBkMode(TRANSPARENT);//设置文字背景透明
	//显示坐标点文字（x,y）
	if (m_nDrawType >= 1 && m_nDrawType <= 3)
	{
		for (int i = 0; i < m_crt_p; i++)
		{
			CPoint point = m_nzValues[i];
			CString str;
			str.Format(_T("(%d ,%d)"),int( point.x - m_translateX), int(m_translateY - point.y));
			MemDC.TextOut(point.x, point.y, str, str.GetLength());
		}
	}
	//CPoint m_fjxpoint_cp[POINT_COUNT][POINT_COUNT];
	//for (int i = 0; i < POINT_COUNT; i++)
	//	for (int j = 0; j < POINT_COUNT; j++)
	//{
	//		m_fjxpoint_cp[i][j] = m_fjxpoint[i][j];
	//		m_fjxpoint[i][j] =  TransformPoint(m_fjxpoint[i][j]);
	//}
	for (int i = 0; i <= m_tid; i++)//重画材料线
	{
		for (int j = 0; j < m_pcrt[i]; j++)//第i条第j点
		{
			CPoint point = m_fjxpoint[i][j];
			MemDC.MoveTo(point);
			MemDC.m_DC.SelectObject(&penl);
			int cross_size = (5 / m_scale);
			if (cross_size <= 0)
				cross_size = 1;
			MemDC.LineTo(MyPoint(point.x + cross_size, point.y)); //*绘制点
			MemDC.LineTo(MyPoint(point.x - cross_size, point.y));
			MemDC.MoveTo(MyPoint(point));
			MemDC.LineTo(MyPoint(point.x, point.y + cross_size));
			MemDC.LineTo(MyPoint(point.x, point.y - cross_size));
			MemDC.MoveTo(MyPoint(point));
			if (m_tfinish[i])MemDC.m_DC.SelectObject(&penfinish);
			if (j<m_pcrt[i] - 1)MemDC.LineTo(m_fjxpoint[i][j + 1]);
		}
	}
	for (int i = 1; i <= tc_step; i++)//重自定义填充
	{
		CBrush tcbrush(tianchong[i].clr);
		MemDC.m_DC.SelectObject(tcbrush);
		CPoint point = tianchong[i].point;

		HDC hDC = MemDC.m_DC.GetSafeHdc(); //获取屏幕DC

		// 再获取当前点位置像素值  
		COLORREF color = ::GetPixel(hDC, point.x, point.y);
		MemDC.ExtFloodFill(point.x, point.y, color, FLOODFILLSURFACE);//取屏幕点的像素，对这个像素填充tcbrush
		DeleteObject(tcbrush);
	}
	
	//重画方格
	if (crt_fg == 2)
	{
		MemDC.m_DC.SelectObject(&penl);
		CPoint p1, p2;
		double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;
		double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;
		
		//画竖线
		for (int i = 0; i <= k1; i++)
		{
			p1.x = fg[1].x + i*jd_fg; p2.x = p1.x;
			p1.y = fg[1].y; p2.y = fg[2].y;
			MemDC.MoveTo(p1);
			MemDC.LineTo(p2);
		}
		
		//画横线
		for (int i = 0; i <= k2; i++)
		{
			p1.x = fg[1].x; p2.x = fg[2].x;
			p1.y = fg[1].y + i*jd_fg; p2.y = p1.y;
			MemDC.MoveTo(p1);
			MemDC.LineTo(p2);
		}

		p1.x = fg[1].x; p1.y = fg[2].y;
		p2.x = fg[2].x; p2.y = p1.y;
		MemDC.MoveTo(p1);
		MemDC.LineTo(p2);
		p1.x = fg[2].x; p1.y = fg[1].y;
		MemDC.LineTo(p1);
	}

	if (m_nDrawType == 8)
	{
		int i; CPoint p0;
		
		//画颜色
		for (i = 0; i < totfg; i++)
		{
			if (fgss[i].k >= 6)continue;
			p0 = fgss[i].p0;
			COLORREF clr = (int)(fgss[i].k / 6 * 16777215);
			clr = rgbcolor[(int)(clr / (16777215.0 / 24.0))];
			CBrush gzbrush(clr);
			MemDC.m_DC.SelectObject(gzbrush);
			MemDC.ExtFloodFill(p0.x, p0.y, RGB(255, 255, 255), FLOODFILLSURFACE);
			DeleteObject(gzbrush);
		}
		i = minkid;
		p0 = fgss[i].p0;
		double bj = fgss[i].bj;
		MemDC.MoveTo(p0);
		MemDC.LineTo(fgss[i].ptStart);
		MemDC.MoveTo(p0);
		MemDC.LineTo(fgss[i].ptEnd);
		HDC hDC = MemDC.m_DC.GetSafeHdc();
		ScalableArc(this, hDC, p0.x - (int)bj, p0.y - (int)bj, p0.x + (int)bj, p0.y + (int)bj, fgss[i].ptStart.x, fgss[i].ptStart.y, fgss[i].ptEnd.x, fgss[i].ptEnd.y);
		if (fgid != minkid)
		{
			i = fgid;
			p0 = fgss[i].p0;
			bj = fgss[i].bj;
			MemDC.m_DC.SelectObject(&pencu);
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[i].ptStart);
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[i].ptEnd);
			ScalableArc(this, hDC, p0.x - (int)bj, p0.y - (int)bj, p0.x + (int)bj, p0.y + (int)bj, fgss[i].ptStart.x, fgss[i].ptStart.y, fgss[i].ptEnd.x, fgss[i].ptEnd.y);
		}
		MemDC.m_DC.SelectObject(&penl);
		for (int j = 1; j < tks; j++)
		{
			CPoint p1, p2;
			if (j == fgss[i].qytks)
			{
				CPen pentkqy(m_nLineStyle, 2, RGB(255, 0, 0));
				MemDC.m_DC.SelectObject(&pentkqy);
				int x = fgss[i].ptStart.x + (fgss[i].ptEnd.x - fgss[i].ptStart.x) / tks*j;
				p1.x = x; p2.x = x;
				p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
				p2.y = GetBoundPoint(x, p1.y);
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			else
			{
				MemDC.m_DC.SelectObject(&penl);
				int x = fgss[i].ptStart.x + (fgss[i].ptEnd.x - fgss[i].ptStart.x) / tks*j;
				p1.x = x; p2.x = x;
				p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
				p2.y = GetBoundPoint(x, p1.y);
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			
		}
	}
	CBrush brushjx(RGB(0, 128, 225));//蓝色
	CPen penjx(m_nLineStyle, 1, RGB(225, 225, 255));
	MemDC.m_DC.SelectObject(&brushjx);
	MemDC.m_DC.SelectObject(&penjx);
	MemDC.Rectangle(CRect(0, 610, 1362, 635));
	DeleteObject(penp);
	DeleteObject(penbh);
	DeleteObject(penl);
	DeleteObject(penfinish);
	DeleteObject(penjx);
	DeleteObject(pencu);
	DeleteObject(brush);
	DeleteObject(brushjx);
	//memcpy(m_nzValues, m_nzValues_cp, sizeof m_nzValues);
	//for (int i = 0; i < POINT_COUNT; i++)
	//{
	//	for (int j = 0; j < POINT_COUNT; j++)
	//	{
	//		m_fjxpoint[i][j] = m_fjxpoint_cp[i][j];
	//	}
	//}
}

void CSlopeSView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_nDrawType == 1 || m_nDrawType == 2)
	{
		CEdit* pedit = &m_EditPoint;
		if (point.y >= 612)
		{
			pedit->SetReadOnly(false);
			GetDlgItem(IDC_POINT_EDIT)->SetFocus();
		}
		else
		{
			pedit->SetReadOnly(true);;//失去焦点
			this->SetFocus();//设置焦点为当前窗口
		}
	}
	CPoint saved_point = point;
	//MyPoint pt(point);
	point.x = point.x / m_scale - m_off_x;
	point.y = point.y / m_scale - m_off_y;

	//pt.x = pt.x / m_scale - m_off_x;
	//pt.y = pt.y / m_scale - m_off_y;
	float x = point.x;
	float y = point.y;
	CString crtpoint;
	crtpoint.Format(_T("(X = %f , Y = %f)"), x , (y <= 610 ? m_translateY - y : -100));
	//绘图 （双缓存）
	CDC* pDC = GetDC();
	OnDraw(pDC);
	CDC&  dcMemDC = *pDC;
	CScalableDC MemDC(dcMemDC);
	MemDC.m_Scale = m_scale;
	MemDC.m_off_x = m_off_x;
	MemDC.m_off_y = m_off_y;
	//得到黑色画笔
	CPen penl(m_nLineStyle, m_nLineWidth, m_clrl);
	CPen pencu(m_nLineStyle, 2, RGB(255, 0, 128));
	//得到透明的画刷
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	MemDC.m_DC.SelectObject(&penl);
	MemDC.m_DC.SelectObject(pbrush);
	switch (m_nDrawType){
	case 2:
		//判断是否接近起点（形成闭合区域）
		if (m_crt_p>2 && PointHitTest(m_nzValues[0], point)){
			CRect rc;
			float radius = 12 / m_scale;
			rc.SetRect(m_nzValues[0].x - radius, m_nzValues[0].y - radius, m_nzValues[0].x + radius, m_nzValues[0].y + radius);
			MemDC.Ellipse(rc);
		}
		else if (m_crt_p > 0 && XYHitTest(m_nzValues[m_crt_p - 1], point)){
			point = xypoint;
		}
		//实时跟踪鼠标位置(连线至鼠标)
		MemDC.MoveTo(m_nzValues[m_crt_p - 1]);
		MemDC.LineTo(point);
		break;
	case 4:
		if (LineHitTest(point))
		{
			CRect rc;
			CPoint p = (linepoint);
			float radius = 12 / m_scale;
			rc.SetRect(p.x - radius, p.y - radius, p.x + radius, p.y + radius);
			MemDC.Ellipse(rc);
		}
		if (m_pcrt[m_tid] > 0 && !m_drawonelinef){
			CPoint p = m_fjxpoint[m_tid][m_pcrt[m_tid] - 1];

			MemDC.MoveTo(p);
			MemDC.LineTo(point);
		}
		break;
	case 6:
		if (crt_fg == 1 && fg[1].x<point.x && fg[1].y<point.y){
			CPoint p1, p2;
			double dx = (point.x - fg[1].x)*1.0; int k1 = (int)dx / jd_fg;
			double dy = (point.y - fg[1].y)*1.0; int k2 = (int)dy / jd_fg;
			for (int i = 0; i <= k1; i++){//画竖线
				p1.x = fg[1].x + i*jd_fg; p2.x = p1.x;
				p1.y = fg[1].y; p2.y = point.y;
				p1 = (p1);
				p2 = (p2);
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			for (int i = 0; i <= k2; i++){//画横线
				p1.x = fg[1].x; p2.x = point.x;
				p1.y = fg[1].y + i*jd_fg; p2.y = p1.y;
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			p1.x = fg[1].x; p1.y = point.y;
			p2.x = point.x; p2.y = p1.y;
			MemDC.MoveTo(p1);
			MemDC.LineTo(p2);
			p1.x = point.x; p1.y = fg[1].y;
			MemDC.LineTo(p1);
		}
		break;
	case 7:
		if (fg[1].x < point.x && fg[1].y < point.y &&point.x < fg[2].x && point.y < fg[2].y){
			CPoint p1, p2;
			MemDC.m_DC.SelectObject(&pencu);
			double dx = (point.x - fg[1].x)*1.0; int k1 = (int)dx / jd_fg;
			double dy = (point.y - fg[1].y)*1.0; int k2 = (int)dy / jd_fg;
			p1.x = fg[1].x + jd_fg*k1; p1.y = fg[1].y + jd_fg*k2;
			p2.x = fg[1].x + jd_fg*(k1 + 1); p2.y = fg[1].y + jd_fg*(k2 + 1);
			if (p2.x>fg[2].x)p2.x = fg[2].x;
			if (p2.y>fg[2].y)p2.y = fg[2].y;
			MemDC.Rectangle(CRect(p1, p2));
		}
		break;
	case 8:
		if (fg[1].x < point.x && fg[1].y < point.y &&point.x < fg[2].x && point.y < fg[2].y){
			CPoint p1, p2, p0;
			MemDC.m_DC.SelectObject(&pencu);
			double x0 = point.x - fg[1].x; int i = (int)x0 / jd_fg;//第i列
			double y0 = point.y - fg[1].y; int j = (int)y0 / jd_fg;//第j列
			double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//共k1+1列小方格
			double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//共k2+1行小方格
			int n = j*(k1 + 1) + i;//第n个方格

			//crtpoint.Format(_T("(X = %d , Y = %d)   K = %1.4f"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[n].k);

			if (fgss[n].qytks>0 && fgss[n].qytks <= tks)
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%1.4f  安全系数：%1.4f 牵引条块数为：%d"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[n].bj, fgss[n].k, fgss[n].qytks);
			else
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%1.4f  安全系数：%1.4f"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[n].bj, fgss[n].k);

			if (fgss[n].k == 10000)break;

			p1.x = fg[1].x + jd_fg*i; p1.y = fg[1].y + jd_fg*j;
			p2.x = fg[1].x + jd_fg*(i + 1); p2.y = fg[1].y + jd_fg*(j + 1);
			if (p2.x>fg[2].x)p2.x = fg[2].x;
			if (p2.y>fg[2].y)p2.y = fg[2].y;
			MemDC.Rectangle(CRect(p1, p2));

			p0 = fgss[n].p0;
			double bj = fgss[n].bj;
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[n].ptStart);
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[n].ptEnd);
			HDC hDC = MemDC.m_DC.GetSafeHdc();
			ScalableArc(this,hDC, p0.x - (int)bj, p0.y - (int)bj, p0.x + (int)bj, p0.y + (int)bj, fgss[n].ptStart.x, fgss[n].ptStart.y, fgss[n].ptEnd.x, fgss[n].ptEnd.y);
		}
		else
			if (fgss[minkid].qytks>0 && fgss[minkid].qytks <= tks)
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%.2f  安全系数：%1.4f 牵引条块数为：%d"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[minkid].bj, fgss[minkid].k, fgss[minkid].qytks);
			else
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%.2f  安全系数：%1.4f"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[minkid].bj, fgss[minkid].k);
		break;
	}
	ReleaseDC(pDC);//释放pDC

	// 在状态栏的当前点窗格中显示点坐标字符串
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(crtpoint);
	CView::OnMouseMove(nFlags, saved_point);
}
BOOL CSlopeSView::OnMouseWheel(UINT f, short d, CPoint p)
{
	CPoint saved_point = p;
	m_scale += d/1200.0f;
	//p.x = p.x / m_scale - m_off_x;
	//p.y = p.y / m_scale - m_off_y;
	m_off_x = p.x - p.x* m_scale;
	m_off_y = p.y - p.y * m_scale;

	this->Invalidate();
	return CView::OnMouseWheel(f, d, saved_point);
}
void CSlopeSView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint saved_point = point;
	point.x = point.x / m_scale - m_off_x;
	point.y = point.y / m_scale - m_off_y;
	if (point.y >= 612) return;
	CDC* pDC = GetDC();//新建pDC
	switch (m_nDrawType)
	{
	case 1://边界线
		m_nzValues[m_crt_p] = point;
		m_crt_p++;
		m_nDrawType = 2;//这时绘出了第一个点
		break;

	case 2://这时绘出了第i个点（i>1）
		if (bihe || point == m_nzValues[0])//画出的边界闭合
		{
			bihe = true;
			m_nzValues[m_crt_p] = m_nzValues[0];
			m_crt_p++;//共m_crt_p个点，从 m_nzValues[0]到 m_nzValues[m_crt_p-1];
			m_nDrawType = 3;//画完边界图了
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//标准光标
			if (!FirstFocus){
				CEdit* pedit = &m_EditPoint;
				pedit->ShowWindow(SW_HIDE);
			}
			m_bDraw[0] = false;
			m_bDraw[1] = true;

		}
		else{
			if (xybz) point = xypoint;
			m_nzValues[m_crt_p] = point;
			m_crt_p++;
		}
		break;

	case 4://材料线
		if (m_pcrt[m_tid] == 0)
		{//第一点
			if (Linebz)
			{
				m_drawonelinef = false;
				point = linepoint;
				m_fjxpoint[m_tid][m_pcrt[m_tid]] = point;
				m_pcrt[m_tid]++;
			}
		}
		else
		{//不是第一点
			if (Linebz)
			{//最后一点
				point = linepoint;
				m_fjxpoint[m_tid][m_pcrt[m_tid]] = point;
				m_pcrt[m_tid]++;
				m_tfinish[m_tid] = true;//这条已绘完
				m_drawonelinef = true;
				m_tid++;//绘制下一条
				m_tcnt++;
			}
			else
			{//中间点
				m_fjxpoint[m_tid][m_pcrt[m_tid]] = point;
				m_pcrt[m_tid]++;
			}
		}
		break;
	case 6://方格
		if (crt_fg == 0)
			fg[++crt_fg] = point;
		else if (crt_fg == 1){
			fg[++crt_fg] = point;
			m_bDraw[3] = true;
			m_bDraw[2] = false;//外加
			m_nDrawType = 7;
		}
		break;

	case 8:
		HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
		struct fgss  &cntfg = fgss[fgid];
		if (cntfg.ptStart.x < point.x && point.x < cntfg.ptEnd.x && PtInRegion(rg, point.x, point.y))
		{
			if (sqrt((point.x - cntfg.p0.x)*(point.x - cntfg.p0.x) + (point.y - cntfg.p0.y)*(point.y - cntfg.p0.y)) < cntfg.bj)
			{
				tkid = (int)((point.x - cntfg.ptStart.x)*1.0 / (cntfg.ptEnd.x - cntfg.ptStart.x)* tks);
			}
		}
		else if (fg[1].x < point.x && fg[1].y < point.y &&point.x < fg[2].x && point.y < fg[2].y){
			CPoint p1, p2, p0;
			double x0 = point.x - fg[1].x; int i = (int)x0 / jd_fg;//第i列
			double y0 = point.y - fg[1].y; int j = (int)y0 / jd_fg;//第j列
			double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//共k1+1列小方格
			double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//共k2+1行小方格
			int n = j*(k1 + 1) + i;//第n个方格
			if (fgss[n].k == 10000)break;
			fgid = n;
		}
		if (m_DataDlg != NULL)
		{
			Write();
		}
		DeleteObject(rg);
		break;
	}
	OnDraw(pDC);
	ReleaseDC(pDC);
	CView::OnLButtonUp(nFlags, saved_point);
}

void CSlopeSView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)//按下键触发事件
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_ESCAPE && m_nDrawType<4)
	{//按下ESC时触发
		if (m_crt_p>0) m_crt_p--;
		//Invalidate();//清屏
		if (bihe){
			bihe = false;
			m_nDrawType = 2;
			m_bDraw[1] = false;
			m_bDraw[0] = true;
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//十字光标
			if (!FirstFocus)
			{
				CEdit* pedit = &m_EditPoint;
				pedit->ShowWindow(SW_SHOW);
			}
			return;
		}
		else if (m_crt_p <= 0){
			if (m_nDrawType == 2)
				m_nDrawType = 1;
			else{
				m_nDrawType = 0;
				SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//标准光标
				if (!FirstFocus){
					CEdit* pedit = &m_EditPoint;
					pedit->ShowWindow(SW_HIDE);
				}
			}
			return;
		}
	}
	else if (nChar == VK_RETURN)//按下回车
	{
		if (m_pcrt[m_tid] == 0 && (m_nDrawType == 4 || m_nDrawType == 3))
		{
			m_nDrawType = 5;
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//标准光标
			m_bDraw[1] = false;
			m_bDraw[2] = true;
		}
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSlopeSView::OnRButtonUp(UINT /* nFlags */, CPoint point)//右键事件
{
	RButtonUp = point;
	CMenu menu;       // 菜单（包含主菜单栏和子菜单）   
	CMenu *pSubMenu; CMenu *pSubMenu1; // 右键菜单   
	CBitmap m_bitmap1, m_bitmap2, m_bitmap[20];//位图（图标）
	m_bitmap1.LoadBitmap(IDB_BITMAP1);
	m_bitmap2.LoadBitmap(IDB_BITMAP2);
	for (int i = 0; i < 10; i++)
		m_bitmap[i].LoadBitmap(IDB_COLOR);
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);//多边形区域

	if (((m_delete_id = MLHitTest(point)) != -1) && (m_nDrawType >= 4))//删除分界线
	{
		crt_tc = point;
		// 加载菜单资源到menu对象   
		menu.LoadMenu(IDR_DELETE_ML);
		// 因为右键菜单是弹出式菜单，不包含主菜单栏，所以取子菜单   
		pSubMenu = menu.GetSubMenu(0);
		//为第一个项设置图标
		pSubMenu->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_bitmap1, &m_bitmap1);
		// 将坐标值由客户坐标转换为屏幕坐标 
		ClientToScreen(&point);
		// 弹出右键菜单，菜单左侧与point.x坐标值对齐   
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	else if (PtInRegion(rg, point.x, point.y) && m_nDrawType >= 5)//添加分界线&&选择土层参数
	{
		crt_tc = point;
		menu.LoadMenu(IDR_SELECT_PA);
		pSubMenu = menu.GetSubMenu(0);
		pSubMenu->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_bitmap2, &m_bitmap2);
		pSubMenu1 = pSubMenu->GetSubMenu(1);

		for (int k = 0; k < 10; k++){
			int color = tuceng[k].clr;
			pSubMenu1->ModifyMenu(ID_CENG1 + k, MF_BYCOMMAND, ID_CENG1 + k, tuceng[k].name);//改变菜单标题名

			BITMAP bitStru;
			m_bitmap[k].GetBitmap(&bitStru);

			int n = bitStru.bmWidthBytes / bitStru.bmWidth;
			int bitmapSize = bitStru.bmHeight *  bitStru.bmWidthBytes;// lineBytes;
			BYTE *px = (BYTE *)GlobalAlloc(GPTR, bitmapSize);
			m_bitmap[k].GetBitmapBits(bitmapSize, px);

			int    nBytePreLine, head;
			nBytePreLine = (bitStru.bmWidth * n + 1) / 2 * 2;        //    一般是4字节对齐, GetBitmapBits实际对齐是2
			for (int i = 0; i<bitStru.bmHeight - 1; i++)
			{
				if (i == 0)continue;
				head = nBytePreLine*i;
				for (int j = 0; j<bitStru.bmWidth - 1; j++)
				{
					if (j == 0)continue;
					px[head + j * n] = GetBValue(color);
					px[head + j * n + 1] = GetGValue(color);
					px[head + j * n + 2] = GetRValue(color);
				}
			}
			m_bitmap[k].SetBitmapBits(bitmapSize, px);
			pSubMenu1->SetMenuItemBitmaps(k, MF_BYPOSITION, &m_bitmap[k], &m_bitmap[k]);
		}
		ClientToScreen(&point);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
}



//功能函数

//对象捕捉
bool  CSlopeSView::PointHitTest(CPoint origin, CPoint point)//检测函数,捕捉闭合点
{
	CRect rc;
	rc.SetRect(origin.x - 5, origin.y - 5, origin.x + 5, origin.y + 5);//以point为中心建边长为10的矩形;
	if (rc.PtInRect(point)){//检测origin是否在建立的矩形内;
		bihe = true;
		return true;
	}
	bihe = false;
	return false;
}
//检测函数,XY轴对象捕捉
bool  CSlopeSView::XYHitTest(CPoint origin, CPoint point)
{
	if (origin.x - point.x <= 5 && origin.x - point.x >= -5) 
	{
		xypoint.x = origin.x;
		xypoint.y = point.y;
		xybz = true;
		return true;
	}
	else if (origin.y - point.y <= 5 && origin.y - point.y >= -5)
	{
		xypoint.y = origin.y;
		xypoint.x = point.x;
		xybz = true;
		return true;
	}
	else xybz = false;
	return false;
}

//检测函数,捕捉是否靠近边界线
bool  CSlopeSView::LineHitTest(CPoint point)
{
	for (int i = 0; i < m_crt_p - 1; i++)//靠近边界拐点
	{
		CPoint p = m_nzValues[i];
		double d = sqrt((p.x - point.x) *(p.x - point.x) + (p.y - point.y)*(p.y - point.y));
		if (d < 5.0 && d>-5.0)
		{
			linepoint.x = p.x;
			linepoint.y = p.y;
			Linebz = true;
			return true;
		}
	}
	for (int i = 0; i < m_crt_p - 1; i++){//遍历每条线段
		double a1, b1, c1, d, a2, b2, c2, k1, k2, ju1, ju2;
		int x1, x2, y1, y2;
		a1 = b1 = c1 = d = a2 = b2 = c2 = k1 = k2 = ju1 = ju2 = 0.0; x1 = x2 = y1 = y2 = 0;
		CPoint p1, p2, m1, m2;
		p1 = m_nzValues[i]; p2 = m_nzValues[i + 1];
		CRect rc;//坐标轴是向右下方的。。。。
		x1 = min(p1.x, p2.x); x2 = max(p1.x, p2.x); y1 = min(p1.y, p2.y); y2 = max(p1.y, p2.y);
		rc.SetRect(x1 - 5, y1 - 5, x2 + 5, y2 + 5);//左，上，右，下(顺时针)
		a1 = p2.y - p1.y; b1 = p1.x - p2.x; c1 = p2.x*p1.y - p2.y*p1.x;//a1,b1,c1为边界线数据
		d = (a1*point.x + b1*point.y + c1) *1.0 / sqrt(a1*a1 + b1*b1);
		if (m_pcrt[m_tid]>0){//采用鼠标点延长线与边界相交的方式
			m1 = m_fjxpoint[m_tid][m_pcrt[m_tid] - 1]; m2 = point;
			a2 = m2.y - m1.y; b2 = m1.x - m2.x; c2 = m2.x*m1.y - m2.y*m1.x;//a2,b2,c2为材料线数据
			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)){//靠近了边界线
				if (b1 == 0){//边界线斜率为无穷大,k1=无穷
					k2 = -a2 / b2; ju2 = a2*m1.x*1.0 / b2 + m1.y;
					linepoint.x = p1.x;
					linepoint.y = (int)(k2*linepoint.x + ju2);
				}
				else if (b2 == 0){//材料线斜率为无穷大,k2=无穷
					k1 = -a1 / b1; ju1 = a1*p1.x*1.0 / b1 + p1.y;
					linepoint.x = m1.x;
					linepoint.y = (int)(k1*linepoint.x + ju1);
				}
				else{//边界线及材料线斜率不为无穷大，采用两线相交的方式
					k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
					k2 = -a2*1.0 / b2; ju2 = a2*m1.x*1.0 / b2 + m1.y;
					if (fabs(k1) > fabs(k2)){//边界线斜率大，先求y后求x
						if (k2 == 0){
							linepoint.y = (int)ju2;
							linepoint.x = (int)((ju2 - ju1)*1.0 / k1);
						}
						else{
							linepoint.y = (int)((k2*ju1 - k1*ju2)*1.0 / (k2 - k1));
							linepoint.x = (int)(((k2*ju1 - k1*ju2)*1.0 / (k2 - k1) - ju1)*1.0 / k1);//不能用y直接求x
						}
					}
					else{//材料线斜率大，先求x后求y
						if (fabs(k1) < 0.03){
							linepoint.x = point.x;
							linepoint.y = (int)(k1*point.x + ju1);
						}
						else{
							linepoint.x = (int)((ju2 - ju1)*1.0 / (k1 - k2));
							linepoint.y = (int)(k1*(ju2 - ju1)*1.0 / (k1 - k2) + ju1);//不能用x直接求y
						}
					}
				}
				//当linepoint没落在边界线范围时
				if (!rc.PtInRect(linepoint)){//取鼠标所在点y值
					linepoint.y = point.y;
					if (b1 != 0){
						k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
						linepoint.x = (int)((point.y - ju1)*1.0 / k1);
					}
					else linepoint.x = p1.x;
					if (!rc.PtInRect(linepoint)){//取鼠标所在点x值
						k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
						linepoint.x = point.x;
						linepoint.y = (int)(k1*point.x + ju1);
						if (!rc.PtInRect(linepoint)){
							Linebz = false;
							return false;
						}
					}
				}
				Linebz = true;
				return true;
			}
		}
		else{//所画的第一个点，采用从鼠标点向边界线做垂线的方式

			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)){
				if (b1 == 0){
					linepoint.x = p1.x;
					linepoint.y = point.y;
				}
				else{
					k1 = -a1*1.0 / b1; ju1 = a1*p1.x*1.0 / b1 + p1.y;
					linepoint.x = (int)((point.x + k1*point.y - k1*ju1)*1.0 / (1 + k1*k1));
					linepoint.y = (int)(k1*linepoint.x + ju1);
				}
				Linebz = true;
				return true;
			}
		}
	}
	Linebz = false;
	return false;
}


//右键捕捉材料线
int  CSlopeSView::MLHitTest(CPoint point)
{
	for (int i = 0; i <= m_tid; i++)//第i条第j点
	{
		for (int j = 0; j < m_pcrt[i] - 1; j++)
		{
			CPoint p1 = m_fjxpoint[i][j], p2 = m_fjxpoint[i][j + 1];
			double a1, b1, c1, d, k1, ju1;
			int x1, x2, y1, y2;
			a1 = b1 = c1 = d = k1 = ju1 = 0.0; x1 = x2 = y1 = y2 = 0;
			CRect rc;
			x1 = min(p1.x, p2.x); x2 = max(p1.x, p2.x); y1 = min(p1.y, p2.y); y2 = max(p1.y, p2.y);
			rc.SetRect(x1 - 5, y1 - 5, x2 + 5, y2 + 5);//左，下，右，上
			a1 = p2.y - p1.y; b1 = p1.x - p2.x; c1 = p2.x*p1.y - p2.y*p1.x;
			d = (a1*point.x + b1*point.y + c1) / sqrt(a1*a1 + b1*b1);
			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)) return i;
		}
	}
	return -1;
}

//防止自动清屏（闪屏）
BOOL CSlopeSView::OnEraseBkgnd(CDC* pDC)//防止自动清屏（闪屏）
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return FALSE;
}

//允许菜单键//允许绘制边界线
void CSlopeSView::OnUpdateNewEb(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	// 根据m_bDraw的值设置是否激活   
	pCmdUI->Enable(m_bDraw[0]);
}

//允许绘制材料线
void CSlopeSView::OnUpdateNewM(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bDraw[1]);
}

//允许绘制方格
void CSlopeSView::OnUpdateAddL(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bDraw[2]);
}
void CSlopeSView::OnUpdateComt(CCmdUI *pCmdUI)//允许进行计算
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bDraw[3]);
}

//处理函数
int CSlopeSView::GetTopTC(CPoint p, CDC* pDC)
{
	HDC hDC = pDC->GetSafeHdc(); //获取当前屏幕DC
	COLORREF clr = ::GetPixel(hDC, p.x, p.y); //获取当前鼠标点像素值
	int color = clr;
	for (int i = 0; i < TUCENG_COUNT; i++){
		if (color == tuceng[i].clr)
			return i;
	}
	return 0;

}

//看p上方还有无点
void CSlopeSView::SearchToptcid(int tcid)
{
	CPoint p = RButtonUp;
	for (int i = 0; i < m_tid; i++){//第i条材料线
		for (int j = 0; j < m_pcrt[i] - 1; j++){//第k段线
			CPoint p1, p2;//startp,nextp
			p1 = m_fjxpoint[i][j]; p2 = m_fjxpoint[i][j + 1];
			double xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x);
			double ju = p1.y - xlv*p1.x;
			int y = (int)(p.x*xlv + ju);
			if (p1.x <= p.x && p.x <= p2.x){
				if (y<p.y){
					return;
				}
				break;
			}
		}
	}
	toptcid = tcid;
}

//搜索土层上下材料线
void CSlopeSView::SetSXtuceng(CDC* pDC){
	for (int tid = 0; tid < m_tid; tid++){
		CPoint p1, p2, p3, p4;
		//	if (m_pcrt[tid] == 2){//只有两个点
		p1 = m_fjxpoint[tid][0]; p2 = m_fjxpoint[tid][1];
		double xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x);
		double ju = p1.y - xlv*p1.x;
		p3.x = (int)((p1.x + p2.x)*0.5 + 0.5); p4.x = (int)((p1.x + p2.x)*0.5 + 0.5);
		p3.y = (int)(xlv*p3.x + ju + 0.5) + 3;//线中点下点
		p4.y = (int)(xlv*p4.x + ju + 0.5) - 3;//线中点上点
		buttomtc[tid] = checkcolor(p3, 1, pDC);//材料线的下方土为p3点所在土层
		toptc[tid] = checkcolor(p4, -1, pDC);//材料线的上方土为p4点所在土层
	}
}

//坐标转换，原坐标轴是x向右，y向下的，变更为x向上，y向左//x，y为相对坐标
double CSlopeSView::GetAng(double x, double y)
{
	double z = x; x = -y; y = -z;
	if (x>0 && y>0){ return atan(y / x); }
	else if (x < 0 && y>0){ x = -x; return pi - atan(y / x); }
	else if (x < 0 && y < 0){ x = -x; y = -y; return pi + atan(y / x); }
	else if (x>0 && y<0){ y = -y; return 2 * pi - atan(y / x); }
	else if (x == 0){
		if (y == 0)return 0;
		else if (y>0) return pi / 2;
		else return 3 / 2 * pi;
	}
	else if (y == 0){
		if (x >= 0) return 0;
		else return pi;
	}
	return 0;
}

//要求:p.x= x , p.y< y,返回x落在边界上的y值
int CSlopeSView::GetBoundPoint(double x, double y){
	//注意处理垂直边界线p1.x=p2.x
	for (int i = 0; i < m_crt_p - 1; i++){
		CPoint p1, p2;//边界线的左右点
		p1 = m_nzValues[i]; p2 = m_nzValues[i + 1];
		if ((int)x<min(p1.x, p2.x) || (int)x>max(p1.x, p2.x))continue;
		if (p1.x == p2.x){
			if ((int)x == p1.x){
				if (y>max(p1.y, p2.y))return max(p1.y, p2.y);
				else if (y >= min(p1.y, p2.y) && y <= max(p1.y, p2.y))return (int)y;
			}
		}
		else{
			int yl = (int)(p1.y + (p2.y - p1.y)*(x - p1.x)*1.0 / (p2.x - p1.x));
			if ((int)yl <= (int)y)return yl;
		}
	}
	return (int)y;
}

//按升序排序函数(坐标轴是向右下方)
int cmp(const CPoint &a, const CPoint &b)
{
	if (a.x < b.x) return 1;
	else if (a.x>b.x) return 0;
	else{
		if (a.y >= b.y) return 1;
		else return 0;
	}
}

int CSlopeSView::checkcolor(CPoint p, int sx, CDC* pDC){
	int id = -1;
	HDC hDC = pDC->GetSafeHdc(); //获取当前屏幕DC
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
	do{
		COLORREF clr = ::GetPixel(hDC, p.x, p.y); //获取当前鼠标点像素值
		int color = clr;
		for (int i = 0; i < TUCENG_COUNT; i++){
			if (color == tuceng[i].clr){
				DeleteObject(rg);
				return id = i;
			}
		}
		p.y += sx;
	} while (id == -1 && PtInRegion(rg, p.x, p.y));
	DeleteObject(rg);
	return id;
}

//面积计算
double CalAre(double k1, double b1, double k2, double b2, int x1, int x2){
	return fabs((k1 - k2)*(x2*x2 - x1*x1)*0.5 + (b1 - b2)*(x2 - x1));
}

//拦截回车键
BOOL CSlopeSView::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		HWND hWnd = ::GetFocus(); /*   判断当前焦点是否在编辑框上    */
		if (::GetDlgCtrlID(hWnd) == IDC_POINT_EDIT){
			// 添加对消息处理的代码，做你想做的事情!
			CEdit * pedit = (CEdit*)GetDlgItem(IDC_POINT_EDIT);
			CString str, str1, str2;
			UpdateData(TRUE);
			pedit->GetWindowText(str);
			int index = str.Find(_T(","));   //找.的位置
			if (index != -1){
				int l = str.GetLength();//获取字符长，中文也算1个字符
				str1 = str.Left(index);
				str2 = str.Right(l - index - 1);
				int x = _wtoi(str1);
				int y = _wtoi(str2);
				if (x >= 0 && x <= 1362 && y >= 0 && y <= 610)
				{//点在用户界面区
					//x - m_translateX, m_translateY - y 
					x += m_translateX; y = m_translateY - y;
					x = (x + m_off_x)*m_scale;
					y = (y + m_off_y)*m_scale;
					//str.Format(_T("x=%d,y=%d"), x, y);
					//AfxMessageBox(str);//弹出消息
					::SendMessage(m_hWnd, WM_LBUTTONUP, 0, MAKELONG(x, y));//模拟鼠标左键
				}
				else
					AfxMessageBox(_T("请注意输入范围: -m_translateX<=x<=1042, -100<=y<=m_translateY "));
				pedit->SetWindowText(_T(""));
			}
			else
				AfxMessageBox(_T("请注意输入格式: x,y "));
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

//寻找交点
void CSlopeSView::FindAllCrossPoint(int n, CPoint p0, double bj){
	struct fgss  &cntfg = fgss[n];
	for (int i = 0; i < m_crt_p - 1; i++)
	{//1.找圆心为p0,半径为bj的圆与边界的交点
		if ((i == bianjie[0] && bianjie[0] != -1) || (i == bianjie[1] && bianjie[1] != -1) || (i == bianjie[2] && bianjie[2] != -1))
			continue;
		double  xlv, ju;//斜率，截距
		int x1, x2, y1, y2;//(x1,y1),(x2,y2)为两组解
		int xl, xr, yu, yd;//左，右，下，上
		double a, b, c, dta;//二项式a，b，c，dta
		xlv = ju = a = b = c = dta = 0; x1 = x2 = y1 = y2 = xl = xr = yu = yd = 0;
		CPoint p1, p2;//边界线的左右点
		p1 = m_nzValues[i]; p2 = m_nzValues[i + 1];
		if (p1.x != p2.x){//斜率存在
			xl = min(p1.x, p2.x); xr = max(p1.x, p2.x);
			xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x); ju = p1.y - xlv*p1.x;
			//圆心是p1
			a = xlv*xlv + 1; b = 2 * xlv*(ju - p0.y) - 2 * p0.x; c = p0.x*p0.x + (ju - p0.y)*(ju - p0.y) - bj*bj;
			dta = b*b - 4 * a*c;
			if (dta <= 0)continue;
			else{
				x1 = (int)((-b + sqrt(dta)) / 2 / a); x2 = (int)((-b - sqrt(dta)) / 2 / a);
				if (x1 >= xl&&x1 < xr){
					cntfg.p[cntfg.cnt].p.x = x1;
					cntfg.p[cntfg.cnt].p.y = (int)(xlv*x1 + ju);
					cntfg.cnt++;
				}
				if (x2 >= xl&&x2 < xr){
					cntfg.p[cntfg.cnt].p.x = x2;
					cntfg.p[cntfg.cnt].p.y = (int)(xlv*x2 + ju);
					cntfg.cnt++;
				}
			}
		}
		else{//斜率不存在
			yu = min(p1.y, p2.y); yd = max(p1.y, p2.y);
			y1 = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
			y2 = (int)(p0.y - sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
			if (y1 >= yu&&y1 < yd){
				cntfg.p[cntfg.cnt].p.x = p1.x;
				cntfg.p[cntfg.cnt].p.y = y1;
				cntfg.cnt++;
			}
			if (y2 >= yu&&y2 < yd){
				cntfg.p[cntfg.cnt].p.x = p1.x;
				cntfg.p[cntfg.cnt].p.y = y2;
				cntfg.cnt++;
			}
		}
	}
}
//对每段圆弧进行计算
   void CSlopeSView::Calculate(int n, int f, CPoint p0, double bj, CDC* pDC){
	struct fgss  &cntfg = fgss[n];
	struct point &start = cntfg.p[f];//开始点（按逆时针得到的）
	struct point &end = cntfg.p[f + 1];//结束点

	double cosa[30], sina[30], alf[30], tgf[30], fai[30], A[30], W[30], L[30], cl[30], m[30], cd[30], E[30];
	CPoint pp1, pp2;//圆弧的左右端点
	
	//判断哪个是左点哪个是右点
	if (start.p.x < end.p.x)
	{ 
		pp1 = start.p; pp2 = end.p; 
	}
	else
	{ 
		return; 
	}

	double b;//土条宽
	b = (pp2.x - pp1.x)*1.0 / tks;
	for (int i = 0; i < tks; i++)
	{//对每个条块(第i个条块)
		cosa[i] = sina[i] = alf[i] = tgf[i] = fai[i] = A[i] = W[i] = L[i] = cl[i] = m[i] = cd[i] = E[i] = 0.0;
		double line[20], are[20];//记录每个土层在土条底层所占长,及土条内所占面积
		for (int j = 0; j < 10; j++)
		{
			are[j] = line[j] = 0.0;
		}
		CPoint tkp[100];//每个条块围成的不规则区域
		CPoint sbj[100];//记录条块上边界线
		int tkpid, sbjpid;

		CPoint p1, p2, p3, p4; //条块的下左，下右，上左，上右点
		p1.x = (int)(pp1.x + b*i);//下左
		if (i == 0)p1.y = pp1.y;
		else p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));//取下方的点
		p2.x = (int)(pp1.x + b*(i + 1));//下右
		if (i == tks - 1)p2.y = pp2.y;
		else p2.y = (int)(p0.y + sqrt(bj*bj - (p2.x - p0.x)*(p2.x - p0.x)));
		p3.x = p1.x;//上左
		p3.y = GetBoundPoint(p1.x, p1.y);
		p4.x = p2.x;//上右
		p4.y = GetBoundPoint(p2.x, p2.y);
		tkp[0] = p3;//记录条块边界的多边形（从上左开始顺时针）
		tkpid = 1;
		double xl = p1.x, xr = p2.x;//条块的左右x坐标
		double yd = max(p1.y, p2.y), yu = min(p3.y, p4.y);
		//条块下上y坐标
		//效验边界线点是否在条块内(遍历所有边界点)
		for (int j = 0; j < m_crt_p - 1; j++)//1.x在xl~xr  2.y>line(p1,p2)
		{
			if ((j == bianjie[0] && bianjie[0] != -1) || (j == bianjie[1] && bianjie[1] != -1) || (j == bianjie[2] && bianjie[2] != -1))
				continue;
			if (m_nzValues[j].x>xl && m_nzValues[j].x < xr && m_nzValues[j].y < p1.y + (p2.y - p1.y)*(m_nzValues[j].x - p1.x) / (p2.x - p1.x))
				tkp[tkpid++] = m_nzValues[j];
		}
		std::sort(tkp, tkp + tkpid, cmp);//按逆时针排序
		tkp[tkpid++] = p4; //找出条块上边界线
		memcpy(sbj, tkp, sizeof(CPoint)*tkpid); sbjpid = tkpid;
		if (p2.y != p4.y)tkp[tkpid++] = p2; if (p1.y != p3.y) tkp[tkpid++] = p1;//补齐下边界线（仅两点）
		tkp[tkpid++] = p3;//闭合

		double s = 0;	//double s1=0;
		for (int j = 0; j < tkpid - 1; j++)
		{
			s += (tkp[j].x*  tkp[j + 1].y - tkp[j + 1].x*tkp[j].y)*0.5;//（计算三角形面积法）//s1 += (tkp[j+1].x- tkp[j].x)*(tkp[j + 1].y+tkp[j].y)*0.5;//（定积分法）
			
		}
		s = fabs(s);//总面积
		A[i] = s;
		//m_fjxpoint[m_tid][m_pcrt[m_tid]] 第m_tid条共m_pcrt[m_tid]个点
		//条块与所有材料线相交

		double xlv0, ju0;//底线的斜率和截距
		xlv0 = (p1.y - p2.y)*1.0 / (p1.x - p2.x);
		ju0 = p1.y - xlv0*p1.x;

		struct mianji
		{
			int yl;//材料线左端点
			double s;//这个面积是材料线与条块下边缘围成的面积
			int id;//材料线id
			bool   operator <  (const  mianji&   rhs)  const   //升序排序时必须写的函数
			{
				return   yl < rhs.yl;
			}
		}xj[POINT_COUNT];//这个记录条块与材料线相交各参数
		int xjid = 0;//记录土条与材料线相交数

		for (int j = 0; j < m_tid; j++){//第j条材料线
			double xjs = 0;//相交面积
			bool zuos; //左点是否在上方
			bool cross = false;//有没有与此材料线相交
			if (m_fjxpoint[j][0].x >= p2.x)continue;//材料线在条块右端（无交点）
			else if (m_fjxpoint[j][0].x>p1.x && m_fjxpoint[j][0].x < p2.x){//特殊情况：材料线起点在条块内
				zuos = true; cross = true;
				for (int k = 0; k < sbjpid - 1; k++){
					CPoint ps, pn;//startp,nextp
					ps = sbj[k]; pn = sbj[k + 1];
					double xlv = (pn.y - ps.y)*1.0 / (pn.x - ps.x);
					double ju = pn.y - xlv*pn.x;
					if (m_fjxpoint[j][0].x>ps.x && m_fjxpoint[j][0].x < pn.x){
						xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, m_fjxpoint[j][0].x);
						break;
					}
					else if (m_fjxpoint[j][0].x >= pn.x)
						xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, pn.x);
				}
			}
			//int id1 = -1, id2 = -1;
			int yy1, yy2;//p1.x与p2.x在材料线上相应的y值

			int beforey = p1.y;//底线前点

			//3.用这条材料线与底边缘线相交，得到面积
			for (int k = 0; k < m_pcrt[j] - 1; k++){
				CPoint ps, pn;//startp,nextp
				ps = m_fjxpoint[j][k]; pn = m_fjxpoint[j][k + 1];
				if (pn.x <= p1.x)continue; //线段在条块左端（无交点）
				else if (ps.x >= p2.x)break;//线段在条块右端（无交点）
				double xlv = (pn.y - ps.y)*1.0 / (pn.x - ps.x);
				double ju = pn.y - xlv*pn.x;
				if (ps.x <= p1.x && p1.x < pn.x){//需替换前点
					ps.x = p1.x;
					ps.y = (int)(xlv*p1.x + ju + 0.5);
					yy1 = ps.y;
					if (ps.y < p1.y)zuos = true;
					else zuos = false;
				}
				if (ps.x <= p2.x && p2.x < pn.x){//需替换后点
					pn.x = p2.x;
					pn.y = (int)(xlv*p2.x + ju + 0.5);
					yy2 = pn.y;
				}

				int y0 = (int)(xlv0*pn.x + ju0 + 0.5);//底线后点
				if (zuos && pn.y < y0)
				{//两个点都在上方
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, pn.x);
					cross = true;
				}
				else if (zuos &&pn.y >= y0)
				{//左上右下
					int x = (int)((ju - ju0) / (xlv0 - xlv) + 0.5); int y = (int)(xlv*x + ju + 0.5);
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, x);
					line[toptc[j]] += sqrt((pn.x - x)*(pn.x - x) + (y0 - y)*(y0 - y));
					line[buttomtc[j]] += sqrt((ps.x - x)*(ps.x - x) + (beforey - y)*(beforey - y));
					zuos = false;
					cross = true;
				}
				else if (!zuos&& pn.y < y0)
				{//左下右上
					int x = (int)((ju - ju0) / (xlv0 - xlv) + 0.5); int y = (int)(xlv*x + ju + 0.5);
					xjs += CalAre(xlv, ju, xlv0, ju0, x, pn.x);
					line[buttomtc[j]] += sqrt((pn.x - x)*(pn.x - x) + (y0 - y)*(y0 - y));
					line[toptc[j]] += sqrt((ps.x - x)*(ps.x - x) + (beforey - y)*(beforey - y));
					zuos = true;
					cross = true;
				}
				beforey = y0;
			}
			if (cross)
			{
				xj[xjid].s = xjs;//此材料线到条块底线的相交面积
				xj[xjid].id = j;//得到材料线id
				xj[xjid].yl = yy2;
				xjid++;
			}
		}
		std::sort(xj, xj + xjid);//按从上往下对交得的材料线排序
		for (int j = 0; j < xjid; j++)
		{
			if (j == 0)are[toptc[xj[j].id]] += s - xj[j].s;
			else are[toptc[xj[j].id]] += xj[j - 1].s - xj[j].s;//计算材料线之间的面积（即土层面积）
		}
		double totl = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));//总的土条底边长
		L[i] = totl;
		for (int j = 0; j < 10; j++){
			totl -= line[j];//剩下长
		}
		if (m_tcnt>0 && xjid>0){//画了材料线 且 土条与材料线有交点
			are[buttomtc[xj[xjid - 1].id]] += xj[xjid - 1].s;//最下的面积给最下材料线的下方土层
			if (totl > 0)line[buttomtc[xjid - 1]] += totl;//剩下的长度给最下材料线的下方土层
		}
		else if (m_tcnt > 0 && xjid == 0){//画了材料线 但 土条与材料线无交点
			CPoint p;
			p.x = (p1.x + p2.x) / 2; p.y = (p1.y + p2.y) / 2;
			int topclx = GetTopTC(p, pDC);
			are[topclx] += s;
			line[topclx] += totl;
		}
		else if (m_tcnt <= 0)
		{//没画材料线
			are[toptcid] += s;
			line[toptcid] += totl;
		}
		fai[i] = 0;//加权平均fai值
		for (int j = 0; j < 10; j++)
		{
			fai[i] += tuceng[j].phi*line[j];
		}
		fai[i] /= L[i];
		cosa[i] = (p2.x - p1.x) / L[i];
		sina[i] = (p1.y - p2.y) / L[i];
		alf[i] = acos(cosa[i]);
		tgf[i] = tan(fai[i] * pi / 180);
		W[i] = 0; cl[i] = 0;
		for (int j = 0; j < 10; j++)
		{//把10个土层在一个条块内做的贡献加起来
			W[i] += are[j] * tuceng[j].gama;
			cl[i] += tuceng[j].c*line[j];
		}
		if (W[i] == 0)return;
	}
	double fenzi = 0; double fenmu = 0;//分子，分母
	double K, K0;
	double fsl, fsr;
	Mqytks = 0;
	double fs;
	switch (mode)
	{
	case 1:
		for (int i = 0; i < tks; i++)
		{
			fenzi += W[i] * tgf[i] * cosa[i] + cl[i];
			fenmu += W[i] * sina[i];
		}
		if (fenmu < 0.0001)return;
		K = fenzi / fenmu;//得到这一圆弧的K
		break;
	case 2:
		K0 = 1;
		for (int cishu = 0; cishu < 50; cishu++)
		{
			fenzi = 0; fenmu = 0;
			for (int i = 0; i < tks; i++)
			{
				m[i] = cosa[i] + 1.0 / K0*tgf[i] * sina[i];
				fenzi += (W[i] * tgf[i] + cl[i] * cosa[i]) / m[i];
				fenmu += W[i] * sina[i];
			}
			K = fenzi / fenmu;
			if (fabs(K - K0) < bishop)
			{
				break;
			}
			K0 = K;
		}
		break;

	case 3:
		fsl = 0.01, fsr = 6.0;//Fs的左右界
		E[tks] = 0;
		for (int i = tks - 1; i >= 0; i--)
		{
			if (i == tks - 1)cd[i] = 0;
			else cd[i] = cos(alf[i + 1] - alf[i]) - tgf[i] / fsl*(sin(alf[i + 1] - alf[i]));
			E[i] = W[i] * sina[i] - 1 / fsl*(W[i] * cosa[i] * tgf[i] + cl[i]) + E[i + 1] * cd[i];
		}
		if (E[0] >= 0)return;

		for (int i = tks - 1; i >= 0; i--)
		{
			if (i == tks - 1)cd[i] = 0;
			else cd[i] = cos(alf[i + 1] - alf[i]) - tgf[i] / fsr*(sin(alf[i + 1] - alf[i]));
			E[i] = W[i] * sina[i] - 1 / fsr*(W[i] * cosa[i] * tgf[i] + cl[i]) + E[i + 1] * cd[i];
		}
		if (E[0] <= 0)return;

		do{
			fs = (fsl + fsr) / 2.0;
			for (int i = tks - 1; i >= 0; i--)
			{
				if (i == tks - 1)cd[i] = 0;
				else cd[i] = cos(alf[i + 1] - alf[i]) - tgf[i] / fs*(sin(alf[i + 1] - alf[i]));
				E[i] = W[i] * sina[i] - 1 / fs*(W[i] * cosa[i] * tgf[i] + cl[i]) + E[i + 1] * cd[i];
			}
			if (E[0] >0) 
				fsr = fs;
			else
			{
				fsl = fs;
				if (E[0] == 0) break;
			}
		} while (fabs(E[0]) >= bph);//while (fabs(fsl - fsr) >= bph);




		K =fs;
		break;

	case 4:
		fsl = 0.01, fsr = 6.0;//Fs的左右界
		for (int i = 0; i <tks; i++)
		{
			if (i == 0)cd[i] = 0;
			else cd[i] = cos(alf[i] - alf[i - 1]) + tgf[i] / fsl*(sin(alf[i] - alf[i - 1]));
			E[i] = -W[i] * sina[i] + 1 / fsl*(W[i] * cosa[i] * tgf[i] + cl[i]);
			if (i>0)E[i] += E[i - 1] * cd[i];
		}
		if (E[tks - 1] <= 0)return;
		for (int i = 0; i <tks; i++)
		{
			if (i == 0)cd[i] = 0;
			else cd[i] = cos(alf[i] - alf[i - 1]) + tgf[i] / fsr*(sin(alf[i] - alf[i - 1]));
			E[i] = -W[i] * sina[i] + 1 / fsr*(W[i] * cosa[i] * tgf[i] + cl[i]);
			if (i>0)E[i] += E[i - 1] * cd[i];
		}
		if (E[tks - 1] >= 0)return;


		do{
			fs = (fsl + fsr) / 2.0;
			for (int i = 0; i <tks; i++){
				if (i == 0)cd[i] = 0;
				else cd[i] = cos(alf[i] - alf[i - 1]) + tgf[i] / fs*(sin(alf[i] - alf[i - 1]));
				E[i] = -W[i] * sina[i] + 1 / fs*(W[i] * cosa[i] * tgf[i] + cl[i]);
				if (i>0)E[i] += E[i - 1] * cd[i];
			}
			if (E[tks - 1] <0) fsr = fs;
			else{
				fsl = fs;
				if (E[tks - 1] == 0) break;
			}
		} while (fabs(E[tks - 1]) >= bph);
		K = fs;
		break;


		//牵引式边坡瑞典圆弧
	case 5:
		Mqytks = 0;
		TempK = 100;
		for (qytks = 1; qytks <= tks; qytks++)
		{
			for (int i = 0; i < tks; i++)
			{
				fenzi += W[i] * tgf[i] * cosa[i] + cl[i];
				fenmu += W[i] * sina[i];
			}
			if (fenmu < 0.0001)return;
			K = fenzi / fenmu;//得到这一圆弧的K
			if (TempK > K)
			{
				TempK = K;
				Mqytks = qytks;
			}
		}
		K = TempK;
		break;

		//牵引式边坡毕肖普
	case 6:
		Mqytks = 0;
		TempK = 100;
		for (qytks = 1 ;qytks <= tks; qytks++)
		{ 
			K0 = 1;
			for (int cishu = 0; cishu < 50; cishu++)
			{
				fenzi = 0; fenmu = 0;
				for (int i = 0; i < qytks; i++)
				{
					m[i] = cosa[i] + 1.0 / K0*tgf[i] * sina[i];
					fenzi += (W[i] * tgf[i] + cl[i] * cosa[i]) / m[i];
					fenmu += W[i] * sina[i];
				}
				K = fenzi / fenmu;
				if (fabs(K - K0) < bishop)
				{
					break;
				}
				K0 = K;
			}
			if (TempK > K)
			{
				TempK = K;
				Mqytks = qytks;
			}
		}
		K = TempK;
		break;


		//牵引式边坡不平衡推力
	case 7:
		Mqytks = 0;
		TempK = 100;
		for (qytks = 1; qytks <= tks; qytks++)
		{
			fsl = 0.01, fsr = 6.0;//Fs的左右界
			for (int i = 0; i < qytks; i++)
			{
				if (i == 0)cd[i] = 0;
				else cd[i] = cos(alf[i] - alf[i - 1]) + tgf[i] / fsl*(sin(alf[i] - alf[i - 1]));
				E[i] = -W[i] * sina[i] + 1 / fsl*(W[i] * cosa[i] * tgf[i] + cl[i]);
				if (i>0)E[i] += E[i - 1] * cd[i];
			}

			if (E[qytks - 1] <= 0)return;

			for (int i = 0; i < qytks; i++)
			{
				if (i == 0)cd[i] = 0;
				else cd[i] = cos(alf[i] - alf[i - 1]) + tgf[i] / fsr*(sin(alf[i] - alf[i - 1]));
				E[i] = -W[i] * sina[i] + 1 / fsr*(W[i] * cosa[i] * tgf[i] + cl[i]);
				if (i>0)E[i] += E[i - 1] * cd[i];
			}
			if (E[qytks - 1] >= 0)return;
			double fs;
			do{
				fs = (fsl + fsr) / 2.0;
				for (int i = 0; i < qytks; i++)
				{
					if (i == 0)cd[i] = 0;
					else cd[i] = cos(alf[i] - alf[i - 1]) + tgf[i] / fs*(sin(alf[i] - alf[i - 1]));
					E[i] = -W[i] * sina[i] + 1 / fs*(W[i] * cosa[i] * tgf[i] + cl[i]);
					if (i>0)E[i] += E[i - 1] * cd[i];
				}
				if (E[qytks - 1] < 0 ) 
					fsr = fs;
				else
				{
					fsl = fs;
					if (E[qytks - 1] == 0) break;
				}
			} while (fabs(E[qytks - 1]) >= bph);//();

			K = fs;
			
			if (TempK > K)
			{
				TempK = K;
				Mqytks = qytks;
			}
		}
		K = TempK;
		break;
	}


	if (K < cntfg.k && K>0)
	{
		
		cntfg.k = K;
		cntfg.ptStart = pp1;
		cntfg.ptEnd = pp2;
		cntfg.p0 = p0;
		cntfg.bj = bj;
		cntfg.qytks = Mqytks;
		for (int i = 0; i < tks; i++)
		{
			cntfg.t[i].cl = cl[i];
			cntfg.t[i].fai = fai[i];
			cntfg.t[i].alf = alf[i];
			cntfg.t[i].are = A[i];
			cntfg.t[i].sina = sina[i];
			cntfg.t[i].cosa = cosa[i];
			cntfg.t[i].W = W[i];
			cntfg.t[i].line = L[i];
		}
	}
}


//右键进程

//（右键）添加边界线
void CSlopeSView::OnAddMl()
{
	// TODO:  在此添加命令处理程序代码
	m_nDrawType = 4;
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//十字光标
	m_bDraw[1] = true;
}
//（右键）删除边界线
void CSlopeSView::OnDelete()
{
	// TODO:  在此添加命令处理程序代码
	m_pcrt[m_delete_id] = 0;
	m_tcnt--;
}

//土层
void CSlopeSView::OnCeng1()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[0].clr;
	OnDraw(pDC);
	SearchToptcid(0);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng2()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[1].clr;
	OnDraw(pDC);
	SearchToptcid(1);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng3()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[2].clr;
	OnDraw(pDC);
	SearchToptcid(2);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng4()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[3].clr;
	OnDraw(pDC);
	SearchToptcid(3);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng5()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[4].clr;
	OnDraw(pDC);
	SearchToptcid(4);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng6()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[5].clr;
	OnDraw(pDC);
	SearchToptcid(5);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng7()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[6].clr;
	OnDraw(pDC);
	SearchToptcid(6);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng8()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[7].clr;
	OnDraw(pDC);
	SearchToptcid(7);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng9()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[8].clr;
	OnDraw(pDC);
	SearchToptcid(8);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng10()
{
	// TODO:  在此添加命令处理程序代码
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[9].clr;
	OnDraw(pDC);
	SearchToptcid(9);
	ReleaseDC(pDC);
}

//控制总进程

//开始画边界线
void CSlopeSView::OnNewEb()
{
	// TODO:  在此添加命令处理程序代码
	m_nDrawType = 1;//开始画边界线
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//十字光标
	CEdit* pedit = & m_EditPoint;
	if (FirstFocus){
		ASSERT(pedit->Create(WS_CHILD | WS_VISIBLE, CRect(1062, 615, 1262, 630), this, IDC_POINT_EDIT));
		FirstFocus = false;
	}
	pedit->ShowWindow(SW_SHOW);
}

//开始画材料线
void CSlopeSView::OnNewM()
{
	// TODO:  在此添加命令处理程序代码
	m_nDrawType = 4;//开始画材料线
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//十字光标
	if (!FirstFocus){
		CEdit* pedit = & m_EditPoint;
		pedit->ShowWindow(SW_HIDE);//Test时要去掉
	}
}


//相关设定

//设定土层参数
void CSlopeSView::OnParameter()
{
	// TODO:  在此添加命令处理程序代码
	Ctab dlg;//创建Ctab类对话框
	dlg.pTuCeng = pTuCeng;
	dlg.pFocus = pFocus;
	for (int i = 0; i < TUCENG_COUNT; i++){
		dlg.m_tuceng[i].phi = tuceng[i].phi;
		dlg.m_tuceng[i].c = tuceng[i].c;
		dlg.m_tuceng[i].gama = tuceng[i].gama;
		dlg.m_tuceng[i].name = tuceng[i].name;
		dlg.m_tuceng[i].clr = tuceng[i].clr;
	}
	if (IDOK == dlg.DoModal()){//点击OK
		for (int i = 0; i < TUCENG_COUNT; i++){
			if (dlg.m_tuceng[i].phi >= 0)
				tuceng[i].phi = dlg.m_tuceng[i].phi;
			else tuceng[i].phi = 0;
			if (dlg.m_tuceng[i].c >= 0)
				tuceng[i].c = dlg.m_tuceng[i].c;
			else tuceng[i].c = 0;
			if (dlg.m_tuceng[i].gama >= 0)
				tuceng[i].gama = dlg.m_tuceng[i].gama;
			else tuceng[i].gama = 0;
			tuceng[i].name = dlg.m_tuceng[i].name;
			tuceng[i].clr = dlg.m_tuceng[i].clr;
			dlg.pTuCeng = pTuCeng;
			dlg.pFocus = pFocus;
		}
	}
}

//设定精度
void CSlopeSView::OnSearch()
{
	// TODO:  在此添加命令处理程序代码
	Cjdsd dlg;
	dlg.bianchang = jd_fg;
	dlg.banjing = jd_yh;
	dlg.tiaokuaishu = tks;
	dlg.check1 = check1;
	if (IDOK == dlg.DoModal()){
		if (dlg.bianchang>5 && dlg.bianchang <= 20)
			jd_fg = dlg.bianchang;
		else jd_fg = 5;
		if (dlg.banjing>3 && dlg.banjing <= 20)
			jd_yh = dlg.banjing;
		else jd_yh = 3;
		if (dlg.tiaokuaishu>1 && dlg.tiaokuaishu <= 20)
			tks = dlg.tiaokuaishu;
		else tks = 10;
		if (m_DataDlg != NULL) m_DataDlg->tks = tks;
		check1 = dlg.check1;
	}
}

//采用计算模型
void CSlopeSView::OnUsM()
{
	// TODO:  在此添加命令处理程序代码
/*CMethod dlg;
	dlg.mode = mode;
	dlg.m_bishop = bishop;
	dlg.m_bph = bph;
	if (IDOK == dlg.DoModal())
	{
		mode = dlg.mode;
		bishop = dlg.m_bishop;
		bph = dlg.m_bph;
	}*/
		CMethod dlg;
	   dlg.mode = mode;
		dlg.ks=ks;
		dlg.kn=kn  ;	
	if (IDOK == dlg.DoModal())
	{
		mode = dlg.mode;
		ks = dlg.ks;
		kn = dlg.kn;	
	}


}

//搜索区域设定
void CSlopeSView::OnAddL()
{
	// TODO:  在此添加命令处理程序代码
	m_nDrawType = 6;
	crt_fg = 0;//归零
	m_bDraw[3] = false;
}



//进行计算

void CSlopeSView::OnComt()
{
	// TODO:  在此添加命令处理程序代码
	m_nDrawType = 7;
	totfg = 0;
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	OnDraw(pDC);
	//记录材料线上下土层
	Sleep(100);
	SetSXtuceng(pDC);
	buttomtc[0];//材料线的下方土为p3点所在土层
	toptc[0];
	int maxy = 0, maxx = 0, minx = 10000;
	memset(bianjie, -1, sizeof(bianjie));
	for (int i = 0; i < m_crt_p - 1; i++){
		if (m_nzValues[i].y + m_nzValues[i + 1].y>maxy){
			maxy = m_nzValues[i].y + m_nzValues[i + 1].y;
			bianjie[0] = i;//下边界线
		}
		if (m_nzValues[i].x + m_nzValues[i + 1].x > maxx){
			maxx = m_nzValues[i].x + m_nzValues[i + 1].x;
			bianjie[1] = i;//右边界线
		}
		if (check1 == 1 && m_nzValues[i].x + m_nzValues[i + 1].x < minx){
			minx = m_nzValues[i].x + m_nzValues[i + 1].x;
			bianjie[2] = i;//左边界线
		}
	}

	double dbk = (m_nzValues[bianjie[0]].y - m_nzValues[bianjie[0] + 1].y)*1.0 / (m_nzValues[bianjie[0]].x - m_nzValues[bianjie[0] + 1].x);
	double dbb = m_nzValues[bianjie[0]].y - dbk*m_nzValues[bianjie[0]].x;
	double maxbj;
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
	CPoint p0;//p0为圆心
	double ban = (double)(jd_fg) / 2.0;
	double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//共k1+1列小方格
	double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//共k2+1行小方格
	double min_k = 10000;//所有方格的minK
	int txt_num=1;
//	clock_t start,finish; 
//	start=clock(); 
	for (int i = 0; i <= k1; i++)//搜索到第i列小方格(0~k1+1)
	{   // ((i*k1+j)*k2+(k-1))*k3+l  文件编号
		p0.x = (int)(fg[1].x + i*jd_fg + ban);
		if (i == k1)p0.x = (int)((fg[2].x - fg[1].x - i*jd_fg) / 2.0 + fg[1].x + i*jd_fg);
		for (int j = 0; j <= k2; j++)//搜索到第j行小方格(0~k2+1)
		{
			int n = j*(k1 + 1) + i;//搜索到第n个小方格
			totfg++;
			struct fgss  &cntfg = fgss[n];//引用
			struct fgss  &cntfg1 = fgss[n];
			struct fgss  &cntfg2 = fgss[n];
			cntfg.k = 10000;//这一方格的minK
			p0.y = (int)(fg[1].y + j*jd_fg + ban);
			if (j == k2)p0.y = (int)((fg[2].y - fg[1].y - j*jd_fg) / 2.0 + fg[1].y + j*jd_fg);
			//这就已经找到了圆心p0,接下来找半径。
			//最大半径应该是圆心到底边的距离
			maxbj = fabs(dbk*p0.x - p0.y + dbb) / sqrt(1 + dbk*dbk);
			int k3 = (int)maxbj / jd_yh, bj;

			for (int k = 1; k <= k3; k++)
			{
				cntfg.cnt = 0;//清零（对每个半径）
				bj = k*jd_yh;
				//半径为bj，接下来搜索圆弧与边界线所有的交点。
				//画圆弧的方法：找与边界的交点→交点按圆心角排序（逆时针：根据斜率与象限判断）→判断相邻交点在圆上的中点是否在边界内→如果在边界内绘制圆弧线。

				FindAllCrossPoint(n, p0, bj);//1.找圆心为p0,半径为bj的圆与边界的交点

				for (int l = 0; l < cntfg.cnt; l++)//2.交点按圆心角排序（逆时针：根据斜率与象限判断）
				{
					double x = cntfg.p[l].p.x - p0.x, y = cntfg.p[l].p.y - p0.y;
					cntfg.p[l].ang = GetAng(x, y);
				}
				std::sort(cntfg.p, cntfg.p + cntfg.cnt);

				for (int l = 0; l < cntfg.cnt - 1; l++)//3.判断相邻交点在圆上的中点是否在边界线以内
				{
					double ang = (cntfg.p[l].ang + cntfg.p[l+1].ang) / 2.0;
					int x = (int)(cos(ang)*bj + 0.5), y = (int)(sin(ang)*bj + 0.5);
					int z = x; x = -y; y = -z;//之前转换了坐标，这里转回来
					x += p0.x; y += p0.y;
					if (PtInRegion(rg, x, y))
					{//接下来对每段圆弧进行条分(一个半径下可能产生多段圆弧)
					//	double K1,K2;
						//oldmethod
				//		cntfg2=cntfg;
				//		Calculate(n, l, p0, bj, pDC);//第n个方格,圆心p0,半径bj,下第l条圆弧
				//		cntfg1=cntfg;
				//		cntfg=cntfg2;
						//new method
						if (txt_num%10==0)
						Calculate1(n, l, p0, bj, pDC,txt_num);
				//		cntfg1=cntfg;
						txt_num+=1;
						//cntfg2=cntfg;
					  //fout<<"      "<<double(cntfg2-cntfg1)<<endl;
					}
				}
		    }
			if (cntfg.k < min_k)
			{
				min_k = cntfg.k;
				minkid = n;
			}
		}
	}
	 //    finish=clock(); 
	 //    fout<<"      "<<double(finish-start)<<endl;
	ReleaseDC(pDC);
	DeleteObject(rg);
	m_nDrawType = 8;

	fgid = minkid;
	if (m_DataDlg != NULL)
	{
		Write();
	}
	
}

//条块数据显示

void CSlopeSView::OnShow()
{
	// TODO:  在此添加命令处理程序代码	
	if (m_nDrawType != 8)return;
	HWND hWnd = ::FindWindow(NULL, _T("条块数据"));
	if (m_DataDlg == NULL)
	{
		m_DataDlg = new CDataShow(this);
		m_DataDlg->Create(IDD_DataDlg, this);
		m_DataDlg->ShowWindow(SW_SHOW);
		m_DataDlg->m_pMain = (void *)this;
		m_DataDlg->tks = tks;
		m_DataDlg->tkid = tkid;
		CFont font;
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = -14;   //字体大小
		font.CreateFontIndirect(&lf);
		m_DataDlg->m_text.SetFont(&font);
		Write();
	}
	else
	{
		if (::IsWindowVisible(hWnd))
		{
			m_DataDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_DataDlg->tks = tks;
			m_DataDlg->tkid = tkid;
			m_DataDlg->ShowWindow(SW_SHOW);
			Write();
		}
	}
}

/*void CSlopeSView::Write()
{
	struct fgss  &cntfg = fgss[fgid];
	CString s0, s1, s2;
	s0.Format(_T("圆心(x,y)：(%d,%d)\r\n半径：%.2f\r\n最小安全系数:%.4f\r\n--------------------------------------------------------------\r\n条块%d\r\n--------------------------------------------------------------\r\n"), fgss[fgid].p0.x - m_translateX, m_translateY - fgss[fgid].p0.y, fgss[fgid].bj, fgss[fgid].k, tkid + 1);
	s1.Format(_T("重量：%.2f\r\nα:%.2f°\r\ncos(α)：%.2f\r\nsin(α)：%.2f\r\n加权平均φ：%.2f°\r\n面积：%.2f\r\n底边长：%.2f\r\n"), fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosa, fgss[fgid].t[tkid].sina, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
	double fenzi = 0, fenmu = 0;
	double E[30], cd[30];
	switch (mode)
	{
	case 1:
		for (int i = 0; i <= tkid; i++)
		{
			fenzi += cntfg.t[i].W * tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].cosa + cntfg.t[i].cl;
			fenmu += cntfg.t[i].W *cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\n瑞典条分法\r\n--------------------------------------------------------------\r\n∑W*cos(α)*tg(φ)+c*l：%.4f\r\n∑W*sin(α)：%.4f"), fenzi, fenmu);
		break;
	case 2:
		double m;
		for (int i = 0; i <= tkid; i++)
		{
			m = cntfg.t[i].cosa + 1.0 / cntfg.k* tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].sina;
			fenzi += (cntfg.t[i].W *tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl * cntfg.t[i].cosa) / m;
			fenmu += cntfg.t[i].W* cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\nBishop条分法\r\n--------------------------------------------------------------\r\nm=cos(α)+1/k*tg(φ)*sin(α)：%.4f\r\n∑1/m*(W*tg(φ)+c*l*cos(α))：%.4f\r\n∑W*sin(α)：%.4f"), m, fenzi, fenmu);
		break;
	case 3:
		for (int i = tks - 1; i >= tkid; i--)
		{
			if (i == tks - 1) cd[i] = 0;
			else cd[i] = cos(cntfg.t[i + 1].alf - cntfg.t[i].alf) - tan(cntfg.t[i].fai / 180 * pi) / cntfg.k*(sin(cntfg.t[i + 1].alf - cntfg.t[i].alf));
			E[i] = cntfg.t[i].W * cntfg.t[i].sina - 1 / cntfg.k*(cntfg.t[i].W * cntfg.t[i].cosa * tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl) + E[i + 1] * cd[i];
		}
		s2.Format(_T("--------------------------------------------------------------\r\n不平衡推力法\r\n--------------------------------------------------------------\r\n传递系数Ψi：%.4f\r\n不平衡推力Ei：%.4f\r\n"), cd[tkid], E[tkid]);
		break;
	case 4:
		for (int i = 0; i <= tkid; i++)
		{
			if (i == 0)cd[i] = 0;
			else cd[i] = cos(cntfg.t[i].alf - cntfg.t[i - 1].alf) + tan(cntfg.t[i].fai / 180 * pi) / cntfg.k*(sin(cntfg.t[i].alf - cntfg.t[i - 1].alf));
			E[i] = -cntfg.t[i].W * cntfg.t[i].sina + 1 / cntfg.k*(cntfg.t[i].W * cntfg.t[i].cosa * tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl);
			if (i>0) E[i] += E[i - 1] * cd[i];
		}
		s2.Format(_T("--------------------------------------------------------------\r\n新不平衡推力法\r\n--------------------------------------------------------------\r\n传递系数Ψi：%.4f\r\n不平衡推力Ei：%.4f\r\n"), cd[tkid], E[tkid]);
		break;

	case 5:
		for (int i = 0; i <= tkid; i++)
		{
			fenzi += cntfg.t[i].W * tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].cosa + cntfg.t[i].cl;
			fenmu += cntfg.t[i].W *cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\n牵引式瑞典条分法\r\n--------------------------------------------------------------\r\n∑W*cos(α)*tg(φ)+c*l：%.4f\r\n∑W*sin(α)：%.4f"), fenzi, fenmu);
		break;

	case 6:
		double sm;
		for (int i = 0; i <= tkid; i++)
		{
			sm = cntfg.t[i].cosa + 1.0 / cntfg.k* tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].sina;
			fenzi += (cntfg.t[i].W *tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl * cntfg.t[i].cosa) / sm;
			fenmu += cntfg.t[i].W* cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\n牵引式Bishop条分法\r\n--------------------------------------------------------------\r\nm=cos(α)+1/k*tg(φ)*sin(α)：%.4f\r\n∑1/m*(W*tg(φ)+c*l*cos(α))：%.4f\r\n∑W*sin(α)：%.4f"), sm, fenzi, fenmu);
		break;
	
	case 7:
		for (int i = 0; i <= tkid; i++)
		{
			if (i == 0)cd[i] = 0;
			else cd[i] = cos(cntfg.t[i].alf - cntfg.t[i - 1].alf) + tan(cntfg.t[i].fai / 180 * pi) / cntfg.k*(sin(cntfg.t[i].alf - cntfg.t[i - 1].alf));
			E[i] = -cntfg.t[i].W * cntfg.t[i].sina + 1 / cntfg.k*(cntfg.t[i].W * cntfg.t[i].cosa * tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl);
			if (i>0) E[i] += E[i - 1] * cd[i];
		}
		s2.Format(_T("--------------------------------------------------------------\r\n牵引式不平衡推力法\r\n--------------------------------------------------------------\r\n传递系数Ψi：%.4f\r\n不平衡推力Ei：%.4f\r\n"), cd[tkid], E[tkid]);
		break;
	}
	m_DataDlg->m_text.SetWindowText(s0 + s1 + s2);
}*/

void CSlopeSView::Write()
{
	struct fgss  &cntfg = fgss[fgid];
	CString s0, s1, s2, s3;
	CString ss;
	s0.Format(_T("圆心(x,y)：(%d,%d)\r\n半径：%.2f\r\n最小安全系数:%.4f\r\n--------------------------------------------------------------\r\n条块%d\r\n--------------------------------------------------------------\r\n"),
		fgss[fgid].p0.x - m_translateX, m_translateY - fgss[fgid].p0.y, fgss[fgid].bj, fgss[fgid].k, tkid + 1);
	//	s1.Format(_T("重量：%.2f\r\nα:%.2f°\r\ncos(α)：%.2f\r\nsin(α)：%.2f\r\n加权平均φ：%.2f°\r\n面积：%.2f\r\n底边长：%.2f\r\n"), fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosAlpha, fgss[fgid].t[tkid].sinAlpha, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
	s1.Format(_T("重量：%.2f\r\nα:%.2f°\r\ncos(α)：%.2f\r\nsin(α)：%.2f\r\n加权平均φ：%.2f°\r\n面积：%.2f\r\n底边长：%.2f\r\n"),
		fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosa, fgss[fgid].t[tkid].sina, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
	
	ss.Format(_T("\r\nsin(alpha) = %.2f, cos(alpha) = %.2f\r\ndeltaX, Y, Theta = %.4f, %.4f, %.4f\r\nF1 = %.4f, %.4f\r\nF2 = %.4f, %.4f\r\nF3 = %.4f, %.4f, %.4f, %.4f\r\nF4 = %.4f, %.4f, %.4f, %.4f\r\n"),
		fgss[fgid].allBlockData[tkid].sinA0, fgss[fgid].allBlockData[tkid].cosA0, 
		fgss[fgid].allBlockData[tkid].deltaX, fgss[fgid].allBlockData[tkid].deltaY, fgss[fgid].allBlockData[tkid].deltaTheta,
		fgss[fgid].allBlockData[tkid].F1[0], fgss[fgid].allBlockData[tkid].F1[1], fgss[fgid].allBlockData[tkid].F2[0], fgss[fgid].allBlockData[tkid].F2[1],
		fgss[fgid].allBlockData[tkid].F3[0], fgss[fgid].allBlockData[tkid].F3[1], fgss[fgid].allBlockData[tkid].F3[2], fgss[fgid].allBlockData[tkid].F3[3],
		fgss[fgid].allBlockData[tkid].F4[0], fgss[fgid].allBlockData[tkid].F4[1], fgss[fgid].allBlockData[tkid].F4[2], fgss[fgid].allBlockData[tkid].F4[3]
		);
	ss.AppendFormat(_T("Fx = %.4f, %.4f, %.4f, %.4f\r\nFy = %.4f, %.4f, %.4f, %.4f\r\n"),
		fgss[fgid].allBlockData[tkid].Fx[0], fgss[fgid].allBlockData[tkid].Fx[1], fgss[fgid].allBlockData[tkid].Fx[2], fgss[fgid].allBlockData[tkid].Fx[3],
		fgss[fgid].allBlockData[tkid].Fy[0], fgss[fgid].allBlockData[tkid].Fy[1], fgss[fgid].allBlockData[tkid].Fy[2], fgss[fgid].allBlockData[tkid].Fy[3]
	);

	//double fenzi = 0, fenmu = 0;
	//double E[30], cd[30];
	switch (mode)
	{
	case 1:
		s3.Format(_T("--------------------------------------------------------------\r\n细胞机条分法\r\n--------------------------------------------------------------\r\n安全系数：%.4f\r\n"),
			fgss[fgid].k);
		break;
	}
	m_DataDlg->m_text.SetWindowText(s0 + s1 + s2 + ss + s3);
}



//数据文件存储函数，默认不启用

//以下是增加部分
//对每段圆弧进行计算
void CSlopeSView::Calculate1(int n, int f, CPoint p0, double bj, CDC* pDC, int txt_num) 
{
struct fgss  &cntfg = fgss[n];
	struct point &start = cntfg.p[f];//开始点（按逆时针得到的）
	struct point &end = cntfg.p[f + 1];//结束点
	MyPoint pp[30][4];	//记录条块上所有点坐标,上左,上右,下右,下左
	double cosAlpha[30], sinAlpha[30],cosa[30], sina[30], alf[30], tgf[30], fai[30], A[30], W[30], L[30], cl[30], m[30], cd[30], E[30];	//W:重力
	int number[30];		//记录该块有几个点
	double Fxx[30][4]; double Fxy[30][4];
	CPoint pp1, pp2;//圆弧的左右端点

	//判断哪个是左点哪个是右点
	if (start.p.x < end.p.x)
	{
		pp1 = start.p; pp2 = end.p;
	}
	else
	{
		return;
	}

	double b;//土条宽
	b = (pp2.x - pp1.x)*1.0 / tks;

	for (int i = 0; i < tks; i++)//tks 分层数
	{
		//对每个条块(第i个条块)
		cosAlpha[i] = sinAlpha[i] = alf[i] = tgf[i] = fai[i] = A[i] = W[i] = L[i] = cl[i] = m[i] = cd[i] = E[i] = 0.0;
		number[i] = 0;
		double line[20], are[20];	//记录每个土层在土条底层所占长,及土条内所占面积
		
		for (int j = 0; j < 20; j++)
		//for (int j = 0; j < 10; j++)
		{
			are[j] = line[j] = 0.0;
		}
		CPoint tkp[100];//每个条块围成的不规则区域
		CPoint sbj[100];//记录条块上边界线
		int tkpid, sbjpid;

		CPoint p1, p2, p3, p4; //条块的下左，下右，上左，上右点
		p1.x = (int)(pp1.x + b*i);//下左
		if (i == 0)
			p1.y = pp1.y;
		else
			p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));//取下方的点

		p2.x = (int)(pp1.x + b*(i + 1));//下右
		if (i == tks - 1)
			p2.y = pp2.y;
		else
			p2.y = (int)(p0.y + sqrt(bj*bj - (p2.x - p0.x)*(p2.x - p0.x)));
		p3.x = p1.x;//上左
		p3.y = GetBoundPoint(p1.x, p1.y);
		p4.x = p2.x;//上右
		p4.y = GetBoundPoint(p2.x, p2.y);

		tkp[0] = p3;		//记录条块边界的多边形（从上左开始顺时针）
		tkpid = 1;
		double xl = p1.x, xr = p2.x;	//条块的左右x坐标
		double yd = max(p1.y, p2.y), yu = min(p3.y, p4.y);

		//条块下上y坐标
		//效验边界线点是否在条块内(遍历所有边界点)
		for (int j = 0; j < m_crt_p - 1; j++)
			//1.x在xl~xr  2.y>line(p1,p2)
		{
			if ((j == bianjie[0] && bianjie[0] != -1) || (j == bianjie[1] && bianjie[1] != -1) || (j == bianjie[2] && bianjie[2] != -1))
				continue;
			if (m_nzValues[j].x > xl && m_nzValues[j].x < xr && m_nzValues[j].y < p1.y + (p2.y - p1.y)*(m_nzValues[j].x - p1.x) / (p2.x - p1.x))
				tkp[tkpid++] = m_nzValues[j];
		}

		std::sort(tkp, tkp + tkpid, cmp);//按逆时针排序
		tkp[tkpid++] = p4; //找出条块上边界线
		memcpy(sbj, tkp, sizeof(CPoint)*tkpid);


		
		/* sbjpid = tkpid;
		if (p2.y != p4.y)tkp[tkpid++] = p2; if (p1.y != p3.y) tkp[tkpid++] = p1;//补齐下边界线（仅两点）
		tkp[tkpid++] = p3;//闭合*/

		sbjpid = tkpid;

		if(p2.y != p4.y)
		{
			tkp[tkpid++] = p2;
			pp[i][0] = p3;
			pp[i][1] = p4;
			pp[i][2] = p2;
			pp[i][3] = p1;
			number[i] = 4;
		}
		if (p2.y == p4.y)
		{
			tkp[tkpid++] = p2;
			pp[i][0] = p3;
			pp[i][1] = p2;
			pp[i][2] = p1;
			number[i] = 3;
		}
		if (p1.y == p3.y)
		{
			tkp[tkpid++] = p1;//补齐下边界线（仅两点）
			pp[i][0] = p3;
			pp[i][1] = p4;
			pp[i][2] = p2;
			number[i] = 3;
		}
		if (p1.y != p3.y)
		{
			tkp[tkpid++] = p1;
			pp[i][0] = p3;
			pp[i][1] = p4;
			if(p2.y==p4.y)
			{
				pp[i][2] = p1;
				number[i] =3;
			}
			else
			{
				pp[i][2] = p2;
				pp[i][3] = p1;
				number[i] =4;
			}
		}
		tkp[tkpid++] = p3;
		/*sbjpid = tkpid;

		if (p2.y == p4.y)
		{
			tkp[tkpid++] = p2;
			pp[i][0] = p3;
			pp[i][1] = p2;
			pp[i][2] = p1;
			number[i] = 3;
		}
		if (p1.y == p3.y)
		{
			tkp[tkpid++] = p1;//补齐下边界线（仅两点）
			pp[i][0] = p3;
			pp[i][1] = p4;
			pp[i][2] = p2;
			number[i] = 3;
		}
		if (p1.y != p3.y && p2.y != p4.y)
		{
			tkp[tkpid++] = p2;
			tkp[tkpid++] = p1;
			pp[i][0] = p3;
			pp[i][1] = p4;
			pp[i][2] = p2;
			pp[i][3] = p1;
			number[i] = 4;
		}
		tkp[tkpid++] = p3;//闭合*/


		double s = 0;	//double s1=0;
		for (int j = 0; j < tkpid - 1; j++)
		{
			s += (tkp[j].x*  tkp[j + 1].y - tkp[j + 1].x*tkp[j].y)*0.5;//（计算三角形面积法）//s1 += (tkp[j+1].x- tkp[j].x)*(tkp[j + 1].y+tkp[j].y)*0.5;//（定积分法）
		}
		s = fabs(s);//总面积
		A[i] = s;
		//m_fjxpoint[m_tid][m_pcrt[m_tid]] 第m_tid条共m_pcrt[m_tid]个点
		//条块与所有材料线相交

		double xlv0, ju0;//底线的斜率和截距
		xlv0 = (p1.y - p2.y)*1.0 / (p1.x - p2.x);
		ju0 = p1.y - xlv0*p1.x;
		struct mianji
		{
			int yl;//材料线左端点
			double s;//这个面积是材料线与条块下边缘围成的面积
			int id;//材料线id

			//升序排序时必须写的函数
			bool operator <(const mianji &rhs) const
			{
				return yl < rhs.yl;
			}
		}xj[POINT_COUNT];//这个记录条块与材料线相交各参数

		int xjid = 0;//记录土条与材料线相交数

		for (int j = 0; j < m_tid; j++)
		{
			//第j条材料线
			double xjs = 0;//相交面积
			bool zuos; //左点是否在上方
			bool cross = false;//有没有与此材料线相交
			if (m_fjxpoint[j][0].x >= p2.x)
				continue;//材料线在条块右端（无交点）
			else if (m_fjxpoint[j][0].x > p1.x && m_fjxpoint[j][0].x < p2.x)
			{
				//特殊情况：材料线起点在条块内
				zuos = true; cross = true;
				for (int k = 0; k < sbjpid - 1; k++)
				{
					CPoint ps, pn;//startp,nextp
					ps = sbj[k]; pn = sbj[k + 1];
					double xlv = (pn.y - ps.y)*1.0 / (pn.x - ps.x);
					double ju = pn.y - xlv*pn.x;
					if (m_fjxpoint[j][0].x > ps.x && m_fjxpoint[j][0].x < pn.x)
					{
						xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, m_fjxpoint[j][0].x);
						break;
					}
					else if (m_fjxpoint[j][0].x >= pn.x)
						xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, pn.x);
				}
			}
			//int id1 = -1, id2 = -1;
			int yy1, yy2;//p1.x与p2.x在材料线上相应的y值

			int beforey = p1.y;//底线前点

			//3.用这条材料线与底边缘线相交，得到面积
			for (int k = 0; k < m_pcrt[j] - 1; k++)
			{
				CPoint ps, pn;//startp,nextp
				ps = m_fjxpoint[j][k]; pn = m_fjxpoint[j][k + 1];
				if (pn.x <= p1.x)continue; //线段在条块左端（无交点）
				else if (ps.x >= p2.x)break;//线段在条块右端（无交点）
				double xlv = (pn.y - ps.y)*1.0 / (pn.x - ps.x);
				double ju = pn.y - xlv*pn.x;
				if (ps.x <= p1.x && p1.x < pn.x) 
				{//需替换前点
					ps.x = p1.x;
					ps.y = (int)(xlv*p1.x + ju + 0.5);
					yy1 = ps.y;
					if (ps.y < p1.y)
						zuos = true;
					else
						zuos = false;
				}
				if (ps.x <= p2.x && p2.x < pn.x)
				{
					//需替换后点
					pn.x = p2.x;
					pn.y = (int)(xlv*p2.x + ju + 0.5);
					yy2 = pn.y;
				}

				int y0 = (int)(xlv0*pn.x + ju0 + 0.5);//底线后点
				if (zuos && pn.y < y0)
				{
					//两个点都在上方
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, pn.x);
					cross = true;
				}
				else if (zuos &&pn.y >= y0)
				{
					//左上右下
					int x = (int)((ju - ju0) / (xlv0 - xlv) + 0.5); int y = (int)(xlv*x + ju + 0.5);
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, x);
					line[toptc[j]] += sqrt((pn.x - x)*(pn.x - x) + (y0 - y)*(y0 - y));
					line[buttomtc[j]] += sqrt((ps.x - x)*(ps.x - x) + (beforey - y)*(beforey - y));
					zuos = false;
					cross = true;
				}
				else if (!zuos&& pn.y < y0)
				{
					//左下右上
					int x = (int)((ju - ju0) / (xlv0 - xlv) + 0.5); int y = (int)(xlv*x + ju + 0.5);
					xjs += CalAre(xlv, ju, xlv0, ju0, x, pn.x);
					line[buttomtc[j]] += sqrt((pn.x - x)*(pn.x - x) + (y0 - y)*(y0 - y));
					line[toptc[j]] += sqrt((ps.x - x)*(ps.x - x) + (beforey - y)*(beforey - y));
					zuos = true;
					cross = true;
				}
				beforey = y0;
			}
			if (cross)
			{
				xj[xjid].s = xjs;//此材料线到条块底线的相交面积
				xj[xjid].id = j;//得到材料线id
				xj[xjid].yl = yy2;
				xjid++;
			}
		}
		std::sort(xj, xj + xjid);//按从上往下对交得的材料线排序
		for (int j = 0; j < xjid; j++)
		{
			if (j == 0)
				are[toptc[xj[j].id]] += s - xj[j].s;
			else
				are[toptc[xj[j].id]] += xj[j - 1].s - xj[j].s;//计算材料线之间的面积（即土层面积）
		}
		double totl = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));//总的土条底边长
		L[i] = totl;
		for (int j = 0; j < 10; j++)
		{
			totl -= line[j];//剩下长
		}
		if (m_tcnt > 0 && xjid > 0)
		{
			//画了材料线 且 土条与材料线有交点
			are[buttomtc[xj[xjid - 1].id]] += xj[xjid - 1].s;//最下的面积给最下材料线的下方土层
			if (totl > 0)
				line[buttomtc[xjid - 1]] += totl;//剩下的长度给最下材料线的下方土层
		}
		else if (m_tcnt > 0 && xjid == 0)
		{
			//画了材料线 但 土条与材料线无交点
			CPoint p;
			p.x = (p1.x + p2.x) / 2; p.y = (p1.y + p2.y) / 2;
			int topclx = GetTopTC(p, pDC);
			are[topclx] += s;
			line[topclx] += totl;
		}
		else if (m_tcnt <= 0)
		{
			//没画材料线
			are[toptcid] += s;
			line[toptcid] += totl;
		}
		fai[i] = 0;//加权平均fai值
		for (int j = 0; j < 10; j++)
		{
			fai[i] += tuceng[j].phi*line[j];
		}
		fai[i] /= L[i];
		//cosAlpha[i] = (p2.x - p1.x) / L[i];
		//sinAlpha[i] = (p1.y - p2.y) / L[i];
		calAngle(p2, p1, &sinAlpha[i], &cosAlpha[i], nullptr, nullptr);	//计算AB连线与x轴的夹角<BAx,与y轴的夹角<BAy

		cosa[i] = (p2.x - p1.x) / L[i];
		sina[i] = (p1.y - p2.y) / L[i];
		alf[i] = acos(cosa[i]);

		//alf[i] = acos(cosAlpha[i]);
		tgf[i] = tan(fai[i] * pi / 180);
		W[i] = 0; cl[i] = 0;
		for (int j = 0; j < 10; j++) 
		{
			//把10个土层在一个条块内做的贡献加起来
			W[i] +=1000*are[j] * tuceng[j].gama;
			cl[i] += tuceng[j].c*line[j];
		}
		if (W[i] == 0)
			return;
	}

	double K(0);		
	vector<double> iterationDeltaSum;	//迭代终止条件：位移 
	vector<double> iterationDeltaFSum;
	vector<BlockData> allBlockData;		//所有块的数据,只保存最后一个迭代
	allBlockData.resize(tks);	//多加一项
	switch (mode)
	{
	case 1:
	{   

		vector<BlockData> lastData=allBlockData;				//保存上一次迭代所有块的数据
		vector<BlockData> last_lastData=allBlockData;      //保存上上一次迭代所有
		const int maxIterationNum = 1000;		//最大迭代次数
		MyPoint pp12[30][4]; //存储节点位置
		for (int kk=0; kk<tks;kk++)
		{
		     pp12[kk][0]=pp[kk][0];
		     pp12[kk][1]=pp[kk][1]; 
		     pp12[kk][2]=pp[kk][2]; 
		     pp12[kk][3]=pp[kk][3];
		
			 if  ((pp[kk][3].x==0 && pp[kk][3].y==0)|| (pp[kk][3].x==pp[kk][0].x && pp[kk][3].y==pp[kk][0].y))
			 {
			 pp[kk][1]=pp12[kk][2];
		     pp[kk][2]=pp12[kk][1]; 
			 }
			 else
			 {
			 pp[kk][0]=pp12[kk][3];
		     pp[kk][1]=pp12[kk][2]; 
		     pp[kk][2]=pp12[kk][1]; 
			 pp[kk][3]=pp12[kk][0]; 
			 }
		}///原程序中节点位置不对
		char s[100];
		sprintf_s(s, "%d.txt",txt_num);
		ofstream fout(s);
		
		for (int calIter = 0; calIter < maxIterationNum; ++calIter)
		{   
		//	tks=tks-1;
			for (int i = 0; i < tks; i++)
			{
				//定义
				BlockData blockData = allBlockData[i];			//每个块的数据
				double AMat[3][3];								//A3*3总矩阵
				double deltaX(0), deltaY(0), deltaTheta(0);		//形心的位移增量
				double FxSum(0), FySum(0), MSum(0);	//计算中的x,y合分量,全力矩

				//0.0. 计算块面积,形心
				MyPoint centroid;			//形心
				double area(0);			//块面积
				for (int m = 0; m < number[i] - 1; ++m)
				{
					area += pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x * pp[i][m].y;
					centroid.x += (pp[i][m].x + pp[i][m + 1].x) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
					centroid.y += (pp[i][m].y + pp[i][m + 1].y) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
				}
				//最后一个点为(x0,y0)
				area += pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x * pp[i][number[i] - 1].y;
				centroid.x += (pp[i][number[i] - 1].x + pp[i][0].x) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				centroid.y += (pp[i][number[i] - 1].y + pp[i][0].y) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				//求形心
				area /= 2;
				centroid.x /= 6 * area;
				centroid.y /= 6 * area;
				//0.1 计算AMat, 3*3矩阵
				AMat[0][0] = 0; AMat[0][1] = 0; AMat[0][2] = 0;
				AMat[1][0] = 0; AMat[1][1] = 0; AMat[1][2] = 0;
				AMat[2][0] = 0; AMat[2][1] = 0; AMat[2][2] = 0;
				double aMat[3][3];			//每个节点的A3*3总矩阵
				aMat[0][0] = 0; aMat[0][1] = 0; aMat[0][2] = 0;
				aMat[1][0] = 0; aMat[1][1] = 0; aMat[1][2] = 0;
				aMat[2][0] = 0; aMat[2][1] = 0; aMat[2][2] = 0;

				double sinA, cosA;
				if  (number[i]==3 && i==0)  //左三角形
				{    
					for (int m = 0; m < number[i]; ++m)
				     {
				         if (m==0)
				         calAngle(pp[i][0], pp[i][2], &sinA, &cosA);
						 else
                          {  
						 sinA=1; cosA=0;
						   }
					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					if (m==number[i]-1) //左弹簧4号弹簧，再算一遍
				{	
					calAngle(pp[i][0], pp[i][2], &sinA, &cosA);

					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}

				      }
				  }
				else if (number[i]==3 && i==tks-1) //右边界三角形
				{  
					for (int m = 0; m < number[i]; ++m)
				     {
				       if (m==1)
				         calAngle(pp[i][2], pp[i][1], &sinA, &cosA);
					   else
					   { sinA=1; cosA=0;}

					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					 if (m==2) //左边界三角形4号弹簧 再算一次
				   {    
					calAngle(pp[i][2], pp[i][1], &sinA, &cosA);

					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					 }
				      }
		
				}
				else //四边形
				{		
					for (int m = 0; m < number[i]; ++m)
				     {
				      sinA=1; cosA=0;
                     aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					if (m==2 || m==3)
                    {
				    calAngle(pp[i][3], pp[i][2], &sinA, &cosA);
					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}
				  }
				}
				    
				//1.0.求FxSum, FySum, MSum
				//对每个块的X,Y力和力矩求和 
				MyPoint dis[4];				//每个节点到形心的距离

				if (3 == number[i])
				{ 
		             if (i==0) //来自右侧的力
                   {
			          blockData.Fx[0]=0;
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
			          blockData.Fy[0]=0;
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
			     	}
			 else if (i==tks-1) //来自左侧的力，已经更新的力
				   {
					 blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=0;
                      blockData.Fx[2]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=0;
                      blockData.Fy[2]=-allBlockData[i-1].Fy[2];
			     	}
				    FxSum = blockData.Fx[0] + blockData.Fx[1] + blockData.Fx[2];
					FySum = blockData.Fy[0] + blockData.Fy[1] + blockData.Fy[2]-W[i];
					dis[0] = pp[i][0] - centroid;
					dis[1] = pp[i][1] - centroid;
					dis[2] = pp[i][2] - centroid;
					MSum = blockData.Fy[0] * dis[0].x + blockData.Fy[1] * dis[1].x + blockData.Fy[2] * dis[2].x
						-blockData.Fx[0] * dis[0].y + blockData.Fx[1] * dis[1].y + blockData.Fx[2] * dis[2].y;
				}
				else if (4 == number[i])
				{
					//需要考虑迭代的力
				 if (i==0) //来自右侧的力
                   {
			          blockData.Fx[0]=0;
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
					  blockData.Fx[3]=0;
			          blockData.Fy[0]=0;
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
					  blockData.Fy[3]=0;
			     	}
				else if (i==tks-1) //来自左侧的力，已经更新的力
			    	{
					  blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=0;
                      blockData.Fx[2]=0;
					  blockData.Fx[3]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=0;
                      blockData.Fy[2]=0;
					  blockData.Fy[3]=-allBlockData[i-1].Fy[2];
			    	}

				else if (i==tks-2) //倒数第二个条块四边形，来自左侧的力，已经更新的力
			    	{
					  blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[2];
					  blockData.Fx[3]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[2];
					  blockData.Fy[3]=-allBlockData[i-1].Fy[2];
			    	}

				   else
			    	{
				      blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
					  blockData.Fx[3]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
					  blockData.Fy[3]=-allBlockData[i-1].Fy[2];	
		        		}

			    	FxSum = blockData.Fx[0] + blockData.Fx[1] + blockData.Fx[2]+ blockData.Fx[3];
				    FySum = blockData.Fy[0] + blockData.Fy[1] + blockData.Fy[2]+ blockData.Fy[3] -W[i];
					dis[0] = pp[i][0] - centroid;
					dis[1] = pp[i][1] - centroid;
					dis[2] = pp[i][2] - centroid;
					dis[3] = pp[i][3] - centroid;
				   MSum =blockData.Fy[0] * dis[0].x + blockData.Fy[1] * dis[1].x + blockData.Fy[2] * dis[2].x + blockData.Fy[3] * dis[3].x
					 -blockData.Fx[0] * dis[0].y - blockData.Fx[1] * dis[1].y - blockData.Fx[2] * dis[2].y - blockData.Fx[3] * dis[3].y;		
				}
				//1.1. 求解deltaX, deltaY, deltaTheta

	       double temp = AMat[0][0] * AMat[1][1] * AMat[2][2] - AMat[0][0] * AMat[1][2] * AMat[2][1] - AMat[0][1] * AMat[1][0] * AMat[2][2] +
					AMat[0][1] * AMat[1][2] * AMat[2][0] + AMat[0][2] * AMat[1][0] * AMat[2][1] - AMat[0][2] * AMat[1][1] * AMat[2][0];

           double a[4],AMat_1[3][3];
           int ii,jj,mm,nn,kk;
           for (ii=0;ii<3;ii++) 
              for (jj=0;jj<3;jj++) 
	                AMat_1[ii][jj]=0;

for ( mm=0;mm<3;mm++)
   for (nn=0;nn<3;nn++)    
      {  
		  kk=-1;    
       for (ii=0;ii<3;ii++)   
          {
			  for (jj=0;jj<3;jj++)          
              { 
				  if   (ii !=mm && jj !=nn) 
                     {
				      kk=kk+1;           
                     a[kk]=AMat[ii][jj];
		   }
		   }
	   }
       AMat_1[nn][mm]=(pow(-1,mm+nn)*(a[0]*a[3]-a[1]*a[2]))/temp;
	   }
	          deltaX = -(FxSum * AMat_1[0][0]+ FySum *AMat_1[0][1] +MSum *AMat_1[0][2]);
	          deltaY = -(FxSum * AMat_1[1][0]+ FySum *AMat_1[1][1]+MSum * AMat_1[1][2]);
              deltaTheta = -(FxSum * AMat_1[2][0]+FySum * AMat_1[2][1]+MSum *AMat_1[2][2]);
				//2.0. 求每个节点的位移
				MyPoint nodeDeltaPos[4];		//每个节点的偏移deltaX, deltaY
				for (int m = 0; m < number[i]; ++m)
				{
					nodeDeltaPos[m].x = -deltaX +(pp[i][m].y - centroid.y) * deltaTheta;
					nodeDeltaPos[m].y = -deltaY - (pp[i][m].x - centroid.x) * deltaTheta;
				}
				//3.0. 求每个节点的受力
				//3.1. 将每个节点的力分解到x,y分量
			double cosX(0), sinX(0), sinY(0), cosY(0);		//弹簧与x,y轴的夹角的cos值
			double NSum(0), TSum(0);		//所有块的法向力和切向力之和
			double NSum_moment(0);  //法向方向力矩之和
				if (3 == number[i])
				{
	              	if (i==0)  //左边缘的时候
					{
                   //第1个节点，上左  
					calAngle(pp[i][0], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					blockData.F1[0] = ks * nodeDeltaPos[0].x * cosX + ks * nodeDeltaPos[0].y * sinX;			//1号,切向,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * sinX + kn * nodeDeltaPos[0].y * -cosX;		//2号,法向,theta-pi/2
					
					//第2个节点,上右
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;				//1号,切向,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;					//2号,法向,+X

       
					//第3个节点, 下右
                     blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1号,切向,+Y
				     blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2号,法向,+X
				     calAngle(pp[i][number[i] - 1], pp[i][0], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					 blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;	//切向			//3号,切向,theta
					 blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4号,法向,theta+pi/2		
					
					 //条块判断条件
					 //底边力
                       	NSum =(allBlockData[i].F1[1]+allBlockData[i].F3[3]);//法向方向	
						TSum =allBlockData[i].F1[0]-allBlockData[i].F3[2];//切向力方向
						NSum_moment=NSum* tgf[i] + cl[i];
					   if (abs(NSum_moment)<abs(TSum))
					   {
					   allBlockData[i].F1[0]=Sign(TSum)*abs(NSum_moment)/2;
					   allBlockData[i].F3[2]=-Sign(TSum)*abs(NSum_moment)/2;
					   }
                     //y右边界
					    NSum = (allBlockData[i].F2[1]+allBlockData[i].F3[1]);//法向方向	
						TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];//切向力方向
						NSum_moment=NSum* tgf[i] + cl[i];
					  if (abs(NSum_moment)>abs(TSum))
					   {
					   allBlockData[i].F3[0]=Sign(TSum)*abs(NSum_moment)/2;
					   allBlockData[i].F2[0]=-Sign(TSum)*abs(NSum_moment)/2;
					   }
					 //第1个节点，上左 
					blockData.Fx[0] = blockData.F1[0] * cosX + blockData.F1[1] * sinX;		//X分量
					blockData.Fy[0] = blockData.F1[0] * sinX + blockData.F1[1] * -cosX;		//Y分量
					//第2个节点,上右
				    blockData.Fx[1] = blockData.F2[1];					//X分量
					blockData.Fy[1] = blockData.F2[0] * -1;			//Y分量			
					//第3个节点, 下右					
				     blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X分量
					 blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * cosX;	//Y分量	
					
					}
					else
					{
                     //第1个节点，上左  		
					blockData.F1[0] =  ks * nodeDeltaPos[0].y *-1;						//1号,切向,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1;					//2号,法向,theta-pi/2

					//第2个节点,上右
					calAngle(pp[i][1], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					blockData.F2[0] = ks * nodeDeltaPos[1].x * cosX+ ks * nodeDeltaPos[1].y * sinX;				//1号,切向,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * -sinX + kn * nodeDeltaPos[1].y * cosX;					//2号,法向,+X
					//第3个节点,下左
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;						//1号,切向,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * -1 + kn * nodeDeltaPos[2].y * 0;					//2号,法向,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][1], &sinX, &cosX, &sinY, &cosY);	
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3号,切向, theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x *sinX + kn * nodeDeltaPos[2].y * -cosX;				//2号,法向, theta+pi/2
				
					//条块判断条件
					//底边方向
					NSum = allBlockData[i].F2[1]+allBlockData[i].F3[1];//法向方向
					TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{
					allBlockData[i].F3[0]=Sign(TSum)*abs(NSum_moment)/2;
					allBlockData[i].F2[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}	
					//y轴左边界
					NSum = allBlockData[i].F3[3]+allBlockData[i].F1[1];//法向方向
					TSum=allBlockData[i].F3[2]-allBlockData[i].F1[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{
					allBlockData[i].F3[2]=Sign(TSum)*abs(NSum_moment)/2;
					allBlockData[i].F1[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}	


					//第1个节点，上左
					blockData.Fx[0] =  blockData.F1[1] *-1;		//X分量
					blockData.Fy[0] = blockData.F1[0] * -1;		//Y分量
					
					//第2个节点,上右
					blockData.Fx[1] = blockData.F2[0]*cosX +blockData.F2[1]*-sinX;					//X分量
					blockData.Fy[1] = blockData.F2[0] *sinX +blockData.F2[1]*cosX;			//Y分量
					//第3个节点,下左
					blockData.Fx[2] = -blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * sinX;	//X分量
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * -cosX;	//Y分量

					}
					//第4个节点
					blockData.Fx[3] = 0;	//X分量
					blockData.Fy[3] = 0;	//Y分量
				}
				else if (4 == number[i])
				{
		//第1个节点,上左
					blockData.F1[0] = ks * nodeDeltaPos[0].x * 0 + ks * nodeDeltaPos[0].y * -1;					//1号,切向,-Y
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1 + kn * nodeDeltaPos[0].y * 0;					//2号,法向,-X

					//第2个节点,上右
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;					//1号,切向,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;						//2号,法向,+X

					//第3个节点,下右
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1号,切向,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2号,法向,+X
			  
				    calAngle(pp[i][2], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);				//计算cos(thetaX), cos(thetaY);
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3号,切向,theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4号,法向,theta+pi/2
				
					//第4个节点,下左
					blockData.F4[0] = ks * nodeDeltaPos[3].x * 0 + ks * nodeDeltaPos[3].y * 1;						//1号,切向,+Y
					blockData.F4[1] = kn * nodeDeltaPos[3].x * -1 + kn * nodeDeltaPos[3].y * 0;					//2号,法向,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	
					blockData.F4[2] = ks * nodeDeltaPos[3].x * cosX + ks * nodeDeltaPos[3].y * sinX;				//3号,切向, theta
					blockData.F4[3] = kn * nodeDeltaPos[3].x * sinX + kn * nodeDeltaPos[3].y * -cosX;				//2号,法向, theta+pi/2
	
			      //底边
					NSum = (allBlockData[i].F3[3] +allBlockData[i].F4[3]);//法向力
					TSum=allBlockData[i].F4[2]-allBlockData[i].F3[2];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{	
						allBlockData[i].F4[2]=Sign(TSum)*abs(NSum_moment)/2;
					    allBlockData[i].F3[2]=-Sign(TSum)*abs(NSum_moment)/2;
					}
					//左边界
					NSum = (allBlockData[i].F4[1] +allBlockData[i].F1[1]);//法向力
					TSum=allBlockData[i].F1[0]-allBlockData[i].F4[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{	
						allBlockData[i].F1[0]=Sign(TSum)*abs(NSum_moment)/2;
					    allBlockData[i].F4[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}
					//右边界

					NSum = (allBlockData[i].F2[1] +allBlockData[i].F3[1]);//法向力
					TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{	
						allBlockData[i].F3[0]=Sign(TSum)*abs(NSum_moment)/2;
					    allBlockData[i].F2[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}

	               //第1个节点,上左
					blockData.Fx[0] = blockData.F1[1] * -1;				//X分量
					blockData.Fy[0] = blockData.F1[0] * -1;				//Y分量

					//第2个节点,上右
					blockData.Fx[1] = blockData.F2[1];				//X分量
					blockData.Fy[1] = blockData.F2[0] * -1;				//Y分量

					//第3个节点,下右
					blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X分量
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX+ blockData.F3[3] * cosX;	//Y分量

					//第4个节点,下左
		  		    blockData.Fx[3] = -blockData.F4[1] + blockData.F4[2] * cosX + blockData.F4[3] * sinX;	//X分量
					blockData.Fy[3] = blockData.F4[0] + blockData.F4[2] * sinX + blockData.F4[3] * -cosX;	//Y分量
}
	//		      blockData.NF_x=abs(blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]+blockData.Fx[3]+FxSum);
	//		      blockData.NF_y=abs(blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]+blockData.Fy[3]+FySum);
	//		   if (abs(blockData.NF_x)>0.0001 || abs(blockData.NF_y)>0.0001)
	//		   {
	//		   blockData.NF_x=blockData.NF_x;
	//		   }
				//4.0 计算下一块的Fx,Fy
				//不是最后一个块

			//保存数据 保存当前块的位移角度等数据
			blockData.deltaX = deltaX;
			blockData.deltaY = deltaY;
			blockData.deltaTheta = deltaTheta;
			allBlockData[i] = blockData; 
	
			//输出当前迭代情况下，条块i的力
 
             int  out_num=4;  //定义输出条块数
                if (i==out_num-1&&out_num<tks&&txt_num%100==0) 
                   {
                       if (i==0&&number[i]==3) //第一个条块三个节点情况下
                              fout<<calIter<<"      "<<blockData.Fx[0]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fy[0]<<"      "<<blockData.Fy[1]<<"      "<<blockData.Fy[2]<<endl;
                       else if(i==tks-1&&number[i]==3) //最后一个条块三个节点情况下
                              fout<<calIter<<"      "<<blockData.Fx[0]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fy[0]<<"      "<<blockData.Fy[1]<<"      "<<blockData.Fy[2]<<endl;
                       else
                              fout<<calIter<<"      "<<blockData.Fx[0]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fx[3]<<"      "<<blockData.Fy[0]<<"      "<<blockData.Fy[1]<<"      "<<blockData.Fy[2]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fx[3]<<endl;	
                  }
		
		//上面是输出代码
	//		     fout<<"      "<<double(cntfg2-cntfg1)<<endl;
		}

/*
lastData.assign(allBlockData.begin(), allBlockData.end());//保存上次的数据到lastData
  //  第二次迭代计算
			for (int i = tks-1; i >=0; i--)  //
			{
				//定义
				BlockData blockData = allBlockData[i];			//每个块的数据
				double AMat[3][3];								//A3*3总矩阵
				double deltaX(0), deltaY(0), deltaTheta(0);		//形心的位移增量
				double FxSum(0), FySum(0), MSum(0);	//计算中的x,y合分量,全力矩
				//0.0. 计算块面积,形心
				MyPoint centroid;			//形心
				double area(0);			//块面积
					for (int m = 0; m < number[i] - 1; ++m)
				{
					area += pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x * pp[i][m].y;
					centroid.x += (pp[i][m].x + pp[i][m + 1].x) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
					centroid.y += (pp[i][m].y + pp[i][m + 1].y) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
				}
				//最后一个点为(x0,y0)
				area += pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x * pp[i][number[i] - 1].y;
				centroid.x += (pp[i][number[i] - 1].x + pp[i][0].x) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				centroid.y += (pp[i][number[i] - 1].y + pp[i][0].y) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				//求形心
				area /= 2;
				centroid.x /= 6 * area;
				centroid.y /= 6 * area;
				//0.1 计算AMat, 3*3矩阵
				AMat[0][0] = 0; AMat[0][1] = 0; AMat[0][2] = 0;
				AMat[1][0] = 0; AMat[1][1] = 0; AMat[1][2] = 0;
				AMat[2][0] = 0; AMat[2][1] = 0; AMat[2][2] = 0;
				double aMat[3][3];			//每个节点的A3*3总矩阵
				aMat[0][0] = 0; aMat[0][1] = 0; aMat[0][2] = 0;
				aMat[1][0] = 0; aMat[1][1] = 0; aMat[1][2] = 0;
				aMat[2][0] = 0; aMat[2][1] = 0; aMat[2][2] = 0;

				double sinA, cosA;
				if  (number[i]==3 && i==0)  //左三角形
				{    
					for (int m = 0; m < number[i]; ++m)
				     {
				         if (m==0)
				         calAngle(pp[i][0], pp[i][2], &sinA, &cosA);
						 else
                          {  
						 sinA=1; cosA=0;
						   }
					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					if (m==number[i]-1) //左弹簧4号弹簧，再算一遍
				{	
					calAngle(pp[i][0], pp[i][2], &sinA, &cosA);

					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}

				      }
				  }
				else if (number[i]==3 && i==tks-1) //右边界三角形
				{  
					for (int m = 0; m < number[i]; ++m)
				     {
				       if (m==1)
				         calAngle(pp[i][2], pp[i][1], &sinA, &cosA);
					   else
					   { sinA=1; cosA=0;}

					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					 if (m==2) //左边界三角形4号弹簧 再算一次
				   {    
					calAngle(pp[i][2], pp[i][1], &sinA, &cosA);

					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					 }
				      }
		
				}
				else //四边形
				{		
					for (int m = 0; m < number[i]; ++m)
				     {
				      sinA=1; cosA=0;
                     aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					if (m==2 || m==3)
                    {
				    calAngle(pp[i][3], pp[i][2], &sinA, &cosA);
					aMat[0][0] = -ks * cosA * cosA - kn * sinA * sinA;
					aMat[0][1] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[0][2] = ks * (cosA * cosA * (pp[i][m].y - centroid.y) - sinA * cosA * (pp[i][m].x - centroid.x)) +
						kn * (cosA * sinA * (pp[i][m].x - centroid.x) + sinA * sinA * (pp[i][m].y - centroid.y));

					aMat[1][0] = -ks * cosA * sinA + kn * cosA * sinA;
					aMat[1][1] = -ks * sinA * sinA - kn * cosA * cosA;
					aMat[1][2] = ks * (cosA * sinA * (pp[i][m].y - centroid.y) - sinA * sinA * (pp[i][m].x - centroid.x)) -
						kn * (cosA * cosA * (pp[i][m].x - centroid.x) + cosA * sinA * (pp[i][m].y - centroid.y));

					aMat[2][0] = aMat[1][0] * (pp[i][m].x - centroid.x) - aMat[0][0] * (pp[i][m].y - centroid.y);
					aMat[2][1] = aMat[1][1] * (pp[i][m].x - centroid.x) - aMat[0][1] * (pp[i][m].y - centroid.y);
					aMat[2][2] = aMat[1][2] * (pp[i][m].x - centroid.x) - aMat[0][2] * (pp[i][m].y - centroid.y);

					//求和
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}
				  }
				}
				    
				//1.0.求FxSum, FySum, MSum
				//对每个块的X,Y力和力矩求和 
				MyPoint dis[4];				//每个节点到形心的距离

				if (3 == number[i])
				{ 
		     	 if (i==0) //来自右侧的力
                   {
			          blockData.Fx[0]=0;
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
			          blockData.Fy[0]=0;
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
			     	}
			     else if (i==tks-1) //来自左侧的力，已经更新的力
				   {
					  blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=0;
                      blockData.Fx[2]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=0;
                      blockData.Fy[2]=-allBlockData[i-1].Fy[2];
			     	}
				    FxSum = blockData.Fx[0] + blockData.Fx[1] + blockData.Fx[2];
					FySum = blockData.Fy[0] + blockData.Fy[1] + blockData.Fy[2]-W[i];
					dis[0] = pp[i][0] - centroid;
					dis[1] = pp[i][1] - centroid;
					dis[2] = pp[i][2] - centroid;
					MSum = blockData.Fy[0] * dis[0].x + blockData.Fy[1] * dis[1].x + blockData.Fy[2] * dis[2].x
						-blockData.Fx[0] * dis[0].y + blockData.Fx[1] * dis[1].y + blockData.Fx[2] * dis[2].y;
				}
				else if (4 == number[i])
				{
					//需要考虑迭代的力
				 if (i==0) //来自右侧的力
                   {
			          blockData.Fx[0]=0;
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
					  blockData.Fx[3]=0;
			          blockData.Fy[0]=0;
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
					  blockData.Fy[3]=0;
			     	}
				else if (i==tks-1) //来自左侧的力，已经更新的力
			    	{
					  blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=0;
                      blockData.Fx[2]=0;
					  blockData.Fx[3]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=0;
                      blockData.Fy[2]=0;
					  blockData.Fy[3]=-allBlockData[i-1].Fy[2];
			    	}
				   else
			    	{
				      blockData.Fx[0]=-allBlockData[i-1].Fx[1];
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
					  blockData.Fx[3]=-allBlockData[i-1].Fx[2];
			          blockData.Fy[0]=-allBlockData[i-1].Fy[1];
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
					  blockData.Fy[3]=-allBlockData[i-1].Fy[2];	
		        		}

			    	FxSum = blockData.Fx[0] + blockData.Fx[1] + blockData.Fx[2]+ blockData.Fx[3];
				    FySum = blockData.Fy[0] + blockData.Fy[1] + blockData.Fy[2]+ blockData.Fy[3] -W[i];
					dis[0] = pp[i][0] - centroid;
					dis[1] = pp[i][1] - centroid;
					dis[2] = pp[i][2] - centroid;
					dis[3] = pp[i][3] - centroid;
				   MSum =blockData.Fy[0] * dis[0].x + blockData.Fy[1] * dis[1].x + blockData.Fy[2] * dis[2].x + blockData.Fy[3] * dis[3].x
					 -blockData.Fx[0] * dis[0].y - blockData.Fx[1] * dis[1].y - blockData.Fx[2] * dis[2].y - blockData.Fx[3] * dis[3].y;		
				}
	

	double temp = AMat[0][0] * AMat[1][1] * AMat[2][2] - AMat[0][0] * AMat[1][2] * AMat[2][1] - AMat[0][1] * AMat[1][0] * AMat[2][2] +
					AMat[0][1] * AMat[1][2] * AMat[2][0] + AMat[0][2] * AMat[1][0] * AMat[2][1] - AMat[0][2] * AMat[1][1] * AMat[2][0];

              double a[4],AMat_1[3][3];
                     int ii,jj,mm,nn,kk;
                for (ii=0;ii<3;ii++) 
                    for (jj=0;jj<3;jj++) 
	                       AMat_1[ii][jj]=0;

for ( mm=0;mm<3;mm++)
   for (nn=0;nn<3;nn++)    
      {  
		  kk=-1;    
       for (ii=0;ii<3;ii++)   
          {
			  for (jj=0;jj<3;jj++)          
              { 
				  if   (ii !=mm && jj !=nn) 
                     {
				      kk=kk+1;           
                     a[kk]=AMat[ii][jj];
		   }
		   }
	   }
       AMat_1[nn][mm]=(pow(-1,mm+nn)*(a[0]*a[3]-a[1]*a[2]))/temp;
	   }
	
	deltaX = -(FxSum * AMat_1[0][0]+ FySum *AMat_1[0][1] +MSum *AMat_1[0][2]);

	deltaY = -(FxSum * AMat_1[1][0]+ FySum *AMat_1[1][1]+MSum * AMat_1[1][2]);

    deltaTheta = -(FxSum * AMat_1[2][0]+FySum * AMat_1[2][1]+MSum *AMat_1[2][2]);

				//2.0. 求每个节点的位移
				MyPoint nodeDeltaPos[4];		//每个节点的偏移deltaX, deltaY
				for (int m = 0; m < number[i]; ++m)
				{
					nodeDeltaPos[m].x = -deltaX +(pp[i][m].y - centroid.y) * deltaTheta;
					nodeDeltaPos[m].y = -deltaY - (pp[i][m].x - centroid.x) * deltaTheta;
				}

				//3.0. 求每个节点的受力
				//3.1. 将每个节点的力分解到x,y分量
			double cosX(0), sinX(0), sinY(0), cosY(0);		//弹簧与x,y轴的夹角的cos值
				if (3 == number[i])
				{
			if (i==0)  //左边缘的时候
					{
                   //第1个节点，上左  
					calAngle(pp[i][0], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					blockData.F1[0] = ks * nodeDeltaPos[0].x * cosX + ks * nodeDeltaPos[0].y * sinX;						//1号,切向,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * sinX + kn * nodeDeltaPos[0].y * -cosX;					//2号,法向,theta-pi/2
					blockData.Fx[0] = blockData.F1[0] * cosX + blockData.F1[1] * sinX;		//X分量
					blockData.Fy[0] = blockData.F1[0] * sinX + blockData.F1[1] * -cosX;		//Y分量

					//第2个节点,上右
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;				//1号,切向,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;					//2号,法向,+X
					blockData.Fx[1] = blockData.F2[1];					//X分量
					blockData.Fy[1] = blockData.F2[0] * -1;			//Y分量
       
					//第3个节点, 下右
                      		blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1号,切向,+Y
				         	blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2号,法向,+X
				        	calAngle(pp[i][number[i] - 1], pp[i][0], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					        blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;	//切向			//3号,切向,theta
					        blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4号,法向,theta+pi/2
			               blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X分量
					       blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * cosX;	//Y分量	
						}
					else
					{
                     //第1个节点，上左  		
					blockData.F1[0] =  ks * nodeDeltaPos[0].y *-1;						//1号,切向,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1;					//2号,法向,theta-pi/2
					blockData.Fx[0] =  blockData.F1[1] *-1;		//X分量
					blockData.Fy[0] = blockData.F1[0] * -1;		//Y分量
					//第2个节点,上右
					calAngle(pp[i][1], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					blockData.F2[0] = ks * nodeDeltaPos[1].x * cosX+ ks * nodeDeltaPos[1].y * sinX;				//1号,切向,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * -sinX + kn * nodeDeltaPos[1].y * cosX;					//2号,法向,+X
					blockData.Fx[1] = blockData.F2[0]*cosX +blockData.F2[1]*-sinX;					//X分量
					blockData.Fy[1] = blockData.F2[0] *sinX +blockData.F2[1]*cosX;			//Y分量
				    //第3个节点,下左
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;						//1号,切向,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * -1 + kn * nodeDeltaPos[2].y * 0;					//2号,法向,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][1], &sinX, &cosX, &sinY, &cosY);	
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3号,切向, theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x *sinX + kn * nodeDeltaPos[2].y * -cosX;				//2号,法向, theta+pi/2
		  		    blockData.Fx[2] = -blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * sinX;	//X分量
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * -cosX;	//Y分量
					}
					//第4个节点
					blockData.Fx[3] = 0;	//X分量
					blockData.Fy[3] = 0;	//Y分量
				}
				else if (4 == number[i])
				{
					//第1个节点,上左
					blockData.F1[0] = ks * nodeDeltaPos[0].x * 0 + ks * nodeDeltaPos[0].y * -1;					//1号,切向,-Y
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1 + kn * nodeDeltaPos[0].y * 0;					//2号,法向,-X
					blockData.Fx[0] = blockData.F1[1] * -1;				//X分量
					blockData.Fy[0] = blockData.F1[0] * -1;				//Y分量

					//第2个节点,上右
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;					//1号,切向,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;						//2号,法向,+X
					blockData.Fx[1] = blockData.F2[1];				//X分量
					blockData.Fy[1] = blockData.F2[0] * -1;				//Y分量

					//第3个节点,下右
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1号,切向,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2号,法向,+X
			  
				    calAngle(pp[i][2], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);				//计算cos(thetaX), cos(thetaY);
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3号,切向,theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4号,法向,theta+pi/2
				
					blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X分量
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX+ blockData.F3[3] * cosX;	//Y分量

					//第4个节点,下左
					blockData.F4[0] = ks * nodeDeltaPos[3].x * 0 + ks * nodeDeltaPos[3].y * 1;						//1号,切向,+Y
					blockData.F4[1] = kn * nodeDeltaPos[3].x * -1 + kn * nodeDeltaPos[3].y * 0;					//2号,法向,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//计算cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	
					blockData.F4[2] = ks * nodeDeltaPos[3].x * cosX + ks * nodeDeltaPos[3].y * sinX;				//3号,切向, theta
					blockData.F4[3] = kn * nodeDeltaPos[3].x * sinX + kn * nodeDeltaPos[3].y * -cosX;				//2号,法向, theta+pi/2
		  		    blockData.Fx[3] = -blockData.F4[1] + blockData.F4[2] * cosX + blockData.F4[3] * sinX;	//X分量
					blockData.Fy[3] = blockData.F4[0] + blockData.F4[2] * sinX + blockData.F4[3] * -cosX;	//Y分量
			}

			      blockData.NF_x=abs(blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]+blockData.Fx[3]+FxSum);
			      blockData.NF_y=abs(blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]+blockData.Fy[3]+FySum);
			//考察节点分配的力是否相等
			if (abs(blockData.NF_x)>0.0001 || abs(blockData.NF_y)>0.0001)
			   {
			   blockData.NF_x=blockData.NF_x;
			   }
			//考察节点合外力是否相等 //从右往左计算
			if (i==0 && number[i]==3) //最左侧的三角形
			{
			blockData.NF_x=blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]-(allBlockData[1].Fx[0]+allBlockData[1].Fx[3]);
			blockData.NF_y=blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]-(allBlockData[1].Fy[0]+allBlockData[1].Fy[3]+W[0]);
			}
			else if  (i==tks-1 &&  number[i]==3) //最右侧三角形
			{
			blockData.NF_x=blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]-(lastData[i-1].Fx[1]+lastData[i-1].Fx[2]);
			blockData.NF_y=blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]-(lastData[i-1].Fy[1]+lastData[i-1].Fy[2]+W[i]);
			}
			else
            {
			blockData.NF_x=blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]+blockData.Fx[3]-(lastData[i-1].Fx[1]+lastData[i-1].Fx[2]+allBlockData[i+1].Fx[0]+allBlockData[i+1].Fx[3]);
			blockData.NF_y=blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]+blockData.Fy[3]-(lastData[i-1].Fy[1]+lastData[i-1].Fy[2]+allBlockData[i+1].Fy[0]+allBlockData[i+1].Fy[3]+W[i]);
			}

			 if (abs(blockData.NF_x)>0.0001 || abs(blockData.NF_y)>0.0001)
			   {
			   blockData.NF_x=blockData.NF_x;
			   }

				//4.0 计算下一块的Fx,Fy
			  //保存数据 保存当前块的位移角度等数据
			blockData.deltaX = deltaX;
			blockData.deltaY = deltaY;
			blockData.deltaTheta = deltaTheta;
			allBlockData[i] = blockData; 
			}
			*/
		  //第二次迭代计算完毕
			//迭代终止条件
			//1.按位移
			double deltaSum(0);
			for (auto it = allBlockData.begin(); it != allBlockData.end(); ++it)
			{
				deltaSum += abs(it->deltaX) + abs(it->deltaY) + abs(it->deltaTheta);
			}
			iterationDeltaSum.push_back(deltaSum);
			//2.按力
			double deltaFSumx(0),deltaFSumy(0),deltaFSum(0);		 	
			if (0 != calIter)
			{
				for (int i = 0; i < tks-1; ++i)
				{
     			for (int j = 0; j < 4; ++j)
				  {
					deltaFSum += abs(allBlockData[i].Fx[j]- lastData[i].Fx[j]);
					deltaFSum += abs(allBlockData[i].Fy[j]- lastData[i].Fy[j]);
				  }
				}

			}
		lastData.assign(allBlockData.begin(), allBlockData.end());

			//迭代终止,或者已经到最后一次迭代
			if ((0 != calIter && deltaFSum<= 20) || calIter == maxIterationNum - 1)
			{
				//5.0 计算K
				double NSum(0), TSum(0);		//所有块的法向力和切向力之和
				double NSum_moment(0);  //法向方向力矩之和
				double fenzi(0); double fenmu(0);//分子，分母
				double Count(0);



		 //删除文件操作
		    fout.close(); //关闭文件操作
		//如果迭代次数大于最大迭代次数或者中间的力出现较大的幅度时删除数据文件
		   if (calIter == maxIterationNum - 1||allBlockData[tks-3].Fx[1]>40e8||txt_num%100!=0)
		    remove(s);

				for (int i = 0; i < tks; i++)
				{
					//根据倾斜面的cos(alpha)来判断力的方向,需注意四个点时C,D的3号弹簧方向相反
					if (3 == number[i]&& i==tks-1)
					{   //右三角形
					     NSum = -(allBlockData[i].F3[3]+allBlockData[i].F3[1]);//法向方向
			           //  if (NSum>0)
			          //	 NSum=0;
					     TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];
						 NSum_moment=NSum* tgf[i] + cl[i];
					}
				    else if (3 == number[i] && i==0)
					{   //左三角形
				    	NSum = -(allBlockData[i].F1[1]+allBlockData[i].F3[3]);//法向方向	
				        //		if (NSum>0)
			        	//			NSum=0;
						TSum=allBlockData[i].F1[0]-allBlockData[i].F3[2];
						NSum_moment=NSum* tgf[i] + cl[i];
					}
					else if (4 == number[i])
					{
					//	NSum = -(allBlockData[i].F3[2] +allBlockData[i].F4[2]);
						NSum = -(allBlockData[i].F3[3] +allBlockData[i].F4[1]);//规定的正方向是与坐标轴y轴相反
					//	if (NSum>0)
                    //        NSum=0;
						TSum=allBlockData[i].F4[2]-allBlockData[i].F3[2];
						NSum_moment=NSum* tgf[i] + cl[i];
					}

				//	fenzi += NSum * tgf[i] - cl[i];//有问题
					if (abs(NSum_moment)<abs(TSum))
					{	
						TSum=Sign(TSum)*abs(NSum_moment);
					     Count++;
					}
			    	fenzi += NSum_moment;
					fenmu += TSum;
				}
				if  (Count==tks)
			      K = 0.2;//得到这一圆弧的K
				else
				{
					K =abs(fenzi)/abs(fenmu);
				}

//				fout<<"      "<<K;
		//		tks=tks+1;
				break;
			}

			
			//保存当前迭代的计算数据
			//blocksData.Fx = nodeFx;
			//blocksData.Fy = nodeFy;
	       //tks=tks+1;
		}
//节点还原
	  for (int kk=0; kk<30; kk++)
		{
			if  ((pp12[kk][3].x==0 &&pp12[kk][3].y==0)|| (pp12[kk][3].x==pp12[kk][0].x &&pp12[kk][3].y==pp12[kk][0].y))
			 {
			 pp[kk][0]=pp12[kk][0];
		     pp[kk][1]=pp12[kk][1]; 
		     pp[kk][2]=pp12[kk][2]; 
			 }
			 else
			 {
			 pp[kk][0]=pp12[kk][0];
		     pp[kk][1]=pp12[kk][1]; 
		     pp[kk][2]=pp12[kk][2]; 
			 pp[kk][3]=pp12[kk][3]; 
			 
			 }
	  }

		break;
	}

	}

	if (K < cntfg.k && K>0)
	//if(K>0)
	{
		cntfg.k = K;
		cntfg.ptStart = pp1;
		cntfg.ptEnd = pp2;
		cntfg.p0 = p0;
		cntfg.bj = bj;
		cntfg.qytks = Mqytks;
		cntfg.allBlockData = allBlockData;
		for (int i = 0; i < tks; i++)
		{
			cntfg.t[i].cl = cl[i];
			cntfg.t[i].fai = fai[i];
			cntfg.t[i].alf = alf[i];
			cntfg.t[i].are = A[i];
			cntfg.t[i].sina = sina[i];
			cntfg.t[i].cosa = cosa[i];
			cntfg.t[i].W = W[i];
			cntfg.t[i].line = L[i];
		}
	}
}


int CSlopeSView:: Sign(double  A)  //判断符号
{
	if (A>=0)
		return 1;
	else
	   return  -1;
}

//计算角度<ABC>
void CSlopeSView::calAngle(MyPoint& A, MyPoint& B, MyPoint& C, double* sina, double* cosa)
{
	MyPoint BA = A - B;
	MyPoint BC = C - B;

	//计算夹角
	double temp = (BA.x * BC.x + BA.y * BC.y) / sqrt(BA.x * BA.x + BA.y * BA.y) / sqrt(BC.x * BC.x + BC.y * BC.y);
	if (nullptr != cosa)
		*cosa = temp;
	if (nullptr != sina)
		*sina = sqrt(1 - temp * temp);
}

//计算BA连线与x轴的夹角<BAx, 与y轴的夹角<BAy
void CSlopeSView::calAngle(MyPoint& A, MyPoint& B, double* sinX, double* cosX, double* sinY, double* cosY)
{
/*	
    MyPoint AB = B - A;
	MyPoint xVec(1, 0), yVec(0, 1);
	if (AB.x!=0||AB.y!=0)
	{
	//计算AB与x的夹角
	double tempCosX = (AB.x * xVec.x + AB.y * xVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	//计算AB与y的夹角
	double tempCosY = (AB.x * yVec.x + AB.y * yVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);

	if (nullptr != cosX)
		*cosX = tempCosX;
	if (nullptr != sinX)
	{
		if (AB.y >= 0)
			*sinX = sqrt(1 - tempCosX * tempCosX);
		else
			*sinX = -sqrt(1 - tempCosX * tempCosX);
	}
		
	if (nullptr != cosY)
		*cosY = tempCosY;
	if (nullptr != sinY)
	{
		if (AB.y >= 0)
			*sinY = sqrt(1 - tempCosY * tempCosY);
		else
			*sinY = -sqrt(1 - tempCosY * tempCosY);
	}
	}
	else
	{
		*sinX=0;
		*cosX=0;
		*cosY=0;
		*sinY=0;
	}
*/


	MyPoint AB = B - A;
	MyPoint xVec(1, 0);
	if (AB.x!=0||AB.y!=0)
	{
	//计算AB与x的夹角
	double tempCosX = (AB.x * xVec.x + AB.y * xVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	
	if (nullptr != cosX)
		*cosX = tempCosX;
	if (nullptr != sinX)
	{
		if (AB.y >= 0)
			*sinX = sqrt(1 - tempCosX * tempCosX);
		else
			*sinX = -sqrt(1 - tempCosX * tempCosX);
	}
		
	}
	else
	{
		*sinX=0;
		*cosX=0;
	}

	MyPoint ABB(AB.y,-AB.x);
	AB=ABB;
	if (AB.x!=0||AB.y!=0)
	{
	//计算AB与x的夹角
	double tempCosY = (AB.x * xVec.x + AB.y * xVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	
	if (nullptr != cosY)
		*cosY = tempCosY;
	if (nullptr != sinY)
	{
		if (AB.y >= 0)
			*sinY= sqrt(1 - tempCosY * tempCosY);
		else
			*sinY = -sqrt(1 - tempCosY * tempCosY);
	}

	}
	else
	{
		*sinY=0;
		*cosY=0;
	}
}

//计算BA连线与x轴的夹角<BAx, 与y轴的夹角<BAy
void CSlopeSView::calAngle(CPoint& A, CPoint& B, double* sinX, double* cosX, double* sinY, double* cosY)
{
	CPoint AB = B - A;
	CPoint xVec(1, 0), yVec(0,1);

	//计算AB与x的夹角
	if (AB.x!=0||AB.y!=0)
	{
	double tempCosX = (AB.x * xVec.x + AB.y * xVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	//计算AB与y的夹角
	double tempCosY = (AB.x * yVec.x + AB.y * yVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	double temp1=tempCosX * tempCosX;
	double temp2=tempCosY * tempCosY;
	if (temp1>1) temp1=1;
	if (temp2>1) temp2=1;

	if (nullptr != cosX)
		*cosX = tempCosX;
	if (nullptr != sinX)
	{
		if (AB.y >= 0)
			*sinX = sqrt(1 - temp1);
		else
			*sinX = -sqrt(1 - temp1);
		//	*sinX = sqrt(1 - temp1);
	}
	if (nullptr != cosY)
		*cosY = tempCosY;
	if (nullptr != sinY)
	{
		if (AB.y>= 0)
			*sinY = sqrt(1 - temp2);
		else
			*sinY = -sqrt(1 - temp2);
			//*sinY = sqrt(1 - temp2);
	}
	}
	else
	{ 		
    //添加的
	//	*sinX=0;
	//	*cosX=0;
	//	*cosY=0;
	//	*sinY=0;
	sinX=0;
	cosX=0;
	cosY=0;
	sinY=0;
	}
}