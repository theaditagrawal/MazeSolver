#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 30
#define MAZE_SIZE 8

char matrix[MAZE_SIZE][MAZE_SIZE];

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int width, height;
char maze[MAX_SIZE][MAX_SIZE];

void initializeMaze() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            maze[i][j] = '#'; 
        }
    }
}

void printMaze(FILE *file) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fprintf(file, "%c ", maze[i][j] == ' ' ? '-' : maze[i][j]);
        }
        fprintf(file, "\n");
    }
}

int isValidMove(int row, int col) {
    return row >= 0 && row < height && col >= 0 && col < width && maze[row][col] == '#';
}

void generateMazeDFS(int row, int col) {
    maze[row][col] = ' ';

    int directions[4] = {UP, DOWN, LEFT, RIGHT};
    for (int i = 0; i < 4; ++i) {
        int randDir = rand() % 4;
        int nextRow = row + 2 * (randDir == UP) - 2 * (randDir == DOWN);
        int nextCol = col + 2 * (randDir == RIGHT) - 2 * (randDir == LEFT);

        if (nextRow >= 0 && nextRow < height && nextCol >= 0 && nextCol < width && maze[nextRow][nextCol] == '#') {
            maze[row + (nextRow - row) / 2][col + (nextCol - col) / 2] = ' ';
            generateMazeDFS(nextRow, nextCol);
        }
    }
}

void generateMaze() {
    generateMazeDFS(1, 1);
}

typedef struct {
    int x;
    int y;
} Final_route;

Final_route stack[MAZE_SIZE * MAZE_SIZE];
int top = -1;

void push(Final_route pos) {
    if (top < MAZE_SIZE * MAZE_SIZE - 1) {
        stack[++top] = pos;
    }
    else{
        printf("maximum size reached");
    }
}

Final_route pop() {
    Final_route emptyPos = {-1, -1};
    if (top >= 0) {
        return stack[top--];
    }
    return emptyPos;
}

void GetMaze() {
    int x, y;
    FILE *f;
    f = fopen("Maze.txt", "r");
    if(!f){
        printf("Maze not found");
    }

    for (x = 0; x < MAZE_SIZE; x++) {
        for (y = 0; y < MAZE_SIZE; y++) {
            fscanf(f, " %c", &matrix[x][y]);
        }
        printf("\n");
    }

    fclose(f);
}

void PrintMaze() {
    int x, y;
    for (x = 0; x < MAZE_SIZE; x++) {
        printf("                ");
        for (y = 0; y < MAZE_SIZE; y++) {
            printf(" %c ", matrix[x][y]);
        }
        printf("\n");
    }
    printf("\n\n\n");
}

int search(int x, int y) {
    if (matrix[x][y] == 'g') {
        return 1;
    }

    if (matrix[x][y - 1] == '-') { // WEST
        matrix[x][y - 1] = '.';
        push((Final_route){x, y - 1});
        PrintMaze();

        if (search(x, y - 1)) {
            matrix[x][y - 1] = '.';
            PrintMaze();
            return 1;
        } else {
            pop(); 
        }
    }

    if (matrix[x - 1][y] == '-') { // NORTH
        matrix[x - 1][y] = '.';
        push((Final_route){x - 1, y});
        PrintMaze();

        if (search(x - 1, y)) {
            matrix[x - 1][y] = '.';
            PrintMaze();
            return 1;
        } else {
            pop(); 
        }
    }

        if (matrix[x + 1][y] == '-') { // SOUTH
            matrix[x + 1][y] = '.';
            push((Final_route){x + 1, y});
            PrintMaze();

        if (search(x + 1, y)) {
            matrix[x + 1][y] = '.';
            PrintMaze();
            return 1;
        } else {
            pop(); 
        }
    }

    if (matrix[x][y + 1] == '-') { // EAST
        matrix[x][y + 1] = '.';
        push((Final_route){x, y + 1});
        PrintMaze();

        if (search(x, y + 1)) {
            matrix[x][y + 1] = '.';
            PrintMaze();
            return 1;
        } else {
            pop(); 
        }
    }

    return 0;
}

void Maze_Solver() {
    int x, y;

    for (x = 0; x < MAZE_SIZE; x++) {
        for (y = 0; y < MAZE_SIZE; y++) {
            if (matrix[x][y] == 's') {
                push((Final_route){x, y});
                if (search(x, y)) {
                    return; 
                }
            }
        }
        printf("\n");
    }
}

int main() {
        srand(time(NULL));

    printf("Enter the size of the maze (width and height): ");
    scanf("%d", &width);

    if (width > MAX_SIZE) {
        printf("Error: Size too large. Maximum size is %d.\n", MAX_SIZE);
        return 1;
    }

    height = width;

    initializeMaze();
    generateMaze();

    maze[height - 2][width - 2] = 's';
    maze[1][1] = 'g';

    FILE *file = fopen("Maze.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    printMaze(file);

    fclose(file);
    
    GetMaze();
    PrintMaze();
    Maze_Solver();
    PrintMaze();
    
    return 0;
}