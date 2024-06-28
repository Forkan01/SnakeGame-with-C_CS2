#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72          //Macros for the Key codes
#define DOWN 80
#define LEFT 75
#define RIGHT 77

int length;
int bend_no;      //number of bends/turns the snake has made
int len;          //Temporary length
int life;         //Number of lives remaining
char key;         //key pressed by the user

struct coordinate {  // coordinate structure stores x,y,direction
    int x;
    int y;
    int direction;
};

typedef struct coordinate coordinate;    // Created an alias for the struct

coordinate head, bend[500], food, body[30];   // Used the new alias to declare a variable

void gotoxy(int x, int y);     //Positions the cursor at specific coordinates
void delay(int ms);            //delay in milliseconds
void printWelcomeMessage();
void loadScreen();
void initializeGame();
void drawBoard();
void generateFood();
void moveSnake();
void checkCollision();
void changeDirection();
void updateSnakeBody();
void recordScore();
int calculateScore();

int main() {
    printWelcomeMessage();
    loadScreen();
    initializeGame();
    moveSnake();
    return 0;
}

void gotoxy(int x, int y) {
    COORD coord;       //Windows API for 2D
    coord.X = x;       //horizontal
    coord.Y = y;       //vertical
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);  //Moves the cursor to a specific position (x, y) on the console screen using Windows API
}

void delay(int ms) {
    Sleep(ms);
}

void printWelcomeMessage() {
    printf("\tWelcome to the Snake game with C. Press any key to continue...\n");
    getch();              //Waits for a single character input
    system("cls");        //Clears the console screen
    printf("\tGame Instructions:\n");
    printf("-> Use arrow keys to move the snake.\n");
    printf("-> Eat food to grow and increase your score.\n");
    printf("-> Avoid hitting the walls or your own body.\n");
    printf("-> You have three lives.\n");
    printf("-> Press 'Esc' to exit.\n");
    printf("\nPress any key to start the game...");
    if (getch() == 27) exit(0);          //Checks if the key pressed= escape key, then program exits
    system("cls");
}

void loadScreen() {
    printf("\n \n \n \n \n \n \n                                      Loading...");
    for (int i = 0; i < 20; i++) {   //This loop executes 20 times
        delay(50);
        printf("%c", 177);           //each time printing ASCII value 177-shaded block(bar segment)
    }
    getch();
}

void initializeGame() {
    length = 5;
    life = 3;
    head.x = 25;
    head.y = 20;
    head.direction = RIGHT;
    bend[0] = head;
    generateFood();
}

void drawBoard() {
    system("cls");
    for (int i = 10; i < 71; i++) {      //iterates column 10 - 70
        gotoxy(i, 10); printf("!");      // Draw top borders
        gotoxy(i, 30); printf("!");      // Draw Bottom borders
    }
    for (int i = 10; i < 31; i++) {      //iterates row 10 - 70
        gotoxy(10, i); printf("!");      // Draw left borders
        gotoxy(70, i); printf("!");      // Draw right borders
    }
    gotoxy(food.x, food.y);
    printf("F");
}

void generateFood() {
    srand(time(0));               //seeds the random number each time
    food.x = (rand() % 60) + 11;  //When rand() produces any integer, taking it modulo 60 restricts the result to the range [0, 59] because any integer divided by 60 has a remainder in that range.
    food.y = (rand() % 20) + 11;  //adding 11 to the result of rand() % 60, the minimum value (0) becomes 11, and the maximum value (59) becomes 70
}

void moveSnake() {
    while (1) {                   //continuously updates and renders the game state
        drawBoard();
        checkCollision();
        updateSnakeBody();

        if (kbhit()) {            //kbhit() checks if a key has been pressed
            changeDirection();
        }

        delay(120);
    }
}

void checkCollision() {
    if (head.x == food.x && head.y == food.y) {       //Checking for Food Collision:
        length++;
        generateFood();
    }

    if (head.x <= 10 || head.x >= 70 || head.y <= 10 || head.y >= 30) {  //Checking for Wall Collision
        life--;
        if (life == 0) {
            printf("Game Over! Your score: %d\n", calculateScore());
            recordScore();
            exit(0);
        } else {
            initializeGame();
        }
    }

    for (int i = 1; i < length; i++) {                  //Checking for Self Collision
        if (head.x == body[i].x && head.y == body[i].y) {
            life--;
            if (life == 0) {
                printf("Game Over! Your score: %d\n", calculateScore());
                recordScore();
                exit(0);
            } else {
                initializeGame();
            }
        }
    }
}

void changeDirection() {      //Changes the direction of the snake based on user input
    key = getch();
    switch (key) {            //checks the value of key
        case RIGHT:
            if (head.direction != LEFT) head.direction = RIGHT;
            break;
        case LEFT:
            if (head.direction != RIGHT) head.direction = LEFT;
            break;
        case UP:
            if (head.direction != DOWN) head.direction = UP;
            break;
        case DOWN:
            if (head.direction != UP) head.direction = DOWN;
            break;
        case 27:
            exit(0);
    }
}

void updateSnakeBody() {
    coordinate prev = head;           //Saving the Current Head Positio
    coordinate temp;
    for (int i = 0; i < length; i++) {  //iterates through each segment of the snake's body
        temp = body[i];          //Temporarily saves the current position of the body segment at index i
        body[i] = prev;          //Updates the body segment at index i to the previous position
        prev = temp;      //prev for the next iteration of the loop, ensuring the next body segment moves to the correct position.
    }

    switch (head.direction) {
        case RIGHT: head.x++; break;
        case LEFT: head.x--; break;
        case UP: head.y--; break;
        case DOWN: head.y++; break;
    }
    gotoxy(head.x, head.y); printf("0:");
    for (int i = 0; i < length; i++) {     //Moves the cursor to the new position of the snake's head
        gotoxy(body[i].x, body[i].y);
        printf("*");
    }
}

void recordScore() {
    char name[20];
    printf("Enter your name: ");
    scanf("%s", name);

    FILE *file = fopen("record.txt", "a");
    if (file == NULL) {
        printf("Unable to open file for recording score.\n");
        return;
    }

    fprintf(file, "Name: %s, Score: %d\n", name, calculateScore());
    fclose(file);
}

int calculateScore() {
    return length - 5;
}
