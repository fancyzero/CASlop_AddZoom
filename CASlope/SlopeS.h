
// SlopeS.h : SlopeS Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSlopeSApp:
// �йش����ʵ�֣������ SlopeS.cpp
//

class CSlopeSApp : public CWinApp
{
public:
	CSlopeSApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSlopeSApp theApp;
