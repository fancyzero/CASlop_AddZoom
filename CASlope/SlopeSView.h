
// SlopeSView.h : CSlopeSView 类的接口
//
#pragma once
#include "SlopeSDoc.h"
#include "DataShow.h"
#include <vector>

#define POINT_COUNT 100  //点数，这里用define将其定义为符号常量是为了方便以后可能的修改，假如我们以后想将点数改为200，则只改此宏定义就可以了。
#define pi 3.14159265

using namespace std;


//自定义数据结构
class MyPoint
{
public:
	double x;
	double y;

	MyPoint() : x(0), y(0) {}
	MyPoint(double x, double y) : x(x), y(y) {}

	MyPoint& operator =(CPoint point)
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

	operator POINT()
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		return pt;
	}
};


//每个块的数据结果
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

	double deltaX;		//形心的位移增量
	double deltaY;
	double deltaTheta;

	double sinA0;		//倾斜面与+X的角度sin(alpha),cos(alpha)
	double cosA0;

	double F1[2];		//1,2,3,4节点的法向和切向受力
	double F2[2];
	double F3[4];
	double F4[4];

	double Fx[4];		//1,2,3,4节点的x,y受力
	double Fy[4];

	double NF_x; //除重力之外的合外力
	double NF_y;

};


class CSlopeSView : public CView
{
protected: // 仅从序列化创建
	CSlopeSView();
	DECLARE_DYNCREATE(CSlopeSView)

	// 特性
public:
	CSlopeSDoc* GetDocument() const;

	// 操作
public:
	CEdit m_EditPoint;
	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// 实现
public:
	virtual ~CSlopeSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
		// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

public:
	afx_msg void OnNewEb();
public:
	int rgbcolor[25];
	bool FirstFocus;
	UINT m_nDrawType;
	CPoint m_nzValues[POINT_COUNT];
	int  m_crt_p;//边界的点数统计
	int m_nLineWidth;
	int m_nLineStyle;
	int m_clrl;
	int m_clrp;
	int m_tid;//材料线的条数id
	int m_tcnt;
	int m_pcrt[POINT_COUNT];//第m_tid条材料线的点数统计
	CPoint m_fjxpoint[POINT_COUNT][POINT_COUNT];//第m_tid条的第m_pcrt点为m_fjxpoint[m_tid][m_pcrt[m_tid]]
	int toptc[POINT_COUNT], buttomtc[POINT_COUNT];//记录每条材料线上下土层id号
	int toptcid;//最上层土层id(针对没画材料线时)
	bool m_tfinish[POINT_COUNT];
	bool m_drawonelinef;
	int mode;
	CPoint xypoint;
	CPoint linepoint;
	CPoint RButtonUp;
	bool xybz;
	bool bihe;
	bool Linebz;
	float m_scale;
	float m_translateX;
	float m_translateY;
	float m_off_x;
	float m_off_y;

	bool m_bDraw[20];//控制菜单开关
	int m_delete_id;//删除材料线id
	int bianjie[5];//边界id

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
		CPoint point;
		int clr;
	}tianchong[300];

	int tc_step;
	CPoint crt_tc;
	int crt_fg;
	CPoint fg[5];
	int jd_fg, jd_yh, tks;

	struct point
	{
		CPoint p;//圆弧与边界线交点的坐标
		double ang;//点的极坐标角,圆心为极坐标原点
		//int nub;//记录这点在哪条边界线上,on line: m_nzValues[nub]~m_nzValues[(nub + 1) % m_crt_p]

		//升序排序时必须写的函数
		bool operator<(const point &rhs) const
		{
			return   ang < rhs.ang;
		}
	};

	//记录条块数据
	struct tksj
	{
		double W, cosa, sina, fai, are, line, cl, alf;
		int number;
		double Fxx[4];
		double Fxy[4];
	};

	//方格搜索
	struct stfgss {
		struct point p[10];		//与边界线的交点
		int cnt;				//点的个数
		int qytks;
		double k, bj;			//k值与半径，用于绘图
		CPoint ptStart, ptEnd, p0;	//用于绘图
		struct tksj t[30];
		vector<BlockData> allBlockData;	//记录最后迭代所有块的数据结果
	}fgss[10000];//fgss存每个的可容纳10000个小方格即100行100列

	int minkid;
	int totfg;
	CDataShow *m_DataDlg;
	double bishop, bph;
	int check1;
public:
	
	CPoint TransformPoint(const CPoint& p);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void DoLButtonUp(CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool  CSlopeSView::PointHitTest(CPoint origin, CPoint point);
	bool  CSlopeSView::XYHitTest(CPoint origin, CPoint point);
	bool  CSlopeSView::LineHitTest(CPoint point);
	int  CSlopeSView::MLHitTest(CPoint point);
	double CSlopeSView::GetAng(double x, double y);//(x,y)为相对坐标
	int CSlopeSView::GetBoundPoint(double x, double y);
	int CSlopeSView::checkcolor(CPoint p, int sx, CDC* pDC);
	void CSlopeSView::FindAllCrossPoint(int n, CPoint p0, double bj);
	void CSlopeSView::Calculate(int n, int f, CPoint p0, double bj, CDC* pDC);
	void CSlopeSView::Calculate1(int n, int f, CPoint p0, double bj, CDC* pDC,int ii);//new method
	void CSlopeSView::SetSXtuceng(CDC* pDC);
	void CSlopeSView::SearchToptcid(int tcid);//针对没画材料线时
	int CSlopeSView::GetTopTC(CPoint p, CDC* pDC);//针对画了材料线但没交点
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
	int qytks;//牵引条块数目
	double TempK;//当前最小的安全系数
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
			}zhuang[20];//桩的参数（设定）

			int pZhuang, pFocus2; //记录tab2起始焦点和当前焦点

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


//添加命令
private:
	void calAngle(MyPoint& A, MyPoint& B, MyPoint& C, double* sina = nullptr, double* cosa = nullptr);		//计算角度<ABC>
	void calAngle(MyPoint& A, MyPoint& B, double* sinX = nullptr, double* cosX = nullptr, double* sinY = nullptr, double* cosY = nullptr);	//计算AB连线与x轴的夹角<BAx,与y轴的夹角<BAy
	void calAngle(CPoint& A, CPoint& B, double* sinX = nullptr, double* cosX = nullptr, double* sinY = nullptr, double* cosY = nullptr);	//计算AB连线与x轴的夹角<BAx,与y轴的夹角<BAy
	int   Sign(double);
	//变量
	double ks;		//切向弹簧劲度系数N/m
	double kn;		//法向弹簧劲度系数N/m
public:
	afx_msg void OnTestCalculateangle();
};

#ifndef _DEBUG  // SlopeSView.cpp 中的调试版本
inline CSlopeSDoc* CSlopeSView::GetDocument() const
{
	return reinterpret_cast<CSlopeSDoc*>(m_pDocument);
}
#endif

