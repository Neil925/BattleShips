/*
* Project Team: Neil, Bao, James
* Project Team: Neil, Bao, James, David, Piers
* Date: 9/28/2023
* Purpose: Making a really cool game!
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define CLS system("cls")

#define ROW 10
#define COL 10


typedef struct {
    char name[255];
    char letter;
    int length;
    int hits;
} Ship;

typedef struct {
    int shotsFired;
    int highest;
    int lowest;
} Stats;

void setupBoard(char gameBoard[ROW][COL], Ship ships[5]);

void addShip(char gameBoard[10][10], Ship ship);

bool shipImpedes(char gameBoard[10][10], bool vert, int row, int col, int length);

void displayGame(char gameBoard[10][10], char originalBoard[10][10], Ship ships[5], Stats pstats);

void playGame(char[ROW][COL], char originalGame[ROW][COL], Ship ships[], Stats *);

bool getInput(char[]);

int getNum(char input[]);

void strupr(char *input);

void clearInputBuffer();

bool isPositionSunk(char letter, Ship *ships);

int main() {
    srand(time(NULL));
    Ship ships[5] = {
            {"Zulu",   'Z', 2, 0},
            {"Yeti",   'Y', 3, 0},
            {"XMax",   'X', 3, 0},
            {"Wiener", 'W', 4, 0},
            {"Violet", 'V', 5, 0}
    };
    char gameBoard[ROW][COL];
    char originalBoard[ROW][COL];

    Stats game = {0, 0, 0};

    setupBoard(gameBoard, ships);

    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            originalBoard[i][j] = gameBoard[i][j];

    displayGame(gameBoard, originalBoard, ships, game);
    playGame(gameBoard, originalBoard, ships, &game);

    return 0;
}

void displayGame(char gameBoard[10][10], char originalBoard[10][10], Ship ships[5], Stats stats) {
    char divider[255] = "----";
    printf("   |");

    for (int i = 0; i < COL; i++) {
        strcat(divider, "----");
        printf(" %c |", 'A' + i);
    }

    printf("\n");
    strcat(divider, "\0");

    for (int i = 0; i < ROW; i++) {
        printf("%s\n", divider);
        printf("%2i |", i + 1);
        for (int j = 0; j < COL; j++) {
            if (gameBoard[i][j] == 'H' && isPositionSunk(originalBoard[i][j], ships)) {
                printf(" %c |", originalBoard[i][j]);
            } else if (gameBoard[i][j] == 'M' || gameBoard[i][j] == 'H') // H = hit, M = miss
                printf(" %c |", gameBoard[i][j]);
            else
                printf("   |");
        }
        printf("\n");
    }
    for (int i = 0; i < 5; i++) {
        if (ships[i].length == ships[i].hits)
            printf("\n%s (%c): Sunk", ships[i].name, ships[i].letter);
        else
            printf("\n%s (%c): Floating", ships[i].name, ships[i].letter);

    }

    printf("\n\nShots Fired: %i\n", stats.shotsFired);
}

bool isPositionSunk(char letter, Ship *ships) {
    for (int z = 0; z < 5; z++) {
        if (ships[z].hits == ships[z].length && ships[z].letter == letter)
            return true;
    }

    return false;
}

void setupBoard(char gameBoard[ROW][COL], Ship ships[5]) {
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            gameBoard[i][j] = ' ';

    for (int i = 0; i < 5; i++)
        addShip(gameBoard, ships[i]);
}

void addShip(char gameBoard[10][10], Ship ship) {
    bool vert = rand() % 2;
    int row = 0;
    int col = 0;
    do {
        if (vert) {
            row = rand() % (ROW - ship.length);
            col = rand() % COL;
        } else {
            row = rand() % ROW;
            col = rand() % (COL - ship.length);
        }
    } while (shipImpedes(gameBoard, vert, row, col, ship.length));

    for (int i = 0; i < ship.length; i++) {
        if (vert)
            gameBoard[row + i][col] = ship.letter;
        else
            gameBoard[row][col + i] = ship.letter;
    }
}

bool shipImpedes(char gameBoard[10][10], bool vert, int row, int col, int length) {
    for (int i = 0; i < length; i++) {
        if (vert) {
            if (gameBoard[row + i][col] != ' ')
                return true;
        } else {
            if (gameBoard[row][col + i] != ' ')
                return true;
        }
    }

    return false;
}


void playGame(char gameBoard[ROW][COL], char originalGame[ROW][COL], Ship ships[], Stats *pstats) {
    char playerInput[255];

    bool winGame = false; //winGame();

    while (strcmp(playerInput, "QQ") != 0 && !winGame) {
        bool badInput = getInput(playerInput);

        if (strcmp(playerInput, "SS") == 0) {
            FILE* ptr = fopen("saveData.bin", "wb");
            fwrite(gameBoard, sizeof(Ship), sizeof(Stats), ptr);
            printf("The game data has been saved!");
            fclose(ptr);
            continue;
        }

        if (strcmp(playerInput, "LL") == 0) {
            FILE* ptr = fopen("saveData.bin", "rb");
            fwrite(gameBoard, sizeof(Ship), sizeof(Stats), ptr);
            printf("Previous game data has been loaded!");
            fclose(ptr);
            continue;
        }

        while (badInput == true) {
            printf("\nBad input.");
            clearInputBuffer();
            badInput = getInput(playerInput);
        }

        if (strcmp(playerInput, "QQ") == 0)
            return;

        int colLetter = playerInput[0] - 'A';
        int rowNumber = getNum(playerInput);

        switch (gameBoard[rowNumber][colLetter]) {
            case ' ':
                gameBoard[rowNumber][colLetter] = 'M';
                pstats->shotsFired++;
                break;
            case 'M':
                pstats->shotsFired++;
                break;
            case 'H':
                pstats->shotsFired++;
                break;
            default:
                for (int i = 0; i < 5; i++) {
                    if (gameBoard[rowNumber][colLetter] == ships[i].letter)
                        ships[i].hits++;
                }

                gameBoard[rowNumber][colLetter] = 'H';
                pstats->shotsFired++;
                break;
        }

        CLS;
        displayGame(gameBoard, originalGame, ships, *pstats);
    }
}

void strupr(char *input) {
    for (int i = 0; i < strlen(input); i++)
        input[i] = toupper(input[i]);
}

bool getInput(char input[]) {
    printf("\nEnter column letter + row number (Ex: A1) (QQ to quit): ");
    scanf("%s", input);

    strupr(input);

    if (strcmp(input, "QQ") == 0)
        return false;

    if (strlen(input) > 3 || strlen(input) == 0)
        return true;

    if (isalpha(input[0]) == 0 || isdigit(input[1]) == 0)
        return true;

    if (input[0] - 'A' >= 10 || input[1] - '0' == 0)
        return true;

    if (input[1] - '0' == 1 && input[2] - '0' > 0)
        return true;

    if (input[1] - '0' > 1 && input[2] - '0' >= 0)
        return true;

    return false;
}

int getNum(char input[]) {
    int result;

    if (strlen(input) == 2)
        result = input[1] - '1';
    else
        result = 9;

    return result;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}