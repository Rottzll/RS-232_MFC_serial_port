
// MFCserial.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMFCserialApp:
// �� Ŭ������ ������ ���ؼ��� MFCserial.cpp�� �����Ͻʽÿ�.
//

class CMFCserialApp : public CWinApp
{
public:
	CMFCserialApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMFCserialApp theApp;