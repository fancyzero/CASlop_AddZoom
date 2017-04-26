#pragma once
#include "afxwin.h"
#include "Resource.h"

// CDataShow 对话框

class CDataShow : public CDialogEx
{
	DECLARE_DYNAMIC(CDataShow)

public:
	CDataShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataShow();

	// 对话框数据
	enum { IDD = IDD_DataDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_text;
	int tks, tkid;
	void *m_pMain;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};