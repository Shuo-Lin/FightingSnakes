#ifndef MyConsole
#define MyConsole

#include<Windows.h>
#pragma comment( lib, "winmm.lib" )

class CMyConsole
{
	HANDLE hOut;
	CONSOLE_CURSOR_INFO cursor;
public:
	CMyConsole();
	~CMyConsole();
	void SetTitle(const char *);//设置窗口名 
	void SetSize(short, short);//设置窗口大小
	void SetSize(HANDLE hOut, short x, short y);//+2重载
	void SetCursor(short, short);//设置光标位置
	void HideCursor();//隐藏光标
	void HideCursor(HANDLE hOut);//重载
	void ShowCursor(DWORD dwSize);//显示光标（光标厚度百分比）
	void DrawBox(SMALL_RECT rc, bool bDouble);
	HANDLE GetHandle();
};

/*--------构造函数--------*/
CMyConsole::CMyConsole()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
}
CMyConsole::~CMyConsole()
{
	CloseHandle(hOut);
}

/*-------窗口设置--------*/
void CMyConsole::SetTitle(const char* t)
{
	SetConsoleTitleA(t);
}
void CMyConsole::SetSize(short x, short y)//设置窗口和缓冲区大小，缓冲区>=窗口，同大则没有滚动条
{
	COORD pos = { x, y };
	SMALL_RECT rc = { 0, 0, x - 1, y - 1 };

	SetConsoleWindowInfo(hOut, true, &rc);

	SetConsoleScreenBufferSize(hOut, pos);
}
void CMyConsole::SetSize(HANDLE hOut,short x, short y)//设置窗口和缓冲区大小，缓冲区>=窗口，同大则没有滚动条
{
	COORD pos = { x, y };
	SMALL_RECT rc = { 0, 0, x - 1, y - 1 };

	SetConsoleWindowInfo(hOut, true, &rc);

	SetConsoleScreenBufferSize(hOut, pos);
}

/*-------光标设置-------*/
void CMyConsole::SetCursor(short x, short y)//设置光标位置
{
	//	COORD pos = { x, y };

	SetConsoleCursorPosition(hOut, COORD{ x, y });
}
void  CMyConsole::HideCursor()//隐藏光标
{
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(hOut, &cursor);
}
void  CMyConsole::HideCursor(HANDLE hOut)
{
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(hOut, &cursor);
}
void CMyConsole::ShowCursor(DWORD dwSize = 20)//显示光标
{
	cursor.dwSize = dwSize;
	cursor.bVisible = true;
	SetConsoleCursorInfo(hOut, &cursor);
}

HANDLE CMyConsole::GetHandle()
{
	return hOut;
}


#endif