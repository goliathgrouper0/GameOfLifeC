#include <ncurses.h>

int main() {
    initscr(); // Init terminal in curses mode NEEDS TO BE CALLED
    //start_color(); // Starts color functionality
    //raw();
    curs_set(0); // Sets cursor invisible
    noecho();
    //keypad(stdscr, TRUE); // Enables reading of F1... arrows etc.
    printw("Hello World !!");
    int rows,cols;
    getmaxyx(stdscr, rows, cols); // Get terminal window size in rows and cols
    getyx(stdscr, rows, cols); // Get current curser coords
    clear(); // Clears the screens
    move(0, 0); // move cursor to y,x coords
    refresh();
    getch();  // This is similar to getchar but for curses
    endwin(); // NEEDS TO BE CALLED

    return 0;
}