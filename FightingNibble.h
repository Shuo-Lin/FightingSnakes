#include<iostream>
#include<conio.h>
#include<thread>
#include<string>

#include"MyConsole.h"
#include"mytimer.h"
#include"printAsciiGraph.h"

#define width 50
#define height 25
#define BornLength 6
//#define APPLES 10
#define ShiftX 4 //打印位置对抽象位置的偏移
#define ShiftY 1
#define SNAKES 2

enum Direction{ UP, RIGHT, DOWN, LEFT };
enum Situ{ Nothing, Snakeon, Appleon, Barriaon };//网格上各点情况

extern char presentKey;
extern void RefreshScreenData();

class Snake
{
	std::string name;
	friend class Board;
	COORD head;//X是c，Y是r
	COORD all[width*height];
	int length;
	Direction direction;
	Direction nextdirec;
public:
	Snake( Direction d = RIGHT);//需要头的位置，方向
	void complete();
};


class Board
{
	Situ a[height][width];
	Snake snake[SNAKES];
	CMyConsole myconsole;
	HANDLE hOut;
	double SPEED;
	unsigned int APPLES;
	
public:
	Board();
	void load(const Snake &snake);//把snake加载到主面板上（非打印）
	void deload(const Snake &snake);//反上
	void printsnake(const Snake &snake);
	void cleansnake();
	void printlength();

	/*两个单点打印清除函数的声明*///传个原坐标进去就行，自动把X*2
	void prhpoint(COORD h);
	void prApoint(COORD A);
	void prapoint(COORD a);
	void prpoint(COORD p);
	void clpoint(COORD p);

	void move_print();
	void kboperate();
	void kboperate0(Snake &snake);
	void kboperate1(Snake &snake);
	Situ check(const Snake &snake);
	void bordercheck(const Snake &snake);

	void headup(Snake &snake);

	void singlerun();
	void singlerun0(Snake &snake);
	void singlerun1(Snake &snake);
	void run();
	void gameover();
	void newapple();

	
	void drawborder();

};

/*------Snake类的实现--------*/
Snake::Snake(Direction d )
{
	direction = d;
	length = BornLength;

	srand((int)time(0));
	switch (d)
	{
	case UP:
		head.X = rand() % (width / 2-1);//0~24
		head.Y = height - length;
		break;
	case RIGHT:
		head.X = length;
		//head.X = rand() % (width-10 - length+1) + length-1;
		head.Y = rand() % (height / 2-1)+1;//0~12
		break;
	case DOWN:
		head.X = rand() % (width / 2) + width / 2 ;
		head.Y = length+ShiftY;
		break;
	case LEFT:
		head.X = width-length-1;
		//		head.X = rand() % (width - length + 1-10)+10;
		head.Y = rand() % (height / 2-1)+1 + height / 2;
		break;
	}

	for (int i = 0; i < length; i++)
	{
		switch (d)
		{
		case UP:
			all[i].X = head.X;
			all[i].Y = head.Y + i;
			break;
		case RIGHT:
			all[i].X = head.X - i;
			all[i].Y = head.Y;
			break;
		case DOWN:
			all[i].X = head.X;
			all[i].Y = head.Y - i;
			break;
		case LEFT:
			all[i].X = head.X + i;
			all[i].Y = head.Y;
			break;
		}	
	}
}
//改变head后调用
void Snake::complete()
{
	for (int i = length; i > 0; i--)
	{
		all[i] = all[i - 1];
	}
	all[0] = head;
}

