
// SlopeSView.h : CSlopeSView ��Ľӿ�
//
#pragma once
#include "SlopeSDoc.h"
#include "DataShow.h"
#include <vector>

#define POINT_COUNT 100  //������������define���䶨��Ϊ���ų�����Ϊ�˷����Ժ���ܵ��޸ģ����������Ժ��뽫������Ϊ200����ֻ�Ĵ˺궨��Ϳ����ˡ�
#define pi 3.14159265

using namespace std;

class MyRect
{
public:
	double l;
	double t;
	double r;
	double b;
	MyRect(double _l, double _t, double _r, double _b)
		:l(_l),t(_t), r(_r) , b(_b)
	{
	}
};
//�Զ������ݽṹ
class MyPoint
{
public:
	double x;
	double y;

	MyPoint() : x(0), y(0) {}
	MyPoint(double x, double y) : x(x), y(y) {}

	MyPoint& operator =(MyPoint point)
	{
		this->x = point.x;
		this->y = point.y;
		return *this;
	}

	MyPoint operator +(MyPoint point) const
	{
		return MyPoint(x + point.x, y + point.y);
	}

	MyPoint operator -(MyPoint point) const
	{
		return MyPoint(x - point.x, y - point.y);
	}

	MyPoint(const POINT& p)
	{
		x = p.x;
		y = p.y;
	}

	operator POINT() const
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		return pt;
	}
};


//ÿ��������ݽ��
class BlockData
{
public:
	BlockData()
	{
		deltaX = deltaY = deltaTheta = 0;
		sinA0 = cosA0 = 0;
		F1[0] = F1[1] = 0;
		F2[0] = F2[1] = 0;
		F3[0] = F3[1] = F3[2] = F3[3] = 0;
		F4[0] = F4[1] = F4[2] = F4[3] = 0;
		Fx[0] = Fx[1] = Fx[2] = Fx[3] = 0;
		Fy[0] = Fy[1] = Fy[2] = Fy[3] = 0;
		NF_x=NF_y=0;
	}

	double deltaX;		//���ĵ�λ������
	double deltaY;
	double deltaTheta;

	double sinA0;		//��б����+X�ĽǶ�sin(alpha),cos(alpha)
	double cosA0;

	double F1[2];		//1,2,3,4�ڵ�ķ������������
	double F2[2];
	double F3[4];
	double F4[4];

	double Fx[4];		//1,2,3,4�ڵ��x,y����
	double Fy[4];

	double NF_x; //������֮��ĺ�����
	double NF_y;

};


class CSlopeSView : public CView
{
protected: // �������л�����
	CSlopeSView();
	DECLARE_DYNCREATE(CSlopeSView)

	// ����
public:
	CSlopeSDoc* GetDocument() const;

	// ����
public:
	CEdit m_EditPoint;
	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// ʵ��
public:
	virtual ~CSlopeSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
		// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

public:
	afx_msg void OnNewEb();
public:
	int rgbcolor[25];
	bool FirstFocus;
	UINT m_nDrawType;
	MyPoint m_nzValues[POINT_COUNT];
	int  m_crt_p;//�߽�ĵ���ͳ��
	int m_nLineWidth;
	int m_nLineStyle;
	int m_clrl;
	int m_clrp;
	int m_tid;//�����ߵ�����id
	int m_tcnt;
	int m_pcrt[POINT_COUNT];//��m_tid�������ߵĵ���ͳ��
	MyPoint m_fjxpoint[POINT_COUNT][POINT_COUNT];//��m_tid���ĵ�m_pcrt��Ϊm_fjxpoint[m_tid][m_pcrt[m_tid]]
	int toptc[POINT_COUNT], buttomtc[POINT_COUNT];//��¼ÿ����������������id��
	int toptcid;//���ϲ�����id(���û��������ʱ)
	bool m_tfinish[POINT_COUNT];
	bool m_drawonelinef;
	int mode;
	MyPoint xypoint;
	MyPoint linepoint;
	MyPoint RButtonUp;
	bool xybz;
	bool bihe;
	bool Linebz;
	float m_scale;
	float m_translateX;
	float m_translateY;
	float m_off_x;
	float m_off_y;

	bool m_bDraw[20];//���Ʋ˵�����
	int m_delete_id;//ɾ��������id
	int bianjie[5];//�߽�id

	struct tuceng
	{
		CString name;
		int clr;
		double gama, phi, c;
	}tuceng[20];

	int pTuCeng, pFocus;
	int fgid, tkid;

	struct tianchong
	{
		MyPoint point;
		int clr;
	}tianchong[300];

	int tc_step;
	MyPoint crt_tc;
	int crt_fg;
	MyPoint fg[5];
	int jd_fg, jd_yh, tks;

	struct pointex
	{
		MyPoint p;//Բ����߽��߽��������
		double ang;//��ļ������,Բ��Ϊ������ԭ��
		//int nub;//��¼����������߽�����,on line: m_nzValues[nub]~m_nzValues[(nub + 1) % m_crt_p]

		//��������ʱ����д�ĺ���
		bool operator<(const pointex &rhs) const
		{
			return   ang < rhs.ang;
		}
	};

