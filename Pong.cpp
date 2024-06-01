#include <iostream>
#include "Pong.h"

#define BALL_CHAR 'O'

#ifdef _WIN32
#include <Windows.h>

void getTerminalSizeInRowsAndColumns(int& rows, int& columns) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void getTerminalSizeInRowsAndColumns(int& rows, int& columns) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    rows = w.ws_row;
    columns = w.ws_col;
}
#endif


PlayField::PlayField(int rows, int cols) {
    location = new char* [rows];
    for (int i = 0; i < rows; i++) {
        location[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            location[i][j] = ' ';
        }
    }
    this->rows = rows;
    this->cols = cols;

    int centerXOfScreen = cols / 2;
    int centerYOfScreen = rows / 2;

    location[centerYOfScreen][centerXOfScreen] = BALL_CHAR;
    ballLoc.y = centerYOfScreen;
    ballLoc.x = centerXOfScreen;
    prevBallLoc.y = centerYOfScreen;
    prevBallLoc.x = centerXOfScreen;

    // 15% of the whole screen
    barrierSize = rows * 0.15;
    if (barrierSize % 2 == 0) { ++barrierSize; }

    barrierOneStart = centerYOfScreen - (barrierSize / 2);
    barrierTwoStart = barrierOneStart;
    prevBarOneY = barrierOneStart;
    prevBarTwoY = barrierOneStart;


    for (int i = barrierOneStart; i < barrierOneStart + barrierSize; i++) {
        location[i][0] = '|';
        location[i][cols - 1] = '|';
    }

    for (int i = 0; i < cols; i++) {
        location[0][i] = '-';
        location[rows - 1][i] = '-';
    }
}

PlayField::~PlayField() {
    for (int i = 0; i < rows; i++) {
        delete[] location[i];
    }
    delete[] location;
}

bool PlayField::MoveBall() {
    if (ballLoc.x + velocity.x < 1) {
        int nextLocation = ballLoc.y + velocity.y;
        if (nextLocation >= barrierOneStart && nextLocation <= barrierOneStart + barrierSize - 1) {
            ballLoc.x = 1;
            velocity.x *= -1;
            bouncesCount++;
            if (bouncesCount % 10 == 0) {
                velocity.x > 0 ? velocity.x++ : velocity.x--;
            }

            int barrierCenter = barrierOneStart + barrierSize / 2;
            if (nextLocation == barrierCenter) {
                velocity.y = 0;
            }
            else {
                velocity.y += (barrierCenter - ballLoc.y) * -1;
            }

        }
        else{
            system("cls");
            std::cout << "Player two won!";
            return true;
        }
    }
    else if (ballLoc.x + velocity.x > cols - 2) {
        int nextLocation = ballLoc.y + velocity.y;

        if (nextLocation >= barrierTwoStart && nextLocation <= barrierTwoStart + barrierSize) {
            ballLoc.x = cols - 2;
            velocity.x *= -1;
            bouncesCount++;
            if (bouncesCount % 10 == 0) {
                velocity.x > 0 ? velocity.x++ : velocity.x--;
            }

            int barrierCenter = barrierTwoStart + barrierSize / 2;
            if (nextLocation == barrierCenter) {
                velocity.y = 0;
            }
            else {
                velocity.y += (barrierCenter - ballLoc.y) * -1;
            }

        }
        else {
            system("cls");
            std::cout << "Player one won!";
            return true;
        }
    }


    if (ballLoc.y + velocity.y < 1) {
        ballLoc.y = 1;
        velocity.y *= -1;
    }
    else if (ballLoc.y + velocity.y > rows - 2) {
        ballLoc.y = rows - 2;
        velocity.y *= -1;
    }


    if (changedYVelocityCount % 4 == 0) {
        ballLoc.y += velocity.y;
        changedYVelocityCount++;
    }
    else {
        changedYVelocityCount++;
    }

    ballLoc.x += velocity.x;


    setCursorAtPosition(prevBallLoc.x, prevBallLoc.y);
    std::cout << ' ';
    setCursorAtPosition(ballLoc.x, ballLoc.y);
    std::cout << BALL_CHAR;

    prevBallLoc.x = ballLoc.x;
    prevBallLoc.y = ballLoc.y;

    return false;
}


void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}


void clearScreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void setCursorAtPosition(int x, int y) {
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = x;
    Position.Y = y;
    SetConsoleCursorPosition(hOut, Position);
}

extern void PlayOne(PlayField& field);

int main()
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    short green = 10, white = 7;
    short twoPlayersColor = green, onePlayerColor = white;
    bool playAlone = false;

    hideCursor();
    std::cout << "\nOnly two players mode works as of now!";
   
    while ((GetKeyState(13) & 0x8000) == 0) {
        clearScreen();

        SetConsoleTextAttribute(hConsole, twoPlayersColor);
        std::cout << "Two players";
        SetConsoleTextAttribute(hConsole, white);
        std::cout << " / ";
        SetConsoleTextAttribute(hConsole, onePlayerColor);
        std::cout << "One player\n";


        if (GetKeyState(VK_LEFT) & 0x8000) {
            twoPlayersColor = green;
            onePlayerColor = white;
            playAlone = false;
        }
        if (GetKeyState(VK_RIGHT) & 0x8000) {
            twoPlayersColor = white;
            onePlayerColor = green;
            playAlone = true;
        }
    }
    int rows, cols;
    getTerminalSizeInRowsAndColumns(rows, cols);
    PlayField field(rows, cols);

    Sleep(100);
    SetConsoleTextAttribute(hConsole, white);
    std::cout << "Player one moves with 'W' and 'S' and player two moves with arrow up and arrow down.\nPlaying on a bigger console window will likely result in a better experience as the side bars will be bigger\npress enter to start the game";
    while((GetKeyState(13) & 0x8000) == 0){}

    system("cls");

    hideCursor();
    if (playAlone)
        PlayOne(field);
    else
        PlayTwo(field);

}

