#pragma once
#include "afxwin.h"


// CEditPoint �Ի���

class CEditPoint : public CDialogEx
{
	DECLARE_DYNAMIC(CEditPoint)

public:
	CEditPoint(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditPoint();

	// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangePointEdit();
	CEdit m_EditPoint;
};
