#include<WinSock2.h>
#include<string>
#include<iostream>
#include<conio.h>
#include<thread>
#include"MyConsole.h"
#include"cls.h"
#include"printAsciiGraph.h"
#pragma comment(lib,"WS2_32.LIB")
using namespace std;

#define DATASIZE 3850 

WSADATA wsaDATA;
SOCKET s;
SOCKADDR_IN ServerAddr;
string IP_Server = "172.20.10.2";
int Port = 4399;

CMyConsole mc;
char screenData[DATASIZE];
void recvScreenData();
void sendkbhit();

int main()
{

	mc.SetSize(110, 35);
	mc.HideCursor();

	WSAStartup(MAKEWORD(2, 2), &wsaDATA);
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cout << "创建socket失败\n";
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.s_addr = inet_addr(IP_Server.c_str());

	cout << "Connecting...";
	if ((connect(s, (sockaddr *)&ServerAddr, sizeof(ServerAddr))) == -1)
	{
		cout << "\nfailed to connect\n";
		cout << "Error Code: " << WSAGetLastError();
		system("pause");
		exit(0);
	}
	cout << "\n成功connect IP: " << inet_ntoa(ServerAddr.sin_addr) << " !\n";
	Sleep(1000);

	printAG("ASCII-SNAKE开始界面.txt", 11);
	PlaySoundA("Aspire - CelDro.wav", 0, SND_ASYNC | SND_LOOP);
	char msg[100];
	memset(msg, 0, 100);
	recv(s, msg, 100, 0);
	mc.SetCursor(20, 28);
	cout << msg;
	mc.SetCursor(47, 30);
	SetConsoleTextAttribute(mc.GetHandle(), 252);
	cout << "开始游戏";
	SetConsoleTextAttribute(mc.GetHandle(), 7);

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD record;
	DWORD  res;
	COORD  crPos, crHome = { 0, 0 };
	while (1)
	{
		ReadConsoleInputA(hIn, &record, 1, &res);
		if (record.EventType == MOUSE_EVENT)
		{
			if (record.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				crPos = record.Event.MouseEvent.dwMousePosition;
				if (crPos.X >= 47 && crPos.X <= 54 && crPos.Y == 30)
					break;
			}
		}
		else if (record.EventType == KEY_EVENT)
		{
			if (record.Event.KeyEvent.bKeyDown)
			{
				switch (record.Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_RETURN: goto here; break;
				default:
					break;
				}
			}
		}
	}
	//同步
here:
	mc.SetCursor(47, 30);
	SetConsoleTextAttribute(mc.GetHandle(), 252);
	cout << "准备启动";
	SetConsoleTextAttribute(mc.GetHandle(), 7);
    send(s, "0", 1, 0);
	char rbuf[2];
	recv(s, rbuf, 1, 0);
	/*-----------IO------------*/
	thread t1(recvScreenData);
	thread t2(sendkbhit);
	t1.join();
	t2.join();

	/*---------IO-------------*/
	cout << "程序结束\n";
	WSACleanup();
	Sleep(10000);
	system("pause");
	return 0;
}
void recvScreenData()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord{ 0, 0 };
	DWORD bytes;
	while (1)
	{
		if (recv(s, screenData, DATASIZE, 0) != -1)
		{
			WriteConsoleOutputCharacterA(hOut, screenData, DATASIZE, coord, &bytes);
		}
		else
		{
			PlaySoundA("死亡配音.wav", 0, SND_ASYNC);
			break;
		}
	}
}
void sendkbhit()
{
	char k[2];
	k[1] = '\n';
	while (1)
	{
		if (_kbhit())
		{
			k[0] = _getch();
			switch (k[0])
			{
			case 'w':send(s, k, 1, 0); break;
			case 's':send(s, k, 1, 0); break;
			case 'a':send(s, k, 1, 0); break;
			case 'd':send(s, k, 1, 0); break;
			default:
				break;
			}
		}
	}
}