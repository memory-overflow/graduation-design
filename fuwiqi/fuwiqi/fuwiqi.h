
// fuwiqi.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfuwiqiApp:
// �йش����ʵ�֣������ fuwiqi.cpp
//

class CfuwiqiApp : public CWinApp
{
public:
	CfuwiqiApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfuwiqiApp theApp;