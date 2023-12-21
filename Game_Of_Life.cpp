/**
 *
 * Solution to course project # 02
 * Introduction to programming course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2023/2024
 *
 * @author Yulian Lyutskanov
 * @idnumber 0MI0600295
 * @compiler VC
 *
 * Main File
 *
 */

#include <cstdlib> // Required for srand and rand functions
#include <ctime>   // Required for time function for srand
#include <iomanip> // setw() for the fieldSize
#include <iostream>

const unsigned short MAX_ROWS = 24;
const unsigned short MAX_COLS = 80;

const unsigned short STARTING_ROWS = 8;
const unsigned short STARTING_COLS = 16;

const unsigned short STARTING_MENU_OPTIONS = 3; // in case we want to add/remove functionalities
const unsigned short INGAME_MENU_OPITONS = 7;

void validateChoice(int &choice, int from, int to)
{
    while (!(choice >= from && choice <= to))
    {
        std::cout << "Invalid input, try again" << std::endl;
        std::cin >> choice;
    }
}

int getInputBetween(int from, int to)
{
    int choice;
    std::cin >> choice;

    validateChoice(choice, from, to);

    return choice;
}

void displayStartingMenu()
{
    std::cout << "--------GAME--OF--LIFE--------" << std::endl << std::endl;
    std::cout << "Choose one from the following:" << std::endl << std::endl;
    std::cout << "1. New game" << std::endl;
    std::cout << "2. Load file" << std::endl;
    std::cout << "3. Exit" << std::endl << std::endl;
    std::cout << "Chose an option (give a number): ";
}

void displayInGameMenu()
{
    std::cout << "Ingame menu:" << std::endl;
    std::cout << "1. Step forward" << std::endl;
    std::cout << "2. Resize" << std::endl;
    std::cout << "3. Toggle cell" << std::endl;
    std::cout << "4. Clear" << std::endl;
    std::cout << "5. Randomize" << std::endl;
    std::cout << "6. Save to file" << std::endl;
    std::cout << "7. End" << std::endl;
    std::cout << "Chose an option (give a number): ";
}

short countDigits(int num)
{
    if (!num)
    {
        return 1; // edge case if num = 0
    }

    int res = 0;
    while (num)
    {
        num /= 10;
        res++;
    }

    return res;
}

void printField(bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols)
{
    std::cout << std::setw(countDigits(curRows) + 1) << " ";
    std::cout << 1 << std::setw(curCols) << curCols << std::endl; // for first line with coords

    for (unsigned short row = 0; row < curRows; row++)
    {
        // for coords on the leftside
        if (row == 0) // first row 1
            std::cout << std::setw(countDigits(curRows)) << 1 << ' ';
        else if (row == curRows - 1) // end row
            std::cout << curRows << ' ';
        else
            std::cout << " " << std::setw(countDigits(curRows) + 1); // mid rows

        for (unsigned short col = 0; col < curCols; col++)
        {
            if (arr[row][col])
                std::cout << '@'; // if alive
            else
                std::cout << '-'; // if dead
        }
        std::cout << std::endl; // start printing new row
    }

    std::cout << std::endl;
}

void initMatrixWithValue(bool arr[][MAX_COLS], bool value)
{
    for (short i = 0; i < MAX_ROWS; i++)
    {
        for (short j = 0; j < MAX_COLS; j++)
        {
            arr[i][j] = value;
        }
    }
}

bool isCellValid(int x, int y, unsigned short curRows, unsigned short curCols)
{
    if (x >= 1 && x <= curCols && y >= 1 && y <= curRows)
        return true;

    return false;
}

void toggleCell(bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols)
{
    std::cout << "Give two numbers - coordinates of the cell: ";

    unsigned short x = getInputBetween(1, curCols); // x is the horisontal axis - Cols
    unsigned short y = getInputBetween(1, curRows); // y is the vertical axis - Rows

    std::cout << std::endl;

    unsigned short rowToToggle = y - 1; // the coordinates start from 1, not from 0
    unsigned short colToToggle = x - 1;

    arr[rowToToggle][colToToggle] = !arr[rowToToggle][colToToggle]; // toggle the chosen cell
}

void randomize(bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols)
{
    std::cout << " Give a natural number N so that the chance of a cell to be alive is 1/N: ";
    int chance;
    std::cin >> chance;
    validateChoice(chance, 0, INT_MAX);

    if (!chance)
    {
        initMatrixWithValue(arr, false);
        return;
    }

    srand(time(NULL));

    for (short row = 0; row < curRows; row++)
    {
        for (short col = 0; col < curCols; col++)
        {

            int random = rand() % chance + 1;
            arr[row][col] = (random == 1);
        }
    }
}

void deleteRow(bool arr[], unsigned short curCols)
{
    for (unsigned short i = 0; i < curCols; i++)
        arr[i] = false;
}

void deleteCol(bool arr[][MAX_COLS], unsigned short colToDelete, unsigned short curRows)
{
    for (unsigned short i = 0; i < curRows; i++)
        arr[i][colToDelete] = false;
}

void resize(bool arr[][MAX_COLS], unsigned short &curRows, unsigned short &curCols)
{
    std::cout << "Current size: " << curCols << "x" << curRows << std::endl;
    std::cout << "Give new X and Y to resize the field: ";

    int newCols = getInputBetween(1, MAX_COLS);
    int newRows = getInputBetween(1, MAX_ROWS);

    if (newCols < curCols) // delete the lost cells
    {
        for (unsigned short i = newCols; i < curCols; i++)
            deleteCol(arr, i, curRows);
    }

    if (newRows < curRows)
    {
        for (unsigned short i = newRows; i < curRows; i++)
            deleteRow(arr[i], curCols);
    }
    curRows = newRows;
    curCols = newCols;
}

void playGame(bool cur[][MAX_COLS], bool old[][MAX_COLS])
{
    initMatrixWithValue(cur, false);
    initMatrixWithValue(old, false);

    unsigned short curRows = STARTING_ROWS;
    unsigned short curCols = STARTING_COLS;
    bool inGame = true;

    while (inGame)
    {
        printField(cur, curRows, curCols);
        displayInGameMenu();

        unsigned short choice = getInputBetween(1, INGAME_MENU_OPITONS); // options 1 - last

        switch (choice)
        {
        case 1:
            break;
        case 2:
            resize(cur, curRows, curCols);
            break;
        case 3:
            toggleCell(cur, curRows, curCols);
            break;
        case 4: // clear
            initMatrixWithValue(cur, false);
            break;
        case 5:
            randomize(cur, curRows, curCols);
            break;
        case 6:
            break;
        case 7:
            std::cout << std::endl;
            inGame = false; // Exit
            break;
        default:
            break;
        }
    }
}

int main()
{
    while (true)
    {
        displayStartingMenu();
        unsigned short input = getInputBetween(1, STARTING_MENU_OPTIONS); // from first - last

        if (input == 3) // if player choses Exit
            return 0;

        bool curField[MAX_ROWS][MAX_COLS]; // the cells are "alive"(true) or "dead"(false)
        bool oldFIeld[MAX_ROWS][MAX_COLS]; // needed for

        if (input == 1)
            playGame(curField, oldFIeld);
        else if (input == 2)
        {
            // loadFile(curField, oldFIeld);
            playGame(curField, oldFIeld);
        }
    }
}
