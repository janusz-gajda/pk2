/*  checkthis.c  */

#include <ncurses.h>

int main()

{

initscr();


printw("hello world\n");

refresh();

getch();


endwin();

return 0;

}