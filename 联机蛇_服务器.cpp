#include<WinSock2.h>
#include<iostream>
#include<fstream>
#include<string>
#include<thread>
#include"MyConsole.h"
#include"cls.h"
#include"mytimer.h"
#include"FightingNibble.h"
#pragma comment(lib,"WS2_32.LIB")

#define width 50
#define height 25
#define DATASIZE 3850 //110*35

void recvkbhit(SOCKET);
void RefreshScreenData();

WSADATA wsaDATA;
SOCKET ListeningSocket;
SOCKET NewConnection1;
SOCKET NewConnection2;
SOCKADDR_IN ServerAddr;
SOCKADDR_IN ClientAddr;
SOCKADDR_IN ClientAddr2;
int ClientAddrLength = sizeof(SOCKADDR_IN);//这里的赋值，没有赋的话会导致accept函数不阻塞  
int ClientAddrLength2 = sizeof(SOCKADDR_IN);
int Port = 4399;
std::string s_msg = "hahahahaha";
char rbuf[2];
char screenData[DATASIZE];
char presentKey = 0;
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{
	CMyConsole mc;
	mc.SetSize(hOutput, 110, 35);

	/*----------连接客户端---------------*/

	WSAStartup(MAKEWORD(2, 2), &wsaDATA);

	if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		std::cout << "创建监听Socket失败\n";
		std::cout << "Error Code: " << WSAGetLastError();
	}

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((bind(ListeningSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr))) == -1)
	{
		std::cout << "绑定监听Socket失败\n";
		std::cout << "Error Code: " << WSAGetLastError();
	}
	//连接第一个客户端
	std::cout << "Listening...\n";
	if ((listen(ListeningSocket, 1)) == -1)
	{
		std::cout << "监听失败\n";
		std::cout << "Error Code: " << WSAGetLastError();
	}
	std::cout << "Accepting...\n";
	if ((NewConnection1 = accept(ListeningSocket, (sockaddr *)&ClientAddr, &ClientAddrLength)) == -1)
	{
		std::cout << "accept失败\n";
		std::cout << "Error Code: " << WSAGetLastError();
	}
	std::cout << "成功accept " << inet_ntoa(ClientAddr.sin_addr) << " !\n";
	s_msg = "你是不喜欢数分的蛇 (wsad键控制方向,按回车键或者点开始游戏即可开始)";
	send(NewConnection1, s_msg.c_str(), s_msg.length(), 0);

	//连接第二个客户端
	std::cout << "Listening...\n";
	if ((listen(ListeningSocket, 1)) == -1)
	{
	std::cout << "监听失败\n";
	std::cout << "Error Code: " << WSAGetLastError();
	}
	std::cout << "Accepting...\n";
	if ((NewConnection2 = accept(ListeningSocket, (sockaddr *)&ClientAddr2, &ClientAddrLength2)) == -1)
	{
	std::cout << "accept失败\n";
	std::cout << "Error Code: " << WSAGetLastError();
	}
	std::cout << "成功accept " << inet_ntoa(ClientAddr.sin_addr) << " !\n";
	s_msg = "你是会转弯的蛇 (wsad键控制方向,按回车键或者点开始游戏即可开始)";
	send(NewConnection2, s_msg.c_str(), s_msg.length(), 0);

	PlaySoundA("Aspire - CelDro.wav", 0, SND_ASYNC | SND_LOOP);
	printAG("ASCII-SNAKE开始界面.txt", 11);
	//等待两个客户端准备好并同步三方
	recv(NewConnection1, rbuf, 1, 0);
	recv(NewConnection2, rbuf, 1, 0);
	send(NewConnection1, rbuf, 1, 0);
	send(NewConnection2, rbuf, 1, 0);
	//同步完毕，开始游戏
	cls(hOutput);
	mc.HideCursor();
	/* ------IO,游戏部分-------*/
	Board board;
	std::thread t1(recvkbhit, NewConnection1);
	std::thread t2(recvkbhit, NewConnection2);
	t1.detach();
	t2.detach();
	board.run();

	/*-------IO-------*/
	closesocket(NewConnection1);
	closesocket(ListeningSocket);
	std::cout << "程序结束\n";
	WSACleanup();
	Sleep(10000);
	system("pause");
	return 0;
}

void recvkbhit(SOCKET Client)
{
	while (1)
	{
		memset(rbuf, 0, 2);
		if (recv(Client, rbuf, 1, 0) == -1)
		{
			std::cout << "接受失败\n";
			std::cout << "Error Code: " << WSAGetLastError() << "\n";
			Sleep(1000);
			break;
		}
		presentKey = rbuf[0];
	}
}
void RefreshScreenData()
{
	DWORD bytes = 0;
	COORD coord = { 0, 0 };
	ReadConsoleOutputCharacterA(hOutput, screenData, DATASIZE, coord, &bytes);
	if (send(NewConnection1, screenData, DATASIZE, 0) == -1)
	{
		std::cout << "发送失败\n";
		std::cout << "Error Code: " << WSAGetLastError() << "\n";
		Sleep(10000);
	}
	if (send(NewConnection2, screenData, DATASIZE, 0) == -1)
	{
		std::cout << "发送失败\n";
		std::cout << "Error Code: " << WSAGetLastError() << "\n";
		Sleep(10000);
	}
}

