
#include "stdafx.h"
#include "ClientData.h"
#include "iostream"
#include "MD5.h"

ClientData::ClientData()
{
	Usercount = 0;
	UserLists.clear();
	//h_Chat.clear();
}
ClientData::~ClientData()
{
	for(auto chat : h_Chat)
	{
		delete chat.second;
	}
}
int ClientData::Info()
{
	if(Port.IsEmpty())
		return 1;
	if(UserName.IsEmpty())
		return 2;
	if(UserPass.IsEmpty())
		return 3;
	return 0;
}

bool ClientData::ConnectToServer()
{
	WSAData wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		WSACleanup();
		return false;
	}
	SockClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN AddrLink;
	AddrLink.sin_addr.s_addr = inet_addr(ServerIp);
	//MessageBoxExA(NULL, port, "Message", 0, MB_OK);
	AddrLink.sin_port = htons(_ttoi(Port));
	AddrLink.sin_family = AF_INET;

	if(connect(SockClient, (SOCKADDR*)&AddrLink, sizeof(SOCKADDR)))
	{
		return false;
	}
	return true;
}

bool ClientData::Login()
{
	if(!ConnectToServer())
	{
		AfxMessageBox(_T("无法连接服务器"));
		return false;
	}
	//TODO 发送登陆信息
	USES_CONVERSION;
	char SendBuf[1000];

	MD5 md5;
	md5.update(W2A(UserPass.GetBuffer()));
	UserPass = A2W(md5.toString().c_str());

	sprintf_s(SendBuf, "1+%s+%s", W2A(UserName), W2A(UserPass));
	send(SockClient, SendBuf, strlen(SendBuf)+1, 0);
	char RecvBuf[1000];
	recv(SockClient, RecvBuf, 1000, 0);
	if(RecvBuf[0] == '0')
	{
		AfxMessageBox(_T("登陆失败，用户名或者密码不正确"));
		return false;
	}
	return true;
}

bool ClientData::Register()
{
	if(!ConnectToServer())
	{
		AfxMessageBox(_T("无法连接服务器"));
		return false;
	}
	// TODO　发送注册信息
	USES_CONVERSION;
	char SendBuf[1000];

	MD5 md5;
	md5.update(W2A(UserPass.GetBuffer()));
	UserPass = A2W(md5.toString().c_str());

	sprintf_s(SendBuf, "0+%s+%s", W2A(UserName), W2A(UserPass));
	send(SockClient, SendBuf, strlen(SendBuf)+1, 0);
	char RecvBuf[1000];
	recv(SockClient, RecvBuf, 1000, 0);
	if(RecvBuf[0] == '0')
	{
		AfxMessageBox(_T("注册失败，用户名存在"));
		return false;
	}
	AfxMessageBox(_T("注册成功"));
	return true;
}

void ClientData::Exit()
{
	send(SockClient, "exit", 4, 0);
}