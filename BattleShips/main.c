/*
* Project Team: Neil, Bao, James, David
* Date: 9/28/2023
* Purpose: Making a really cool game!
*/

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

void displayGame(char gameBoard[10][10], Ship ships[5], Stats stats);

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

	return 0;
}

void startGame(char gameBoard[10][10], Ship* pShips, Stats* pStats) {
	displayGame(gameBoard, pShips, *pStats);

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
			if (gameBoard[i][j] == 'H' || gameBoard[i][j] == 'M')
				printf(" %c |", gameBoard[i][j]);
			else
				printf("   |");
		}
		printf("\n");
	}

	for (int i = 0; i < 5; i++)
		printf("\n%s (%c): %i", ships[i].name, ships[i].letter, ships[i].hits);

	printf("\n\nShots Fired: %i", stats.shotsFired);
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
