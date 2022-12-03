#ifndef RAYLIBGAME_H
#define RAYLIBGAME_H
#include "raylib.h"

#define SQUARE_WIDTH 50
#define SQUARE_SHRINK 5

static void UpdateScreen(void);
//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 600;

static const int squareWidth = 50;
static const int squareShrink = 5;
//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
typedef enum
{
    LOBBY,
    STARTED,
    MYTURN,
    END,
} GAMESTATE;
int currentScreenWidth;
int currentScreenHeight;
int state = LOBBY;

Rectangle squares[9];

int board[9] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0};


#endif /* RAYLIBGAME_H */