/*-----Board类的实现---------*/
Board::Board()
{
	SPEED = 0.3;
	APPLES = 3;
	
	hOut = myconsole.GetHandle();
	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	{
		a[i][j] = Nothing;
	}

	
	drawborder();
	snake[0] = Snake(RIGHT);

	/*-----------玩家自定义-----------*/
	snake[0].name="不喜欢数分的蛇";
	snake[1] = Snake(LEFT);
	snake[1].name="会转弯的蛇";

	/*
	for (int i = 0; i < SNAKES; i++)
	{
		myconsole.SetCursor(width / 2, height + 5+i);
		cout << "设置" << snake[i].name << " 的长度 （2~30）";
		cin >> snake[i].length;
		if (snake[i].length < 2)
		{
			snake[i].length = 2;
			myconsole.SetCursor(width / 2, height + 5 + i);
			printf("                                                                ");
			myconsole.SetCursor(width / 2, height + 5 + i);
			cout << snake[i].name << " 的长度：" << snake[i].length;
		}	
		else if (snake[i].length>30)
		{
			snake[i].length = 30;
			myconsole.SetCursor(width / 2, height + 5 + i);
			printf("                                                                   ");
			myconsole.SetCursor(width / 2, height + 5 + i);
			cout << snake[i].name << " 的长度：" << snake[i].length;
		}		
	}*/
	Sleep(300);
	
	/*-----------玩家自定义-----------*/
	myconsole.HideCursor();

	system("cls");
	drawborder();
	for (int i = 0; i < SNAKES; i++)
	{
		load(snake[i]);
		printsnake(snake[i]);
	}



	for (int i = 0; i < APPLES; i++)//放苹果
		newapple();
	
	//画边框和信息


	printlength();
	RefreshScreenData();
}

void Board::load(const Snake &snake)
{
	for (int i = 0; i < snake.length; i++)
		a[snake.all[i].Y][snake.all[i].X] = Snakeon;
}
void Board::deload(const Snake &snake)
{
	for (int i = 0; i < snake.length; i++)
		a[snake.all[i].Y][snake.all[i].X] = Nothing;
}
void Board::printsnake(const Snake &snake)
{
	myconsole.SetCursor(snake.head.X * 2 + ShiftX, snake.head.Y + ShiftY);
	printf("□");
	myconsole.SetCursor(snake.head.X * 2 + ShiftX, snake.head.Y + ShiftY-1);
	
	
	std::cout << "我是 " << snake.name;
	
	for (int i = 1; i < snake.length; i++)
	{
		myconsole.SetCursor(snake.all[i].X * 2 + ShiftX, snake.all[i].Y + ShiftY);
		printf("■");
	}
}
void Board::printlength()
{
	for (int i = 0; i < SNAKES; i++)
	{
		myconsole.SetCursor(ShiftX, height + ShiftY + 2 + i);
		std::cout << snake[i].name;
		printf(" 的长度： %d \n",  snake[i].length);
	}
}

void Board::newapple()
{
	int r, c;
	srand((unsigned int)time(0));
	for (int i = 0; i < 1;)//放苹果
	{
		r = rand() % height;
		c = rand() % width;
		if (a[r][c] == Nothing)
		{
			a[r][c] = Appleon;
			i++;
		}
	}
	prApoint(COORD{ c, r });
}

/*------单点打印清除--------*/
void Board::prhpoint(COORD p)
{
	myconsole.SetCursor(p.X * 2 + ShiftX, p.Y + ShiftY);
	printf("□");
	a[p.Y][p.X] = Snakeon;
}
void Board::prapoint(COORD p)
{
	myconsole.SetCursor(p.X * 2 + ShiftX, p.Y + ShiftY);
	printf("█");
	a[p.Y][p.X] = Snakeon;
}
void Board::prApoint(COORD p)
{
	SetConsoleTextAttribute(hOut, 12);
	myconsole.SetCursor(p.X * 2 + ShiftX, p.Y + ShiftY);
	printf("●");
	drawborder();
	a[p.Y][p.X] = Appleon;
	SetConsoleTextAttribute(hOut, 7);
}
void Board::prpoint(COORD p)
{
	myconsole.SetCursor(p.X * 2 + ShiftX, p.Y + ShiftY);
	printf("■");
	a[p.Y][p.X] = Snakeon;
}
void Board::clpoint(COORD p)
{
	myconsole.SetCursor(p.X * 2 + ShiftX, p.Y + ShiftY);
	printf("  ");
	a[p.Y][p.X] = Nothing;
}

