#pragma once
#include "afxwin.h"


// CEditPoint 对话框

class CEditPoint : public CDialogEx
{
	DECLARE_DYNAMIC(CEditPoint)

public:
	CEditPoint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditPoint();

	// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangePointEdit();
	CEdit m_EditPoint;
};