	//��¼��������
	struct tksj
	{
		double W, cosa, sina, fai, are, line, cl, alf;
		int number;
		double Fxx[4];
		double Fxy[4];
	};

	//��������
	struct stfgss {
		struct pointex p[10];		//��߽��ߵĽ���
		int cnt;				//��ĸ���
		int qytks;
		double k, bj;			//kֵ��뾶�����ڻ�ͼ
		MyPoint ptStart, ptEnd, p0;	//���ڻ�ͼ
		struct tksj t[30];
		vector<BlockData> allBlockData;	//��¼���������п�����ݽ��
	}fgss[10000];//fgss��ÿ���Ŀ�����10000��С����100��100��

	int minkid;
	int totfg;
	CDataShow *m_DataDlg;
	double bishop, bph;
	int check1;
public:
	
	MyPoint TransformPoint(const MyPoint& p);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void DoLButtonUp(MyPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool  CSlopeSView::PointHitTest(MyPoint origin, MyPoint point);
	bool  CSlopeSView::XYHitTest(MyPoint origin, MyPoint point);
	bool  CSlopeSView::LineHitTest(MyPoint point);
	int  CSlopeSView::MLHitTest(MyPoint point);
	double CSlopeSView::GetAng(double x, double y);//(x,y)Ϊ�������
	int CSlopeSView::GetBoundPoint(double x, double y);
	int CSlopeSView::checkcolor(MyPoint p, int sx, CDC* pDC);
	void CSlopeSView::FindAllCrossPoint(int n, MyPoint p0, double bj);
	void CSlopeSView::Calculate(int n, int f, MyPoint p0, double bj, CDC* pDC);
	void CSlopeSView::Calculate1(int n, int f, MyPoint p0, double bj, CDC* pDC,int ii);//new method
	void CSlopeSView::SetSXtuceng(CDC* pDC);
	void CSlopeSView::SearchToptcid(int tcid);//���û��������ʱ
	int CSlopeSView::GetTopTC(MyPoint p, CDC* pDC);//��Ի��˲����ߵ�û����
	void CSlopeSView::Write();

	void CSlopeSView::OnFileOpen();
	void CSlopeSView::OnFileSave();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNewM();
	afx_msg BOOL OnMouseWheel(UINT f, short d, CPoint p);
	afx_msg void OnUpdateNewEb(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNewM(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddL(CCmdUI *pCmdUI);
	afx_msg void OnUpdateComt(CCmdUI *pCmdUI);
	afx_msg void OnAddMl();
	afx_msg void OnDelete();
	afx_msg void OnParameter();
	afx_msg void OnCeng1();
	afx_msg void OnCeng2();
	afx_msg void OnCeng3();
	afx_msg void OnCeng4();
	afx_msg void OnCeng5();
	afx_msg void OnCeng6();
	afx_msg void OnCeng7();
	afx_msg void OnCeng8();
	afx_msg void OnCeng9();
	afx_msg void OnCeng10();
	afx_msg void OnAddL();
	afx_msg void OnSearch();
	afx_msg void OnComt();
	afx_msg void OnUsM();
	afx_msg void Onlianxi();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShow();

public:
	int qytks;//ǣ��������Ŀ
	double TempK;//��ǰ��С�İ�ȫϵ��
	int Mqytks;
	CString tkjs;

	/*		int tkfjs[100][100];

			int tkfjtemp;
			int ii;
			int jj;


		public:
			int Drawty5, pZhuang, pFocus2, z_sum, z_show;
			double sd_fs;
			bool GetBool();
			CString GetStr(CString s);
			CString str_1;
			struct zhuang
			{
				CString name;
				double gama, phi, c;
				bool check;
				int clr;
				double l, d;
			}zhuang[20];//׮�Ĳ������趨��

			int pZhuang, pFocus2; //��¼tab2��ʼ����͵�ǰ����

			bool GetBool(int i)
			{
				if (i == 0)return false;
				else return true;
			}

			*/
public:
	double EnData[2000][1000];
	//int FSKcout = 0, FSKnum = 0;

protected:
	DECLARE_MESSAGE_MAP()


//�������
private:
	void calAngle(MyPoint& A, MyPoint& B, MyPoint& C, double* sina = nullptr, double* cosa = nullptr);		//����Ƕ�<ABC>
	void calAngle(MyPoint& A, MyPoint& B, double* sinX = nullptr, double* cosX = nullptr, double* sinY = nullptr, double* cosY = nullptr);	//����AB������x��ļн�<BAx,��y��ļн�<BAy
	void calAngle(CPoint& A, CPoint& B, double* sinX = nullptr, double* cosX = nullptr, double* sinY = nullptr, double* cosY = nullptr);	//����AB������x��ļн�<BAx,��y��ļн�<BAy
	int   Sign(double);
	//����
	double ks;		//���򵯻ɾ���ϵ��N/m
	double kn;		//���򵯻ɾ���ϵ��N/m
public:
	afx_msg void OnTestCalculateangle();
};

#ifndef _DEBUG  // SlopeSView.cpp �еĵ��԰汾
inline CSlopeSDoc* CSlopeSView::GetDocument() const
{
	return reinterpret_cast<CSlopeSDoc*>(m_pDocument);
}
#endif

