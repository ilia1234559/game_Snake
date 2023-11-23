#include <iostream>
#include<Windows.h>
#include <ctime>
using namespace std;

enum class Direction
{
	UP,    
	DOWN,
	LEFT,
	RIGHT
};

class Map
{
public:
	char field[346] =
		"######################\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"#                    #\n"
		"######################\n";
};

class Snake
{
public:
	Snake(int length, Direction dir, int max_len, int start_x, int start_y, bool play)
	{
		this->len = length;
		this->dir = dir;
		this->max_len_snake = max_len;
		this->x[0] = start_x;
		this->y[0] = start_y;
		this->isRunning = play;
	}


private:
	int len;
	Direction dir;
	int max_len_snake;
	int x[240];
	int y[240];

public:
	bool isRunning;
	char snake = 'O';


	void len_pp()
	{
		++this->len;
	}

	void setDir(Direction x)
	{
		this->dir = x;
	}

	Direction getDir()
	{
		return dir;
	}

	void swapDir()
	{
		if (GetKeyState('W') & 0x8000)
		{
			if (dir != Direction::DOWN)
				dir = Direction::UP;
		}
		if (GetKeyState('S') & 0x8000)
		{
			if (dir != Direction::UP)
				dir = Direction::DOWN;
		}
		if (GetKeyState('A') & 0x8000)
		{
			if (dir != Direction::RIGHT)
				dir = Direction::LEFT;
		}
		if (GetKeyState('D') & 0x8000)
		{
			if (dir != Direction::LEFT)
				dir = Direction::RIGHT;
		}
	}

	void move()
	{
		if (dir == Direction::UP)
		{
			--y[0];
		}

		if (dir == Direction::DOWN)
		{
			++y[0];
		}

		if (dir == Direction::LEFT)
		{
			--x[0];
		}

		if (dir == Direction::RIGHT)
		{
			++x[0];
		}
	}


	int getLen()
	{
		return len;
	}

	void setX(int i, int value)
	{
		this->x[i] = value;
	}
	int getX(int i)
	{
		return x[i];
	}
	int getXpp(int i)
	{
		return ++x[i];
	}
	int getXmm(int i)
	{
		return --x[i];
	}
	

	void setY(int i, int value)
	{
		this->y[i] = value;
	}
	int getY(int i)
	{
		return y[i];
	}
	int getYpp(int i)
	{
		return ++y[i];
	}
	int getYmm(int i)
	{
		return --y[i];
	}

	bool checkRound(int width, int height)
	{
		for (int i = 1; i <= len + 1; ++i)
		{
			if (x[0] == x[i] && y[0] == y[i])
			{
				isRunning = false;
				return isRunning;
			}
			if ((x[0] == 0 || x[0] == (width - 2)) || (y[0] == 0 || y[0] == (height)))
			{
				isRunning = false;
				return isRunning;
			}
		}
	}
};

class Food
{
public:
	Food(int x, int y, char symbol)
	{
		this->x = x;
		this->y = y;
		this->foodS = symbol;
	}

private:
	int x;
	int y;
public:
	char foodS;

	void setX(int coord)
	{
		x = coord;
	}
	int getX()
	{
		return x;
	}

	void setY(int coord)
	{
		y = coord;
	}
	int getY()
	{
		return y;
	}

	char getSymbol()
	{
		return foodS;
	}
};

void gotoxy(int x, int y);
int getRandom(int min, int max);

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	rand();
	const int width = 23;
	const int height = 14;

	Snake snake(1, Direction::UP, (width - 3) * (height - 2), width / 2, height / 2, true);
	Map map;
	Food food(getRandom(1, width - 2), getRandom(1, height - 2), '+');

	double time = clock();
	while (snake.checkRound(width, height))
	{
		snake.swapDir();

		if ((clock() - time) / CLOCKS_PER_SEC >= 0.1)
		{

			time = clock();

			if (snake.getY(0) == food.getY() && snake.getX(0) == food.getX())
			{
				snake.len_pp();
				food.setX(getRandom(1, width - 3));
				food.setY(getRandom(1, height - 2));
			}

			for (int i = snake.getLen() - 2; i >= 0; --i)
			{
				snake.setX(i + 1, snake.getX(i));
				snake.setY(i + 1, snake.getY(i));
			}

			snake.move();

			map.field[food.getX() + (width * food.getY())] = food.getSymbol();
			for (int i = 0; i < snake.getLen(); ++i)
			{
				map.field[snake.getX(i) + (width)*snake.getY(i)] = snake.snake;
			}
			cout << "Result: " << snake.getLen() << endl;
			cout << map.field;
			for (int i = 0; i < snake.getLen(); ++i)
			{
				map.field[snake.getX(i) + (width)*snake.getY(i)] = ' ';
			}
			gotoxy(0, 0);
		}
	}
	gotoxy(3, height / 2);
	std::cout << "Your score is " << snake.getLen() << std::endl;
	gotoxy(width, height + 1);
}

void gotoxy(int x, int y)
{
	COORD position = { x,y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, position);
}

int getRandom(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
