#pragma once
#ifdef _WIN32
#include <Windows.h>
#endif

struct MoveDir {
    int x = 0;
    int y = 0;
};

class PlayField {
public:
    struct Loc {
        int x;
        int y;
    };

    // change this name to field
    char** location;
    Loc ballLoc;
    Loc prevBallLoc;
    MoveDir velocity;

    int barrierOneStart;
    int prevBarOneY;
    int barrierTwoStart;
    int prevBarTwoY;
    int barrierSize;

    int bouncesCount;
    // the rows are a lot less than the columns and so even a sligth change in y value causes the ball to look really fast.
    // this keeps track of the times the ball should have changed its y value and instead just increments
    // the ball changes y value every 4 "frames"
    int changedYVelocityCount;

    size_t rows;
    size_t cols;

    PlayField(int rows, int cols);

    ~PlayField();
    
    bool MoveBall();
};

void setCursorAtPosition(int x, int y);

void PlayTwo(PlayField& field);