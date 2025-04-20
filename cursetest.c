#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#define COLS 60
#define ROWS 20
#define L ACS_DIAMOND // Live cell
#define D '0' // Dead cell
#define TIMEOUT 300 // Defines timeout for getch

unsigned int** copy2DArr(unsigned int **arr, int rows, int cols);
unsigned int** make2DArr(int rows, int cols);
void print2DArr(unsigned int **arr, int rows, int cols);
void free2DArr(unsigned int **arr, int rows);
void fill(unsigned int** arr, int height, int width);
unsigned int calc(unsigned int **arr, int rows, int cols, int y, int x);
void printToScreen(unsigned int **arr, int rows, int cols, int sy, int sx);


int main() {
    srandom(time(NULL)); //NEEDS TO BE AT THE TOP OF MAIN! seeding random
    
    unsigned int **nextArea = make2DArr(ROWS, COLS);
    unsigned int **area; // Current area
    fill(nextArea, ROWS, COLS); 


    initscr();
    cbreak();
    timeout(TIMEOUT);  // getch() waits for TIMEOUT ms before continuing 
    curs_set(0); // sets cursor invisible
    noecho();
    
    // Press Q to quit
    while (getch() != 'q') { // getch() calls refresh()
        //printf("\n\n\n\n");
        area = copy2DArr(nextArea, ROWS, COLS);;
        printToScreen(area, ROWS, COLS, 3, 6);

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


void printToScreen(unsigned int **arr, int rows, int cols, int sy, int sx) {
    //clear();
    int y = sy;
    for (int row = 0; row < rows; row++) {
        move(y,sx);
        for (int col = 0; col < cols; col++) {
            addch(arr[row][col]);
        }
        y++;
    }
}


// Calculates cell state (at y x) and returns cells unsigned int
unsigned int calc(unsigned int **arr, int rows, int cols, int y, int x) {
    int neighbors = 0;
    int left = (x - 1 < 0) ? cols - 1 : x - 1;
    int right = (x + 1 > cols - 1) ? 0 : x + 1;
    int up = (y - 1 < 0) ? rows - 1 : y - 1;
    int down = (y + 1 > rows - 1) ? 0 : y + 1;

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