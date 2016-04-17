//服务器
#include "stdafx.h"
#include "Server.h"
#include "fuwiqiDlg.h"
#include "fuwiqi.h"
#include "Resource.h"
#include "iostream"
#include "map"
#include "string"
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")


extern CfuwiqiApp theApp;
namespace Serverspace
{
	bool Server::OnRun = false;
	
	SOCKET Server::SockServer = NULL;

	Server::Server()
	{
		strcpy_s(IpAddr, "0.0.0.0");
		strcpy_s(Port, "8080");
		UserCount = 0;
		UserLists.clear();
	}
	bool Server::SetIpAddr()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		char Name[256] = {0};
		if(!gethostname(Name, 256))
		{
			hostent *hostinfo;
			if(hostinfo = gethostbyname(Name))
			{
				strcpy_s(IpAddr, inet_ntoa( *(in_addr*) hostinfo-> h_addr));
				return true;
			}
		}
		WSACleanup();
		return true;
	}
	Server::~Server() {} ;
	bool Server::Startup()
	{
		if(!SetIpAddr())
		{
			MessageBoxExA(NULL, "启动失败,不能获取本机IP地址", "Message", 0, MB_OK);
			return false;
		}
		// TODO 启动网络服务
		//MessageBoxExA(NULL, "启动成功", "Message", 0, MB_OK);

		WSADATA WsaData;
		if(WSAStartup(MAKEWORD(2, 2), &WsaData))
		{
			OnRun = false;
			return OnRun;
		}

		SockServer = socket(AF_INET, SOCK_STREAM, 0);	
		SOCKADDR_IN AddrServer;

		AddrServer.sin_addr.s_addr = inet_addr(IpAddr);
		AddrServer.sin_family = AF_INET;
		AddrServer.sin_port = htons(8080);

		if(bind(SockServer, (SOCKADDR*)&AddrServer, sizeof(SOCKADDR)))
		{
			OnRun = false;
			return OnRun;
		}

		OnRun = true;
		return OnRun;
	}
	void Server::Stop()
	{
		//MessageBoxExA(NULL, "停止成功", "Message", 0, MB_OK);
		closesocket(SockServer);
		OnRun = false;
		WSACleanup();
	}
	DWORD WINAPI Server::Listen(LPVOID lpPara)
	{
		SOCKADDR_IN AddrClient; 
		int len = sizeof(SOCKADDR);
		listen(SockServer, 10);

		while(OnRun)
		{
			SOCKET SockConn = accept(SockServer, (SOCKADDR*)&AddrClient, &len);
			if(SockConn != INVALID_SOCKET)
			{
				//std::cout << inet_ntoa(AddrClient.sin_addr) << std::endl;
				HANDLE hThread;
				hThread = CreateThread(NULL, 0, Solve, (LPVOID)SockConn, 0, NULL);
			}
		}
		return 0;
	}
	int Server::GetUserCount()
	{
		return UserCount;
	}
}



