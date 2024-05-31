/*------- File name: Game.c ---------*
* Minesweeper Game                   *
*                                    *
* Amit Erlich (206362568) 27/11/2020 *
*------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 22
#define SIZE2 2

// FUNCTIONS DECLARATION
void Menu(int rowAndCol[], int size);
void printMenu();

// Game functions
void Game(int board[][SIZE], int rows, int cols);
void getUserRowColChoice(int rowAndCol[], int board[][SIZE], int rows, int cols);
char openOrMark();

// Set \ fill board functions
void setEmptyBoard(int arr[][SIZE], int rows, int cols);
void fillArray(int arr[][SIZE], int rows, int cols);
void fillArrWithNum(int arr[][SIZE], int rows, int cols);

// Print functions
void printBoard(int board[][SIZE], int rows, int cols, int revealAll);
void printLineOfNum(int col);
void printALine(int col);
void clearScreen();

// Reveal board functions
int openAllEpmtySpotsNear(int board[][SIZE], int rows, int cols, int currentRow, int currentCol);
void revealBoard(int board[][SIZE], int rows, int cols, int revealNum);

int checkIfWonOrLost(int revealedSpots, int board[][SIZE], int rows, int cols, int row, int col);


/* -------------------- main --------------------
* Prints game's menu and starts the selected game
*
* 1. Prints menu and gets user choice
* 2. If user does not choose to exit (0), then:
*   1. Sets an empty board in size 22X22
*   2. Start the selected game
* 3. Else - exit program
-----------------------------------------------*/
void main()
{
	int boardSize;;
	int board[SIZE][SIZE];
	int rowAndCol[2]; // contains row and col value selected by user's game choice

	Menu(rowAndCol, SIZE2); // get user choice from menu
	clearScreen();

	// enter if user want to play, else - exit program
	if (rowAndCol[0] != 0)
	{
		setEmptyBoard(board, SIZE, SIZE);

		// calls for the selected game
		Game(board, rowAndCol[0], rowAndCol[1]);
	}
	printf("\nBye bye!\n\n");
}

/* ----------------------------------- Menu ------------------------------------
* Gets: rowAndCol[], size
*
* Prints the menu and gets user choice.
* Check if the choice is valid - if not ask user again until his choice is valid
* By user selection, the board size (row and col) is saved in rowAndCol
------------------------------------------------------------------------------*/
void Menu(int rowAndCol[], int size)
{
	int choice;

	printMenu(); // prints the menu
	scanf("%d", &choice);

	// if user entered invalid choice (not between 0-4)
	while (choice < 0 || choice > 4)
	{
		clearScreen();
		printf("Invalid choice.\n\n");
		printMenu(); // prints the menu
		scanf("%d", &choice);
	}

	// gets board size according to the user's choice
	switch (choice)
	{
	case 1:
	{
		rowAndCol[0] = 8;
		rowAndCol[1] = 8;
		break;
	}
	case 2:
	{
		rowAndCol[0] = 12;
		rowAndCol[1] = 12;
		break;
	}
	case 3:
	{
		rowAndCol[0] = 15;
		rowAndCol[1] = 15;
		break;
	}
	case 4: // ask user to enter row and col for custom size
	{
		printf("Please enter the size of the board (Rows and cols between 1-22):\n");
		scanf("%d %d", &rowAndCol[0], &rowAndCol[1]);

		// if input is not valid
		while (rowAndCol[0] < 1 || rowAndCol[0] > 22 || rowAndCol[1] < 1 || rowAndCol[1] > 22)
		{
			printf("Invalid choice.\n\n");
			printf("Please enter the size of the board (Rows and cols between 1-22):\n");
			scanf("%d %d", &rowAndCol[0], &rowAndCol[1]);
		}
		break;
	}
	default: // if user choose to exit
		rowAndCol[0] = 0;
		rowAndCol[1] = 0;
		break;
	}
}

/* --- printMenu ---
* Prints the menu
------------------*/
void printMenu()
{
	printf(" --------------------- Welcome to Minesweeper! --------------------- \n");
	printf("|                                                                   |\n");
	printf("| Please choose one of the following options and enter it's number: |\n");
	printf("|                                                                   |\n");
	printf("| (1) 8X8                                                           |\n");
	printf("| (2) 12X12                                                         |\n");
	printf("| (3) 15X15                                                         |\n");
	printf("| (4) Custom size                                                   |\n");
	printf("| (0) Exit                                                          |\n");
	printf("|                                                                   |\n");
	printf(" ------------------------------------------------------------------- \n");
	printf("> Your choice: ");
}

