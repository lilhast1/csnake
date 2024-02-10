#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

int SCR_WIDTH = 40;
int SCR_HEGIHT = 40;

typedef struct {
	int x, y;
} vec2;


void draw(const vec2* p, char c)
{
	mvaddch(p->y, p->x, c);
}

int main()
{
	WINDOW* win = initscr();

	keypad(win, true);
	nodelay(win, true);
	curs_set(0);

	vec2 head = {0, 0}, dir = {1, 0};
	char snkhead = '>';

	vec2 snake
	while (1) {
		int pressed = wgetch(win);
		
		if (pressed == '\e')
			break;
		switch (pressed) {
		case KEY_LEFT:
			dir.x = -1;
			dir.y = 0;
			break;
		case KEY_RIGHT:
			dir.x = 1;
			dir.y = 0;
			break;
		case KEY_UP:
			dir.x = 0;
			dir.y = -1;
			break;
		case KEY_DOWN:
			dir.x = 0;
			dir.y = 1;
			break;
		}
		
		head.x += dir.x;
		head.y += dir.y;
		
		erase();
		draw(&head, snkhead);

		usleep(100000);
	}

	endwin();

	return 0;
}