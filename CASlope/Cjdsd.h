
#pragma once
#include "afxwin.h"


// Cjdsd �Ի���

class Cjdsd : public CDialogEx
{
	DECLARE_DYNAMIC(Cjdsd)

public:
	Cjdsd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Cjdsd();

	// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int bianchang;
	int banjing;
	int tiaokuaishu;
	int check1;
	CButton m_check1;
	CButton m_check2;
	CButton m_check3;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