namespace Serverspace
{
	Server *server = NULL;
	HANDLE hThread;
	bool StartServer()
	{
		if(server)
			delete server;
		server = new Server();
		if(!server)
		{
			//MessageBoxExA(NULL, "启动失败,内存申请失败", "Message", 0, MB_OK);
			return false;
		}
		//std::cout << "ok" << std::endl;
		if(!server -> Startup())
			return false;
		//std::cout << "ok" << std::endl;
		hThread = CreateThread(NULL, 0, Server::Listen, NULL, 0, NULL);
		return true;
	}
	void StopServer()
	{
		TerminateThread(hThread, 0);
		CloseHandle(hThread);
		server -> Stop();
		delete server;
		server = NULL;
	}
	bool GetServerStaues()
	{
		return server!=NULL;
	}
	int GetUserCount()
	{
		if(server)
		{
			return server -> GetUserCount();
		}
		return 0;
	}
	DWORD WINAPI Solve(LPVOID lpPara)
	{
		char myName[1000];
		SOCKET SockConn = (SOCKET)lpPara;
		char RecvBuf[1000];
		char SendBuf[1000];
		while(true)
		{
			memset(RecvBuf ,0, sizeof RecvBuf);
			memset(SendBuf, 0, sizeof SendBuf);
			int ret = recv(SockConn, RecvBuf, 1000, 0);
			std::cout << ret << std::endl;
			if(ret == -1)
			{
				closesocket(SockConn);
				return 0;
			}
			std::cout << "Receive:" <<  RecvBuf << std::endl;

			if(!strcmp(RecvBuf, "exit"))
			{
				server -> UserLists.erase(myName);
				server -> UserCount--;

				CListBox *m_box = (CListBox*)(theApp.m_pMainWnd -> GetDlgItem(IDC_LIST2));
				int index = m_box -> FindString(0, CString(myName));
				m_box -> DeleteString(index);

				USES_CONVERSION;
				CString temp;
				temp.Format(_T("%s 退出聊天室"), A2W(myName));
				((CfuwiqiDlg*)theApp.m_pMainWnd) -> UpdataEvent(temp);

				sprintf_s(SendBuf, "0+%s", myName);
				for(auto User = server -> UserLists.begin(); User != server -> UserLists.end(); ++User)
				{
					send(User -> second, SendBuf, strlen(SendBuf) + 1, 0);
				}

				closesocket(SockConn);
				return 0;
			}


			char Name[1000] = {0}, Pass[1000] = {0};
			char *Message = Pass;
			int i = 2, j = 0;
			for(j = 0; RecvBuf[i]; ++i, ++j)
			{
				if(RecvBuf[i] == '+')
				{
					Name[j] = 0;
					break;
				}
				Name[j] = RecvBuf[i];
			}
			for(++i, j = 0; RecvBuf[i]; ++i, ++j)
			{
				Pass[j] = RecvBuf[i];
			}
			Pass[j] = 0;
			std::cout << Name << " " << Pass << std::endl;
			if(RecvBuf[0] == '0' || RecvBuf[0] == '1')
			{
				// 注册 登陆
				MYSQL *mysql;
				mysql = mysql_init(NULL);
				if(mysql_real_connect(mysql, "127.0.0.1", "root", "vagrant", "data", 0, NULL, 0))
				{
					std::cout << "连接数据库成功" <<std::endl;
					char query[1000];
					if(RecvBuf[0] == '0')
					{
						sprintf_s(query, "select * from user where name=\"%s\"", Name);
						if(mysql_query(mysql, query))
						{
							std::cout << "查询失败" << std::endl;
							std::cout << mysql_errno(mysql) << ":" << mysql_error(mysql) << std::endl;
							send(SockConn, "0", 1, 0);

							closesocket(SockConn);
							return 0;
						}
						MYSQL_RES *result = mysql_store_result(mysql);
						if(!mysql_num_rows(result))
						{
							std::cout << "注册成功" << std::endl;
							sprintf_s(query, "insert into user(name,pass) values(\"%s\",\"%s\")", Name, Pass);
							mysql_query(mysql, query);
							send(SockConn, "1", 1, 0);

							USES_CONVERSION;
							CString temp;
							temp.Format(_T("%s 注册成功"), A2W(Name));
							((CfuwiqiDlg*)theApp.m_pMainWnd) -> UpdataEvent(temp);

							closesocket(SockConn);
							return 0;
						}
						else
						{
							std::cout << "注册失败" << std::endl;
							send(SockConn, "0", 1, 0);

							closesocket(SockConn);
							return 0;
						}

					}
					else if(RecvBuf[0] == '1')
					{
						sprintf_s(query, "select * from user where name=\"%s\" and pass=\"%s\"", Name, Pass);
						if(mysql_query(mysql, query))
						{
							std::cout << "登陆失败" << std::endl;
							std::cout << mysql_errno(mysql) << ":" << mysql_error(mysql) << std::endl;
							send(SockConn, "0", 1, 0);

							closesocket(SockConn);
							return 0;
						}
						MYSQL_RES *result = mysql_store_result(mysql);
						if(mysql_num_rows(result))
						{
							std::cout << "登陆成功" << std::endl;

							send(SockConn, "1", 1, 0);
							sprintf_s(SendBuf, "1+%s", Name);
							for(auto User = server -> UserLists.begin(); User != server -> UserLists.end(); ++User)
							{
								send(User -> second, SendBuf, strlen(SendBuf) + 1, 0);
							}
							for(auto User = server -> UserLists.begin(); User != server -> UserLists.end(); ++User)
							{
								USES_CONVERSION;
								sprintf_s(SendBuf, "1+%s", User -> first.c_str());
								std::cout << User->first << std::endl;
								int len =send(SockConn, SendBuf, strlen(SendBuf) + 1, 0);
								std::cout << len << std::endl;
								Sleep(500);
							}
							strcpy_s(myName, Name);
							server -> UserLists[Name] = SockConn;
							server -> UserCount++;

							CListBox *m_box = (CListBox*)(theApp.m_pMainWnd -> GetDlgItem(IDC_LIST2));
							m_box -> AddString(CString(myName));

							CString temp;
							USES_CONVERSION;
							temp.Format(_T("%s 登陆服务器"), A2W(Name));
							((CfuwiqiDlg*)theApp.m_pMainWnd) -> UpdataEvent(temp);
						}
						else
						{
							std::cout << "登陆失败" << std::endl;
							send(SockConn, "0", 1, 0);

							closesocket(SockConn);
							return 0;
						}
					}
				}
				else
				{
					std::cout << "连接服务器失败" << std::endl;
					std::cout << mysql_errno(mysql) << ":" << mysql_error(mysql) << std::endl;
					send(SockConn, "0", 1, 0);

					closesocket(SockConn);
					return 0;
				}
			}
			else if(RecvBuf[0] == '2')
			{
				// 发送群消息
				std::map<std::string,SOCKET>::iterator User;
				for( User = server -> UserLists.begin(); User != server -> UserLists.end(); ++User)
				{
					send(User -> second, RecvBuf, strlen(RecvBuf) + 1, 0);
				}

				USES_CONVERSION;
				CString temp;
				temp.Format(_T("%s 对说有人说: %s"), A2W(Name), A2W(Message));
				((CfuwiqiDlg*)theApp.m_pMainWnd) -> UpdataEvent(temp);
			}
			else if(RecvBuf[0] == '3')
			{
				// 发送私信
				auto User = server -> UserLists.find(Name);
				sprintf_s(SendBuf, "3+%s+%s", myName, Message);
				send(User -> second, SendBuf, strlen(SendBuf) + 1, 0);

				USES_CONVERSION;
				CString temp;
				temp.Format(_T("%s 对说 %s 说: %s"), A2W(myName), A2W(Name), A2W(Message));
				((CfuwiqiDlg*)theApp.m_pMainWnd) -> UpdataEvent(temp);
			}
		}
		return 0;
	}

}
