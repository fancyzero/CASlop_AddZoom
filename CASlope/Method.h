#pragma once
#include "afxwin.h"


// CMethod 对话框

class CMethod : public CDialogEx
{
	DECLARE_DYNAMIC(CMethod)

public:
	CMethod(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMethod();

	// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	int mode;
	CComboBox m_comboWeb;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeWebCombo();
	afx_msg void OnBnClickedCancel();

public:
	double ks;
	double kn;
};
