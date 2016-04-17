// 客户端连接

#pragma once

#include "stdafx.h"
#include "vector"
#include "map"
#include "ChatDlg.h"
#include "memory"

class ClientData
{
public:
	ClientData();
	~ClientData();

public:
	int Usercount;
	SOCKET SockClient;
	char ServerIp[256];
	CString Port;
	CString UserName;
	CString UserPass;

	std::vector<CString> UserLists;
	std::map<CString, CChatDlg*> h_Chat;
	std::map<CString, CString> messageRead;
	std::map<CString, CString> messageLeft;
	std::map<CString, int> messageCount;

	int Info();
	bool ConnectToServer();
	bool Login();
	bool Register();
	void Exit();
};