void Board::drawborder()
{
	myconsole.SetCursor(ShiftX, 0);
	for (int i = 0; i < width; i++)
	{
		printf("▁");
	}

	for (int i = ShiftY; i < height + ShiftY; i++)
	{
		myconsole.SetCursor(2, i);
		printf("▕");
		myconsole.SetCursor(width * 2 + ShiftX, i);
		printf("▏");
	}

	myconsole.SetCursor(ShiftX, height + ShiftY);
	for (int i = 0; i < width; i++)
	{
		printf("▔");
	}
	SetConsoleTextAttribute(hOut, 7);
}


void Board::gameover()
{
	PlaySoundA("死亡配音.wav", 0, SND_ASYNC);
	myconsole.SetCursor(width - 1, height / 2 + ShiftY);
	SetConsoleTextAttribute(myconsole.GetHandle(), 252);
	printf(" Gameover! ");
	RefreshScreenData();
	extern SOCKET ListeningSocket;
	closesocket(ListeningSocket);
	Sleep(10000);
	_getch();
	exit(0);
}

Situ Board::check(const Snake &snake)
{
	return a[snake.head.Y][snake.head.X];
}
void Board::bordercheck(const Snake &snake)
{
	if (snake.head.X < 0 || snake.head.Y < 0 || snake.head.X >= width || snake.head.Y >= height)
	{
		myconsole.SetCursor(width -3, height / 2 + ShiftY-1);
		SetConsoleTextAttribute(hOut,252);
		std::cout << "   " << snake.name << "died   ";
		gameover();
	}
}
/********************/
void Board::headup (Snake &snake)
{
	switch (snake.direction)
	{
	case UP:
		snake.head.Y--;
		break;
	case RIGHT:
		snake.head.X++;
		break;
	case DOWN:
		snake.head.Y++;
		break;
	case LEFT:
		snake.head.X--;
		break;
	}
	bordercheck(snake);
	clpoint(snake.all[snake.length - 1]);
	switch (check(snake))
	{
	case Nothing:
		clpoint(snake.all[snake.length - 1]);
		prhpoint(snake.head);
		prpoint(snake.all[0]);
		break;
	case Snakeon:
		myconsole.SetCursor(width - 1, height / 2 + ShiftY - 1);
		SetConsoleTextAttribute(hOut, 252);
		std::cout << "   " << snake.name << "died   ";
		gameover();
		break;
	case Appleon:
	//	PlaySoundA("snd_吃苹果.wav", 0, SND_ASYNC);
		snake.length++;
		printlength();
		prapoint(snake.head);
		prapoint(snake.all[0]);
		newapple();
		break;
	case Barriaon:
		myconsole.SetCursor(width - 1, height / 2 + ShiftY - 1);
		SetConsoleTextAttribute(hOut, 252);
		std::cout << "   " << snake.name << "died   ";
		gameover();

		break;
	default:
		break;
	}

	snake.complete();
	RefreshScreenData();
}

