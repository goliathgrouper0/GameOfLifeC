#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



int main()
{

    
    
    
    
    initscr();
    cbreak();
    WINDOW *mywin = newwin(10, 20, (LINES-10)/2, (COLS-20)/2);
    box(mywin, 0, 0);
    wgetch(mywin);
    getch();
    wgetch(mywin);
    wgetch(mywin);

    delwin(mywin);
    endwin();
    
	return 0;
}