/* ----------------- setEmptyBoard -----------------
* Gets: arr[][SIZE], rows, cols
*
* Sets arr[][SIZE] with empty spots (empty means -9)
--------------------------------------------------*/
void setEmptyBoard(int arr[][SIZE], int rows, int cols)
{
	int i, j;

	// fill array with empty spots (= -9)
	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			arr[i][j] = -9;
}

/* -------------------------------------- Game ---------------------------------------
* Gets: board[][SIZE], rows, cols
*
* Sets board with bombs and numbers
* As long as the user has not won or lost, the game continues:
* The game board is displayed and user enter his choice
* After checking the integity of the user's input,
* depending on the input, multiple squares or one square are exposed or marked.
* Checks if the game is over, if so - a message is displayed accordingly (won or lost)
* Else - game continues.
------------------------------------------------------------------------------------*/
void Game(int board[][SIZE], int rows, int cols)
{
	int row, col, exit = 0; // exit is a flag variable: when exit != 0, exit game (stop game loop)
	int revealedSpots = 0; // count revealed spots in the board to know when user has won
	int rowAndCol[2]; // user's choice for a selected spot (contains row and col)
	char openMark;

	fillArray(board, rows, cols); // sets game board (fills board with bombs and numbers)

	while (exit == 0) // as long as the user has not won or lost
	{
		// prints the game board
		clearScreen();
		printBoard(board, rows, cols, 0);
		
		// gets row and col from user
		getUserRowColChoice(rowAndCol, board, rows, cols);
		row = rowAndCol[0];
		col = rowAndCol[1];
		
		// if row = -1, open a number (col value) of spots
		if (row == -1)
		{
			revealBoard(board, rows, cols, col);
			revealedSpots += col;
		}
		else
		{
			// ask user if to open or mark the selected spot
			openMark = openOrMark();

			// mark with 'F'
			if (openMark == 'f' || openMark == 'F')
			{
				board[row][col] -= 10;
			}
			else
			{
				// if current spot is marked and needs to be revealed
				if (board[row][col] <= -10)
					board[row][col] += 10;
				// if current spot is a bomb
				if (board[row][col] == -9)
					revealedSpots += openAllEpmtySpotsNear(board, rows, cols, row, col);// , 0);
				else // reveal current spot
				{
					board[row][col] = (-1) * board[row][col]; //reveal this spot
					revealedSpots++;
				}
			}
		}
		// check if game is over (user won or lost)
		exit = checkIfWonOrLost(revealedSpots, board, rows, cols, row, col);
	}
}

/* ------------ getUserRowColChoice --------------
* Gets: rowAndCol[], board[][SIZE], rows, cols
*
* Asks user for row and col.
* If input invalid - asks again until input is ok.
* Else - saves row and col in rowAndCol[].
-------------------------------------------------*/
void getUserRowColChoice(int rowAndCol[], int board[][SIZE], int rows, int cols)
{
	printf("\n");
	printf("Please enter your move, row and column: ");
	scanf("%d %d", &rowAndCol[0], &rowAndCol[1]); // gets user's input
	//rowAndCol[0] is row, rowAndCol[1] is col
	
	// while row or col invalid or the spot has already been revealed - ask for different input
	while (rowAndCol[0] < 0 || rowAndCol[0] >= rows || rowAndCol[1] < 0 || rowAndCol[1] >= cols || board[rowAndCol[0]][rowAndCol[1]] > 0)
	{
		if (rowAndCol[0] == -1) // if row = -1 it is ok, quit loop
		{
			if (rowAndCol[1] < 0)
				printf("\nPlease enter a valid choice!\n");
			else
				break;
		}
		else if (rowAndCol[0] < 0 || rowAndCol[0] >= rows || rowAndCol[1] < 0 || rowAndCol[1] >= cols) // if row or col exceeding the array
			printf("\nPlease enter a valid choice!\n");
		else // if the spot has already been revealed
			printf("\nThis spot has already been revealed.\nPlease enter your move, row and column: ");

		scanf("%d %d", &rowAndCol[0], &rowAndCol[1]); // gets user's input
	}
}

