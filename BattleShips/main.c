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

void startGame(char gameBoard[10][10], Ship* pShips, Stats* pStats);

void userInput(char gameBoard[10][10], Ship pShips[5], Stats pStats);

void displayGame(char gameBoard[10][10], Ship ships[5], Stats pstats);

void playGame(char[][COL], Ship ships[], Stats*);

bool getInput(char[]);

int getNum(char input[]);


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
	Stats game = { 0, 0, 0 };

	setupBoard(gameBoard, ships);
	startGame(gameBoard, ships, &game);

	playGame(gameBoard, ships, &game);

	return 0;
}

void startGame(char gameBoard[10][10], Ship* pShips, Stats* pStats) {
	displayGame(gameBoard, pShips, *pStats);
	// userInput(gameBoard, pShips, *pStats);
}


void userInput(char gameBoard[10][10], Ship pShips[5], Stats pStats) {
	printf("\nEnter a letter than number: ");
	scanf("%...");
}

void displayGame(char gameBoard[10][10], Ship ships[5], Stats stats) {
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
			if (gameBoard[i][j] == 'M' || gameBoard[i][j] == 'H') // H = hit, M = miss
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
		}
		else {
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
		}
		else {
			if (gameBoard[row][col + i] != ' ')
				return true;
		}
	}

	return false;
}


void playGame(char gameBoard[][COL], Ship ships[], Stats* pstats) {

	char playerInput[4];


	// bool winGame = winGame();
	// separate function to determine game win

	while (strcmp(playerInput, "QQ") != 0 /*  || !winGame */) {

		bool badInput = getInput(playerInput);

		while (badInput == true)
			badInput = getInput(playerInput);

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
		displayGame(gameBoard, ships, *pstats);
	}
}


bool getInput(char input[]) {

	printf("Enter column letter + row number (Ex: A1) (QQ to quit): ");
	scanf("%s", input);

	_strupr(input);

	if (strcmp(input, "QQ") == 0)
		return false;


	else if (strlen(input) > 3 || strlen(input) == 0) {
		printf("Invalid input!\n");
		return true;
	}


	else if (isalpha(input[0]) == 0 || isdigit(input[1]) == 0) {
		printf("Invalid input!\n");
		return true;
	}

	else if (input[0] - 'A' >= 10 || input[1] - '0' == 0) {
		printf("Invalid input!\n");
		return true;
	}

	else if (input[1] - '0' == 1 && input[2] - '0' > 0) {
		printf("Invalid input!\n");
		return true;
	}

	else if (input[1] - '0' > 1 && input[2] - '0' >= 0) {
		printf("Invalid input!\n");
		return true;
	}

	else
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