
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

#include <iomanip>

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

	for (int i = 0; i < 10; i++)
	{
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
	mode = 3;

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


// CSlopeSView 事件处理函数
void CSlopeSView::OnDraw(CDC* pDC)//重画机制（双缓冲）
{
	CSlopeSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMemDC dcMem(*pDC, this);
	CDC&  MemDC = dcMem.GetDC();

	CRect   rect;
	GetClientRect(&rect);
	MemDC.FillSolidRect(&rect, RGB(255, 255, 255));//将背景填白pDC->GetBkColor()
	//画笔
	CPen penp(m_nLineStyle, m_nLineWidth, m_clrp);
	CPen penbh(m_nLineStyle, 4, RGB(128, 128, 128));
	CPen penfinish(m_nLineStyle, 2, RGB(211, 208, 207));
	CPen penl(m_nLineStyle, m_nLineWidth, m_clrl);
	CPen pencu(m_nLineStyle, 1, RGB(0, 0, 255));
	//画刷
	CBrush brush(tuceng[0].clr);
	//画图
	for (int i = 0; i < m_crt_p; i++)//重画边界线
	{
		CPoint point = m_nzValues[i];
		MemDC.MoveTo(point);
		MemDC.SelectObject(&penp);
		if (bihe)MemDC.SelectObject(&penl);
		MemDC.LineTo(point.x + 5, point.y); //*绘制点
		MemDC.LineTo(point.x - 5, point.y);
		MemDC.MoveTo(point);
		MemDC.LineTo(point.x, point.y + 5);
		MemDC.LineTo(point.x, point.y - 5);
		MemDC.MoveTo(point);
		MemDC.SelectObject(&penl);
		if (bihe)MemDC.SelectObject(&penbh);
		if (i < m_crt_p - 1)MemDC.LineTo(m_nzValues[i + 1]);//*绘制线
	}

	//填充黄色
	if (bihe)
	{
		MemDC.SelectObject(brush);
		HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
		FillRgn(MemDC, rg, brush);
		DeleteObject(rg);
	}

	MemDC.SetBkMode(TRANSPARENT);//设置文字背景透明
	//显示坐标点文字（x,y）
	if (m_nDrawType >= 1 && m_nDrawType <= 3)
	{
		for (int i = 0; i < m_crt_p; i++)
		{
			CPoint point = m_nzValues[i];
			CString str;
			str.Format(_T("(%d ,%d)"), point.x - 320, 510 - point.y);
			MemDC.TextOut(point.x, point.y, str, str.GetLength());
		}
	}
	for (int i = 0; i <= m_tid; i++)//重画材料线
	{
		for (int j = 0; j < m_pcrt[i]; j++)//第i条第j点
		{
			CPoint point = m_fjxpoint[i][j];
			MemDC.MoveTo(point);
			MemDC.SelectObject(&penl);
			MemDC.LineTo(point.x + 5, point.y); //*绘制点
			MemDC.LineTo(point.x - 5, point.y);
			MemDC.MoveTo(point);
			MemDC.LineTo(point.x, point.y + 5);
			MemDC.LineTo(point.x, point.y - 5);
			MemDC.MoveTo(point);
			if (m_tfinish[i])MemDC.SelectObject(&penfinish);
			if (j < m_pcrt[i] - 1)MemDC.LineTo(m_fjxpoint[i][j + 1]);
		}
	}
	for (int i = 1; i <= tc_step; i++)//重自定义填充
	{
		CBrush tcbrush(tianchong[i].clr);
		MemDC.SelectObject(tcbrush);
		CPoint point = tianchong[i].point;

		HDC hDC = MemDC.GetSafeHdc(); //获取屏幕DC

		// 再获取当前点位置像素值  
		COLORREF color = ::GetPixel(hDC, point.x, point.y);
		MemDC.ExtFloodFill(point.x, point.y, color, FLOODFILLSURFACE);//取屏幕点的像素，对这个像素填充tcbrush
		DeleteObject(tcbrush);
	}

	//重画方格
	if (crt_fg == 2)
	{
		MemDC.SelectObject(&penl);
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
			MemDC.SelectObject(gzbrush);
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
		HDC hDC = MemDC.GetSafeHdc();
		Arc(hDC, p0.x - (int)bj, p0.y - (int)bj, p0.x + (int)bj, p0.y + (int)bj, fgss[i].ptStart.x, fgss[i].ptStart.y, fgss[i].ptEnd.x, fgss[i].ptEnd.y);
		if (fgid != minkid)
		{
			i = fgid;
			p0 = fgss[i].p0;
			bj = fgss[i].bj;
			MemDC.SelectObject(&pencu);
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[i].ptStart);
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[i].ptEnd);
			Arc(hDC, p0.x - (int)bj, p0.y - (int)bj, p0.x + (int)bj, p0.y + (int)bj, fgss[i].ptStart.x, fgss[i].ptStart.y, fgss[i].ptEnd.x, fgss[i].ptEnd.y);
		}
		MemDC.SelectObject(&penl);
		for (int j = 1; j < tks; j++)
		{
			CPoint p1, p2;
			if (j == fgss[i].qytks)
			{
				CPen pentkqy(m_nLineStyle, 2, RGB(255, 0, 0));
				MemDC.SelectObject(&pentkqy);
				int x = fgss[i].ptStart.x + (fgss[i].ptEnd.x - fgss[i].ptStart.x) / tks*j;
				p1.x = x; p2.x = x;
				p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
				p2.y = GetBoundPoint(x, p1.y);
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			else
			{
				MemDC.SelectObject(&penl);
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
	MemDC.SelectObject(&brushjx);
	MemDC.SelectObject(&penjx);
	MemDC.Rectangle(CRect(0, 610, 1362, 635));
	DeleteObject(penp);
	DeleteObject(penbh);
	DeleteObject(penl);
	DeleteObject(penfinish);
	DeleteObject(penjx);
	DeleteObject(pencu);
	DeleteObject(brush);
	DeleteObject(brushjx);
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
	int x = point.x;
	int y = point.y;
	CString crtpoint;
	crtpoint.Format(_T("(X = %d , Y = %d)"), x - 320, (y <= 610 ? 510 - y : -100));
	//绘图 （双缓存）
	CDC* pDC = GetDC();
	OnDraw(pDC);
	CDC&  MemDC = *pDC;
	//得到黑色画笔
	CPen penl(m_nLineStyle, m_nLineWidth, m_clrl);
	CPen pencu(m_nLineStyle, 2, RGB(255, 0, 128));
	//得到透明的画刷
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	MemDC.SelectObject(&penl);
	MemDC.SelectObject(pbrush);
	switch (m_nDrawType)
	{
	case 2:
		//判断是否接近起点（形成闭合区域）
		if (m_crt_p > 2 && PointHitTest(m_nzValues[0], point)) {
			CRect rc;
			rc.SetRect(m_nzValues[0].x - 12, m_nzValues[0].y - 12, m_nzValues[0].x + 12, m_nzValues[0].y + 12);
			MemDC.Ellipse(rc);
		}
		else if (m_crt_p > 0 && XYHitTest(m_nzValues[m_crt_p - 1], point)) {
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
			rc.SetRect(linepoint.x - 12, linepoint.y - 12, linepoint.x + 12, linepoint.y + 12);
			MemDC.Ellipse(rc);
		}
		if (m_pcrt[m_tid] > 0 && !m_drawonelinef) {
			MemDC.MoveTo(m_fjxpoint[m_tid][m_pcrt[m_tid] - 1]);
			MemDC.LineTo(point);
		}
		break;
	case 6:
		if (crt_fg == 1 && fg[1].x < point.x && fg[1].y < point.y) {
			CPoint p1, p2;
			double dx = (point.x - fg[1].x)*1.0; int k1 = (int)dx / jd_fg;
			double dy = (point.y - fg[1].y)*1.0; int k2 = (int)dy / jd_fg;
			for (int i = 0; i <= k1; i++) {//画竖线
				p1.x = fg[1].x + i*jd_fg; p2.x = p1.x;
				p1.y = fg[1].y; p2.y = point.y;
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			for (int i = 0; i <= k2; i++) {//画横线
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
		if (fg[1].x < point.x && fg[1].y < point.y &&point.x < fg[2].x && point.y < fg[2].y) {
			CPoint p1, p2;
			MemDC.SelectObject(&pencu);
			double dx = (point.x - fg[1].x)*1.0; int k1 = (int)dx / jd_fg;
			double dy = (point.y - fg[1].y)*1.0; int k2 = (int)dy / jd_fg;
			p1.x = fg[1].x + jd_fg*k1; p1.y = fg[1].y + jd_fg*k2;
			p2.x = fg[1].x + jd_fg*(k1 + 1); p2.y = fg[1].y + jd_fg*(k2 + 1);
			if (p2.x > fg[2].x)p2.x = fg[2].x;
			if (p2.y > fg[2].y)p2.y = fg[2].y;
			MemDC.Rectangle(CRect(p1, p2));
		}
		break;
	case 8:
		if (fg[1].x < point.x && fg[1].y < point.y &&point.x < fg[2].x && point.y < fg[2].y) {
			CPoint p1, p2, p0;
			MemDC.SelectObject(&pencu);
			double x0 = point.x - fg[1].x; int i = (int)x0 / jd_fg;//第i列
			double y0 = point.y - fg[1].y; int j = (int)y0 / jd_fg;//第j列
			double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//共k1+1列小方格
			double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//共k2+1行小方格
			int n = j*(k1 + 1) + i;//第n个方格

			//crtpoint.Format(_T("(X = %d , Y = %d)   K = %1.4f"), x - 320, (y <= 610 ? 510 - y : -100), fgss[n].k);

			if (fgss[n].qytks > 0 && fgss[n].qytks <= tks)
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%1.4f  安全系数：%1.4f 牵引条块数为：%d"), x - 320, (y <= 610 ? 510 - y : -100), fgss[n].bj, fgss[n].k, fgss[n].qytks);
			else
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%1.4f  安全系数：%1.4f"), x - 320, (y <= 610 ? 510 - y : -100), fgss[n].bj, fgss[n].k);

			if (fgss[n].k == 10000)break;

			p1.x = fg[1].x + jd_fg*i; p1.y = fg[1].y + jd_fg*j;
			p2.x = fg[1].x + jd_fg*(i + 1); p2.y = fg[1].y + jd_fg*(j + 1);
			if (p2.x > fg[2].x)p2.x = fg[2].x;
			if (p2.y > fg[2].y)p2.y = fg[2].y;
			MemDC.Rectangle(CRect(p1, p2));

			p0 = fgss[n].p0;
			double bj = fgss[n].bj;
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[n].ptStart);
			MemDC.MoveTo(p0);
			MemDC.LineTo(fgss[n].ptEnd);
			HDC hDC = MemDC.GetSafeHdc();
			Arc(hDC, p0.x - (int)bj, p0.y - (int)bj, p0.x + (int)bj, p0.y + (int)bj, fgss[n].ptStart.x, fgss[n].ptStart.y, fgss[n].ptEnd.x, fgss[n].ptEnd.y);
		}
		else
			if (fgss[minkid].qytks > 0 && fgss[minkid].qytks <= tks)
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%.2f  安全系数：%1.4f 牵引条块数为：%d"), x - 320, (y <= 610 ? 510 - y : -100), fgss[minkid].bj, fgss[minkid].k, fgss[minkid].qytks);
			else
				crtpoint.Format(_T("(X = %d , Y = %d) 半径：%.2f  安全系数：%1.4f"), x - 320, (y <= 610 ? 510 - y : -100), fgss[minkid].bj, fgss[minkid].k);
		break;
	}
	ReleaseDC(pDC);//释放pDC

	// 在状态栏的当前点窗格中显示点坐标字符串
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(crtpoint);
	CView::OnMouseMove(nFlags, point);
}

void CSlopeSView::OnLButtonUp(UINT nFlags, CPoint point)
{
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
			if (!FirstFocus) {
				CEdit* pedit = &m_EditPoint;
				pedit->ShowWindow(SW_HIDE);
			}
			m_bDraw[0] = false;
			m_bDraw[1] = true;

		}
		else {
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
		else if (crt_fg == 1) {
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
		else if (fg[1].x < point.x && fg[1].y < point.y &&point.x < fg[2].x && point.y < fg[2].y) {
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
	CView::OnLButtonUp(nFlags, point);
}

void CSlopeSView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)//按下键触发事件
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_ESCAPE && m_nDrawType < 4)
	{//按下ESC时触发
		if (m_crt_p > 0) m_crt_p--;
		//Invalidate();//清屏
		if (bihe) {
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
		else if (m_crt_p <= 0) {
			if (m_nDrawType == 2)
				m_nDrawType = 1;
			else {
				m_nDrawType = 0;
				SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//标准光标
				if (!FirstFocus) {
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

		for (int k = 0; k < 10; k++) {
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
			for (int i = 0; i < bitStru.bmHeight - 1; i++)
			{
				if (i == 0)continue;
				head = nBytePreLine*i;
				for (int j = 0; j < bitStru.bmWidth - 1; j++)
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
	if (rc.PtInRect(point)) {//检测origin是否在建立的矩形内;
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
	for (int i = 0; i < m_crt_p - 1; i++) {//遍历每条线段
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
		if (m_pcrt[m_tid] > 0) {//采用鼠标点延长线与边界相交的方式
			m1 = m_fjxpoint[m_tid][m_pcrt[m_tid] - 1]; m2 = point;
			a2 = m2.y - m1.y; b2 = m1.x - m2.x; c2 = m2.x*m1.y - m2.y*m1.x;//a2,b2,c2为材料线数据
			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)) {//靠近了边界线
				if (b1 == 0) {//边界线斜率为无穷大,k1=无穷
					k2 = -a2 / b2; ju2 = a2*m1.x*1.0 / b2 + m1.y;
					linepoint.x = p1.x;
					linepoint.y = (int)(k2*linepoint.x + ju2);
				}
				else if (b2 == 0) {//材料线斜率为无穷大,k2=无穷
					k1 = -a1 / b1; ju1 = a1*p1.x*1.0 / b1 + p1.y;
					linepoint.x = m1.x;
					linepoint.y = (int)(k1*linepoint.x + ju1);
				}
				else {//边界线及材料线斜率不为无穷大，采用两线相交的方式
					k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
					k2 = -a2*1.0 / b2; ju2 = a2*m1.x*1.0 / b2 + m1.y;
					if (fabs(k1) > fabs(k2)) {//边界线斜率大，先求y后求x
						if (k2 == 0) {
							linepoint.y = (int)ju2;
							linepoint.x = (int)((ju2 - ju1)*1.0 / k1);
						}
						else {
							linepoint.y = (int)((k2*ju1 - k1*ju2)*1.0 / (k2 - k1));
							linepoint.x = (int)(((k2*ju1 - k1*ju2)*1.0 / (k2 - k1) - ju1)*1.0 / k1);//不能用y直接求x
						}
					}
					else {//材料线斜率大，先求x后求y
						if (fabs(k1) < 0.03) {
							linepoint.x = point.x;
							linepoint.y = (int)(k1*point.x + ju1);
						}
						else {
							linepoint.x = (int)((ju2 - ju1)*1.0 / (k1 - k2));
							linepoint.y = (int)(k1*(ju2 - ju1)*1.0 / (k1 - k2) + ju1);//不能用x直接求y
						}
					}
				}
				//当linepoint没落在边界线范围时
				if (!rc.PtInRect(linepoint)) {//取鼠标所在点y值
					linepoint.y = point.y;
					if (b1 != 0) {
						k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
						linepoint.x = (int)((point.y - ju1)*1.0 / k1);
					}
					else linepoint.x = p1.x;
					if (!rc.PtInRect(linepoint)) {//取鼠标所在点x值
						k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
						linepoint.x = point.x;
						linepoint.y = (int)(k1*point.x + ju1);
						if (!rc.PtInRect(linepoint)) {
							Linebz = false;
							return false;
						}
					}
				}
				Linebz = true;
				return true;
			}
		}
		else {//所画的第一个点，采用从鼠标点向边界线做垂线的方式

			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)) {
				if (b1 == 0) {
					linepoint.x = p1.x;
					linepoint.y = point.y;
				}
				else {
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
	for (int i = 0; i < TUCENG_COUNT; i++) {
		if (color == tuceng[i].clr)
			return i;
	}
	return 0;

}

//看p上方还有无点
void CSlopeSView::SearchToptcid(int tcid)
{
	CPoint p = RButtonUp;
	for (int i = 0; i < m_tid; i++) {//第i条材料线
		for (int j = 0; j < m_pcrt[i] - 1; j++) {//第k段线
			CPoint p1, p2;//startp,nextp
			p1 = m_fjxpoint[i][j]; p2 = m_fjxpoint[i][j + 1];
			double xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x);
			double ju = p1.y - xlv*p1.x;
			int y = (int)(p.x*xlv + ju);
			if (p1.x <= p.x && p.x <= p2.x) {
				if (y < p.y) {
					return;
				}
				break;
			}
		}
	}
	toptcid = tcid;
}

//搜索土层上下材料线
void CSlopeSView::SetSXtuceng(CDC* pDC) {
	for (int tid = 0; tid < m_tid; tid++) {
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
	if (x > 0 && y > 0) { return atan(y / x); }
	else if (x < 0 && y>0) { x = -x; return pi - atan(y / x); }
	else if (x < 0 && y < 0) { x = -x; y = -y; return pi + atan(y / x); }
	else if (x > 0 && y < 0) { y = -y; return 2 * pi - atan(y / x); }
	else if (x == 0) {
		if (y == 0)return 0;
		else if (y > 0) return pi / 2;
		else return 3 / 2 * pi;
	}
	else if (y == 0) {
		if (x >= 0) return 0;
		else return pi;
	}
	return 0;
}

//要求:p.x= x , p.y< y,返回x落在边界上的y值
int CSlopeSView::GetBoundPoint(double x, double y)
{
	//注意处理垂直边界线p1.x=p2.x
	for (int i = 0; i < m_crt_p - 1; i++)
	{
		CPoint p1, p2;//边界线的左右点
		p1 = m_nzValues[i]; p2 = m_nzValues[i + 1];
		if ((int)x<min(p1.x, p2.x) || (int)x>max(p1.x, p2.x))
			continue;
		if (p1.x == p2.x)
		{
			if ((int)x == p1.x)
			{
				if (y > max(p1.y, p2.y))
					return max(p1.y, p2.y);
				else if (y >= min(p1.y, p2.y) && y <= max(p1.y, p2.y))
					return (int)y;
			}
		}
		else
		{
			int yl = (int)(p1.y + (p2.y - p1.y)*(x - p1.x)*1.0 / (p2.x - p1.x));
			if ((int)yl <= (int)y)
				return yl;
		}
	}
	return (int)y;
}

//按升序排序函数(坐标轴是向右下方)
int cmp(const CPoint &a, const CPoint &b)
{
	if (a.x < b.x) return 1;
	else if (a.x > b.x) return 0;
	else {
		if (a.y >= b.y) return 1;
		else return 0;
	}
}

int CSlopeSView::checkcolor(CPoint p, int sx, CDC* pDC) {
	int id = -1;
	HDC hDC = pDC->GetSafeHdc(); //获取当前屏幕DC
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
	do {
		COLORREF clr = ::GetPixel(hDC, p.x, p.y); //获取当前鼠标点像素值
		int color = clr;
		for (int i = 0; i < TUCENG_COUNT; i++) {
			if (color == tuceng[i].clr) {
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
double CalAre(double k1, double b1, double k2, double b2, int x1, int x2) {
	return fabs((k1 - k2)*(x2*x2 - x1*x1)*0.5 + (b1 - b2)*(x2 - x1));
}

//拦截回车键
BOOL CSlopeSView::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
		HWND hWnd = ::GetFocus(); /*   判断当前焦点是否在编辑框上    */
		if (::GetDlgCtrlID(hWnd) == IDC_POINT_EDIT) {
			// 添加对消息处理的代码，做你想做的事情!
			CEdit * pedit = (CEdit*)GetDlgItem(IDC_POINT_EDIT);
			CString str, str1, str2;
			UpdateData(TRUE);
			pedit->GetWindowText(str);
			int index = str.Find(_T(","));   //找.的位置
			if (index != -1) {
				int l = str.GetLength();//获取字符长，中文也算1个字符
				str1 = str.Left(index);
				str2 = str.Right(l - index - 1);
				int x = _wtoi(str1);
				int y = _wtoi(str2);
				if (x >= 0 && x <= 1362 && y >= 0 && y <= 610)
				{//点在用户界面区
					//x - 320, 510 - y 
					x += 320; y = 510 - y;
					//str.Format(_T("x=%d,y=%d"), x, y);
					//AfxMessageBox(str);//弹出消息
					::SendMessage(m_hWnd, WM_LBUTTONUP, 0, MAKELONG(x, y));//模拟鼠标左键
				}
				else
					AfxMessageBox(_T("请注意输入范围: -320<=x<=1042, -100<=y<=510 "));
				pedit->SetWindowText(_T(""));
			}
			else
				AfxMessageBox(_T("请注意输入格式: x,y "));
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

//寻找交点
void CSlopeSView::FindAllCrossPoint(int n, CPoint p0, double bj) {
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
		if (p1.x != p2.x) {//斜率存在
			xl = min(p1.x, p2.x); xr = max(p1.x, p2.x);
			xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x); ju = p1.y - xlv*p1.x;
			//圆心是p1
			a = xlv*xlv + 1; b = 2 * xlv*(ju - p0.y) - 2 * p0.x; c = p0.x*p0.x + (ju - p0.y)*(ju - p0.y) - bj*bj;
			dta = b*b - 4 * a*c;
			if (dta <= 0)continue;
			else {
				x1 = (int)((-b + sqrt(dta)) / 2 / a); x2 = (int)((-b - sqrt(dta)) / 2 / a);
				if (x1 >= xl&&x1 < xr) {
					cntfg.p[cntfg.cnt].p.x = x1;
					cntfg.p[cntfg.cnt].p.y = (int)(xlv*x1 + ju);
					cntfg.cnt++;
				}
				if (x2 >= xl&&x2 < xr) {
					cntfg.p[cntfg.cnt].p.x = x2;
					cntfg.p[cntfg.cnt].p.y = (int)(xlv*x2 + ju);
					cntfg.cnt++;
				}
			}
		}
		else {//斜率不存在
			yu = min(p1.y, p2.y); yd = max(p1.y, p2.y);
			y1 = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
			y2 = (int)(p0.y - sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));
			if (y1 >= yu&&y1 < yd) {
				cntfg.p[cntfg.cnt].p.x = p1.x;
				cntfg.p[cntfg.cnt].p.y = y1;
				cntfg.cnt++;
			}
			if (y2 >= yu&&y2 < yd) {
				cntfg.p[cntfg.cnt].p.x = p1.x;
				cntfg.p[cntfg.cnt].p.y = y2;
				cntfg.cnt++;
			}
		}
	}
}

//对每段圆弧进行计算
void CSlopeSView::Calculate(int n, int f, CPoint p0, double bj, CDC* pDC) {
	struct fgss  &cntfg = fgss[n];
	struct point &start = cntfg.p[f];//开始点（按逆时针得到的）
	struct point &end = cntfg.p[f + 1];//结束点
	CPoint pp[30][4];	//CPoint pp[4][4];		//记录条块上所有点坐标

	//	double sina = 0.0, cosa = 0.0;
	double cosa[30], sina[30], alf[30], tgf[30], fai[30], A[30], W[30], L[30], cl[30], m[30], cd[30], E[30];
	int number[30];
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

	for (int i = 0; i < tks; i++)
	{
		//对每个条块(第i个条块)
		cosa[i] = sina[i] = alf[i] = tgf[i] = fai[i] = A[i] = W[i] = L[i] = cl[i] = m[i] = cd[i] = E[i] = 0.0;
		number[i] = 0;
		double line[20], are[20];	//记录每个土层在土条底层所占长,及土条内所占面积
		for (int j = 0; j < 10; j++)
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
		sbjpid = tkpid;

		if (p2.y == p4.y)
		{
			tkp[tkpid++] = p2;
			pp[i][0] = p1;
			pp[i][1] = p2;
			pp[i][2] = p3;
			number[i] = 3;
		}
		if (p1.y == p3.y)
		{
			tkp[tkpid++] = p1;//补齐下边界线（仅两点）
			pp[i][0] = p1;
			pp[i][1] = p2;
			pp[i][2] = p4;
			number[i] = 3;
		}
		if (p1.y != p3.y && p2.y != p4.y)
		{
			tkp[tkpid++] = p2;
			tkp[tkpid++] = p1;
			pp[i][0] = p1;
			pp[i][1] = p2;
			pp[i][2] = p3;
			pp[i][3] = p4;
			number[i] = 4;

		}
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
		cosa[i] = (p2.x - p1.x) / L[i];
		sina[i] = (p1.y - p2.y) / L[i];
		alf[i] = acos(cosa[i]);
		tgf[i] = tan(fai[i] * pi / 180);
		W[i] = 0; cl[i] = 0;
		for (int j = 0; j < 10; j++)
		{
			//把10个土层在一个条块内做的贡献加起来
			W[i] += are[j] * tuceng[j].gama;
			cl[i] += tuceng[j].c*line[j];
		}
		if (W[i] == 0)
			return;
	}

	double fenzi = 0; double fenmu = 0;//分子，分母
	double K(0), K0(0);
	double fsl, fsr;
	Mqytks = 0;
	double fs;
	switch (mode)
	{
	case 1:
		double a11 = 0.0, a12 = 0.0, a13 = 0.0, a21 = 0.0, a22 = 0.0, a23 = 0.0, a31 = 0.0, a32 = 0.0, a33 = 0.0;
		double A11 = 0.0, A12 = 0.0, A13 = 0.0, A21 = 0.0, A22 = 0.0, A23 = 0.0, A31 = 0.0, A32 = 0.0, A33 = 0.0;
		double area = 0;
		double offerx = 0.0, offery = 0.0;
		CPoint center;
		center.x = 0;
		center.y = 0;
		double ks = 100000.000000;
		double x = 0.0, y = 0.0, o = 0.0;
		double Fx[POINT_COUNT]; double Fy[POINT_COUNT]; double M[POINT_COUNT];
		for (int i = 0; i < tks; i++)
		{
			//CPoint pp[10] = { 0 };//记录条块上所有点坐标
			Fx[i] = 0.0; Fy[i] = W[i] * ks; M[i] = 0.0;
			for (int l = 0; l < number[i] - 1; l++)
			{
				area += (pp[i][l].x*pp[i][l + 1].y - pp[i][l + 1].x*pp[i][l].y) / 2;
				center.x += (pp[i][l].x*pp[i][l + 1].y - pp[i][l + 1].x*pp[i][l].y) * (pp[i][l].x + pp[i][l + 1].x);
				center.y += (pp[i][l].x*pp[i][l + 1].y - pp[i][l + 1].x*pp[i][l].y) * (pp[i][l].y + pp[i][l + 1].y);
			}
			area += (pp[i][number[i] - 1].x*pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y) / 2;
			center.x += (pp[i][number[i] - 1].x*pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y) * (pp[i][number[i] - 1].x + pp[i][0].x);
			center.y += (pp[i][number[i] - 1].x*pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y) * (pp[i][number[i] - 1].y + pp[i][0].y);

			center.x /= 6 * area;
			center.y /= 6 * area;
			for (int m = 0; m < number[i]; m++)
			{
				a11 = 0.0, a12 = 0.0, a13 = 0.0, a21 = 0.0, a22 = 0.0, a23 = 0.0, a31 = 0.0, a32 = 0.0, a33 = 0.0;
				a11 = -ks;
				a13 = (pp[i][m].y - center.y) * ks;
				a22 = -ks;
				a32 = -(pp[i][m].x - center.x) * ks;

				if (pp[i][m].y > center.y)
				{
					if (pp[i][m].x > center.x)
					{
						a23 = (pp[i][m].x - center.x) * ks;
					}
					else if (pp[i][m].x < center.x)
					{
						a23 = -(pp[i][m].x - center.x) * ks;
					}
					a31 = (pp[i][m].y - center.y) * ks;
					a33 = a23 * (pp[i][m].x - center.x) - a13 * (pp[i][m].y - center.y) - (pp[i][m].y - center.y)* W[i];
					A11 += a11; A12 += a12; A13 += a13; A21 += a21; A22 += a22; A23 += a23; A31 += a31; A32 += a32; A33 += a33;
				}
				else if (pp[i][m].y < center.y)
				{
					if (pp[i][m].x > center.x)
					{
						a23 = -(pp[i][m].x - center.x)*ks;
					}
					else if (pp[i][m].x < center.x)
					{
						a23 = (pp[i][m].x - center.x)*ks;
					}
					a31 = -(pp[i][m].y - center.y)*ks;
					a33 = a23*(pp[i][m].x - center.x) - a13*(pp[i][m].y - center.y) - (pp[i][m].y - center.y)*W[i];
					A11 += 2 * a11; A12 += 2 * a12; A13 += 2 * a13; A21 += 2 * a21; A22 += 2 * a22; A23 += 2 * a23; A31 += 2 * a31; A32 += 2 * a32; A33 += 2 * a33;
				}
			}
			o = (A22 * A31 * Fx[i] + A11 * A32 * Fy[i] - A11 * A22 * M[i]) / (A11 * A22 * A33 - A13 * A22 * A31 - A11 * A23 * A32);
			x = (-Fx[i] - A13) * o / A11;
			y = (-Fy[i] - A23 * o) / A22;
			for (int p = 0; p < n; p++)
			{
				int j = 0;
				if (number[i] == 3)
				{
					offerx = x - (pp[i][p].y - center.y)*o;
					offery = y + (pp[i][p].x - center.x)*o;
					if (pp[i][p].y > center.y)
					{
						Fxx[i][j] = offerx*cos(90 * pi / 180)*ks + offery*cos(0 * pi / 180)*ks;
						Fxy[i][j] = offerx*cos(0 * pi / 180)*ks + offery*cos(90 * pi / 180)*ks;
						j++;
					}
					else
					{
						Fxx[i][j] = offerx*cos(90 * pi / 180)*ks + offery*cos(0 * pi / 180)*ks + (offerx*sina[i] * ks + offery*cosa[i] * ks)*sina[i];
						Fxy[i][j] = offerx*cos(0 * pi / 180)*ks + offery*cos(90 * pi / 180)*ks + (offerx*cosa[i] * ks + offery*sina[i] * ks)*cosa[i];
					}
				}
				else 
				{
					offerx = x - (pp[i][p].y - center.y)*o;
					offery = y + (pp[i][p].x - center.x)*o;
					if (pp[i][p].y > center.y && pp[i][p].x < center.x)
					{
						Fxx[i][j] = offerx*cos(90 * pi / 180)*ks + offery*cos(0 * pi / 180)*ks + Fxx[i - 1][j - 1];
						Fxy[i][j] = offerx*cos(0 * pi / 180)*ks + offery*cos(90 * pi / 180)*ks + Fxy[i - 1][j - 1];
					}
					else if (pp[i][p].y > center.y && pp[i][p].x > center.x)
					{
						Fxx[i][j] = offerx*cos(90 * pi / 180)*ks + offery*cos(0 * pi / 180)*ks;
						Fxy[i][j] = offerx*cos(0 * pi / 180)*ks + offery*cos(90 * pi / 180)*ks;
					}
					else if (pp[i][p].y < center.y && pp[i][p].x < center.x)
					{
						Fxx[i][j] = offerx*cos(90 * pi / 180)*ks + offery*cos(0 * pi / 180)*ks + (offerx*sina[i] * ks + offery*cosa[i] * ks)*sina[i] - Fxx[i - 1][j - 1];
						Fxy[i][j] = offerx*cos(0 * pi / 180)*ks + offery*cos(90 * pi / 180)*ks + (offerx*cosa[i] * ks + offery*sina[i] * ks)*cosa[i] - Fxy[i - 1][j - 1];
					}
				}

				Fx[i + 1] += cntfg.t[i].Fxx[j];
				Fy[i + 1] += cntfg.t[i].Fxy[j];
				if (pp[i][p].x < center.x)
				{
					M[i + 1] += (Fxx[i][j] * sqrt(pow((pp[i][p].y - center.y), 2) - Fxy[i][j] * sqrt((pp[i][p].x - center.x))));
				}
				else
				{
					M[i + 1] += (Fxx[i][j] * sqrt(pow((pp[i][p].y - center.y), 2) + Fxy[i][j] * sqrt((pp[i][p].x - center.x))));
				}
			}


			for (int i = 0; i < tks; i++)
			{
				/*fenzi += N[i] * tgf[i] + cl[i];
				fenmu += W[i] * sina[i];*/
				//fenzi += N[i] * tgf[i] + cl[i];
				fenzi += W[i] * tgf[i] * cosa[i] + cl[i];
				fenmu += M[i];
			}
		}

		if (fenmu < 0.0001)
			return;
		K = fenzi / fenmu;//得到这一圆弧的K
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
	CEdit* pedit = &m_EditPoint;
	if (FirstFocus) {
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
	if (!FirstFocus) {
		CEdit* pedit = &m_EditPoint;
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
	for (int i = 0; i < TUCENG_COUNT; i++) {
		dlg.m_tuceng[i].phi = tuceng[i].phi;
		dlg.m_tuceng[i].c = tuceng[i].c;
		dlg.m_tuceng[i].gama = tuceng[i].gama;
		dlg.m_tuceng[i].name = tuceng[i].name;
		dlg.m_tuceng[i].clr = tuceng[i].clr;
	}
	if (IDOK == dlg.DoModal()) {//点击OK
		for (int i = 0; i < TUCENG_COUNT; i++) {
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
	if (IDOK == dlg.DoModal())
	{
		if (dlg.bianchang > 5 && dlg.bianchang <= 20)
			jd_fg = dlg.bianchang;
		else
			jd_fg = 5;

		if (dlg.banjing > 3 && dlg.banjing <= 20)
			jd_yh = dlg.banjing;
		else
			jd_yh = 3;

		if (dlg.tiaokuaishu > 1 && dlg.tiaokuaishu <= 20)
			tks = dlg.tiaokuaishu;
		else
			tks = 10;

		if (m_DataDlg != NULL)
			m_DataDlg->tks = tks;

		check1 = dlg.check1;
	}
}

//采用计算模型
void CSlopeSView::OnUsM()
{
	// TODO:  在此添加命令处理程序代码
	CMethod dlg;
	dlg.mode = mode;
	dlg.m_bishop = bishop;
	dlg.m_bph = bph;
	if (IDOK == dlg.DoModal())
	{
		mode = dlg.mode;
		bishop = dlg.m_bishop;
		bph = dlg.m_bph;
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
	for (int i = 0; i < m_crt_p - 1; i++)
	{
		if (m_nzValues[i].y + m_nzValues[i + 1].y > maxy)
		{
			maxy = m_nzValues[i].y + m_nzValues[i + 1].y;
			bianjie[0] = i;//下边界线
		}
		if (m_nzValues[i].x + m_nzValues[i + 1].x > maxx)
		{
			maxx = m_nzValues[i].x + m_nzValues[i + 1].x;
			bianjie[1] = i;//右边界线
		}
		if (check1 == 1 && m_nzValues[i].x + m_nzValues[i + 1].x < minx)
		{
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
	double dx = fg[2].x - fg[1].x;
	int k1 = (int)dx / jd_fg;//共k1+1列小方格
	double dy = fg[2].y - fg[1].y;
	int k2 = (int)dy / jd_fg;//共k2+1行小方格
	double min_k = 10000;//所有方格的minK

	for (int i = 0; i <= k1; i++)//搜索到第i列小方格(0~k1+1)
	{
		p0.x = (int)(fg[1].x + i*jd_fg + ban);
		if (i == k1)p0.x = (int)((fg[2].x - fg[1].x - i*jd_fg) / 2.0 + fg[1].x + i*jd_fg);

		for (int j = 0; j <= k2; j++)//搜索到第j行小方格(0~k2+1)
		{
			int n = j*(k1 + 1) + i;//搜索到第n个小方格
			totfg++;
			struct fgss  &cntfg = fgss[n];//引用
			cntfg.k = 10000;//这一方格的minK
			p0.y = (int)(fg[1].y + j*jd_fg + ban);
			if (j == k2)
				p0.y = (int)((fg[2].y - fg[1].y - j*jd_fg) / 2.0 + fg[1].y + j*jd_fg);
			//这就已经找到了圆心p0,接下来找半径。
			//最大半径应该是圆心到底边的距离
			maxbj = fabs(dbk*p0.x - p0.y + dbb) / sqrt(1 + dbk*dbk);
			int k3 = (int)maxbj / jd_yh, bj;

			for (int k = 1; k <= k3; k++)
			{
				cntfg.cnt = 0;//清零（对每个半径）
				bj = k*jd_yh;

				//半径为bj，接下来搜索圆弧与边界线所有的交点。
				//画圆弧的方法：找与边界的交点→交点按圆心角排序（逆时针：根据斜率与象限判断）→判断相邻交点在圆上的中点是否在边界内→如果在边界内绘制圆弧线
				FindAllCrossPoint(n, p0, bj);//1.找圆心为p0,半径为bj的圆与边界的交点

				for (int l = 0; l < cntfg.cnt; l++)//2.交点按圆心角排序（逆时针：根据斜率与象限判断）
				{
					double x = cntfg.p[l].p.x - p0.x, y = cntfg.p[l].p.y - p0.y;
					cntfg.p[l].ang = GetAng(x, y);
				}
				std::sort(cntfg.p, cntfg.p + cntfg.cnt);

				for (int l = 0; l < cntfg.cnt - 1; l++)//3.判断相邻交点在圆上的中点是否在边界线以内
				{
					double ang = (cntfg.p[l].ang + cntfg.p[l + 1].ang) / 2.0;
					int x = (int)(cos(ang)*bj + 0.5), y = (int)(sin(ang)*bj + 0.5);
					int z = x; x = -y; y = -z;//之前转换了坐标，这里转回来
					x += p0.x; y += p0.y;
					if (PtInRegion(rg, x, y))
					{
						//接下来对每段圆弧进行条分(一个半径下可能产生多段圆弧)
						Calculate(n, l, p0, bj, pDC);//第n个方格,圆心p0,半径bj,下第l条圆弧
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

void CSlopeSView::Write()
{
	struct fgss  &cntfg = fgss[fgid];
	CString s0, s1, s2, s3;
	s0.Format(_T("圆心(x,y)：(%d,%d)\r\n半径：%.2f\r\n最小安全系数:%.4f\r\n--------------------------------------------------------------\r\n条块%d\r\n--------------------------------------------------------------\r\n"), fgss[fgid].p0.x - 320, 510 - fgss[fgid].p0.y, fgss[fgid].bj, fgss[fgid].k, tkid + 1);
	//	s1.Format(_T("重量：%.2f\r\nα:%.2f°\r\ncos(α)：%.2f\r\nsin(α)：%.2f\r\n加权平均φ：%.2f°\r\n面积：%.2f\r\n底边长：%.2f\r\n"), fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosa, fgss[fgid].t[tkid].sina, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
	s1.Format(_T("重量：%.2f\r\nα:%.2f°\r\ncos(α)：%.2f\r\nsin(α)：%.2f\r\n加权平均φ：%.2f°\r\n面积：%.2f\r\n底边长：%.2f\r\n"), fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosa, fgss[fgid].t[tkid].sina, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
	if (fgss[fgid].t[tkid].number == 3)
	{
		s2.Format(_T("从下左开始逆时针受力X:%.2f\r\n,Y:%.2f\r\n；X:%.2f\r\n,Y:%.2f\r\n；X:%.2f\r\n,Y:%.2f\r\n"), fgss[fgid].t[tkid].Fxx[0], fgss[fgid].t[tkid].Fxy[0]
			, fgss[fgid].t[tkid].Fxx[1], fgss[fgid].t[tkid].Fxy[1], fgss[fgid].t[tkid].Fxx[2], fgss[fgid].t[tkid].Fxy[2]);
	}
	else {
		s2.Format(_T("从下左开始逆时针受力X:%.2f\r\n,Y:%.2f\r\n；X:%.2f\r\n,Y:%.2f\r\n；X:%.2f\r\n,Y:%.2f\r\n；X:%.2f\r\n,Y:%.2f\r\n"), fgss[fgid].t[tkid].Fxx[0], fgss[fgid].t[tkid].Fxy[0], fgss[fgid].t[tkid].Fxx[1],
			fgss[fgid].t[tkid].Fxy[1], fgss[fgid].t[tkid].Fxx[2], fgss[fgid].t[tkid].Fxy[2], fgss[fgid].t[tkid].Fxx[3], fgss[fgid].t[tkid].Fxy[3]);
	}
	double fenzi = 0, fenmu = 0;
	double E[30], cd[30];
	switch (mode)
	{
	case 1:
		for (int i = 0; i <= tkid; i++)
		{
			if (fgss[fgid].t[tkid].number == 3)
			{
				fenzi += (fgss[fgid].t[tkid].Fxx[2] * fgss[fgid].t[tkid].cosa + fgss[fgid].t[tkid].Fxy[2] * fgss[fgid].t[tkid].sina)*fgss[fgid].t[tkid].sina / fgss[fgid].t[tkid].cosa + cntfg.t[i].cl;
			}
			else {
				fenzi += (fgss[fgid].t[tkid].Fxx[2] * fgss[fgid].t[tkid].cosa + fgss[fgid].t[tkid].Fxy[2] * fgss[fgid].t[tkid].sina)*fgss[fgid].t[tkid].sina / fgss[fgid].t[tkid].cosa + cntfg.t[i].cl;
				fenzi += (fgss[fgid].t[tkid].Fxx[3] * fgss[fgid].t[tkid].cosa + fgss[fgid].t[tkid].Fxy[3] * fgss[fgid].t[tkid].sina)*fgss[fgid].t[tkid].sina / fgss[fgid].t[tkid].cosa + cntfg.t[i].cl;
			}
			fenmu += fgss[fgid].t[tkid].W * fgss[fgid].t[tkid].sina;
		}

		s3.Format(_T("--------------------------------------------------------------\r\n细胞机条分法\r\n--------------------------------------------------------------\r\n安全系数：%.4f\r\n"), fenzi / fenmu);
		break;
	}
	/*
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
	break;*/
	m_DataDlg->m_text.SetWindowText(s0 + s1 + s2 + s3);
}

//数据文件存储函数，默认不启用

