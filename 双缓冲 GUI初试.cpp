#include <iostream>
#include <Windows.h>
using namespace std;
//#include"cls.h"


int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << GetLastError() << endl;

	short x = 50, y = 50;
	COORD pos = { x, y };
	SMALL_RECT rc = { 0, 0, x - 1, y - 1 };
	cout << GetLastError() << endl;

	SetConsoleScreenBufferSize(hOut, pos);
	cout << GetLastError()<<endl;
	SetConsoleWindowInfo(hOut, true, &rc);
	cout << GetLastError();
	return 0;
}

/*
//标准显示缓冲区的句柄
HANDLE hOutput;
COORD coord = { 0, 0 };
hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

//创建新的缓冲区
HANDLE hOutBuf = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

//设置新的缓冲区为ActiveScreenBuffer
SetConsoleActiveScreenBuffer(hOutBuf);

//隐藏两个缓冲区的光标
CONSOLE_CURSOR_INFO cursorInfo;
cursorInfo.bVisible = false;
cursorInfo.dwSize = 10;
SetConsoleCursorInfo(hOutBuf, &cursorInfo);
SetConsoleCursorInfo(hOutput, &cursorInfo);

//设置缓冲区大小

//-----**------双缓冲处理显示------**------*

while (1)
{
	DWORD bytes = 0;
	char data[450];
	for (char c = 'a'; c<'z'; c++)
	{
		cls(hOutput);
		for (int i = 0; i<20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				printf("%c", c);
			}
		}
		ReadConsoleOutputCharacterA(hOutput, data, 450, coord, &bytes);
		cls(hOutBuf);
		WriteConsoleOutputCharacterA(hOutBuf, data, 450, coord, &bytes);

		Sleep(300);
	}
}

*/

/*int main()
{	
	MessageBoxA(NULL, "Hello, Windows!", "Message BOX!", MB_ABORTRETRYIGNORE | MB_ICONHAND);
	MessageBoxA(NULL, "哈哈哈哈哈哈哈", "Messgage Box", MB_YESNO | MB_ICONQUESTION);
	MessageBoxA(NULL, "哈哈哈哈333333", "MB", MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION);
	MessageBoxA(NULL, "Asterisk!", "MB", MB_ICONASTERISK);

	Sleep(10000);
	return 0;
	}*/