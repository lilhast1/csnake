#include <curses.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <deque>
#include <utility>
#include <string>

#define NO_EXPAND 0
#define EXPAND 1

int SCR_WIDTH = 40;
int SCR_HEGIHT = 40;

using std::deque;
using std::pair;
using vec2 = std::pair<int, int>;

struct Snake {
	char head;
	deque<pair<int, int>> snake;
	Snake() : head('>'), snake(1, {1,0}) {}
	void draw();
	bool move(pair<int, int>& dir, int expand = NO_EXPAND);
	pair<int, int> get_head();
};

pair<int, int> operator + (const pair<int, int>&, const pair<int, int>&);
bool operator == (const pair<int, int>&, const pair<int, int>&);


int main()
{
	WINDOW* win = initscr();

	keypad(win, true);
	nodelay(win, true);
	curs_set(0);

	vec2 dir{1, 0};
	vec2 berry{rand() % SCR_WIDTH, rand() % SCR_HEGIHT};
	
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	std::string limit(SCR_WIDTH, '_');

	Snake snake{};
	int score = 1;
	while (1) {
		int pressed = wgetch(win);
		
		if (pressed == '\e')
			break;
		switch (pressed) {
		case KEY_LEFT:
			dir.first = -1;
			dir.second = 0;
			break;
		case KEY_RIGHT:
			dir.first = 1;
			dir.second = 0;
			break;
		case KEY_UP:
			dir.first = 0;
			dir.second = -1;
			break;
		case KEY_DOWN:
			dir.first = 0;
			dir.second = 1;
			break;
		}
		
		bool expand = snake.get_head() == berry;
		if (expand) {
			berry = {rand() % SCR_WIDTH, rand() % SCR_HEGIHT};
			score++;
		}
		bool good = snake.move(dir, expand);
		
		const bool bound = snake.get_head().first <= SCR_WIDTH && 
						snake.get_head().first >= 0 &&
						snake.get_head().second >= 0 &&
						snake.get_head().second <= SCR_HEGIHT;

		erase();
		
		if (good && bound)
			snake.draw();
		else
			break;
		attron(COLOR_PAIR(2));
		mvaddch(berry.second, berry.first, '@');

		attron(COLOR_PAIR(0));
		mvprintw(SCR_HEGIHT, 0, "%s", limit.c_str());
		for (int i = 0; i <= SCR_HEGIHT; i++)
			mvaddch(i, SCR_WIDTH, '|');
		//refresh();
		mvprintw(SCR_HEGIHT / 2, SCR_WIDTH * 1.5, "score: %d", score);
		usleep(100000);
	}

	endwin();

	return 0;
}


bool Snake::move(pair<int, int>& dir, int expand)
{
	snake.push_back(snake.back() + dir);
	if (dir == vec2{1, 0})
		head = '>';
	if (dir == vec2{-1, 0})
		head = '<';
	if (dir == vec2{0, -1})
		head = '^';
	if (dir == vec2{0, 1})
		head = 'v';
	if (!expand)
		snake.pop_front();
	for (int i = 0; i < snake.size() - 1; i++)
		if (snake[i] == snake.back())
			return false;
	return true;
}


void Snake::draw()
{
	attron(COLOR_PAIR(1));
	for (int i = 0; i < snake.size() - 1; i++) {
		mvaddch(snake[i].second, snake[i].first, 'o');
	}
	mvaddch(snake.back().second, snake.back().first, head);
}

pair<int, int> Snake::get_head()
{
	return snake.back();
}

pair<int, int> operator + (const pair<int, int>& a, const pair<int, int>& b)
{
	return {a.first + b.first, a.second + b.second};
}

bool operator == (const pair<int, int>& a, const pair<int, int>& b)
{
	return a.first == b.first && a.second == b.second;
}