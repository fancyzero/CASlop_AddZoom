#pragma once
#include "afxcmn.h"
#include "Parameter.h"
#define TUCENG_COUNT 10
// Ctab �Ի���

class Ctab : public CDialogEx
{
	DECLARE_DYNAMIC(Ctab)

public:
	Ctab(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ctab();

	// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	int pTuCeng, pFocus;
	CParameter m_tuceng[100];//�Ӵ���
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};