/* ----------------- openOrMark ------------------
* Returns: 'O'(open) or 'F'(flag)
*
* Asks user if to open or mark the selected spot.
* If input invalid - asks again until input is ok.
------------------------------------------------*/
char openOrMark()
{
	char openMark;

	printf("Please choose (O) or (F) - open or flag this spot: ");
	scanf(" %c", &openMark);

	// if input invalid
	while (openMark != 'o' && openMark != 'O' && openMark != 'f' && openMark != 'F')
	{
		printf("\nPlease enter a valid choice!\n");
		printf("Please choose (O) or (F) - open or flag this spot: ");
		scanf(" %c", &openMark);
	}
	return openMark;
}

/* ------------------------- fillArray -------------------------
* Gets: arr[][SIZE], rows, cols
*
* Fills an array with an amount of bombs equal to rows randomly,
* and fills in numbers around the bombs.
--------------------------------------------------------------*/
void fillArray(int arr[][SIZE], int rows, int cols)
{
	int i, row, col, bombsNum;

	bombsNum = sqrt(rows * cols);

	// fill array with random bombs
	srand(time(NULL));

	for (i = 0; i < bombsNum; i++)
	{
		row = rand() % rows;
		col = rand() % cols;

		// if there is already a bomb in this spot - draw row and col again
		while (arr[row][col] == 0)
		{
			row = rand() % rows;
			col = rand() % cols;
		}
		// mark a bomb with a 0
		arr[row][col] = 0;
	}

	// fill array with numbers that marks the amount of mines around every spot
	fillArrWithNum(arr, rows, cols);
}

/* ----------------------- fillArrWithNum ------------------------
* Gets: arr[][SIZE], rows, cols
*
* Goes through all cells in the array.
* If the current cell is not a bomb - count bombs around the cell
* and fills it with the number in count * (-1).
* 
* 0 = a bomb, 8-1 = number of bombs around the area
* Negative number means a hidden spot, a bomb (0) is always hidden
* (if exposed - game over)
----------------------------------------------------------------*/
void fillArrWithNum(int arr[][SIZE], int rows, int cols)
{
	int i, j, row, col, countBomb;

	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++)
		{
			countBomb = 0; // reset countBomb

			// if there is no bomb in this cell
			if (arr[row][col] != 0)
			{
				// check around current cell if there are bombs and count them
				for (i = -1; i <= 1; i++)
				{
					for (j = -1; j <= 1; j++)
					{
						if (!(row + i < 0 || row + i > rows))
						{
							if (!(col + j < 0 || col + j > cols))
								if (row + i >= 0 && row + i < rows && col + j >= 0 && col + j < cols && arr[row + i][col + j] == 0)
									countBomb++; // if there is a bomb count it
						}
					}
				}
				// if there are bombs around current spot, put that number in the cell
				if (countBomb > 0)
					arr[row][col] = countBomb * (-1);
			}
		}
	}
}

/* -------------------------- printBoard ---------------------------
* Gets: board[][SIZE], rows, cols, revealAll
*
* If revealAll is true - reveal all hidden spots on the board.
* Print the board on the screen.
*
* 0 = a bomb, 8-1 = number of bombs around the area, 9 = empty cell.
* Negative number means a hidden spot, if number < -10 = marked spot
* A bomb (0) is always hidden (if exposed - game over).
------------------------------------------------------------------*/
void printBoard(int board[][SIZE], int rows, int cols, int revealAll)
{
	int i, j;

	if (revealAll) // if true reveal board
		revealBoard(board, rows, cols, rows * cols);

	printLineOfNum(cols); // print board header

	for (i = 0; i < rows; i++)
	{
		printf("   %2d", i);
		for (j = 0; j < cols; j++)
		{
			if (board[i][j] == 0 && revealAll == 1) // if need to reveal all and current cell is a bomb
				printf(" | *");
			else if (board[i][j] <= -10) // if the current cell is a marked spot
				printf(" | F");
			else if (board[i][j] <= 0) // if the current cell is a hidden spot
				printf(" | X");
			else if (board[i][j] == 9) // if the current cell is an empty spot and not hidden
				printf(" |  ");
			else // if the current cell is a not hidden number spot
				printf(" |%2d", board[i][j]);
		}
		printf("\n");
	}
	printALine(cols); // print "_" line at the bottom of the board
}

