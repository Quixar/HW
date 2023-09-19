#include <iostream>
#include <ctime> // time
#include <Windows.h> // окрашивание, работа с координатами
#include <conio.h> // configurate input output
using namespace std;

// enumeration - перечисление
// набор целочисленных констант
// читаемость кода существенно повышается
enum Directions { LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72 };
enum Colors { RED = 12, BLACK = 0, DARKBLUE = 3, YELLOW = 14, PINK = 13 , BLUE = 1};
enum Objects { HALL = 0, WALL = 1, COIN = 2, ENEMY = 3, AID = 4};

int main()
{
	system("title Maze"); // смена текста в заголовке консольлного окна

	// дескриптор (идентификатор), уникальный номер окна консоли
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	// скрываем мигающий курсор в консоли
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100; // 1...100
	SetConsoleCursorInfo(h, &info);

	srand(time(0)); // запуск алгоритма генерации СЧ

	const int width = 60; // ширина лабиринта
	const int height = 15; // высота лабиринта
	int maze[height][width] = {}; // заготовка для лабиринта в виде двумерного массива, заполненного нулями

	/////////////////////////////////////////////////////

	// заполнение массива (генерация лабиринта)
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			maze[y][x] = rand() % 5; // 0 1 2 3

			// установка стен по бокам (делаем рамки)
			if (x == 0 or y == 0 or x == width - 1 or y == height - 1)
				maze[y][x] = WALL; // стенка

			// определение точки входа и выхода из лабиринта
			if (x == 0 and y == 2 or
				x == 1 and y == 2 or
				x == 2 and y == 2 or
				x == width - 1 and y == height - 3)
				maze[y][x] = HALL; // коридор

			// если текущий элемент 2д-массива - это враг,
			// то ниже код, который уменьшает их количество
			// условно, произойдёт жеребьёвка (или же подбрасывание монетки)

			if (maze[y][x] == ENEMY)
			{
				int r = rand() % 5; // 0...9
				// если выпадает 0 - враг остаётся (10% случаев)
				if (r != 0) // если выпадает не 0 (а число от 1 до 9) - врага убираем (вероятность 90%)
				{
					maze[y][x] = HALL; // убираем врага
				}
			}
			else if (maze[y][x] == AID)
			{
				int ran = rand() % 15;
				if (ran != 0)
				{
					maze[y][x] = HALL;
				}
			}
		}
	}

	///////////////////////////////////////////////////// 

	// показ лабиринта
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (maze[y][x] == HALL)
			{
				SetConsoleTextAttribute(h, BLACK); // чёрные 
				cout << " "; // коридор
			}
			else if (maze[y][x] == WALL)
			{
				SetConsoleTextAttribute(h, DARKBLUE); // бирюзовые 
				cout << (char)178; // стена
				// cout << "#";
			}
			else if (maze[y][x] == COIN)
			{
				SetConsoleTextAttribute(h, YELLOW); // ярко-жёлтый 
				cout << "."; // монетка
			}
			else if (maze[y][x] == ENEMY)
			{
				// cout << "*"; // враг
				SetConsoleTextAttribute(h, RED); // ярко-красный 
				cout << (char)1;
			}
			else if (maze[y][x] == AID)
			{
				SetConsoleTextAttribute(h, BLUE);
				cout << "+";
			}
		}
		cout << "\n";
	}

	// пример установки курсора в любые координаты консоли
	COORD position; // координаты ГГ
	position.X = 0;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, PINK);
	cout << (char)1;


	int coins = 0; // количество собранных монет
	int hp = 100; // количество здоровья ГГ
	int damage = 25; // количество урона которое наносят враги
	int heal = 25; // количество лечения от аптечки


	// начинаем интерактив (польщователь управляет смайликом с помощью клавиатуры)
	while (true)
	{
		int code = _getch(); // get character - получение кода нажатой клавиши
		if (code == 224) // если была нажата какая-то стрелка
		{
			code = _getch();
		}

		// после того, как клавиша стрелки была нажата, программа уже знает, куда побежит смайлик
		// можно стереть смайлик в его текущей позиции
		SetConsoleCursorPosition(h, position);
		cout << " ";

		if (code == RIGHT and maze[position.Y][position.X + 1] != WALL)
			position.X++; // изменение позиции ГГ вправо на 1 по иксу
		else if (code == LEFT and maze[position.Y][position.X - 1] != WALL)
			position.X--; // изменение позиции ГГ вправо на 1 по иксу
		else if (code == DOWN and maze[position.Y + 1][position.X] != WALL)
			position.Y++;
		else if (code == UP and maze[position.Y - 1][position.X] != WALL) 
			position.Y--;

		// показываем смайлик в новых координатах
		SetConsoleCursorPosition(h, position); // установка курсора в новые координаты
		SetConsoleTextAttribute(h, PINK);
		cout << (char)1;

		///////////////////////////////////////////////////
		// проверка на выход из лабиринта

		if (position.X == width - 1 and
			position.Y == height - 3)
		{
			MessageBoxA(0, "A way out of the maze has been found", "YOU WIN!!!", MB_OK);
			break;
		}

		///////////////////////////////////////////////////
		// проверка на подбор монетки

		// если в лабиринте по координатам ГГ находится монетка, то
		if (maze[position.Y][position.X] == COIN)
		{
			// cout << "COIN!!!";
			coins++; // на одну монетку подобрано больше
			maze[position.Y][position.X] = HALL; // убираем монетку из массива
			COORD coins_info; // отдельная переменная, отвечающая за координаты сообщения о количестве монет
			coins_info.X = width + 1; // x = 61
			coins_info.Y = 1; // y = 1
			SetConsoleCursorPosition(h, coins_info); // установка курсора в новые координаты
			SetConsoleTextAttribute(h, YELLOW);
			cout << "COINS: " << coins;
		}

		else if (maze[position.Y][position.X] == ENEMY)
		{
			hp -= damage;
			maze[position.Y][position.X] = HALL;
			COORD hp_info; // переменная отвечающая за координаты сообщения о количестве здоровья
			hp_info.X = width + 2;
			hp_info.Y = 2;
			SetConsoleCursorPosition(h, hp_info); // установка курсора в новые координаты
			SetConsoleTextAttribute(h, BLUE);
			cout << "HP: " << hp;
			if (hp == 0)
			{
				MessageBoxA(0, "You've run out of health", "YOU LOST", MB_OK);
				break;
			}
		}


		// проверка на подбор аптечки
		else if (maze[position.Y][position.X] == AID)
		{
			if (hp < 100)
			{
				hp += heal; // прибавляем здоровье ГГ
				maze[position.Y][position.X] = HALL; // убираем аптечку из массива
				COORD hp_info; // переменная отвечающая за координаты сообщения о количестве здоровья
				hp_info.X = width + 2;
				hp_info.Y = 2;
				SetConsoleCursorPosition(h, hp_info); // установка курсора в новые координаты
				SetConsoleTextAttribute(h, BLUE);
				cout << "HP: " << hp;
			}
			else if (hp == 100)
			{
				maze[position.Y][position.X] = AID;
			}
		}
	}

	system("cls"); // очистка экрана
	cout << "YOU WIN!!!\n\n"; // main(); - для перезапуска приложения
	Sleep(INFINITE); // пауза до тех пор, пока пользователь не закроет окно консоли
}