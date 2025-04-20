#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define COLS 60
#define ROWS 20
#define L '0' // Live cell
#define D '_' // Dead cell

char** copy2DArr(char **arr, int rows, int cols);
char** make2DArr(int rows, int cols);
void print2DArr(char **arr, int rows, int cols);
void free2DArr(char **arr, int rows);
void fill(char** arr, int height, int width);

int main() {
    srandom(time(NULL)); //NEEDS TO BE AT THE TOP OF MAIN! seeding random
    
    char **nextArea = make2DArr(ROWS, COLS);
    char **area; // Current area
    
    fill(nextArea, ROWS, COLS); 
    int left = -1;
    int right = -1;
    int up = -1;
    int down = -1;
    int neighbors = -1;
    // ENTER A CHAR TO QUIT  | IF CTRL+C OR OTHERS -> MEMORY LEAK
    while (getchar() == '\n') { 
        printf("\n\n\n\n");
        area = copy2DArr(nextArea, ROWS, COLS);
        print2DArr(area, ROWS, COLS);

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                neighbors = 0;
                left = (x-1 < 0) ? COLS-1 : x-1;
                right = (x+1 > COLS-1) ? 0 : x+1;
                up = (y-1 < 0) ? ROWS-1 : y-1;
                down = (y+1 > ROWS-1) ? 0 : y+1;

                // COUNT NEIGHBORS
                if (area[up][left] == L) neighbors++; // top left
                if (area[up][x] == L) neighbors++; // top
                if (area[up][right] == L) neighbors++; // top right
                if (area[y][left] == L) neighbors++; // left
                if (area[y][right] == L) neighbors++;  // right
                if (area[down][left] == L) neighbors++; // bottom left
                if (area[down][x] == L) neighbors++; // bottom
                if (area[down][right] == L) neighbors++; // bottom right

                // RULES
                if (area[y][x] == L && (neighbors==2||neighbors==3)) nextArea[y][x] = L;
                else if (area[y][x] == D && neighbors == 3) nextArea[y][x] = L;
                else nextArea[y][x] = D;
            }
        }
        free2DArr(area, ROWS);
    }
    free2DArr(nextArea, ROWS);
    return 0;
}

// Return ptr to new 2D array filled with 0s
char** make2DArr(int rows, int cols) {
    char **res = calloc(rows, sizeof(char*));
    for (int row = 0; row < rows; row++) {
        res[row] = calloc(cols, sizeof(char));
    }
    return res;
}

// Free 2d array mem
void free2DArr(char **arr, int rows) {
    for (int row = 0; row < rows; row++) {
        free(arr[row]);
    }
    free(arr);
}

// Prints 2d array of chars
void print2DArr(char **arr, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%c", arr[row][col]);
        }
        printf("\n");
    }
}

// Returns a ptr to copy of arr
char** copy2DArr(char **arr, int rows, int cols) {
    char **res = calloc(rows, sizeof(char*));
    for (int row = 0; row < rows; row++) {
        res[row] = calloc(cols, sizeof(char));
        memcpy(res[row], arr[row], sizeof(char)*cols);
    }
    return res;
}

// Fills 2d array of chars with either D(dead) or L(alive)
void fill(char** arr, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            arr[row][col] = (random()%2 == 0) ? D : L;
        }
    }
}