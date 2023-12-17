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

#include <iostream>
#include <iomanip> // setw() for the fieldSize


const unsigned short MAX_ROWS = 24;
const unsigned short MAX_COLS = 80;

void validateChoice(char& choice, char from, char to) {
    while (!(choice >= from && choice <= to)) {
        std::cout << "Invalid input, try again" << std::endl;
        std::cin >> choice;
    }
}

char displayStartingMenu() {

    std::cout << "--------GAME--OF--LIFE--------" << std::endl << std::endl;
    std::cout << "Choose one from the following:" << std::endl << std::endl;
    std::cout << "1. New game" << std::endl;
    std::cout << "2. Load file" << std::endl;
    std::cout << "3. Exit" << std::endl << std::endl;
    std::cout << "Give one symbol only: ";

    char choice;
    std::cin >> choice;

    validateChoice(choice,'1','3');
    
    std::cout << std::endl;

    return choice;
}

short displayIngameMenu() {
    std::cout << "Ingame menu:" << std::endl;
    std::cout << "1. Step forward" << std::endl;
    std::cout << "2. Resize" << std::endl;
    std::cout << "3. Toggle cell" << std::endl;
    std::cout << "4. Clear" << std::endl;
    std::cout << "5. Randomize" << std::endl;
    std::cout << "6. Save to file" << std::endl;
    std::cout << "7. End" << std::endl;
    std::cout << "Give one symbol only: ";

    char choice;
    std::cin >> choice;

    validateChoice(choice, '1', '7');

    std::cout<<std::endl;

    return choice;

}

short countDigits(int num) {
    if (!num) {
        return 1; //edge case if num = 0
    }

    int res = 0;
    while (num) {
        num /= 10;
        res++;
    }

    return res;
}

void printField(bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols) {

    std::cout << std::setw(countDigits(curRows) + 1) << " ";
    std::cout << 1 << std::setw(curCols) << curCols << std::endl; //for first line with coords



    for (unsigned short i = 0; i < curRows; i++)
    {
        // for coords on the leftside
        if (i == 0) // start coordinate 1
            std::cout << std::setw(countDigits(curRows)) << 1 << ' '; //
        else if (i == curRows - 1) //end coordinate
            std::cout << curRows << ' ';
        else
            std::cout << " " << std::setw(countDigits(curRows) + 1);
        
        for (unsigned short j = 0; j < curCols; j++)
        {
            if (arr[i][j])
                std::cout << '@'; //if alive
            else
                std::cout << '-'; //if dead
          
        }
        std::cout << std::endl; //start printing new row
    }

    std::cout << std::endl;
}

void initMatrixWithValue(bool arr[][MAX_COLS], bool value) {

    for (short i = 0; i < MAX_ROWS; i++)
    {
        for (short j = 0; j < MAX_COLS; j++)
        {
            arr[i][j] = value;
        }
    }
}

bool isCellValid(int x, int y, unsigned short curRows, unsigned short curCols) {
    if (x >= 1 && x <= curCols && y >= 1 && y <= curRows)
        return true;

    return false;
}

void toggleCell(bool arr[][MAX_COLS], unsigned short curRows, unsigned short curCols) {

    std::cout << "Give two numbers - coordinates of the cell: ";
    int x, y;
    std::cin >> x >> y;
    
    std::cout << std::endl;
    while (!isCellValid(x, y, curRows, curCols))
    {
        std::cout << "Invalid coordinates, try again: ";
        std::cin >> x >> y;
        std::cout << std::endl;
    }

    int row = y - 1;
    int col = x - 1;
    arr[row][col] = !arr[row][col];
}

void playGame(bool cur[][MAX_COLS], bool old[][MAX_COLS]) {
    initMatrixWithValue(cur, false);
    initMatrixWithValue(old, false);

    unsigned short curRows = 8;
    unsigned short curCols = 16;
    bool inGame = true;


    while (inGame) {
        printField(cur, curRows, curCols);
        unsigned short choice = displayIngameMenu();

        switch (choice)
        {
        case '1': break;
        case '2': break;
        case '3':  toggleCell(cur, curRows, curCols); break;
        case '4': break;
        case '5': break;
        case '6': break;
        case '7': break;
        default:
            break;
        }
    }
    
}

int main()
{

    char input = displayStartingMenu();

    if (input == '3') //if player choses Exit
        return 0;

    bool curField[MAX_ROWS][MAX_COLS]; // the cells are "alive"(true) or "dead"(false)
    bool oldFIeld[MAX_ROWS][MAX_COLS]; // needed for

    if (input == '1')
        playGame(curField, oldFIeld);
    else if (input == '2') {
        //loadFile(curField, oldFIeld);
        playGame(curField, oldFIeld);

    }
}
