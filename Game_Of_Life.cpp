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
#include <fstream> // for reading and writing in files
#include <iomanip> // setw() for the fieldSize
#include <iostream>

constexpr unsigned short MAX_ROWS = 24 + 2; // +2 for easier expansion
constexpr unsigned short MAX_COLS = 80 + 2;

const unsigned short STARTING_ROWS = 8;
const unsigned short STARTING_COLS = 16;

const unsigned short STARTING_MENU_OPTIONS = 3; // in case we want to add/remove functionalities
const unsigned short INGAME_MENU_OPITONS = 7;

const unsigned short MAX_FILENAME_LENGTH = 260;

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

unsigned myStrlen(const char *str)
{
    if (!str)
        return 0;
    unsigned res = 0;

    while (*str)
    {
        res++;
        str++;
    }
    return res;
}

void printField(const bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols)
{
    std::cout << std::setw(countDigits(curRows) + 1) << " ";
    std::cout << 1;

    if (curCols != 1)
        std::cout << std::setw(curCols - 2 + countDigits(curCols)) << curCols;
    // for first line with coords , curCols - 2 is the number of spaces between 1 and curCols

    std::cout << std::endl;

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

    int newCols = getInputBetween(1, MAX_COLS - 2); // max is -2, because of the expansion
    int newRows = getInputBetween(1, MAX_ROWS - 2);

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

bool isRowEmpty(const bool arr[][MAX_COLS], unsigned short row, unsigned short curCols)
{
    for (unsigned short i = 0; i < curCols; i++)
    {
        if (arr[row][i] == true)
            return false;
    }

    return true;
}

bool isColEmpty(const bool arr[][MAX_COLS], unsigned short col, unsigned short curRows)
{
    for (unsigned short i = 0; i < curRows; i++)
    {
        if (arr[i][col] == true)
            return false;
    }

    return true;
}

void findUnemptyRectangle(const bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols,
                          unsigned short &startRow, unsigned short &endRow, unsigned short &startCol,
                          unsigned short &endCol)
{
    while (isRowEmpty(arr, startRow, curCols) && startRow < curRows)
        startRow++;

    while (isRowEmpty(arr, endRow, curCols) && endRow > 0)
        endRow--;

    while (isColEmpty(arr, startCol, curRows) && startCol < curCols)
        startCol++;

    while (isColEmpty(arr, endCol, curRows) && endCol > 0)
        endCol--;
}

void saveToFile(const bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols)
{
    std::cout << "The given field will be saved with the name you chose: ";
    char name[MAX_FILENAME_LENGTH];
    std::cin >> name;
    std::cout << std::endl;

    unsigned short startRow = 0;
    unsigned short endRow = curRows - 1;
    unsigned short startCol = 0;
    unsigned short endCol = curCols - 1;

    findUnemptyRectangle(arr, curRows, curCols, startRow, endRow, startCol, endCol);

    std::ofstream ofs(name);

    if (!ofs.is_open())
    {
        std::cout << "Saving failed!" << std::endl;
        return;
    }

    for (unsigned short i = startRow; i <= endRow; i++)
    {
        for (unsigned short j = startCol; j <= endCol; j++)
        {
            ofs.put(arr[i][j] == true ? '@' : '-');
        }
        ofs.put('\n');
    }

    ofs.clear();
    ofs.close();
}

bool isInRange(int a, int b, int rows, int cols)
{
    return a >= 0 && a < rows && b >= 0 && b < cols;
}

unsigned short countAliveNeighbors(const bool arr[][MAX_COLS], int row, int col, unsigned short curRows,
                                   unsigned short curCols)
{

    return (isInRange(row - 1, col - 1, curRows, curCols) && arr[row - 1][col - 1]) +
           (isInRange(row - 1, col, curRows, curCols) && arr[row - 1][col]) +
           (isInRange(row - 1, col + 1, curRows, curCols) && arr[row - 1][col + 1]) + // upper line

           (isInRange(row, col - 1, curRows, curCols) && arr[row][col - 1]) +
           (isInRange(row, col + 1, curRows, curCols) && arr[row][col + 1]) + // curLine

           (isInRange(row + 1, col - 1, curRows, curCols) && arr[row + 1][col - 1]) +
           (isInRange(row + 1, col, curRows, curCols) && arr[row + 1][col]) +
           (isInRange(row + 1, col + 1, curRows, curCols) && arr[row + 1][col + 1]); // lowerLine.
}

void shiftRight(bool arr[][MAX_COLS], unsigned positions, unsigned short &curRows, unsigned short &curCols)
{
    for (int col = curCols - 1 + positions; col >= positions; col--) // shift right
    {
        for (unsigned short row = 0; row < curRows; row++)
        {
            arr[row][col] = arr[row][col - positions];
        }
    }

    for (int i = 0; i < positions; i++) // set cols at the first positions to false
    {
        deleteCol(arr, i, curRows);
    }
    curCols++;
}

void shiftDown(bool arr[][MAX_COLS], unsigned positions, unsigned short &curRows, unsigned short &curCols)
{
    for (int row = curRows - 1 + positions; row >= positions; row--) // shift down
    {
        for (unsigned short col = 0; col < curCols; col++)
        {
            arr[row][col] = arr[row - positions][col];
        }
    }

    for (unsigned short i = 0; i < positions; i++) // set rows at the first positions to false
    {
        deleteRow(arr[i], curCols);
    }
    curRows++;
}

void expand(bool arr[][MAX_COLS], unsigned positions, unsigned short &curRows,
            unsigned short &curCols) // just move all cells 1 down and 1 to right
{
    shiftRight(arr, positions, curRows, curCols);
    shiftDown(arr, positions, curRows, curCols);

    curRows++; // to include the last empty row
    curCols++; // to include the last empty col
}

void stepForward(bool arr[][MAX_COLS], unsigned short &curRows, unsigned short &curCols)
{
    if (curRows < MAX_ROWS - 2 && curCols < MAX_COLS - 2) //-2 because of expansion on both sides
        expand(arr, 1, curRows, curCols);                 // expand with 1 down and right

    bool helpField[MAX_ROWS][MAX_COLS]; // needed for independent check, also for better transfer of the new field
    initMatrixWithValue(helpField, false);

    for (unsigned short i = 0; i < curRows; i++) // fill the helpField
    {
        for (unsigned short j = 0; j < curCols; j++)
        {
            unsigned short aliveNeighbors = countAliveNeighbors(arr, i, j, curRows, curCols);

            if (arr[i][j])
            {
                helpField[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3);
            }
            else
            {
                helpField[i][j] = (aliveNeighbors == 3);
            }
        }
    }

    unsigned short startRow = 0;
    unsigned short endRow = curRows - 1;
    unsigned short startCol = 0;
    unsigned short endCol = curCols - 1;

    // check if the new rows/cols contain any live cells
    if (isRowEmpty(helpField, startRow, curCols))
    {
        startRow++;
    }
    if (isRowEmpty(helpField, endRow, curCols))
    {
        endRow--;
    }
    if (isColEmpty(helpField, startCol, curRows))
    {
        startCol++;
    }
    if (isColEmpty(helpField, endCol, curRows))
    {
        endCol--;
    }

    // initMatrixWithValue(arr, false); //clear before filling

    for (unsigned short row = startRow, i = 0; row <= endRow; row++, i++) // Update the matrix with the new values
    {
        for (unsigned short col = startCol, j = 0; col <= endCol; col++, j++)
        {
            arr[i][j] = helpField[row][col];
        }
    }

    curRows = endRow - startRow + 1; // distance between start - end
    curCols = endCol - startCol + 1;

    if (curCols >= MAX_COLS - 2)
    { //-2 because of expansion on both sides
        curCols = MAX_COLS - 2;
        std::cout << "Warning! Max width reached!" << std::endl;
    }
    if (curRows >= MAX_ROWS - 2)
    { //-2 because of expansion on both sides
        curRows = MAX_ROWS - 2;
        std::cout << "Warning! Max height reached!" << std::endl;
    }
}

void playGame(bool cur[][MAX_COLS], unsigned short &curRows, unsigned short &curCols)
{

    bool inGame = true;

    while (inGame)
    {
        printField(cur, curRows, curCols);
        displayInGameMenu();

        unsigned short choice = getInputBetween(1, INGAME_MENU_OPITONS); // options 1 - last

        switch (choice)
        {
        case 1:
            stepForward(cur, curRows, curCols);
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
            saveToFile(cur, curRows, curCols);
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

bool loadFile(bool cur[][MAX_COLS], unsigned short &curRows, unsigned short &curCols)
{
    char fileName[MAX_FILENAME_LENGTH];
    std::cout << "Write the name of the file which you want to be loaded: ";
    std::cin >> fileName;
    std::cout << std::endl;
    std::ifstream ifs(fileName);

    if (!ifs.is_open())
    {
        std::cout << "Loading failed!" << std::endl;
        return false;
    }

    int start = ifs.tellg(); // to remember start of the stream

    char firstLine[MAX_COLS];
    ifs.getline(firstLine, MAX_COLS);
    curCols = myStrlen(firstLine);

    ifs.seekg(start); // to go back to start

    char ch;
    unsigned short x = 0;
    unsigned short y = 0;

    while (ifs >> ch)
    {
        if (ch == '-')
        {
            cur[y][x] = false;
        }
        else if (ch == '@')
        {
            cur[y][x] = true;
        }
        else
        {
            std::cout << "Loading failed!" << std::endl;
            return false;
        }

        x++;
        if (x == curCols)
        {                 // if we read the next line
            x %= curCols; // set the x to 0 and update the y which is the rows
            y++;
        }
    }
    ifs.clear();
    ifs.close();
    curRows = y;

    return true;
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

        unsigned short curRows = STARTING_ROWS;
        unsigned short curCols = STARTING_COLS;
        initMatrixWithValue(curField, false);

        if (input == 1)
            playGame(curField, curRows, curCols);
        else if (input == 2)
        {
            bool isFileLoaded = loadFile(curField, curRows, curCols);
            if (!isFileLoaded)
                continue;
            playGame(curField, curRows, curCols);
        }
    }
}
