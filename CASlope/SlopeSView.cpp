
// SlopeSView.cpp : CSlopeSView ���ʵ��

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CSlopeSView ����/����

CSlopeSView::CSlopeSView()
{
	// TODO:  �ڴ˴���ӹ������
	m_scale = 1.0f;
	m_translateX = 320;
	m_translateY = 610;
	m_off_x = 0;
	m_off_y = 0;
	FirstFocus = true;
	m_nDrawType = 0;
	m_nLineStyle = PS_SOLID;
	m_nLineWidth = 1;
	m_delete_id = -1;
	m_crt_p = 0; m_tid = 0; m_tcnt = 0;
	m_clrp = RGB(255, 0, 0);//��
	m_clrl = RGB(0, 0, 0);//��
	bihe = false; m_drawonelinef = false;
	memset(m_bDraw, false, sizeof(bool) * 20);
	m_bDraw[0] = true;//���ư�ťenable
	xypoint = 0; linepoint = 0;
	xybz = false;
	Linebz = false;
	// ������m_nzValues��Ԫ�ض���ʼ��Ϊ0   
	memset(m_nzValues, 0, sizeof(CPoint)* POINT_COUNT);
	memset(m_pcrt, 0, sizeof(int)* POINT_COUNT);
	memset(m_fjxpoint, 0, sizeof(CPoint)* POINT_COUNT*POINT_COUNT);
	tc_step = 0;
	memset(m_tfinish, false, sizeof(bool)* POINT_COUNT);
	pTuCeng = 0;
	pFocus = 0;
	for (int i = 0; i < 10; i++){
		CString s;
		s.Format(_T("����%d"), i + 1);
		tuceng[i].gama = 18.0;
		tuceng[i].c = 15.0;
		tuceng[i].phi = 20.0;
		tuceng[i].name = s;
		if (i == 0)
			tuceng[i].clr = (int)(RGB(252, 243, 169));
		else
			tuceng[i].clr = (int)(RGB(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));//����0��255�������
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
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}





// CSlopeSView ����


#ifdef _DEBUG
void CSlopeSView::AssertValid() const
{
	CView::AssertValid();
}

void CSlopeSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSlopeSDoc* CSlopeSView::GetDocument() const // �ǵ��԰汾��������
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
// CSlopeSView �¼�������
void CSlopeSView::OnDraw(CDC* pDC)//�ػ����ƣ�˫���壩
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
	MemDC.m_DC.FillSolidRect(&rect, RGB(255, 255, 255));//���������pDC->GetBkColor()
	//����
	CPen penp(m_nLineStyle, m_nLineWidth, m_clrp);
	CPen penbh(m_nLineStyle, 4, RGB(128, 128, 128));
	CPen penfinish(m_nLineStyle, 2, RGB(211, 208, 207));
	CPen penl(m_nLineStyle, m_nLineWidth, m_clrl);
	CPen pencu(m_nLineStyle, 1, RGB(0, 0, 255));
	//��ˢ
	CBrush brush(tuceng[0].clr);
	//��ͼ
	for (int i = 0; i < m_crt_p; i++)//�ػ��߽���
	{
		CPoint point = m_nzValues[i];
		MemDC.MoveTo(point);
		MemDC.m_DC.SelectObject(&penp);
		if (bihe)MemDC.m_DC.SelectObject(&penl);
		int cross_size = (5 / m_scale) ;
		if (cross_size <= 0)
			cross_size = 1;
		MemDC.LineTo(MyPoint(point.x + cross_size, point.y)); //*���Ƶ�
		MemDC.LineTo(MyPoint(point.x - cross_size, point.y));
		MemDC.MoveTo(MyPoint(point));
		MemDC.LineTo(MyPoint(point.x, point.y + cross_size));
		MemDC.LineTo(MyPoint(point.x, point.y - cross_size));
		MemDC.MoveTo(MyPoint(point));
		MemDC.m_DC.SelectObject(&penl);
		if (bihe)MemDC.m_DC.SelectObject(&penbh);
		if (i<m_crt_p - 1)MemDC.LineTo(m_nzValues[i + 1]);//*������
	}

	//����ɫ
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

	MemDC.m_DC.SetBkMode(TRANSPARENT);//�������ֱ���͸��
	//��ʾ��������֣�x,y��
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

	for (int i = 0; i <= m_tid; i++)//�ػ�������
	{
		for (int j = 0; j < m_pcrt[i]; j++)//��i����j��
		{
			CPoint point = m_fjxpoint[i][j];
			MemDC.MoveTo(point);
			MemDC.m_DC.SelectObject(&penl);
			int cross_size = (5 / m_scale);
			if (cross_size <= 0)
				cross_size = 1;
			MemDC.LineTo(MyPoint(point.x + cross_size, point.y)); //*���Ƶ�
			MemDC.LineTo(MyPoint(point.x - cross_size, point.y));
			MemDC.MoveTo(MyPoint(point));
			MemDC.LineTo(MyPoint(point.x, point.y + cross_size));
			MemDC.LineTo(MyPoint(point.x, point.y - cross_size));
			MemDC.MoveTo(MyPoint(point));
			if (m_tfinish[i])MemDC.m_DC.SelectObject(&penfinish);
			if (j<m_pcrt[i] - 1)MemDC.LineTo(m_fjxpoint[i][j + 1]);
		}
	}
	for (int i = 1; i <= tc_step; i++)//���Զ������
	{
		CBrush tcbrush(tianchong[i].clr);
		MemDC.m_DC.SelectObject(tcbrush);
		CPoint point = tianchong[i].point;

		HDC hDC = MemDC.m_DC.GetSafeHdc(); //��ȡ��ĻDC

		// �ٻ�ȡ��ǰ��λ������ֵ  
		COLORREF color = ::GetPixel(hDC, point.x, point.y);
		MemDC.ExtFloodFill(point.x, point.y, color, FLOODFILLSURFACE);//ȡ��Ļ������أ�������������tcbrush
		DeleteObject(tcbrush);
	}
	
	//�ػ�����
	if (crt_fg == 2)
	{
		MemDC.m_DC.SelectObject(&penl);
		CPoint p1, p2;
		double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;
		double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;
		
		//������
		for (int i = 0; i <= k1; i++)
		{
			p1.x = fg[1].x + i*jd_fg; p2.x = p1.x;
			p1.y = fg[1].y; p2.y = fg[2].y;
			MemDC.MoveTo(p1);
			MemDC.LineTo(p2);
		}
		
		//������
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
		
		//����ɫ
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
	CBrush brushjx(RGB(0, 128, 225));//��ɫ
	CPen penjx(m_nLineStyle, 1, RGB(225, 225, 255));
	MemDC.m_DC.SelectObject(&brushjx);
	MemDC.m_DC.SelectObject(&penjx);
	MemDC.m_DC.Rectangle(CRect(0, 610, 1362, 635));
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			pedit->SetReadOnly(true);;//ʧȥ����
			this->SetFocus();//���ý���Ϊ��ǰ����
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
	//��ͼ ��˫���棩
	CDC* pDC = GetDC();
	OnDraw(pDC);
	CDC&  dcMemDC = *pDC;
	CScalableDC MemDC(dcMemDC);
	MemDC.m_Scale = m_scale;
	MemDC.m_off_x = m_off_x;
	MemDC.m_off_y = m_off_y;
	//�õ���ɫ����
	CPen penl(m_nLineStyle, m_nLineWidth, m_clrl);
	CPen pencu(m_nLineStyle, 2, RGB(255, 0, 128));
	//�õ�͸���Ļ�ˢ
	CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	MemDC.m_DC.SelectObject(&penl);
	MemDC.m_DC.SelectObject(pbrush);
	switch (m_nDrawType){
	case 2:
		//�ж��Ƿ�ӽ���㣨�γɱպ�����
		if (m_crt_p>2 && PointHitTest(m_nzValues[0], point)){
			CRect rc;
			float radius = 12 / m_scale;
			rc.SetRect(m_nzValues[0].x - radius, m_nzValues[0].y - radius, m_nzValues[0].x + radius, m_nzValues[0].y + radius);
			MemDC.Ellipse(rc);
		}
		else if (m_crt_p > 0 && XYHitTest(m_nzValues[m_crt_p - 1], point)){
			point = xypoint;
		}
		//ʵʱ�������λ��(���������)
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
			for (int i = 0; i <= k1; i++){//������
				p1.x = fg[1].x + i*jd_fg; p2.x = p1.x;
				p1.y = fg[1].y; p2.y = point.y;
				p1 = (p1);
				p2 = (p2);
				MemDC.MoveTo(p1);
				MemDC.LineTo(p2);
			}
			for (int i = 0; i <= k2; i++){//������
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
			double x0 = point.x - fg[1].x; int i = (int)x0 / jd_fg;//��i��
			double y0 = point.y - fg[1].y; int j = (int)y0 / jd_fg;//��j��
			double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//��k1+1��С����
			double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//��k2+1��С����
			int n = j*(k1 + 1) + i;//��n������

			//crtpoint.Format(_T("(X = %d , Y = %d)   K = %1.4f"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[n].k);

			if (fgss[n].qytks>0 && fgss[n].qytks <= tks)
				crtpoint.Format(_T("(X = %d , Y = %d) �뾶��%1.4f  ��ȫϵ����%1.4f ǣ��������Ϊ��%d"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[n].bj, fgss[n].k, fgss[n].qytks);
			else
				crtpoint.Format(_T("(X = %d , Y = %d) �뾶��%1.4f  ��ȫϵ����%1.4f"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[n].bj, fgss[n].k);

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
				crtpoint.Format(_T("(X = %d , Y = %d) �뾶��%.2f  ��ȫϵ����%1.4f ǣ��������Ϊ��%d"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[minkid].bj, fgss[minkid].k, fgss[minkid].qytks);
			else
				crtpoint.Format(_T("(X = %d , Y = %d) �뾶��%.2f  ��ȫϵ����%1.4f"), x - m_translateX, (y <= 610 ? m_translateY - y : -100), fgss[minkid].bj, fgss[minkid].k);
		break;
	}
	ReleaseDC(pDC);//�ͷ�pDC

	// ��״̬���ĵ�ǰ�㴰������ʾ�������ַ���
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(crtpoint);
	CView::OnMouseMove(nFlags, saved_point);
}
BOOL CSlopeSView::OnMouseWheel(UINT f, short d, CPoint p)
{
	float olds = m_scale;
	
	CPoint saved_point = p;
	ScreenToClient(&p);
	m_scale += d/120.0f;
	if (m_scale < 0.2)
		m_scale = 0.2;

	m_off_x = p.x / m_scale + m_off_x - p.x / olds;
	m_off_y = p.y / m_scale + m_off_y - p.y / olds;

	this->Invalidate();
	return CView::OnMouseWheel(f, d, saved_point);
}
void CSlopeSView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint saved_point = point;
	point.x = point.x / m_scale - m_off_x;
	point.y = point.y / m_scale - m_off_y;
	if (point.y >= 612) return;
	CDC* pDC = GetDC();//�½�pDC
	switch (m_nDrawType)
	{
	case 1://�߽���
		m_nzValues[m_crt_p] = point;
		m_crt_p++;
		m_nDrawType = 2;//��ʱ����˵�һ����
		break;

	case 2://��ʱ����˵�i���㣨i>1��
		if (bihe || point == m_nzValues[0])//�����ı߽�պ�
		{
			bihe = true;
			m_nzValues[m_crt_p] = m_nzValues[0];
			m_crt_p++;//��m_crt_p���㣬�� m_nzValues[0]�� m_nzValues[m_crt_p-1];
			m_nDrawType = 3;//����߽�ͼ��
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//��׼���
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

	case 4://������
		if (m_pcrt[m_tid] == 0)
		{//��һ��
			if (Linebz)
			{
				m_drawonelinef = false;
				point = linepoint;
				m_fjxpoint[m_tid][m_pcrt[m_tid]] = point;
				m_pcrt[m_tid]++;
			}
		}
		else
		{//���ǵ�һ��
			if (Linebz)
			{//���һ��
				point = linepoint;
				m_fjxpoint[m_tid][m_pcrt[m_tid]] = point;
				m_pcrt[m_tid]++;
				m_tfinish[m_tid] = true;//�����ѻ���
				m_drawonelinef = true;
				m_tid++;//������һ��
				m_tcnt++;
			}
			else
			{//�м��
				m_fjxpoint[m_tid][m_pcrt[m_tid]] = point;
				m_pcrt[m_tid]++;
			}
		}
		break;
	case 6://����
		if (crt_fg == 0)
			fg[++crt_fg] = point;
		else if (crt_fg == 1){
			fg[++crt_fg] = point;
			m_bDraw[3] = true;
			m_bDraw[2] = false;//���
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
			double x0 = point.x - fg[1].x; int i = (int)x0 / jd_fg;//��i��
			double y0 = point.y - fg[1].y; int j = (int)y0 / jd_fg;//��j��
			double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//��k1+1��С����
			double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//��k2+1��С����
			int n = j*(k1 + 1) + i;//��n������
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

void CSlopeSView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)//���¼������¼�
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nChar == VK_ESCAPE && m_nDrawType<4)
	{//����ESCʱ����
		if (m_crt_p>0) m_crt_p--;
		//Invalidate();//����
		if (bihe){
			bihe = false;
			m_nDrawType = 2;
			m_bDraw[1] = false;
			m_bDraw[0] = true;
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//ʮ�ֹ��
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
				SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//��׼���
				if (!FirstFocus){
					CEdit* pedit = &m_EditPoint;
					pedit->ShowWindow(SW_HIDE);
				}
			}
			return;
		}
	}
	else if (nChar == VK_RETURN)//���»س�
	{
		if (m_pcrt[m_tid] == 0 && (m_nDrawType == 4 || m_nDrawType == 3))
		{
			m_nDrawType = 5;
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));//��׼���
			m_bDraw[1] = false;
			m_bDraw[2] = true;
		}
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSlopeSView::OnRButtonUp(UINT /* nFlags */, CPoint point)//�Ҽ��¼�
{
	RButtonUp = point;
	CPoint saved_point = point;
	point.x = point.x / m_scale - m_off_x;
	point.y = point.y / m_scale - m_off_y;

	CMenu menu;       // �˵����������˵������Ӳ˵���   
	CMenu *pSubMenu; CMenu *pSubMenu1; // �Ҽ��˵�   
	CBitmap m_bitmap1, m_bitmap2, m_bitmap[20];//λͼ��ͼ�꣩
	m_bitmap1.LoadBitmap(IDB_BITMAP1);
	m_bitmap2.LoadBitmap(IDB_BITMAP2);
	for (int i = 0; i < 10; i++)
		m_bitmap[i].LoadBitmap(IDB_COLOR);
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);//���������

	if (((m_delete_id = MLHitTest(point)) != -1) && (m_nDrawType >= 4))//ɾ���ֽ���
	{
		crt_tc = point;
		// ���ز˵���Դ��menu����   
		menu.LoadMenu(IDR_DELETE_ML);
		// ��Ϊ�Ҽ��˵��ǵ���ʽ�˵������������˵���������ȡ�Ӳ˵�   
		pSubMenu = menu.GetSubMenu(0);
		//Ϊ��һ��������ͼ��
		pSubMenu->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_bitmap1, &m_bitmap1);
		// ������ֵ�ɿͻ�����ת��Ϊ��Ļ���� 
		ClientToScreen(&point);
		// �����Ҽ��˵����˵������point.x����ֵ����   
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	else if (PtInRegion(rg, point.x, point.y) && m_nDrawType >= 5)//��ӷֽ���&&ѡ���������
	{
		crt_tc = point;
		menu.LoadMenu(IDR_SELECT_PA);
		pSubMenu = menu.GetSubMenu(0);
		pSubMenu->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_bitmap2, &m_bitmap2);
		pSubMenu1 = pSubMenu->GetSubMenu(1);

		for (int k = 0; k < 10; k++){
			int color = tuceng[k].clr;
			pSubMenu1->ModifyMenu(ID_CENG1 + k, MF_BYCOMMAND, ID_CENG1 + k, tuceng[k].name);//�ı�˵�������

			BITMAP bitStru;
			m_bitmap[k].GetBitmap(&bitStru);

			int n = bitStru.bmWidthBytes / bitStru.bmWidth;
			int bitmapSize = bitStru.bmHeight *  bitStru.bmWidthBytes;// lineBytes;
			BYTE *px = (BYTE *)GlobalAlloc(GPTR, bitmapSize);
			m_bitmap[k].GetBitmapBits(bitmapSize, px);

			int    nBytePreLine, head;
			nBytePreLine = (bitStru.bmWidth * n + 1) / 2 * 2;        //    һ����4�ֽڶ���, GetBitmapBitsʵ�ʶ�����2
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



//���ܺ���

//����׽
bool  CSlopeSView::PointHitTest(CPoint origin, CPoint point)//��⺯��,��׽�պϵ�
{
	CRect rc;
	rc.SetRect(origin.x - 5, origin.y - 5, origin.x + 5, origin.y + 5);//��pointΪ���Ľ��߳�Ϊ10�ľ���;
	if (rc.PtInRect(point)){//���origin�Ƿ��ڽ����ľ�����;
		bihe = true;
		return true;
	}
	bihe = false;
	return false;
}
//��⺯��,XY�����׽
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

//��⺯��,��׽�Ƿ񿿽��߽���
bool  CSlopeSView::LineHitTest(CPoint point)
{
	for (int i = 0; i < m_crt_p - 1; i++)//�����߽�յ�
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
	for (int i = 0; i < m_crt_p - 1; i++){//����ÿ���߶�
		double a1, b1, c1, d, a2, b2, c2, k1, k2, ju1, ju2;
		int x1, x2, y1, y2;
		a1 = b1 = c1 = d = a2 = b2 = c2 = k1 = k2 = ju1 = ju2 = 0.0; x1 = x2 = y1 = y2 = 0;
		CPoint p1, p2, m1, m2;
		p1 = m_nzValues[i]; p2 = m_nzValues[i + 1];
		CRect rc;//�������������·��ġ�������
		x1 = min(p1.x, p2.x); x2 = max(p1.x, p2.x); y1 = min(p1.y, p2.y); y2 = max(p1.y, p2.y);
		rc.SetRect(x1 - 5, y1 - 5, x2 + 5, y2 + 5);//���ϣ��ң���(˳ʱ��)
		a1 = p2.y - p1.y; b1 = p1.x - p2.x; c1 = p2.x*p1.y - p2.y*p1.x;//a1,b1,c1Ϊ�߽�������
		d = (a1*point.x + b1*point.y + c1) *1.0 / sqrt(a1*a1 + b1*b1);
		if (m_pcrt[m_tid]>0){//���������ӳ�����߽��ཻ�ķ�ʽ
			m1 = m_fjxpoint[m_tid][m_pcrt[m_tid] - 1]; m2 = point;
			a2 = m2.y - m1.y; b2 = m1.x - m2.x; c2 = m2.x*m1.y - m2.y*m1.x;//a2,b2,c2Ϊ����������
			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)){//�����˱߽���
				if (b1 == 0){//�߽���б��Ϊ�����,k1=����
					k2 = -a2 / b2; ju2 = a2*m1.x*1.0 / b2 + m1.y;
					linepoint.x = p1.x;
					linepoint.y = (int)(k2*linepoint.x + ju2);
				}
				else if (b2 == 0){//������б��Ϊ�����,k2=����
					k1 = -a1 / b1; ju1 = a1*p1.x*1.0 / b1 + p1.y;
					linepoint.x = m1.x;
					linepoint.y = (int)(k1*linepoint.x + ju1);
				}
				else{//�߽��߼�������б�ʲ�Ϊ����󣬲��������ཻ�ķ�ʽ
					k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
					k2 = -a2*1.0 / b2; ju2 = a2*m1.x*1.0 / b2 + m1.y;
					if (fabs(k1) > fabs(k2)){//�߽���б�ʴ�����y����x
						if (k2 == 0){
							linepoint.y = (int)ju2;
							linepoint.x = (int)((ju2 - ju1)*1.0 / k1);
						}
						else{
							linepoint.y = (int)((k2*ju1 - k1*ju2)*1.0 / (k2 - k1));
							linepoint.x = (int)(((k2*ju1 - k1*ju2)*1.0 / (k2 - k1) - ju1)*1.0 / k1);//������yֱ����x
						}
					}
					else{//������б�ʴ�����x����y
						if (fabs(k1) < 0.03){
							linepoint.x = point.x;
							linepoint.y = (int)(k1*point.x + ju1);
						}
						else{
							linepoint.x = (int)((ju2 - ju1)*1.0 / (k1 - k2));
							linepoint.y = (int)(k1*(ju2 - ju1)*1.0 / (k1 - k2) + ju1);//������xֱ����y
						}
					}
				}
				//��linepointû���ڱ߽��߷�Χʱ
				if (!rc.PtInRect(linepoint)){//ȡ������ڵ�yֵ
					linepoint.y = point.y;
					if (b1 != 0){
						k1 = -a1 *1.0 / b1; ju1 = a1*p1.x *1.0 / b1 + p1.y;
						linepoint.x = (int)((point.y - ju1)*1.0 / k1);
					}
					else linepoint.x = p1.x;
					if (!rc.PtInRect(linepoint)){//ȡ������ڵ�xֵ
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
		else{//�����ĵ�һ���㣬���ô�������߽��������ߵķ�ʽ

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


//�Ҽ���׽������
int  CSlopeSView::MLHitTest(CPoint point)
{
	for (int i = 0; i <= m_tid; i++)//��i����j��
	{
		for (int j = 0; j < m_pcrt[i] - 1; j++)
		{
			CPoint p1 = m_fjxpoint[i][j], p2 = m_fjxpoint[i][j + 1];
			double a1, b1, c1, d, k1, ju1;
			int x1, x2, y1, y2;
			a1 = b1 = c1 = d = k1 = ju1 = 0.0; x1 = x2 = y1 = y2 = 0;
			CRect rc;
			x1 = min(p1.x, p2.x); x2 = max(p1.x, p2.x); y1 = min(p1.y, p2.y); y2 = max(p1.y, p2.y);
			rc.SetRect(x1 - 5, y1 - 5, x2 + 5, y2 + 5);//���£��ң���
			a1 = p2.y - p1.y; b1 = p1.x - p2.x; c1 = p2.x*p1.y - p2.y*p1.x;
			d = (a1*point.x + b1*point.y + c1) / sqrt(a1*a1 + b1*b1);
			if (d < 5.0 && d>-5.0 && rc.PtInRect(point)) return i;
		}
	}
	return -1;
}

//��ֹ�Զ�������������
BOOL CSlopeSView::OnEraseBkgnd(CDC* pDC)//��ֹ�Զ�������������
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return FALSE;
}

//����˵���//������Ʊ߽���
void CSlopeSView::OnUpdateNewEb(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	// ����m_bDraw��ֵ�����Ƿ񼤻�   
	pCmdUI->Enable(m_bDraw[0]);
}

//������Ʋ�����
void CSlopeSView::OnUpdateNewM(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->Enable(m_bDraw[1]);
}

//������Ʒ���
void CSlopeSView::OnUpdateAddL(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->Enable(m_bDraw[2]);
}
void CSlopeSView::OnUpdateComt(CCmdUI *pCmdUI)//������м���
{
	// TODO:  �ڴ������������û����洦��������
	pCmdUI->Enable(m_bDraw[3]);
}

//������
int CSlopeSView::GetTopTC(CPoint p, CDC* pDC)
{
	HDC hDC = pDC->GetSafeHdc(); //��ȡ��ǰ��ĻDC
	COLORREF clr = ::GetPixel(hDC, p.x, p.y); //��ȡ��ǰ��������ֵ
	int color = clr;
	for (int i = 0; i < TUCENG_COUNT; i++){
		if (color == tuceng[i].clr)
			return i;
	}
	return 0;

}

//��p�Ϸ������޵�
void CSlopeSView::SearchToptcid(int tcid)
{
	CPoint p = RButtonUp;
	for (int i = 0; i < m_tid; i++){//��i��������
		for (int j = 0; j < m_pcrt[i] - 1; j++){//��k����
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

//�����������²�����
void CSlopeSView::SetSXtuceng(CDC* pDC){
	for (int tid = 0; tid < m_tid; tid++){
		CPoint p1, p2, p3, p4;
		//	if (m_pcrt[tid] == 2){//ֻ��������
		p1 = m_fjxpoint[tid][0]; p2 = m_fjxpoint[tid][1];
		double xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x);
		double ju = p1.y - xlv*p1.x;
		p3.x = (int)((p1.x + p2.x)*0.5 + 0.5); p4.x = (int)((p1.x + p2.x)*0.5 + 0.5);
		p3.y = (int)(xlv*p3.x + ju + 0.5) + 3;//���е��µ�
		p4.y = (int)(xlv*p4.x + ju + 0.5) - 3;//���е��ϵ�
		buttomtc[tid] = checkcolor(p3, 1, pDC);//�����ߵ��·���Ϊp3����������
		toptc[tid] = checkcolor(p4, -1, pDC);//�����ߵ��Ϸ���Ϊp4����������
	}
}

//����ת����ԭ��������x���ң�y���µģ����Ϊx���ϣ�y����//x��yΪ�������
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

//Ҫ��:p.x= x , p.y< y,����x���ڱ߽��ϵ�yֵ
int CSlopeSView::GetBoundPoint(double x, double y){
	//ע�⴦��ֱ�߽���p1.x=p2.x
	for (int i = 0; i < m_crt_p - 1; i++){
		CPoint p1, p2;//�߽��ߵ����ҵ�
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

//������������(�������������·�)
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
	HDC hDC = pDC->GetSafeHdc(); //��ȡ��ǰ��ĻDC
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
	do{
		COLORREF clr = ::GetPixel(hDC, p.x, p.y); //��ȡ��ǰ��������ֵ
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

//�������
double CalAre(double k1, double b1, double k2, double b2, int x1, int x2){
	return fabs((k1 - k2)*(x2*x2 - x1*x1)*0.5 + (b1 - b2)*(x2 - x1));
}

//���ػس���
BOOL CSlopeSView::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		HWND hWnd = ::GetFocus(); /*   �жϵ�ǰ�����Ƿ��ڱ༭����    */
		if (::GetDlgCtrlID(hWnd) == IDC_POINT_EDIT){
			// ��Ӷ���Ϣ����Ĵ��룬��������������!
			CEdit * pedit = (CEdit*)GetDlgItem(IDC_POINT_EDIT);
			CString str, str1, str2;
			UpdateData(TRUE);
			pedit->GetWindowText(str);
			int index = str.Find(_T(","));   //��.��λ��
			if (index != -1){
				int l = str.GetLength();//��ȡ�ַ���������Ҳ��1���ַ�
				str1 = str.Left(index);
				str2 = str.Right(l - index - 1);
				int x = _wtoi(str1);
				int y = _wtoi(str2);
				if (x >= 0 && x <= 1362 && y >= 0 && y <= 610)
				{//�����û�������
					//x - m_translateX, m_translateY - y 
					x += m_translateX; y = m_translateY - y;
					x = (x + m_off_x)*m_scale;
					y = (y + m_off_y)*m_scale;
					//str.Format(_T("x=%d,y=%d"), x, y);
					//AfxMessageBox(str);//������Ϣ
					::SendMessage(m_hWnd, WM_LBUTTONUP, 0, MAKELONG(x, y));//ģ��������
				}
				else
					AfxMessageBox(_T("��ע�����뷶Χ: -m_translateX<=x<=1042, -100<=y<=m_translateY "));
				pedit->SetWindowText(_T(""));
			}
			else
				AfxMessageBox(_T("��ע�������ʽ: x,y "));
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

//Ѱ�ҽ���
void CSlopeSView::FindAllCrossPoint(int n, CPoint p0, double bj){
	struct fgss  &cntfg = fgss[n];
	for (int i = 0; i < m_crt_p - 1; i++)
	{//1.��Բ��Ϊp0,�뾶Ϊbj��Բ��߽�Ľ���
		if ((i == bianjie[0] && bianjie[0] != -1) || (i == bianjie[1] && bianjie[1] != -1) || (i == bianjie[2] && bianjie[2] != -1))
			continue;
		double  xlv, ju;//б�ʣ��ؾ�
		int x1, x2, y1, y2;//(x1,y1),(x2,y2)Ϊ�����
		int xl, xr, yu, yd;//���ң��£���
		double a, b, c, dta;//����ʽa��b��c��dta
		xlv = ju = a = b = c = dta = 0; x1 = x2 = y1 = y2 = xl = xr = yu = yd = 0;
		CPoint p1, p2;//�߽��ߵ����ҵ�
		p1 = m_nzValues[i]; p2 = m_nzValues[i + 1];
		if (p1.x != p2.x){//б�ʴ���
			xl = min(p1.x, p2.x); xr = max(p1.x, p2.x);
			xlv = (p2.y - p1.y)*1.0 / (p2.x - p1.x); ju = p1.y - xlv*p1.x;
			//Բ����p1
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
		else{//б�ʲ�����
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
//��ÿ��Բ�����м���
   void CSlopeSView::Calculate(int n, int f, CPoint p0, double bj, CDC* pDC){
	struct fgss  &cntfg = fgss[n];
	struct point &start = cntfg.p[f];//��ʼ�㣨����ʱ��õ��ģ�
	struct point &end = cntfg.p[f + 1];//������

	double cosa[30], sina[30], alf[30], tgf[30], fai[30], A[30], W[30], L[30], cl[30], m[30], cd[30], E[30];
	CPoint pp1, pp2;//Բ�������Ҷ˵�
	
	//�ж��ĸ�������ĸ����ҵ�
	if (start.p.x < end.p.x)
	{ 
		pp1 = start.p; pp2 = end.p; 
	}
	else
	{ 
		return; 
	}

	double b;//������
	b = (pp2.x - pp1.x)*1.0 / tks;
	for (int i = 0; i < tks; i++)
	{//��ÿ������(��i������)
		cosa[i] = sina[i] = alf[i] = tgf[i] = fai[i] = A[i] = W[i] = L[i] = cl[i] = m[i] = cd[i] = E[i] = 0.0;
		double line[20], are[20];//��¼ÿ�������������ײ���ռ��,����������ռ���
		for (int j = 0; j < 10; j++)
		{
			are[j] = line[j] = 0.0;
		}
		CPoint tkp[100];//ÿ������Χ�ɵĲ���������
		CPoint sbj[100];//��¼�����ϱ߽���
		int tkpid, sbjpid;

		CPoint p1, p2, p3, p4; //������������ң��������ҵ�
		p1.x = (int)(pp1.x + b*i);//����
		if (i == 0)p1.y = pp1.y;
		else p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));//ȡ�·��ĵ�
		p2.x = (int)(pp1.x + b*(i + 1));//����
		if (i == tks - 1)p2.y = pp2.y;
		else p2.y = (int)(p0.y + sqrt(bj*bj - (p2.x - p0.x)*(p2.x - p0.x)));
		p3.x = p1.x;//����
		p3.y = GetBoundPoint(p1.x, p1.y);
		p4.x = p2.x;//����
		p4.y = GetBoundPoint(p2.x, p2.y);
		tkp[0] = p3;//��¼����߽�Ķ���Σ�������ʼ˳ʱ�룩
		tkpid = 1;
		double xl = p1.x, xr = p2.x;//���������x����
		double yd = max(p1.y, p2.y), yu = min(p3.y, p4.y);
		//��������y����
		//Ч��߽��ߵ��Ƿ���������(�������б߽��)
		for (int j = 0; j < m_crt_p - 1; j++)//1.x��xl~xr  2.y>line(p1,p2)
		{
			if ((j == bianjie[0] && bianjie[0] != -1) || (j == bianjie[1] && bianjie[1] != -1) || (j == bianjie[2] && bianjie[2] != -1))
				continue;
			if (m_nzValues[j].x>xl && m_nzValues[j].x < xr && m_nzValues[j].y < p1.y + (p2.y - p1.y)*(m_nzValues[j].x - p1.x) / (p2.x - p1.x))
				tkp[tkpid++] = m_nzValues[j];
		}
		std::sort(tkp, tkp + tkpid, cmp);//����ʱ������
		tkp[tkpid++] = p4; //�ҳ������ϱ߽���
		memcpy(sbj, tkp, sizeof(CPoint)*tkpid); sbjpid = tkpid;
		if (p2.y != p4.y)tkp[tkpid++] = p2; if (p1.y != p3.y) tkp[tkpid++] = p1;//�����±߽��ߣ������㣩
		tkp[tkpid++] = p3;//�պ�

		double s = 0;	//double s1=0;
		for (int j = 0; j < tkpid - 1; j++)
		{
			s += (tkp[j].x*  tkp[j + 1].y - tkp[j + 1].x*tkp[j].y)*0.5;//�������������������//s1 += (tkp[j+1].x- tkp[j].x)*(tkp[j + 1].y+tkp[j].y)*0.5;//�������ַ���
			
		}
		s = fabs(s);//�����
		A[i] = s;
		//m_fjxpoint[m_tid][m_pcrt[m_tid]] ��m_tid����m_pcrt[m_tid]����
		//���������в������ཻ

		double xlv0, ju0;//���ߵ�б�ʺͽؾ�
		xlv0 = (p1.y - p2.y)*1.0 / (p1.x - p2.x);
		ju0 = p1.y - xlv0*p1.x;

		struct mianji
		{
			int yl;//��������˵�
			double s;//�������ǲ������������±�ԵΧ�ɵ����
			int id;//������id
			bool   operator <  (const  mianji&   rhs)  const   //��������ʱ����д�ĺ���
			{
				return   yl < rhs.yl;
			}
		}xj[POINT_COUNT];//�����¼������������ཻ������
		int xjid = 0;//��¼������������ཻ��

		for (int j = 0; j < m_tid; j++){//��j��������
			double xjs = 0;//�ཻ���
			bool zuos; //����Ƿ����Ϸ�
			bool cross = false;//��û����˲������ཻ
			if (m_fjxpoint[j][0].x >= p2.x)continue;//�������������Ҷˣ��޽��㣩
			else if (m_fjxpoint[j][0].x>p1.x && m_fjxpoint[j][0].x < p2.x){//��������������������������
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
			int yy1, yy2;//p1.x��p2.x�ڲ���������Ӧ��yֵ

			int beforey = p1.y;//����ǰ��

			//3.��������������ױ�Ե���ཻ���õ����
			for (int k = 0; k < m_pcrt[j] - 1; k++){
				CPoint ps, pn;//startp,nextp
				ps = m_fjxpoint[j][k]; pn = m_fjxpoint[j][k + 1];
				if (pn.x <= p1.x)continue; //�߶���������ˣ��޽��㣩
				else if (ps.x >= p2.x)break;//�߶��������Ҷˣ��޽��㣩
				double xlv = (pn.y - ps.y)*1.0 / (pn.x - ps.x);
				double ju = pn.y - xlv*pn.x;
				if (ps.x <= p1.x && p1.x < pn.x){//���滻ǰ��
					ps.x = p1.x;
					ps.y = (int)(xlv*p1.x + ju + 0.5);
					yy1 = ps.y;
					if (ps.y < p1.y)zuos = true;
					else zuos = false;
				}
				if (ps.x <= p2.x && p2.x < pn.x){//���滻���
					pn.x = p2.x;
					pn.y = (int)(xlv*p2.x + ju + 0.5);
					yy2 = pn.y;
				}

				int y0 = (int)(xlv0*pn.x + ju0 + 0.5);//���ߺ��
				if (zuos && pn.y < y0)
				{//�����㶼���Ϸ�
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, pn.x);
					cross = true;
				}
				else if (zuos &&pn.y >= y0)
				{//��������
					int x = (int)((ju - ju0) / (xlv0 - xlv) + 0.5); int y = (int)(xlv*x + ju + 0.5);
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, x);
					line[toptc[j]] += sqrt((pn.x - x)*(pn.x - x) + (y0 - y)*(y0 - y));
					line[buttomtc[j]] += sqrt((ps.x - x)*(ps.x - x) + (beforey - y)*(beforey - y));
					zuos = false;
					cross = true;
				}
				else if (!zuos&& pn.y < y0)
				{//��������
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
				xj[xjid].s = xjs;//�˲����ߵ�������ߵ��ཻ���
				xj[xjid].id = j;//�õ�������id
				xj[xjid].yl = yy2;
				xjid++;
			}
		}
		std::sort(xj, xj + xjid);//���������¶Խ��õĲ���������
		for (int j = 0; j < xjid; j++)
		{
			if (j == 0)are[toptc[xj[j].id]] += s - xj[j].s;
			else are[toptc[xj[j].id]] += xj[j - 1].s - xj[j].s;//���������֮�������������������
		}
		double totl = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));//�ܵ������ױ߳�
		L[i] = totl;
		for (int j = 0; j < 10; j++){
			totl -= line[j];//ʣ�³�
		}
		if (m_tcnt>0 && xjid>0){//���˲����� �� ������������н���
			are[buttomtc[xj[xjid - 1].id]] += xj[xjid - 1].s;//���µ���������²����ߵ��·�����
			if (totl > 0)line[buttomtc[xjid - 1]] += totl;//ʣ�µĳ��ȸ����²����ߵ��·�����
		}
		else if (m_tcnt > 0 && xjid == 0){//���˲����� �� ������������޽���
			CPoint p;
			p.x = (p1.x + p2.x) / 2; p.y = (p1.y + p2.y) / 2;
			int topclx = GetTopTC(p, pDC);
			are[topclx] += s;
			line[topclx] += totl;
		}
		else if (m_tcnt <= 0)
		{//û��������
			are[toptcid] += s;
			line[toptcid] += totl;
		}
		fai[i] = 0;//��Ȩƽ��faiֵ
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
		{//��10��������һ�����������Ĺ��׼�����
			W[i] += are[j] * tuceng[j].gama;
			cl[i] += tuceng[j].c*line[j];
		}
		if (W[i] == 0)return;
	}
	double fenzi = 0; double fenmu = 0;//���ӣ���ĸ
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
		K = fenzi / fenmu;//�õ���һԲ����K
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
		fsl = 0.01, fsr = 6.0;//Fs�����ҽ�
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
		fsl = 0.01, fsr = 6.0;//Fs�����ҽ�
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


		//ǣ��ʽ�������Բ��
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
			K = fenzi / fenmu;//�õ���һԲ����K
			if (TempK > K)
			{
				TempK = K;
				Mqytks = qytks;
			}
		}
		K = TempK;
		break;

		//ǣ��ʽ���±�Ф��
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


		//ǣ��ʽ���²�ƽ������
	case 7:
		Mqytks = 0;
		TempK = 100;
		for (qytks = 1; qytks <= tks; qytks++)
		{
			fsl = 0.01, fsr = 6.0;//Fs�����ҽ�
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


//�Ҽ�����

//���Ҽ�����ӱ߽���
void CSlopeSView::OnAddMl()
{
	// TODO:  �ڴ���������������
	m_nDrawType = 4;
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//ʮ�ֹ��
	m_bDraw[1] = true;
}
//���Ҽ���ɾ���߽���
void CSlopeSView::OnDelete()
{
	// TODO:  �ڴ���������������
	m_pcrt[m_delete_id] = 0;
	m_tcnt--;
}

//����
void CSlopeSView::OnCeng1()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[0].clr;
	OnDraw(pDC);
	SearchToptcid(0);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng2()
{
	// TODO:  �ڴ���������������
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
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[2].clr;
	OnDraw(pDC);
	SearchToptcid(2);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng4()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[3].clr;
	OnDraw(pDC);
	SearchToptcid(3);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng5()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[4].clr;
	OnDraw(pDC);
	SearchToptcid(4);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng6()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[5].clr;
	OnDraw(pDC);
	SearchToptcid(5);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng7()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[6].clr;
	OnDraw(pDC);
	SearchToptcid(6);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng8()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[7].clr;
	OnDraw(pDC);
	SearchToptcid(7);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng9()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[8].clr;
	OnDraw(pDC);
	SearchToptcid(8);
	ReleaseDC(pDC);
}
void CSlopeSView::OnCeng10()
{
	// TODO:  �ڴ���������������
	CDC* pDC = GetDC();
	tianchong[++tc_step].point = crt_tc;
	tianchong[tc_step].clr = tuceng[9].clr;
	OnDraw(pDC);
	SearchToptcid(9);
	ReleaseDC(pDC);
}

//�����ܽ���

//��ʼ���߽���
void CSlopeSView::OnNewEb()
{
	// TODO:  �ڴ���������������
	m_nDrawType = 1;//��ʼ���߽���
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//ʮ�ֹ��
	CEdit* pedit = & m_EditPoint;
	if (FirstFocus){
		ASSERT(pedit->Create(WS_CHILD | WS_VISIBLE, CRect(1062, 615, 1262, 630), this, IDC_POINT_EDIT));
		FirstFocus = false;
	}
	pedit->ShowWindow(SW_SHOW);
}

//��ʼ��������
void CSlopeSView::OnNewM()
{
	// TODO:  �ڴ���������������
	m_nDrawType = 4;//��ʼ��������
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));//ʮ�ֹ��
	if (!FirstFocus){
		CEdit* pedit = & m_EditPoint;
		pedit->ShowWindow(SW_HIDE);//TestʱҪȥ��
	}
}


//����趨

//�趨�������
void CSlopeSView::OnParameter()
{
	// TODO:  �ڴ���������������
	Ctab dlg;//����Ctab��Ի���
	dlg.pTuCeng = pTuCeng;
	dlg.pFocus = pFocus;
	for (int i = 0; i < TUCENG_COUNT; i++){
		dlg.m_tuceng[i].phi = tuceng[i].phi;
		dlg.m_tuceng[i].c = tuceng[i].c;
		dlg.m_tuceng[i].gama = tuceng[i].gama;
		dlg.m_tuceng[i].name = tuceng[i].name;
		dlg.m_tuceng[i].clr = tuceng[i].clr;
	}
	if (IDOK == dlg.DoModal()){//���OK
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

//�趨����
void CSlopeSView::OnSearch()
{
	// TODO:  �ڴ���������������
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

//���ü���ģ��
void CSlopeSView::OnUsM()
{
	// TODO:  �ڴ���������������
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

//���������趨
void CSlopeSView::OnAddL()
{
	// TODO:  �ڴ���������������
	m_nDrawType = 6;
	crt_fg = 0;//����
	m_bDraw[3] = false;
}



//���м���

void CSlopeSView::OnComt()
{
	// TODO:  �ڴ���������������
	m_nDrawType = 7;
	totfg = 0;
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc();
	OnDraw(pDC);
	//��¼��������������
	Sleep(100);
	SetSXtuceng(pDC);
	buttomtc[0];//�����ߵ��·���Ϊp3����������
	toptc[0];
	int maxy = 0, maxx = 0, minx = 10000;
	memset(bianjie, -1, sizeof(bianjie));
	for (int i = 0; i < m_crt_p - 1; i++){
		if (m_nzValues[i].y + m_nzValues[i + 1].y>maxy){
			maxy = m_nzValues[i].y + m_nzValues[i + 1].y;
			bianjie[0] = i;//�±߽���
		}
		if (m_nzValues[i].x + m_nzValues[i + 1].x > maxx){
			maxx = m_nzValues[i].x + m_nzValues[i + 1].x;
			bianjie[1] = i;//�ұ߽���
		}
		if (check1 == 1 && m_nzValues[i].x + m_nzValues[i + 1].x < minx){
			minx = m_nzValues[i].x + m_nzValues[i + 1].x;
			bianjie[2] = i;//��߽���
		}
	}

	double dbk = (m_nzValues[bianjie[0]].y - m_nzValues[bianjie[0] + 1].y)*1.0 / (m_nzValues[bianjie[0]].x - m_nzValues[bianjie[0] + 1].x);
	double dbb = m_nzValues[bianjie[0]].y - dbk*m_nzValues[bianjie[0]].x;
	double maxbj;
	HRGN rg = CreatePolygonRgn(m_nzValues, m_crt_p, WINDING);
	CPoint p0;//p0ΪԲ��
	double ban = (double)(jd_fg) / 2.0;
	double dx = fg[2].x - fg[1].x; int k1 = (int)dx / jd_fg;//��k1+1��С����
	double dy = fg[2].y - fg[1].y; int k2 = (int)dy / jd_fg;//��k2+1��С����
	double min_k = 10000;//���з����minK
	int txt_num=1;
//	clock_t start,finish; 
//	start=clock(); 
	for (int i = 0; i <= k1; i++)//��������i��С����(0~k1+1)
	{   // ((i*k1+j)*k2+(k-1))*k3+l  �ļ����
		p0.x = (int)(fg[1].x + i*jd_fg + ban);
		if (i == k1)p0.x = (int)((fg[2].x - fg[1].x - i*jd_fg) / 2.0 + fg[1].x + i*jd_fg);
		for (int j = 0; j <= k2; j++)//��������j��С����(0~k2+1)
		{
			int n = j*(k1 + 1) + i;//��������n��С����
			totfg++;
			struct fgss  &cntfg = fgss[n];//����
			struct fgss  &cntfg1 = fgss[n];
			struct fgss  &cntfg2 = fgss[n];
			cntfg.k = 10000;//��һ�����minK
			p0.y = (int)(fg[1].y + j*jd_fg + ban);
			if (j == k2)p0.y = (int)((fg[2].y - fg[1].y - j*jd_fg) / 2.0 + fg[1].y + j*jd_fg);
			//����Ѿ��ҵ���Բ��p0,�������Ұ뾶��
			//���뾶Ӧ����Բ�ĵ��ױߵľ���
			maxbj = fabs(dbk*p0.x - p0.y + dbb) / sqrt(1 + dbk*dbk);
			int k3 = (int)maxbj / jd_yh, bj;

			for (int k = 1; k <= k3; k++)
			{
				cntfg.cnt = 0;//���㣨��ÿ���뾶��
				bj = k*jd_yh;
				//�뾶Ϊbj������������Բ����߽������еĽ��㡣
				//��Բ���ķ���������߽�Ľ�������㰴Բ�Ľ�������ʱ�룺����б���������жϣ����ж����ڽ�����Բ�ϵ��е��Ƿ��ڱ߽��ڡ�����ڱ߽��ڻ���Բ���ߡ�

				FindAllCrossPoint(n, p0, bj);//1.��Բ��Ϊp0,�뾶Ϊbj��Բ��߽�Ľ���

				for (int l = 0; l < cntfg.cnt; l++)//2.���㰴Բ�Ľ�������ʱ�룺����б���������жϣ�
				{
					double x = cntfg.p[l].p.x - p0.x, y = cntfg.p[l].p.y - p0.y;
					cntfg.p[l].ang = GetAng(x, y);
				}
				std::sort(cntfg.p, cntfg.p + cntfg.cnt);

				for (int l = 0; l < cntfg.cnt - 1; l++)//3.�ж����ڽ�����Բ�ϵ��е��Ƿ��ڱ߽�������
				{
					double ang = (cntfg.p[l].ang + cntfg.p[l+1].ang) / 2.0;
					int x = (int)(cos(ang)*bj + 0.5), y = (int)(sin(ang)*bj + 0.5);
					int z = x; x = -y; y = -z;//֮ǰת�������꣬����ת����
					x += p0.x; y += p0.y;
					if (PtInRegion(rg, x, y))
					{//��������ÿ��Բ����������(һ���뾶�¿��ܲ������Բ��)
					//	double K1,K2;
						//oldmethod
				//		cntfg2=cntfg;
				//		Calculate(n, l, p0, bj, pDC);//��n������,Բ��p0,�뾶bj,�µ�l��Բ��
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

//����������ʾ

void CSlopeSView::OnShow()
{
	// TODO:  �ڴ���������������	
	if (m_nDrawType != 8)return;
	HWND hWnd = ::FindWindow(NULL, _T("��������"));
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
		lf.lfHeight = -14;   //�����С
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
	s0.Format(_T("Բ��(x,y)��(%d,%d)\r\n�뾶��%.2f\r\n��С��ȫϵ��:%.4f\r\n--------------------------------------------------------------\r\n����%d\r\n--------------------------------------------------------------\r\n"), fgss[fgid].p0.x - m_translateX, m_translateY - fgss[fgid].p0.y, fgss[fgid].bj, fgss[fgid].k, tkid + 1);
	s1.Format(_T("������%.2f\r\n��:%.2f��\r\ncos(��)��%.2f\r\nsin(��)��%.2f\r\n��Ȩƽ���գ�%.2f��\r\n�����%.2f\r\n�ױ߳���%.2f\r\n"), fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosa, fgss[fgid].t[tkid].sina, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
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
		s2.Format(_T("--------------------------------------------------------------\r\n������ַ�\r\n--------------------------------------------------------------\r\n��W*cos(��)*tg(��)+c*l��%.4f\r\n��W*sin(��)��%.4f"), fenzi, fenmu);
		break;
	case 2:
		double m;
		for (int i = 0; i <= tkid; i++)
		{
			m = cntfg.t[i].cosa + 1.0 / cntfg.k* tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].sina;
			fenzi += (cntfg.t[i].W *tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl * cntfg.t[i].cosa) / m;
			fenmu += cntfg.t[i].W* cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\nBishop���ַ�\r\n--------------------------------------------------------------\r\nm=cos(��)+1/k*tg(��)*sin(��)��%.4f\r\n��1/m*(W*tg(��)+c*l*cos(��))��%.4f\r\n��W*sin(��)��%.4f"), m, fenzi, fenmu);
		break;
	case 3:
		for (int i = tks - 1; i >= tkid; i--)
		{
			if (i == tks - 1) cd[i] = 0;
			else cd[i] = cos(cntfg.t[i + 1].alf - cntfg.t[i].alf) - tan(cntfg.t[i].fai / 180 * pi) / cntfg.k*(sin(cntfg.t[i + 1].alf - cntfg.t[i].alf));
			E[i] = cntfg.t[i].W * cntfg.t[i].sina - 1 / cntfg.k*(cntfg.t[i].W * cntfg.t[i].cosa * tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl) + E[i + 1] * cd[i];
		}
		s2.Format(_T("--------------------------------------------------------------\r\n��ƽ��������\r\n--------------------------------------------------------------\r\n����ϵ����i��%.4f\r\n��ƽ������Ei��%.4f\r\n"), cd[tkid], E[tkid]);
		break;
	case 4:
		for (int i = 0; i <= tkid; i++)
		{
			if (i == 0)cd[i] = 0;
			else cd[i] = cos(cntfg.t[i].alf - cntfg.t[i - 1].alf) + tan(cntfg.t[i].fai / 180 * pi) / cntfg.k*(sin(cntfg.t[i].alf - cntfg.t[i - 1].alf));
			E[i] = -cntfg.t[i].W * cntfg.t[i].sina + 1 / cntfg.k*(cntfg.t[i].W * cntfg.t[i].cosa * tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl);
			if (i>0) E[i] += E[i - 1] * cd[i];
		}
		s2.Format(_T("--------------------------------------------------------------\r\n�²�ƽ��������\r\n--------------------------------------------------------------\r\n����ϵ����i��%.4f\r\n��ƽ������Ei��%.4f\r\n"), cd[tkid], E[tkid]);
		break;

	case 5:
		for (int i = 0; i <= tkid; i++)
		{
			fenzi += cntfg.t[i].W * tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].cosa + cntfg.t[i].cl;
			fenmu += cntfg.t[i].W *cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\nǣ��ʽ������ַ�\r\n--------------------------------------------------------------\r\n��W*cos(��)*tg(��)+c*l��%.4f\r\n��W*sin(��)��%.4f"), fenzi, fenmu);
		break;

	case 6:
		double sm;
		for (int i = 0; i <= tkid; i++)
		{
			sm = cntfg.t[i].cosa + 1.0 / cntfg.k* tan(cntfg.t[i].fai / 180 * pi) * cntfg.t[i].sina;
			fenzi += (cntfg.t[i].W *tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl * cntfg.t[i].cosa) / sm;
			fenmu += cntfg.t[i].W* cntfg.t[i].sina;
		}
		s2.Format(_T("--------------------------------------------------------------\r\nǣ��ʽBishop���ַ�\r\n--------------------------------------------------------------\r\nm=cos(��)+1/k*tg(��)*sin(��)��%.4f\r\n��1/m*(W*tg(��)+c*l*cos(��))��%.4f\r\n��W*sin(��)��%.4f"), sm, fenzi, fenmu);
		break;
	
	case 7:
		for (int i = 0; i <= tkid; i++)
		{
			if (i == 0)cd[i] = 0;
			else cd[i] = cos(cntfg.t[i].alf - cntfg.t[i - 1].alf) + tan(cntfg.t[i].fai / 180 * pi) / cntfg.k*(sin(cntfg.t[i].alf - cntfg.t[i - 1].alf));
			E[i] = -cntfg.t[i].W * cntfg.t[i].sina + 1 / cntfg.k*(cntfg.t[i].W * cntfg.t[i].cosa * tan(cntfg.t[i].fai / 180 * pi) + cntfg.t[i].cl);
			if (i>0) E[i] += E[i - 1] * cd[i];
		}
		s2.Format(_T("--------------------------------------------------------------\r\nǣ��ʽ��ƽ��������\r\n--------------------------------------------------------------\r\n����ϵ����i��%.4f\r\n��ƽ������Ei��%.4f\r\n"), cd[tkid], E[tkid]);
		break;
	}
	m_DataDlg->m_text.SetWindowText(s0 + s1 + s2);
}*/

void CSlopeSView::Write()
{
	struct fgss  &cntfg = fgss[fgid];
	CString s0, s1, s2, s3;
	CString ss;
	s0.Format(_T("Բ��(x,y)��(%.2f,%.2f)\r\n�뾶��%.2f\r\n��С��ȫϵ��:%.4f\r\n--------------------------------------------------------------\r\n����%d\r\n--------------------------------------------------------------\r\n"),
		cntfg.p0.x - m_translateX, m_translateY - cntfg.p0.y, cntfg.bj, cntfg.k, tkid + 1);
	//	s1.Format(_T("������%.2f\r\n��:%.2f��\r\ncos(��)��%.2f\r\nsin(��)��%.2f\r\n��Ȩƽ���գ�%.2f��\r\n�����%.2f\r\n�ױ߳���%.2f\r\n"), fgss[fgid].t[tkid].W, fgss[fgid].t[tkid].alf / pi * 180, fgss[fgid].t[tkid].cosAlpha, fgss[fgid].t[tkid].sinAlpha, fgss[fgid].t[tkid].fai, fgss[fgid].t[tkid].are, fgss[fgid].t[tkid].line);
	s1.Format(_T("������%.2f\r\n��:%.2f��\r\ncos(��)��%.2f\r\nsin(��)��%.2f\r\n��Ȩƽ���գ�%.2f��\r\n�����%.2f\r\n�ױ߳���%.2f\r\n"),
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
		s3.Format(_T("--------------------------------------------------------------\r\nϸ�������ַ�\r\n--------------------------------------------------------------\r\n��ȫϵ����%.4f\r\n"),
			fgss[fgid].k);
		break;
	}
	m_DataDlg->m_text.SetWindowText(s0 + s1 + s2 + ss + s3);
}



//�����ļ��洢������Ĭ�ϲ�����

//���������Ӳ���
//��ÿ��Բ�����м���
void CSlopeSView::Calculate1(int n, int f, CPoint p0, double bj, CDC* pDC, int txt_num) 
{
struct fgss  &cntfg = fgss[n];
	struct point &start = cntfg.p[f];//��ʼ�㣨����ʱ��õ��ģ�
	struct point &end = cntfg.p[f + 1];//������
	MyPoint pp[30][4];	//��¼���������е�����,����,����,����,����
	double cosAlpha[30], sinAlpha[30],cosa[30], sina[30], alf[30], tgf[30], fai[30], A[30], W[30], L[30], cl[30], m[30], cd[30], E[30];	//W:����
	int number[30];		//��¼�ÿ��м�����
	double Fxx[30][4]; double Fxy[30][4];
	CPoint pp1, pp2;//Բ�������Ҷ˵�

	//�ж��ĸ�������ĸ����ҵ�
	if (start.p.x < end.p.x)
	{
		pp1 = start.p; pp2 = end.p;
	}
	else
	{
		return;
	}

	double b;//������
	b = (pp2.x - pp1.x)*1.0 / tks;

	for (int i = 0; i < tks; i++)//tks �ֲ���
	{
		//��ÿ������(��i������)
		cosAlpha[i] = sinAlpha[i] = alf[i] = tgf[i] = fai[i] = A[i] = W[i] = L[i] = cl[i] = m[i] = cd[i] = E[i] = 0.0;
		number[i] = 0;
		double line[20], are[20];	//��¼ÿ�������������ײ���ռ��,����������ռ���
		
		for (int j = 0; j < 20; j++)
		//for (int j = 0; j < 10; j++)
		{
			are[j] = line[j] = 0.0;
		}
		CPoint tkp[100];//ÿ������Χ�ɵĲ���������
		CPoint sbj[100];//��¼�����ϱ߽���
		int tkpid, sbjpid;

		CPoint p1, p2, p3, p4; //������������ң��������ҵ�
		p1.x = (int)(pp1.x + b*i);//����
		if (i == 0)
			p1.y = pp1.y;
		else
			p1.y = (int)(p0.y + sqrt(bj*bj - (p1.x - p0.x)*(p1.x - p0.x)));//ȡ�·��ĵ�

		p2.x = (int)(pp1.x + b*(i + 1));//����
		if (i == tks - 1)
			p2.y = pp2.y;
		else
			p2.y = (int)(p0.y + sqrt(bj*bj - (p2.x - p0.x)*(p2.x - p0.x)));
		p3.x = p1.x;//����
		p3.y = GetBoundPoint(p1.x, p1.y);
		p4.x = p2.x;//����
		p4.y = GetBoundPoint(p2.x, p2.y);

		tkp[0] = p3;		//��¼����߽�Ķ���Σ�������ʼ˳ʱ�룩
		tkpid = 1;
		double xl = p1.x, xr = p2.x;	//���������x����
		double yd = max(p1.y, p2.y), yu = min(p3.y, p4.y);

		//��������y����
		//Ч��߽��ߵ��Ƿ���������(�������б߽��)
		for (int j = 0; j < m_crt_p - 1; j++)
			//1.x��xl~xr  2.y>line(p1,p2)
		{
			if ((j == bianjie[0] && bianjie[0] != -1) || (j == bianjie[1] && bianjie[1] != -1) || (j == bianjie[2] && bianjie[2] != -1))
				continue;
			if (m_nzValues[j].x > xl && m_nzValues[j].x < xr && m_nzValues[j].y < p1.y + (p2.y - p1.y)*(m_nzValues[j].x - p1.x) / (p2.x - p1.x))
				tkp[tkpid++] = m_nzValues[j];
		}

		std::sort(tkp, tkp + tkpid, cmp);//����ʱ������
		tkp[tkpid++] = p4; //�ҳ������ϱ߽���
		memcpy(sbj, tkp, sizeof(CPoint)*tkpid);


		
		/* sbjpid = tkpid;
		if (p2.y != p4.y)tkp[tkpid++] = p2; if (p1.y != p3.y) tkp[tkpid++] = p1;//�����±߽��ߣ������㣩
		tkp[tkpid++] = p3;//�պ�*/

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
			tkp[tkpid++] = p1;//�����±߽��ߣ������㣩
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
			tkp[tkpid++] = p1;//�����±߽��ߣ������㣩
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
		tkp[tkpid++] = p3;//�պ�*/


		double s = 0;	//double s1=0;
		for (int j = 0; j < tkpid - 1; j++)
		{
			s += (tkp[j].x*  tkp[j + 1].y - tkp[j + 1].x*tkp[j].y)*0.5;//�������������������//s1 += (tkp[j+1].x- tkp[j].x)*(tkp[j + 1].y+tkp[j].y)*0.5;//�������ַ���
		}
		s = fabs(s);//�����
		A[i] = s;
		//m_fjxpoint[m_tid][m_pcrt[m_tid]] ��m_tid����m_pcrt[m_tid]����
		//���������в������ཻ

		double xlv0, ju0;//���ߵ�б�ʺͽؾ�
		xlv0 = (p1.y - p2.y)*1.0 / (p1.x - p2.x);
		ju0 = p1.y - xlv0*p1.x;
		struct mianji
		{
			int yl;//��������˵�
			double s;//�������ǲ������������±�ԵΧ�ɵ����
			int id;//������id

			//��������ʱ����д�ĺ���
			bool operator <(const mianji &rhs) const
			{
				return yl < rhs.yl;
			}
		}xj[POINT_COUNT];//�����¼������������ཻ������

		int xjid = 0;//��¼������������ཻ��

		for (int j = 0; j < m_tid; j++)
		{
			//��j��������
			double xjs = 0;//�ཻ���
			bool zuos; //����Ƿ����Ϸ�
			bool cross = false;//��û����˲������ཻ
			if (m_fjxpoint[j][0].x >= p2.x)
				continue;//�������������Ҷˣ��޽��㣩
			else if (m_fjxpoint[j][0].x > p1.x && m_fjxpoint[j][0].x < p2.x)
			{
				//��������������������������
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
			int yy1, yy2;//p1.x��p2.x�ڲ���������Ӧ��yֵ

			int beforey = p1.y;//����ǰ��

			//3.��������������ױ�Ե���ཻ���õ����
			for (int k = 0; k < m_pcrt[j] - 1; k++)
			{
				CPoint ps, pn;//startp,nextp
				ps = m_fjxpoint[j][k]; pn = m_fjxpoint[j][k + 1];
				if (pn.x <= p1.x)continue; //�߶���������ˣ��޽��㣩
				else if (ps.x >= p2.x)break;//�߶��������Ҷˣ��޽��㣩
				double xlv = (pn.y - ps.y)*1.0 / (pn.x - ps.x);
				double ju = pn.y - xlv*pn.x;
				if (ps.x <= p1.x && p1.x < pn.x) 
				{//���滻ǰ��
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
					//���滻���
					pn.x = p2.x;
					pn.y = (int)(xlv*p2.x + ju + 0.5);
					yy2 = pn.y;
				}

				int y0 = (int)(xlv0*pn.x + ju0 + 0.5);//���ߺ��
				if (zuos && pn.y < y0)
				{
					//�����㶼���Ϸ�
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, pn.x);
					cross = true;
				}
				else if (zuos &&pn.y >= y0)
				{
					//��������
					int x = (int)((ju - ju0) / (xlv0 - xlv) + 0.5); int y = (int)(xlv*x + ju + 0.5);
					xjs += CalAre(xlv, ju, xlv0, ju0, ps.x, x);
					line[toptc[j]] += sqrt((pn.x - x)*(pn.x - x) + (y0 - y)*(y0 - y));
					line[buttomtc[j]] += sqrt((ps.x - x)*(ps.x - x) + (beforey - y)*(beforey - y));
					zuos = false;
					cross = true;
				}
				else if (!zuos&& pn.y < y0)
				{
					//��������
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
				xj[xjid].s = xjs;//�˲����ߵ�������ߵ��ཻ���
				xj[xjid].id = j;//�õ�������id
				xj[xjid].yl = yy2;
				xjid++;
			}
		}
		std::sort(xj, xj + xjid);//���������¶Խ��õĲ���������
		for (int j = 0; j < xjid; j++)
		{
			if (j == 0)
				are[toptc[xj[j].id]] += s - xj[j].s;
			else
				are[toptc[xj[j].id]] += xj[j - 1].s - xj[j].s;//���������֮�������������������
		}
		double totl = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));//�ܵ������ױ߳�
		L[i] = totl;
		for (int j = 0; j < 10; j++)
		{
			totl -= line[j];//ʣ�³�
		}
		if (m_tcnt > 0 && xjid > 0)
		{
			//���˲����� �� ������������н���
			are[buttomtc[xj[xjid - 1].id]] += xj[xjid - 1].s;//���µ���������²����ߵ��·�����
			if (totl > 0)
				line[buttomtc[xjid - 1]] += totl;//ʣ�µĳ��ȸ����²����ߵ��·�����
		}
		else if (m_tcnt > 0 && xjid == 0)
		{
			//���˲����� �� ������������޽���
			CPoint p;
			p.x = (p1.x + p2.x) / 2; p.y = (p1.y + p2.y) / 2;
			int topclx = GetTopTC(p, pDC);
			are[topclx] += s;
			line[topclx] += totl;
		}
		else if (m_tcnt <= 0)
		{
			//û��������
			are[toptcid] += s;
			line[toptcid] += totl;
		}
		fai[i] = 0;//��Ȩƽ��faiֵ
		for (int j = 0; j < 10; j++)
		{
			fai[i] += tuceng[j].phi*line[j];
		}
		fai[i] /= L[i];
		//cosAlpha[i] = (p2.x - p1.x) / L[i];
		//sinAlpha[i] = (p1.y - p2.y) / L[i];
		calAngle(p2, p1, &sinAlpha[i], &cosAlpha[i], nullptr, nullptr);	//����AB������x��ļн�<BAx,��y��ļн�<BAy

		cosa[i] = (p2.x - p1.x) / L[i];
		sina[i] = (p1.y - p2.y) / L[i];
		alf[i] = acos(cosa[i]);

		//alf[i] = acos(cosAlpha[i]);
		tgf[i] = tan(fai[i] * pi / 180);
		W[i] = 0; cl[i] = 0;
		for (int j = 0; j < 10; j++) 
		{
			//��10��������һ�����������Ĺ��׼�����
			W[i] +=1000*are[j] * tuceng[j].gama;
			cl[i] += tuceng[j].c*line[j];
		}
		if (W[i] == 0)
			return;
	}

	double K(0);		
	vector<double> iterationDeltaSum;	//������ֹ������λ�� 
	vector<double> iterationDeltaFSum;
	vector<BlockData> allBlockData;		//���п������,ֻ�������һ������
	allBlockData.resize(tks);	//���һ��
	switch (mode)
	{
	case 1:
	{   

		vector<BlockData> lastData=allBlockData;				//������һ�ε������п������
		vector<BlockData> last_lastData=allBlockData;      //��������һ�ε�������
		const int maxIterationNum = 1000;		//����������
		MyPoint pp12[30][4]; //�洢�ڵ�λ��
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
		}///ԭ�����нڵ�λ�ò���
		char s[100];
		sprintf_s(s, "%d.txt",txt_num);
		ofstream fout(s);
		
		for (int calIter = 0; calIter < maxIterationNum; ++calIter)
		{   
		//	tks=tks-1;
			for (int i = 0; i < tks; i++)
			{
				//����
				BlockData blockData = allBlockData[i];			//ÿ���������
				double AMat[3][3];								//A3*3�ܾ���
				double deltaX(0), deltaY(0), deltaTheta(0);		//���ĵ�λ������
				double FxSum(0), FySum(0), MSum(0);	//�����е�x,y�Ϸ���,ȫ����

				//0.0. ��������,����
				MyPoint centroid;			//����
				double area(0);			//�����
				for (int m = 0; m < number[i] - 1; ++m)
				{
					area += pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x * pp[i][m].y;
					centroid.x += (pp[i][m].x + pp[i][m + 1].x) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
					centroid.y += (pp[i][m].y + pp[i][m + 1].y) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
				}
				//���һ����Ϊ(x0,y0)
				area += pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x * pp[i][number[i] - 1].y;
				centroid.x += (pp[i][number[i] - 1].x + pp[i][0].x) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				centroid.y += (pp[i][number[i] - 1].y + pp[i][0].y) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				//������
				area /= 2;
				centroid.x /= 6 * area;
				centroid.y /= 6 * area;
				//0.1 ����AMat, 3*3����
				AMat[0][0] = 0; AMat[0][1] = 0; AMat[0][2] = 0;
				AMat[1][0] = 0; AMat[1][1] = 0; AMat[1][2] = 0;
				AMat[2][0] = 0; AMat[2][1] = 0; AMat[2][2] = 0;
				double aMat[3][3];			//ÿ���ڵ��A3*3�ܾ���
				aMat[0][0] = 0; aMat[0][1] = 0; aMat[0][2] = 0;
				aMat[1][0] = 0; aMat[1][1] = 0; aMat[1][2] = 0;
				aMat[2][0] = 0; aMat[2][1] = 0; aMat[2][2] = 0;

				double sinA, cosA;
				if  (number[i]==3 && i==0)  //��������
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					if (m==number[i]-1) //�󵯻�4�ŵ��ɣ�����һ��
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}

				      }
				  }
				else if (number[i]==3 && i==tks-1) //�ұ߽�������
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					 if (m==2) //��߽�������4�ŵ��� ����һ��
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					 }
				      }
		
				}
				else //�ı���
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

					//���
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}
				  }
				}
				    
				//1.0.��FxSum, FySum, MSum
				//��ÿ�����X,Y����������� 
				MyPoint dis[4];				//ÿ���ڵ㵽���ĵľ���

				if (3 == number[i])
				{ 
		             if (i==0) //�����Ҳ����
                   {
			          blockData.Fx[0]=0;
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
			          blockData.Fy[0]=0;
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
			     	}
			 else if (i==tks-1) //�������������Ѿ����µ���
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
					//��Ҫ���ǵ�������
				 if (i==0) //�����Ҳ����
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
				else if (i==tks-1) //�������������Ѿ����µ���
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

				else if (i==tks-2) //�����ڶ��������ı��Σ��������������Ѿ����µ���
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
				//1.1. ���deltaX, deltaY, deltaTheta

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
				//2.0. ��ÿ���ڵ��λ��
				MyPoint nodeDeltaPos[4];		//ÿ���ڵ��ƫ��deltaX, deltaY
				for (int m = 0; m < number[i]; ++m)
				{
					nodeDeltaPos[m].x = -deltaX +(pp[i][m].y - centroid.y) * deltaTheta;
					nodeDeltaPos[m].y = -deltaY - (pp[i][m].x - centroid.x) * deltaTheta;
				}
				//3.0. ��ÿ���ڵ������
				//3.1. ��ÿ���ڵ�����ֽ⵽x,y����
			double cosX(0), sinX(0), sinY(0), cosY(0);		//������x,y��ļнǵ�cosֵ
			double NSum(0), TSum(0);		//���п�ķ�������������֮��
			double NSum_moment(0);  //����������֮��
				if (3 == number[i])
				{
	              	if (i==0)  //���Ե��ʱ��
					{
                   //��1���ڵ㣬����  
					calAngle(pp[i][0], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					blockData.F1[0] = ks * nodeDeltaPos[0].x * cosX + ks * nodeDeltaPos[0].y * sinX;			//1��,����,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * sinX + kn * nodeDeltaPos[0].y * -cosX;		//2��,����,theta-pi/2
					
					//��2���ڵ�,����
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;				//1��,����,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;					//2��,����,+X

       
					//��3���ڵ�, ����
                     blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1��,����,+Y
				     blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2��,����,+X
				     calAngle(pp[i][number[i] - 1], pp[i][0], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					 blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;	//����			//3��,����,theta
					 blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4��,����,theta+pi/2		
					
					 //�����ж�����
					 //�ױ���
                       	NSum =(allBlockData[i].F1[1]+allBlockData[i].F3[3]);//������	
						TSum =allBlockData[i].F1[0]-allBlockData[i].F3[2];//����������
						NSum_moment=NSum* tgf[i] + cl[i];
					   if (abs(NSum_moment)<abs(TSum))
					   {
					   allBlockData[i].F1[0]=Sign(TSum)*abs(NSum_moment)/2;
					   allBlockData[i].F3[2]=-Sign(TSum)*abs(NSum_moment)/2;
					   }
                     //y�ұ߽�
					    NSum = (allBlockData[i].F2[1]+allBlockData[i].F3[1]);//������	
						TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];//����������
						NSum_moment=NSum* tgf[i] + cl[i];
					  if (abs(NSum_moment)>abs(TSum))
					   {
					   allBlockData[i].F3[0]=Sign(TSum)*abs(NSum_moment)/2;
					   allBlockData[i].F2[0]=-Sign(TSum)*abs(NSum_moment)/2;
					   }
					 //��1���ڵ㣬���� 
					blockData.Fx[0] = blockData.F1[0] * cosX + blockData.F1[1] * sinX;		//X����
					blockData.Fy[0] = blockData.F1[0] * sinX + blockData.F1[1] * -cosX;		//Y����
					//��2���ڵ�,����
				    blockData.Fx[1] = blockData.F2[1];					//X����
					blockData.Fy[1] = blockData.F2[0] * -1;			//Y����			
					//��3���ڵ�, ����					
				     blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X����
					 blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * cosX;	//Y����	
					
					}
					else
					{
                     //��1���ڵ㣬����  		
					blockData.F1[0] =  ks * nodeDeltaPos[0].y *-1;						//1��,����,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1;					//2��,����,theta-pi/2

					//��2���ڵ�,����
					calAngle(pp[i][1], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					blockData.F2[0] = ks * nodeDeltaPos[1].x * cosX+ ks * nodeDeltaPos[1].y * sinX;				//1��,����,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * -sinX + kn * nodeDeltaPos[1].y * cosX;					//2��,����,+X
					//��3���ڵ�,����
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;						//1��,����,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * -1 + kn * nodeDeltaPos[2].y * 0;					//2��,����,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][1], &sinX, &cosX, &sinY, &cosY);	
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3��,����, theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x *sinX + kn * nodeDeltaPos[2].y * -cosX;				//2��,����, theta+pi/2
				
					//�����ж�����
					//�ױ߷���
					NSum = allBlockData[i].F2[1]+allBlockData[i].F3[1];//������
					TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{
					allBlockData[i].F3[0]=Sign(TSum)*abs(NSum_moment)/2;
					allBlockData[i].F2[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}	
					//y����߽�
					NSum = allBlockData[i].F3[3]+allBlockData[i].F1[1];//������
					TSum=allBlockData[i].F3[2]-allBlockData[i].F1[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{
					allBlockData[i].F3[2]=Sign(TSum)*abs(NSum_moment)/2;
					allBlockData[i].F1[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}	


					//��1���ڵ㣬����
					blockData.Fx[0] =  blockData.F1[1] *-1;		//X����
					blockData.Fy[0] = blockData.F1[0] * -1;		//Y����
					
					//��2���ڵ�,����
					blockData.Fx[1] = blockData.F2[0]*cosX +blockData.F2[1]*-sinX;					//X����
					blockData.Fy[1] = blockData.F2[0] *sinX +blockData.F2[1]*cosX;			//Y����
					//��3���ڵ�,����
					blockData.Fx[2] = -blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * sinX;	//X����
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * -cosX;	//Y����

					}
					//��4���ڵ�
					blockData.Fx[3] = 0;	//X����
					blockData.Fy[3] = 0;	//Y����
				}
				else if (4 == number[i])
				{
		//��1���ڵ�,����
					blockData.F1[0] = ks * nodeDeltaPos[0].x * 0 + ks * nodeDeltaPos[0].y * -1;					//1��,����,-Y
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1 + kn * nodeDeltaPos[0].y * 0;					//2��,����,-X

					//��2���ڵ�,����
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;					//1��,����,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;						//2��,����,+X

					//��3���ڵ�,����
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1��,����,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2��,����,+X
			  
				    calAngle(pp[i][2], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);				//����cos(thetaX), cos(thetaY);
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3��,����,theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4��,����,theta+pi/2
				
					//��4���ڵ�,����
					blockData.F4[0] = ks * nodeDeltaPos[3].x * 0 + ks * nodeDeltaPos[3].y * 1;						//1��,����,+Y
					blockData.F4[1] = kn * nodeDeltaPos[3].x * -1 + kn * nodeDeltaPos[3].y * 0;					//2��,����,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	
					blockData.F4[2] = ks * nodeDeltaPos[3].x * cosX + ks * nodeDeltaPos[3].y * sinX;				//3��,����, theta
					blockData.F4[3] = kn * nodeDeltaPos[3].x * sinX + kn * nodeDeltaPos[3].y * -cosX;				//2��,����, theta+pi/2
	
			      //�ױ�
					NSum = (allBlockData[i].F3[3] +allBlockData[i].F4[3]);//������
					TSum=allBlockData[i].F4[2]-allBlockData[i].F3[2];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{	
						allBlockData[i].F4[2]=Sign(TSum)*abs(NSum_moment)/2;
					    allBlockData[i].F3[2]=-Sign(TSum)*abs(NSum_moment)/2;
					}
					//��߽�
					NSum = (allBlockData[i].F4[1] +allBlockData[i].F1[1]);//������
					TSum=allBlockData[i].F1[0]-allBlockData[i].F4[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{	
						allBlockData[i].F1[0]=Sign(TSum)*abs(NSum_moment)/2;
					    allBlockData[i].F4[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}
					//�ұ߽�

					NSum = (allBlockData[i].F2[1] +allBlockData[i].F3[1]);//������
					TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];
					NSum_moment=NSum* tgf[i] + cl[i];
					if (abs(NSum_moment)>abs(TSum))
					{	
						allBlockData[i].F3[0]=Sign(TSum)*abs(NSum_moment)/2;
					    allBlockData[i].F2[0]=-Sign(TSum)*abs(NSum_moment)/2;
					}

	               //��1���ڵ�,����
					blockData.Fx[0] = blockData.F1[1] * -1;				//X����
					blockData.Fy[0] = blockData.F1[0] * -1;				//Y����

					//��2���ڵ�,����
					blockData.Fx[1] = blockData.F2[1];				//X����
					blockData.Fy[1] = blockData.F2[0] * -1;				//Y����

					//��3���ڵ�,����
					blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X����
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX+ blockData.F3[3] * cosX;	//Y����

					//��4���ڵ�,����
		  		    blockData.Fx[3] = -blockData.F4[1] + blockData.F4[2] * cosX + blockData.F4[3] * sinX;	//X����
					blockData.Fy[3] = blockData.F4[0] + blockData.F4[2] * sinX + blockData.F4[3] * -cosX;	//Y����
}
	//		      blockData.NF_x=abs(blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]+blockData.Fx[3]+FxSum);
	//		      blockData.NF_y=abs(blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]+blockData.Fy[3]+FySum);
	//		   if (abs(blockData.NF_x)>0.0001 || abs(blockData.NF_y)>0.0001)
	//		   {
	//		   blockData.NF_x=blockData.NF_x;
	//		   }
				//4.0 ������һ���Fx,Fy
				//�������һ����

			//�������� ���浱ǰ���λ�ƽǶȵ�����
			blockData.deltaX = deltaX;
			blockData.deltaY = deltaY;
			blockData.deltaTheta = deltaTheta;
			allBlockData[i] = blockData; 
	
			//�����ǰ��������£�����i����
 
             int  out_num=4;  //�������������
                if (i==out_num-1&&out_num<tks&&txt_num%100==0) 
                   {
                       if (i==0&&number[i]==3) //��һ�����������ڵ������
                              fout<<calIter<<"      "<<blockData.Fx[0]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fy[0]<<"      "<<blockData.Fy[1]<<"      "<<blockData.Fy[2]<<endl;
                       else if(i==tks-1&&number[i]==3) //���һ�����������ڵ������
                              fout<<calIter<<"      "<<blockData.Fx[0]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fy[0]<<"      "<<blockData.Fy[1]<<"      "<<blockData.Fy[2]<<endl;
                       else
                              fout<<calIter<<"      "<<blockData.Fx[0]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fx[3]<<"      "<<blockData.Fy[0]<<"      "<<blockData.Fy[1]<<"      "<<blockData.Fy[2]<<"      "<<blockData.Fx[1]<<"      "<<blockData.Fx[2]<<"      "<<blockData.Fx[3]<<endl;	
                  }
		
		//�������������
	//		     fout<<"      "<<double(cntfg2-cntfg1)<<endl;
		}

/*
lastData.assign(allBlockData.begin(), allBlockData.end());//�����ϴε����ݵ�lastData
  //  �ڶ��ε�������
			for (int i = tks-1; i >=0; i--)  //
			{
				//����
				BlockData blockData = allBlockData[i];			//ÿ���������
				double AMat[3][3];								//A3*3�ܾ���
				double deltaX(0), deltaY(0), deltaTheta(0);		//���ĵ�λ������
				double FxSum(0), FySum(0), MSum(0);	//�����е�x,y�Ϸ���,ȫ����
				//0.0. ��������,����
				MyPoint centroid;			//����
				double area(0);			//�����
					for (int m = 0; m < number[i] - 1; ++m)
				{
					area += pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x * pp[i][m].y;
					centroid.x += (pp[i][m].x + pp[i][m + 1].x) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
					centroid.y += (pp[i][m].y + pp[i][m + 1].y) * (pp[i][m].x * pp[i][m + 1].y - pp[i][m + 1].x*pp[i][m].y);
				}
				//���һ����Ϊ(x0,y0)
				area += pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x * pp[i][number[i] - 1].y;
				centroid.x += (pp[i][number[i] - 1].x + pp[i][0].x) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				centroid.y += (pp[i][number[i] - 1].y + pp[i][0].y) * (pp[i][number[i] - 1].x * pp[i][0].y - pp[i][0].x*pp[i][number[i] - 1].y);
				//������
				area /= 2;
				centroid.x /= 6 * area;
				centroid.y /= 6 * area;
				//0.1 ����AMat, 3*3����
				AMat[0][0] = 0; AMat[0][1] = 0; AMat[0][2] = 0;
				AMat[1][0] = 0; AMat[1][1] = 0; AMat[1][2] = 0;
				AMat[2][0] = 0; AMat[2][1] = 0; AMat[2][2] = 0;
				double aMat[3][3];			//ÿ���ڵ��A3*3�ܾ���
				aMat[0][0] = 0; aMat[0][1] = 0; aMat[0][2] = 0;
				aMat[1][0] = 0; aMat[1][1] = 0; aMat[1][2] = 0;
				aMat[2][0] = 0; aMat[2][1] = 0; aMat[2][2] = 0;

				double sinA, cosA;
				if  (number[i]==3 && i==0)  //��������
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					if (m==number[i]-1) //�󵯻�4�ŵ��ɣ�����һ��
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}

				      }
				  }
				else if (number[i]==3 && i==tks-1) //�ұ߽�������
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];

					 if (m==2) //��߽�������4�ŵ��� ����һ��
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					 }
				      }
		
				}
				else //�ı���
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

					//���
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

					//���
					AMat[0][0] += aMat[0][0]; AMat[0][1] += aMat[0][1]; AMat[0][2] += aMat[0][2];
					AMat[1][0] += aMat[1][0]; AMat[1][1] += aMat[1][1]; AMat[1][2] += aMat[1][2];
					AMat[2][0] += aMat[2][0]; AMat[2][1] += aMat[2][1]; AMat[2][2] += aMat[2][2];
					}
				  }
				}
				    
				//1.0.��FxSum, FySum, MSum
				//��ÿ�����X,Y����������� 
				MyPoint dis[4];				//ÿ���ڵ㵽���ĵľ���

				if (3 == number[i])
				{ 
		     	 if (i==0) //�����Ҳ����
                   {
			          blockData.Fx[0]=0;
					  blockData.Fx[1]=-allBlockData[i+1].Fx[0];
                      blockData.Fx[2]=-allBlockData[i+1].Fx[3];
			          blockData.Fy[0]=0;
					  blockData.Fy[1]=-allBlockData[i+1].Fy[0];
                      blockData.Fy[2]=-allBlockData[i+1].Fy[3];
			     	}
			     else if (i==tks-1) //�������������Ѿ����µ���
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
					//��Ҫ���ǵ�������
				 if (i==0) //�����Ҳ����
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
				else if (i==tks-1) //�������������Ѿ����µ���
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

				//2.0. ��ÿ���ڵ��λ��
				MyPoint nodeDeltaPos[4];		//ÿ���ڵ��ƫ��deltaX, deltaY
				for (int m = 0; m < number[i]; ++m)
				{
					nodeDeltaPos[m].x = -deltaX +(pp[i][m].y - centroid.y) * deltaTheta;
					nodeDeltaPos[m].y = -deltaY - (pp[i][m].x - centroid.x) * deltaTheta;
				}

				//3.0. ��ÿ���ڵ������
				//3.1. ��ÿ���ڵ�����ֽ⵽x,y����
			double cosX(0), sinX(0), sinY(0), cosY(0);		//������x,y��ļнǵ�cosֵ
				if (3 == number[i])
				{
			if (i==0)  //���Ե��ʱ��
					{
                   //��1���ڵ㣬����  
					calAngle(pp[i][0], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					blockData.F1[0] = ks * nodeDeltaPos[0].x * cosX + ks * nodeDeltaPos[0].y * sinX;						//1��,����,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * sinX + kn * nodeDeltaPos[0].y * -cosX;					//2��,����,theta-pi/2
					blockData.Fx[0] = blockData.F1[0] * cosX + blockData.F1[1] * sinX;		//X����
					blockData.Fy[0] = blockData.F1[0] * sinX + blockData.F1[1] * -cosX;		//Y����

					//��2���ڵ�,����
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;				//1��,����,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;					//2��,����,+X
					blockData.Fx[1] = blockData.F2[1];					//X����
					blockData.Fy[1] = blockData.F2[0] * -1;			//Y����
       
					//��3���ڵ�, ����
                      		blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1��,����,+Y
				         	blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2��,����,+X
				        	calAngle(pp[i][number[i] - 1], pp[i][0], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					        blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;	//����			//3��,����,theta
					        blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4��,����,theta+pi/2
			               blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X����
					       blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * cosX;	//Y����	
						}
					else
					{
                     //��1���ڵ㣬����  		
					blockData.F1[0] =  ks * nodeDeltaPos[0].y *-1;						//1��,����,theta
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1;					//2��,����,theta-pi/2
					blockData.Fx[0] =  blockData.F1[1] *-1;		//X����
					blockData.Fy[0] = blockData.F1[0] * -1;		//Y����
					//��2���ڵ�,����
					calAngle(pp[i][1], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					blockData.F2[0] = ks * nodeDeltaPos[1].x * cosX+ ks * nodeDeltaPos[1].y * sinX;				//1��,����,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * -sinX + kn * nodeDeltaPos[1].y * cosX;					//2��,����,+X
					blockData.Fx[1] = blockData.F2[0]*cosX +blockData.F2[1]*-sinX;					//X����
					blockData.Fy[1] = blockData.F2[0] *sinX +blockData.F2[1]*cosX;			//Y����
				    //��3���ڵ�,����
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;						//1��,����,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * -1 + kn * nodeDeltaPos[2].y * 0;					//2��,����,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][1], &sinX, &cosX, &sinY, &cosY);	
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3��,����, theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x *sinX + kn * nodeDeltaPos[2].y * -cosX;				//2��,����, theta+pi/2
		  		    blockData.Fx[2] = -blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * sinX;	//X����
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX + blockData.F3[3] * -cosX;	//Y����
					}
					//��4���ڵ�
					blockData.Fx[3] = 0;	//X����
					blockData.Fy[3] = 0;	//Y����
				}
				else if (4 == number[i])
				{
					//��1���ڵ�,����
					blockData.F1[0] = ks * nodeDeltaPos[0].x * 0 + ks * nodeDeltaPos[0].y * -1;					//1��,����,-Y
					blockData.F1[1] = kn * nodeDeltaPos[0].x * -1 + kn * nodeDeltaPos[0].y * 0;					//2��,����,-X
					blockData.Fx[0] = blockData.F1[1] * -1;				//X����
					blockData.Fy[0] = blockData.F1[0] * -1;				//Y����

					//��2���ڵ�,����
					blockData.F2[0] = ks * nodeDeltaPos[1].x * 0 + ks * nodeDeltaPos[1].y * -1;					//1��,����,-Y
					blockData.F2[1] = kn * nodeDeltaPos[1].x * 1 + kn * nodeDeltaPos[1].y * 0;						//2��,����,+X
					blockData.Fx[1] = blockData.F2[1];				//X����
					blockData.Fy[1] = blockData.F2[0] * -1;				//Y����

					//��3���ڵ�,����
					blockData.F3[0] = ks * nodeDeltaPos[2].x * 0 + ks * nodeDeltaPos[2].y * 1;					//1��,����,+Y
					blockData.F3[1] = kn * nodeDeltaPos[2].x * 1 + kn * nodeDeltaPos[2].y * 0;					//2��,����,+X
			  
				    calAngle(pp[i][2], pp[i][number[i] - 1], &sinX, &cosX, &sinY, &cosY);				//����cos(thetaX), cos(thetaY);
					blockData.F3[2] = ks * nodeDeltaPos[2].x * cosX + ks * nodeDeltaPos[2].y * sinX;				//3��,����,theta
					blockData.F3[3] = kn * nodeDeltaPos[2].x * -sinX + kn * nodeDeltaPos[2].y * cosX;				//4��,����,theta+pi/2
				
					blockData.Fx[2] = blockData.F3[1] + blockData.F3[2] * cosX + blockData.F3[3] * -sinX;	//X����
					blockData.Fy[2] = blockData.F3[0] + blockData.F3[2] * sinX+ blockData.F3[3] * cosX;	//Y����

					//��4���ڵ�,����
					blockData.F4[0] = ks * nodeDeltaPos[3].x * 0 + ks * nodeDeltaPos[3].y * 1;						//1��,����,+Y
					blockData.F4[1] = kn * nodeDeltaPos[3].x * -1 + kn * nodeDeltaPos[3].y * 0;					//2��,����,-X
					//calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	//����cos(thetaX), cos(thetaY);
					calAngle(pp[i][number[i] - 1], pp[i][2], &sinX, &cosX, &sinY, &cosY);	
					blockData.F4[2] = ks * nodeDeltaPos[3].x * cosX + ks * nodeDeltaPos[3].y * sinX;				//3��,����, theta
					blockData.F4[3] = kn * nodeDeltaPos[3].x * sinX + kn * nodeDeltaPos[3].y * -cosX;				//2��,����, theta+pi/2
		  		    blockData.Fx[3] = -blockData.F4[1] + blockData.F4[2] * cosX + blockData.F4[3] * sinX;	//X����
					blockData.Fy[3] = blockData.F4[0] + blockData.F4[2] * sinX + blockData.F4[3] * -cosX;	//Y����
			}

			      blockData.NF_x=abs(blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]+blockData.Fx[3]+FxSum);
			      blockData.NF_y=abs(blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]+blockData.Fy[3]+FySum);
			//����ڵ��������Ƿ����
			if (abs(blockData.NF_x)>0.0001 || abs(blockData.NF_y)>0.0001)
			   {
			   blockData.NF_x=blockData.NF_x;
			   }
			//����ڵ�������Ƿ���� //�����������
			if (i==0 && number[i]==3) //������������
			{
			blockData.NF_x=blockData.Fx[0]+blockData.Fx[1]+blockData.Fx[2]-(allBlockData[1].Fx[0]+allBlockData[1].Fx[3]);
			blockData.NF_y=blockData.Fy[0]+blockData.Fy[1]+blockData.Fy[2]-(allBlockData[1].Fy[0]+allBlockData[1].Fy[3]+W[0]);
			}
			else if  (i==tks-1 &&  number[i]==3) //���Ҳ�������
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

				//4.0 ������һ���Fx,Fy
			  //�������� ���浱ǰ���λ�ƽǶȵ�����
			blockData.deltaX = deltaX;
			blockData.deltaY = deltaY;
			blockData.deltaTheta = deltaTheta;
			allBlockData[i] = blockData; 
			}
			*/
		  //�ڶ��ε����������
			//������ֹ����
			//1.��λ��
			double deltaSum(0);
			for (auto it = allBlockData.begin(); it != allBlockData.end(); ++it)
			{
				deltaSum += abs(it->deltaX) + abs(it->deltaY) + abs(it->deltaTheta);
			}
			iterationDeltaSum.push_back(deltaSum);
			//2.����
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

			//������ֹ,�����Ѿ������һ�ε���
			if ((0 != calIter && deltaFSum<= 20) || calIter == maxIterationNum - 1)
			{
				//5.0 ����K
				double NSum(0), TSum(0);		//���п�ķ�������������֮��
				double NSum_moment(0);  //����������֮��
				double fenzi(0); double fenmu(0);//���ӣ���ĸ
				double Count(0);



		 //ɾ���ļ�����
		    fout.close(); //�ر��ļ�����
		//������������������������������м�������ֽϴ�ķ���ʱɾ�������ļ�
		   if (calIter == maxIterationNum - 1||allBlockData[tks-3].Fx[1]>40e8||txt_num%100!=0)
		    remove(s);

				for (int i = 0; i < tks; i++)
				{
					//������б���cos(alpha)���ж����ķ���,��ע���ĸ���ʱC,D��3�ŵ��ɷ����෴
					if (3 == number[i]&& i==tks-1)
					{   //��������
					     NSum = -(allBlockData[i].F3[3]+allBlockData[i].F3[1]);//������
			           //  if (NSum>0)
			          //	 NSum=0;
					     TSum=allBlockData[i].F3[0]-allBlockData[i].F2[0];
						 NSum_moment=NSum* tgf[i] + cl[i];
					}
				    else if (3 == number[i] && i==0)
					{   //��������
				    	NSum = -(allBlockData[i].F1[1]+allBlockData[i].F3[3]);//������	
				        //		if (NSum>0)
			        	//			NSum=0;
						TSum=allBlockData[i].F1[0]-allBlockData[i].F3[2];
						NSum_moment=NSum* tgf[i] + cl[i];
					}
					else if (4 == number[i])
					{
					//	NSum = -(allBlockData[i].F3[2] +allBlockData[i].F4[2]);
						NSum = -(allBlockData[i].F3[3] +allBlockData[i].F4[1]);//�涨������������������y���෴
					//	if (NSum>0)
                    //        NSum=0;
						TSum=allBlockData[i].F4[2]-allBlockData[i].F3[2];
						NSum_moment=NSum* tgf[i] + cl[i];
					}

				//	fenzi += NSum * tgf[i] - cl[i];//������
					if (abs(NSum_moment)<abs(TSum))
					{	
						TSum=Sign(TSum)*abs(NSum_moment);
					     Count++;
					}
			    	fenzi += NSum_moment;
					fenmu += TSum;
				}
				if  (Count==tks)
			      K = 0.2;//�õ���һԲ����K
				else
				{
					K =abs(fenzi)/abs(fenmu);
				}

//				fout<<"      "<<K;
		//		tks=tks+1;
				break;
			}

			
			//���浱ǰ�����ļ�������
			//blocksData.Fx = nodeFx;
			//blocksData.Fy = nodeFy;
	       //tks=tks+1;
		}
//�ڵ㻹ԭ
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


int CSlopeSView:: Sign(double  A)  //�жϷ���
{
	if (A>=0)
		return 1;
	else
	   return  -1;
}

//����Ƕ�<ABC>
void CSlopeSView::calAngle(MyPoint& A, MyPoint& B, MyPoint& C, double* sina, double* cosa)
{
	MyPoint BA = A - B;
	MyPoint BC = C - B;

	//����н�
	double temp = (BA.x * BC.x + BA.y * BC.y) / sqrt(BA.x * BA.x + BA.y * BA.y) / sqrt(BC.x * BC.x + BC.y * BC.y);
	if (nullptr != cosa)
		*cosa = temp;
	if (nullptr != sina)
		*sina = sqrt(1 - temp * temp);
}

//����BA������x��ļн�<BAx, ��y��ļн�<BAy
void CSlopeSView::calAngle(MyPoint& A, MyPoint& B, double* sinX, double* cosX, double* sinY, double* cosY)
{
/*	
    MyPoint AB = B - A;
	MyPoint xVec(1, 0), yVec(0, 1);
	if (AB.x!=0||AB.y!=0)
	{
	//����AB��x�ļн�
	double tempCosX = (AB.x * xVec.x + AB.y * xVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	//����AB��y�ļн�
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
	//����AB��x�ļн�
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
	//����AB��x�ļн�
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

//����BA������x��ļн�<BAx, ��y��ļн�<BAy
void CSlopeSView::calAngle(CPoint& A, CPoint& B, double* sinX, double* cosX, double* sinY, double* cosY)
{
	CPoint AB = B - A;
	CPoint xVec(1, 0), yVec(0,1);

	//����AB��x�ļн�
	if (AB.x!=0||AB.y!=0)
	{
	double tempCosX = (AB.x * xVec.x + AB.y * xVec.y) / sqrt(AB.x * AB.x + AB.y * AB.y);
	//����AB��y�ļн�
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
    //��ӵ�
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