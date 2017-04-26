#pragma once
#include "afxwin.h"

// CParameter �Ի���

class CParameter : public CDialogEx
{
	DECLARE_DYNAMIC(CParameter)

public:
	CParameter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParameter();

	// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);//2.������Ϣ��Ӧ����
	DECLARE_MESSAGE_MAP()
public:
	int clr;
	CString name;
	double gama;
	double c;
	double phi;
	time_t t;
	afx_msg void OnBnClickedButton1();
	CStatic m_picColor;
	CWinThread* pThread;
	static UINT ClientThread(LPVOID lpParm);
	afx_msg void OnPaint();
	void CParameter::OnUpdateData();
	afx_msg void OnEnChangeEdit3();
};