void Board::kboperate()
{
	Direction possidirec[SNAKES];
	for (int i = 0; i < SNAKES; i++)
	{
		possidirec[i] = snake[i].nextdirec;//建立方向缓冲，试图解决高速操作带来的bug
	}
	
	
	/*
	int k = _getch();
	if (k == 224)
	{
		switch (_getch())

		{
		case 72://VK_UP
			switch (possidirec[0])
			{
			case UP:
				headup(snake[0]);
				break;
			case RIGHT:
				possidirec[0] = UP;
				break;
			case DOWN:
				break;
			case LEFT:
				possidirec[0] = UP;
				break;
			}
			break;
		case 80://VK_DOWN
			switch (possidirec[0])
			{
			case UP:
				break;
			case RIGHT:
				possidirec[0] = DOWN;
				break;
			case DOWN:
				headup(snake[0]);
				break;
			case LEFT:
				possidirec[0] = DOWN;
				break;
			default:
				break;
			}
			break;
		case 75://VK_LEFT
			switch (possidirec[0])
			{
			case UP:
				possidirec[0] = LEFT;
				break;
			case RIGHT:
				break;
			case DOWN:
				possidirec[0] = LEFT;
				break;
			case LEFT:
				headup(snake[0]);
				break;
			}
			break;
		case 77://VK_RGIHT
			switch (possidirec[0])
			{
			case UP:
				possidirec[0] = RIGHT;
				break;
			case RIGHT:
				headup(snake[0]);
				break;
			case DOWN:
				possidirec[0] = RIGHT;
				break;
			case LEFT:
				break;
			}
			break;
		}
	}
	else*/
	switch (presentKey)
	{
		/*-------第一个操作盘-------*/
	case 'w'://VK_UP
		switch (possidirec[0])
		{
		case UP:
			headup(snake[0]);
			break;
		case RIGHT:
			possidirec[0] = UP;
			break;
		case DOWN:
			break;
		case LEFT:
			possidirec[0] = UP;
			break;
		}
		break;
	case 's'://VK_DOWN
		switch (possidirec[0])
		{
		case UP:
			break;
		case RIGHT:
			possidirec[0] = DOWN;
			break;
		case DOWN:
			headup(snake[0]);
			break;
		case LEFT:
			possidirec[0] = DOWN;
			break;
		default:
			break;
		}
		break;
	case 'a'://VK_LEFT
		switch (possidirec[0])
		{
		case UP:
			possidirec[0] = LEFT;
			break;
		case RIGHT:
			break;
		case DOWN:
			possidirec[0] = LEFT;
			break;
		case LEFT:
			headup(snake[0]);
			break;
		}
		break;
	case 'd'://VK_RGIHT
		switch (possidirec[0])
		{
		case UP:
			possidirec[0] = RIGHT;
			break;
		case RIGHT:
			headup(snake[0]);
			break;
		case DOWN:
			possidirec[0] = RIGHT;
			break;
		case LEFT:
			break;
		}
		break;
		/*-----------第2个操作盘-------*/
	case 'i'://VK_UP
		switch (possidirec[1])
		{
		case UP:
			headup(snake[1]);
			break;
		case RIGHT:
			possidirec[1] = UP;
			break;
		case DOWN:
			break;
		case LEFT:
			possidirec[1] = UP;
			break;
		}
		break;
	case 'k'://VK_DOWN
		switch (possidirec[1])
		{
		case UP:
			break;
		case RIGHT:
			possidirec[1] = DOWN;
			break;
		case DOWN:
			headup(snake[1]);
			break;
		case LEFT:
			possidirec[1] = DOWN;
			break;
		default:
			break;
		}
		break;
	case 'j'://VK_LEFT
		switch (possidirec[1])
		{
		case UP:
			possidirec[1] = LEFT;
			break;
		case RIGHT:
			break;
		case DOWN:
			possidirec[1] = LEFT;
			break;
		case LEFT:
			headup(snake[1]);
			break;
		}
		break;
	case 'l'://VK_RGIHT
		switch (possidirec[1])
		{
		case UP:
			possidirec[1] = RIGHT;
			break;
		case RIGHT:
			headup(snake[1]);
			break;
		case DOWN:
			possidirec[1] = RIGHT;
			break;
		case LEFT:
			break;
		}
		break;

	case 27://ESC键	
		myconsole.SetCursor(ShiftX, height + 5);
		printf("Pause");
		system("pause");
		break;
	}
	presentKey = 0;

	//缓冲方向防止与蛇反向
	for (int i = 0; i < SNAKES; i++)
	{
		if ((possidirec[i] + 6) % 4 != snake[i].direction)//如果不反向
		{
			snake[i].nextdirec = possidirec[i];
		}
	}	
}
void Board::kboperate0(Snake &snake)
{
	Direction possidirec = snake.nextdirec;//建立方向缓冲，试图解决高速操作带来的bug
	int k = _getch();
	if (k == 224)
	{
		switch (_getch())

		{
		case 72://VK_UP
			switch (possidirec)
			{
			case UP:
				headup(snake);
				break;
			case RIGHT:
				possidirec = UP;
				break;
			case DOWN:
				break;
			case LEFT:
				possidirec = UP;
				break;
			}
			break;
		case 80://VK_DOWN
			switch (possidirec)
			{
			case UP:
				break;
			case RIGHT:
				possidirec = DOWN;
				break;
			case DOWN:
				headup(snake);
				break;
			case LEFT:
				possidirec = DOWN;
				break;
			default:
				break;
			}
			break;
		case 75://VK_LEFT
			switch (possidirec)
			{
			case UP:
				possidirec = LEFT;
				break;
			case RIGHT:
				break;
			case DOWN:
				possidirec = LEFT;
				break;
			case LEFT:
				headup(snake);
				break;
			}
			break;
		case 77://VK_RGIHT
			switch (possidirec)
			{
			case UP:
				possidirec = RIGHT;
				break;
			case RIGHT:
				headup(snake);
				break;
			case DOWN:
				possidirec = RIGHT;
				break;
			case LEFT:
				break;
			}
			break;
		}
	}
	else if (k == 27)//ESC键
	{
		myconsole.SetCursor(ShiftX, height + 5);
		printf("Pause");
		system("pause");
	}

	//缓冲方向防止与蛇反向
	if ((possidirec + 6) % 4 != snake.direction)//如果不反向
	{
		snake.nextdirec = possidirec;
	}
}
void Board::kboperate1(Snake &snake)
{
	Direction possidirec = snake.nextdirec;//建立方向缓冲，试图解决高速操作带来的bug
	int k = _getch();
	switch (k)
	{
	case 'w'://VK_UP
	case 'W':
		switch (possidirec)
		{
		case UP:
			headup(snake);
			break;
		case RIGHT:
			possidirec = UP;
			break;
		case DOWN:
			break;
		case LEFT:
			possidirec = UP;
			break;
		}
		break;
	case 's'://VK_DOWN
    case 'S':
		switch (possidirec)
		{
		case UP:
			break;
		case RIGHT:
			possidirec = DOWN;
			break;
		case DOWN:
			headup(snake);
			break;
		case LEFT:
			possidirec = DOWN;
			break;
		default:
			break;
		}
		break;
	case 'a'://VK_LEFT
	case 'A':
		switch (possidirec)
		{
		case UP:
			possidirec = LEFT;
			break;
		case RIGHT:
			break;
		case DOWN:
			possidirec = LEFT;
			break;
		case LEFT:
			headup(snake);
			break;
		}
		break;
	case 'd'://VK_RGIHT
	case 'D':
		switch (possidirec)
		{
		case UP:
			possidirec = RIGHT;
			break;
		case RIGHT:
			headup(snake);
			break;
		case DOWN:
			possidirec = RIGHT;
			break;
		case LEFT:
			break;
		}
		break;
	}
	

	//缓冲方向防止与蛇反向
	if ((possidirec + 6) % 4 != snake.direction)//如果不反向
	{
		snake.nextdirec = possidirec;
	}
}
void Board::singlerun0(Snake &snake)
{
	static int x = 0;

	snake.nextdirec = snake.direction;
	
	
	ctimer timer(SPEED);
	while (timer.blast())
	{
		if (_kbhit())
		{
			kboperate0(snake);
		}
	}
	snake.direction = snake.nextdirec;
	headup(snake);
}
void Board::singlerun1(Snake &snake)
{
	static int x = 0;

	snake.nextdirec = snake.direction;


	ctimer timer(SPEED);
	while (timer.blast())
	{
		if (_kbhit())
		{
			kboperate1(snake);
		}
	}
	snake.direction = snake.nextdirec;
	headup(snake);
}
void Board::singlerun()
{
	static int x = 0;

	snake[0].nextdirec = snake[0].direction;
	snake[1].nextdirec = snake[1].direction;

	ctimer timer(SPEED);
	while (timer.blast())
	{
		if (presentKey!=0)
		{
			kboperate();
		}
	}
	for (int i = 0; i < SNAKES; i++)
	{
		snake[i].direction = snake[i].nextdirec;
		headup(snake[i]);
	}
	
}


void Board::run()
{

	SetConsoleTextAttribute(hOut, 7);
	myconsole.SetCursor(width / 2 * 2 + ShiftX, height / 2 + ShiftY);
	std::cout << "    ";
	printsnake(snake[1]);
	RefreshScreenData();
	while (1)
	{
		singlerun();
	}
}
