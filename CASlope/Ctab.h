#pragma once
#include "afxcmn.h"
#include "Parameter.h"
#define TUCENG_COUNT 10
// Ctab 对话框

class Ctab : public CDialogEx
{
	DECLARE_DYNAMIC(Ctab)

public:
	Ctab(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ctab();

	// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	int pTuCeng, pFocus;
	CParameter m_tuceng[100];//子窗口
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};