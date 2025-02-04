#include <iostream>
#include <random>
#include <vector>
#include <conio.h> 
#include <thread> 
#include <chrono>

using namespace std;


void render_map();
void RandomPosFood();
void input();
bool update_snake();
void start();
bool end();

struct snake_block{
	int index;
	int x;
	int y;
};

struct snake_head {
	char diraction;
	int x;
	int y;
};

struct food {
	int x;
	int y;
};

int len = 3;
vector<snake_block> body;
snake_head head;
food apple;

int best = 0;

const int mapsize = 15;

char map[mapsize][mapsize] ;

int speed;


void main() {
	while(true){
		start();
		RandomPosFood();
		render_map();

		while (true) {
			input();
			if (update_snake() == false)
				if (end() == false)
					return;
			render_map();
		}
	}
}


bool update_snake() {
	switch (head.diraction)
	{
	case 'd':
		head.x++;
		break;
	case 'w':
		head.y--;
		break;
	case 's':
		head.y++;
		break;
	case 'a':
		head.x--;
		break;
	default:
		break;
	}

	if (map[head.y][head.x] == '#' || map[head.y][head.x] == 'O') {
		return false;
	}
	else if (map[head.y][head.x] == 'H') {
		len++;
		RandomPosFood();
	}

	
	for (snake_block& i : body) {
		i.index++;
	}

	int k = 0;
	for (snake_block& i : body) {
		if (i.index >= len) {
			map[i.y][i.x] = ' ';
			body.erase(body.begin() + k);
		}
		k++;
	}


	snake_block tmp;
	tmp.index = 0;
	tmp.x = head.x;
	tmp.y = head.y;
	body.push_back(tmp);

	map[head.y][head.x] = 'O';
	for (snake_block& i : body) {
		map[i.y][i.x] = 'O';
	}
}

void render_map() {
	cout << "lenght = " << len << endl;
	for (int i = 0; i < mapsize; i++) {
		for (int k = 0; k < mapsize; k++) {
			cout << map[i][k] << ' ';
		}
		cout << endl;
	}
}

void RandomPosFood() {
	while (true) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, mapsize - 2);
		int y = dist(gen);
		int x = dist(gen);

		if (map[y][x] == ' ') {
			apple.x = x;
			apple.y = y;
			map[apple.y][apple.x] = 'H';
			break;
		}
	}
}

void input() {
	int time_left = 0;
	for (int i = 0; i < speed; i++) {
		if (_kbhit()) { 
			char key = _getch();
			if (key == 'd') {
				if (head.diraction == 'a') {
					continue;
				}
				head.diraction = 'd';
				break;
			}	
			else if (key == 'w') {
				
				if (head.diraction == 's') {
					continue;
				}
				head.diraction = 'w';
				break;
			}
			else if (key == 'a') {
				
				if (head.diraction == 'd') {
					continue;
				}
				head.diraction = 'a';
				break;
			}
			else if (key == 's') {
				if (head.diraction == 'w') {
					continue;
				}
				head.diraction = 's';
				break;
			}	
		}
		this_thread::sleep_for(chrono::milliseconds(1));
		time_left = i;
	}
	this_thread::sleep_for(chrono::milliseconds(speed - time_left));
	system("cls");
}

void start() {
	while (true){
		bool temp = true;
		cout << "choose a level of difficulty for the game(print 1, 2 or 3)\n";
		int tmp;
		cin >> tmp;
		switch (tmp) {
		case 1:
			speed = 40;
			break;
		case 2:
			speed = 30;
			break;
		case 3:
			speed = 20;
			break;
		default:
			cout << "invalid input";
			temp = false;
			break;
		}
		if (temp == true)
			break;
	}

	for (int i = 0; i < mapsize; i++) {
		for (int k = 0; k < mapsize; k++) {
			if (i == 0 || k == 0 || i == mapsize - 1 || k == mapsize - 1)
				map[i][k] = '#';
			else
				map[i][k] = ' ';
		}
	}

	for (int i = 0; i < len; i++) {
		snake_block temp;
		temp.index = i;
		temp.x = 8 - i;
		temp.y = 6;
		body.push_back(temp);
	}
	head.x = 8;
	head.y = 6;
	head.diraction = 'd';
	int tmp = body.size();
	map[head.y][head.x] = 'O';
	for (int i = 0; i < tmp; i++) {
		map[body[i].y][body[i].x] = 'O';
	}
}

bool end() {
	while (true){
		bool find = true;
		if (len > best)
			best = len;
		cout << "GAME OVER\n\n\n";
		cout << "your result: " << len << "\t\t best reuslt: " << best << endl;;
		cout << "do you want to start new game? (enter y if yes, or e if no)\n";
		char tmp;
		cin >> tmp;
		switch (tmp)
		{
		case 'y':
			body.clear();
			len = 3;
			system("cls");
			main();
			break;
		case 'e':
			return false;
			break;
		default:
			cout << "invalid input";
			find = false;
			break;
		}
		if (find == true)
			break;
	}	
}