#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#define COLS 60
#define ROWS 40
#define L '0' // Live cell
#define D ' ' // Dead cell
#define TIMEOUT 300 // Defines timeout for getch


void wrapBorder(int starty, int startx, int height, int width);
unsigned int** copy2DArr(unsigned int **arr, int rows, int cols);
unsigned int** make2DArr(int rows, int cols);
void print2DArr(unsigned int **arr, int rows, int cols);
void free2DArr(unsigned int **arr, int rows);
void fill(unsigned int** arr, int height, int width);
unsigned int calc(unsigned int **arr, int rows, int cols, int y, int x);
void printToScreen(unsigned int **arr, int rows, int cols, int sy, int sx);
void spawnBlock(unsigned int **arr, int rows, int cols);
void clearScreen(unsigned int **arr, int rows, int cols);
void spawnGlider(unsigned int **arr, int rows, int cols);

int main() {
    srandom(time(NULL)); //NEEDS TO BE AT THE TOP OF MAIN! seeding random
    
    unsigned int **nextArea = make2DArr(ROWS, COLS);
    unsigned int **area; // Current area
    
    initscr();
    attrset(A_BOLD);
    noecho();
    cbreak();
    timeout(TIMEOUT);  // getch() waits for TIMEOUT ms before continuing 
    curs_set(0); // sets cursor invisible
    fill(nextArea, ROWS, COLS); // needs to be after initscr!!!!
    
    int y,x;
    
    // Press q to quit
    int q = 0; // indicates press of q
    int command = -1;
    while (!q) { 
        command = getch(); // getch() calls refresh()
        switch (command) {
            case 'q': q = 1; break;
            case 'c': clearScreen(nextArea, ROWS, COLS); break;
            case 'b': spawnBlock(nextArea, ROWS, COLS); break;
            case 'g': spawnGlider(nextArea, ROWS, COLS); break;
            default:;
        }

        getmaxyx(stdscr, y, x);
        area = copy2DArr(nextArea, ROWS, COLS);
        printToScreen(area, ROWS, COLS, 1, x/2-COLS/2);  // prints everything to upper center with border wrapped around it
        
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                nextArea[y][x] = calc(area, ROWS, COLS, y, x);
            }
        }
        free2DArr(area, ROWS);
    }

    free2DArr(nextArea, ROWS);
    endwin();
    return 0;
}

// Wraps border around described block
void wrapBorder(int starty, int startx, int height, int width) {
    // Corners
    mvaddch(starty-1, startx-1, ACS_ULCORNER);
    mvaddch(starty-1, startx+width, ACS_URCORNER);
    mvaddch(starty+height, startx-1, ACS_LLCORNER);
    mvaddch(starty+height, startx+width, ACS_LRCORNER);

    // Lines
    mvhline(starty-1,startx,ACS_HLINE,width);
    mvhline(starty+height,startx,ACS_HLINE,width);
    mvvline(starty,startx-1,ACS_VLINE,height);
    mvvline(starty, startx+width,ACS_VLINE,height);
}

void printToScreen(unsigned int **arr, int rows, int cols, int sy, int sx) {
    clear(); // Clears anomalies after resizing
    wrapBorder(sy, sx, rows, cols);
    int y = sy;
    for (int row = 0; row < rows; row++) {
        move(y,sx);
        for (int col = 0; col < cols; col++) {
            addch(arr[row][col]);
        }
        y++;
    }
    y += 1; // skip border and main area
    
    mvaddstr(y,sx,"Commands:"); y++; 
    mvaddstr(y,sx,"q - quit"); y++;
    mvaddstr(y,sx,"c - clear screen"); y++;
    mvaddstr(y,sx,"b - spawn block"); y++;
    mvaddstr(y,sx,"g - spawn glider"); y++;
}

