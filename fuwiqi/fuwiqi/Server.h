//Server¿‡∂®“Â

#pragma once

#include "map"
#include "string"

namespace Serverspace
{
	class Server
	{
	public:
		static SOCKET SockServer;
		static bool OnRun;
		std::map<std::string, SOCKET> UserLists;
		int UserCount;
	public:
		Server();
		~Server();
		bool SetIpAddr();
		bool Startup();
		void Stop();
		int GetUserCount();
		static DWORD WINAPI Listen(LPVOID lpPara);
	private:
		char IpAddr[20];
		char Port[10];
	};
	bool StartServer();
	void StopServer();
	bool GetServerStaues();
	int GetUserCount();
	DWORD WINAPI Solve(LPVOID lpParar);
}