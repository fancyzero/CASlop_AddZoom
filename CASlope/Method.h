#pragma once
#include "afxwin.h"


// CMethod �Ի���

class CMethod : public CDialogEx
{
	DECLARE_DYNAMIC(CMethod)

public:
	CMethod(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMethod();

	// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