/* -------------------------- printLineOfNum ---------------------------
* Gets: col
*
* Prints the board header (a line of col numbers and a ("_") line below)
----------------------------------------------------------------------*/
void printLineOfNum(int col)
{
	int i, j;

	printf("\n       ");

	for (j = 0; j < col; j++)
	{
		printf("%2d  ", j);
	}
	printf("\n");
	
	printALine(col);
}

/* --------- printALine ---------
* Gets: col
*
* Prints "_" line under the board
-------------------------------*/
void printALine(int col)
{
	int i;

	printf("   ");

	for (i = 0; i < col; i++)
		(i + 1 == col) ? (printf("_______")) : (printf("____"));
	printf("\n");
}

/* ------------------ revealBoard -------------------
* Gets: board[][SIZE], rows, cols, revealNum
*
* Reveals cells as the number contained in revealNum.
* (Positive numbers are revealed cells.)
---------------------------------------------------*/
void revealBoard(int board[][SIZE], int rows, int cols, int revealNum)
{
	int i, j, counter = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (board[i][j] < 0) // if the current cell is not already revealed
			{
				if (board[i][j] <= -10) // if the current cell is marked
				{
					if (revealNum == rows * cols) // if needs to reveal all board - reveal this spot too
					{
						board[i][j] += 10;
						board[i][j] = (-1) * board[i][j]; // saves the positive number
						counter++;
					}
				}
				else
				{
					board[i][j] = (-1) * board[i][j]; // saves the positive number
					counter++;
					if (counter >= revealNum) // if revealed the number of cell that needed to be revealed - stop revealing
						break;
				}				
			}
		}
		// if revealed the number of cell that needed to be revealed - stop revealing
		if (counter >= revealNum)
			break;
	}
}

/* ------------------------ openAllEpmtySpotsNear -------------------------
* Gets: board[][SIZE], rows, cols, currentRow, currentCol
* Returns: the number of empty spots exposed in this action
*
* Checks if the current spot is empty and reveal it and all spots around it
-------------------------------------------------------------------------*/
int openAllEpmtySpotsNear(int board[][SIZE], int rows, int cols, int currentRow, int currentCol)
{
	int countRevealSpot = 0;
	int i, j;

	if (board[currentRow][currentCol] < 0) // if current spot is hidden
	{
		board[currentRow][currentCol] = (-1) * board[currentRow][currentCol]; // reveal this spot
		countRevealSpot++;

		if (board[currentRow][currentCol] == 9) // if current spot is an empty spot - reveal all 8 spots around
		{
			for (i = -1; i <= 1; i++)
			{
				for (j = -1; j <= 1; j++)
				{
					if (currentRow + i >= 0 && currentRow + i <= rows - 1)
					{
						if (currentCol + j >= 0 && currentCol + j <= cols - 1)
							countRevealSpot += openAllEpmtySpotsNear(board, rows, cols, currentRow + i, currentCol + j);
					}
				}
			}
		}
		return countRevealSpot;
	}
	return 0;
}

/* ---------------------- checkIfWonOrLost ----------------------
* Gets: revealedSpots, board[][SIZE], rows, cols, row, col
* Returns: 1 - if won or lost, 0 - if neither.
*
* Checks if user has won or lost and print a message accordingly.
---------------------------------------------------------------*/
int checkIfWonOrLost(int revealedSpots, int board[][SIZE], int rows, int cols, int row, int col)
{
	if (row != -1 && board[row][col] == 0) // if hit a bomb
	{
		clearScreen();
		printf("    --------------------------- \n");
		printf("   | Oops!! You've hit a bomb! |\n");
		printf("   |       > GAME OVER <       |\n");
		printf("    --------------------------- \n");
		printBoard(board, rows, cols, 1);
		return 1;
	}
	else if (revealedSpots >= (rows * cols) - sqrt(rows * cols)) // if revealed all cells
	{
		clearScreen();
		printf("    -------------------------- \n");
		printf("   |      WOW! You won!!!     |\n");
		printf("   |       > WELL DONE <      |\n");
		printf("    -------------------------- \n");
		revealBoard(board, rows, cols, rows * cols);
		printBoard(board, rows, cols, 0);
		return 1;
	}
	return 0; // else - game is not over yet
}

/* --- clearScreen ---
* Cleans the screen
--------------------*/
void clearScreen()
{
	system("cls");
}