void spawnGlider(unsigned int **arr, int rows, int cols) {
    // 0 - top left; 1 - top right; 2 - down left; 3 - down right
    int direction = random()%4;
    int y = random()%rows;
    int x = random()%cols;
    int left = (x-1 < 0) ? cols-1 : x-1;
    int right = (x+1 > cols-1) ? 0 : x+1;
    int up = (y-1 < 0) ? rows-1 : y-1;
    int down = (y+1 > rows-1) ? 0 : y+1;
    
    switch (direction) {
        case 0: // top left
            arr[up][left] = L;
            arr[up][x] = L;
            arr[up][right] = L;
            arr[y][left] = L;
            arr[down][x] = L;
            break;
        case 1: // top right
            arr[up][left] = L;
            arr[up][x] = L;
            arr[up][right] = L;
            arr[y][right] = L;
            arr[down][x] = L;
            break;
        case 2: // down left
            arr[down][left] = L;
            arr[down][x] = L;
            arr[down][right] = L;
            arr[y][left] = L;
            arr[up][x] = L;
            break;
        case 3: // down right
            arr[down][left] = L;
            arr[down][x] = L;
            arr[down][right] = L;
            arr[y][right] = L;
            arr[up][x] = L;
            break;
    }
}

// Makes all cells dead
void clearScreen(unsigned int **arr, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = D;
        }
    }
}

void spawnBlock(unsigned int **arr, int rows, int cols) {
    int y = random()%rows;
    int x = random()%cols;
    int down = (y+1 > rows-1) ? 0 : y+1;
    int right = (x+1 > cols-1) ? 0 : x+1;
    arr[y][x] = L;
    arr[y][right] = L;
    arr[down][x] = L;
    arr[down][right] = L;
}

// Calculates cell state (at y x) and returns cells unsigned int
unsigned int calc(unsigned int **arr, int rows, int cols, int y, int x) {
    int neighbors = 0;
    int left = (x-1 < 0) ? cols-1 : x-1;
    int right = (x+1 > cols-1) ? 0 : x+1;
    int up = (y-1 < 0) ? rows-1 : y-1;
    int down = (y+1 > rows-1) ? 0 : y+1;

    // COUNT NEIGHBORS
    if (arr[up][left] == L) neighbors++; // top left
    if (arr[up][x] == L) neighbors++; // top
    if (arr[up][right] == L) neighbors++; // top right
    if (arr[y][left] == L) neighbors++; // left
    if (arr[y][right] == L) neighbors++; // right
    if (arr[down][left] == L) neighbors++; // bottom left
    if (arr[down][x] == L) neighbors++; // bottom
    if (arr[down][right] == L) neighbors++; // bottom right

    // RULES   
    if (arr[y][x] == L && (neighbors == 2 || neighbors == 3)) return L;
    else if (arr[y][x] == D && neighbors == 3) return L;
    else return D;
}

// Return ptr to new 2D array filled with 0s
unsigned int** make2DArr(int rows, int cols) {
    unsigned int **res = calloc(rows, sizeof(unsigned int*));
    for (int row = 0; row < rows; row++) {
        res[row] = calloc(cols, sizeof(unsigned int));
    }
    return res;
}

// Free 2d array mem
void free2DArr(unsigned int **arr, int rows) {
    for (int row = 0; row < rows; row++) {
        free(arr[row]);
    }
    free(arr);
}

// Prints 2d array of chars
void print2DArr(unsigned int **arr, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%c", arr[row][col]);
        }
        printf("\n");
    }
}

// Returns a ptr to copy of arr
unsigned int** copy2DArr(unsigned int **arr, int rows, int cols) {
    unsigned int **res = calloc(rows, sizeof(unsigned int*));
    for (int row = 0; row < rows; row++) {
        res[row] = calloc(cols, sizeof(unsigned int));
        memcpy(res[row], arr[row], sizeof(unsigned int)*cols);
    }
    return res;
}

// Fills 2d array of chars with either D(dead) or L(alive)
void fill(unsigned int** arr, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (random()%2 == 0) ? D : L;
        }
    }
}