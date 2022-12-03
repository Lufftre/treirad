/*******************************************************************************************
 *
 *   Tic
 *   Tic Tac Toe over Websocket
 *
 *   This game has been created using raylib (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Copyright (c) 2021 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <emscripten/emscripten.h>
#include "raylib_game.h"
#include "lobby.h"
#include "network.h"

enum BOARD_STATE {
    EMPTY,
    MINE,
    THERIS
};
int placed = 0;
int selected = -1;
void win()
{
    sprintf(openmsg, "{\"event\": \"win\", \"name\": \"%s\"}", player_id);
    emscripten_websocket_send_utf8_text(ws, openmsg);
    printf("WIN\n");
}

EM_BOOL onmessage(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData)
{
    puts("onmessage");
    if (websocketEvent->isText)
    {
        // For only ascii chars.
        printf("message: %s\n", websocketEvent->data);
        if (*websocketEvent->data == '0')
        {
            printf("START\n");
            state = STARTED;
        }
        else if (strcmp(websocketEvent->data, player_id) == 0)
        {
            state = MYTURN;
            printf("MYTURN\n");
        }
        else if (strncmp(websocketEvent->data, "win", 3) == 0)
        {
            state = STARTED;
            printf("GAME OVER\n");
        }
        else if (strncmp(websocketEvent->data, "move", 4) == 0)
        {
            char playerid[10];
            int move_square;
            int from_square;
            sscanf(websocketEvent->data, "move %s %d %d", playerid, &move_square, &from_square);
            if(from_square >= 0 && from_square < 9)
            {
                board[from_square] = EMPTY;
            }
            printf("MOVE player %s move_square %d FROM %d\n", playerid, move_square, from_square);
            if (strcmp(playerid, player_id) == 0)
            {
                board[move_square] = MINE;
            }
            else
            {
                board[move_square] = THERIS;
            }

            int sum = 0;
            for (size_t i = 0; i < 3; i++)
            {
                if (board[i * 3 + 0] == MINE && board[i * 3 + 1] == MINE && board[i * 3 + 2] == MINE)
                    win();
                if (board[0 * 3 + i] == MINE && board[1 * 3 + i] == MINE && board[2 * 3 + i] == MINE)
                    win();
            }
            if (board[0] == MINE && board[4] == MINE && board[8] == MINE)
                win();
            if (board[2] == MINE && board[4] == MINE && board[6] == MINE)
                win();
        }
    }
    return EM_TRUE;
}

int main(void)
{

    // ----------------------------------------
    // - WINDOW
    // ----------------------------------------
    currentScreenHeight = GetScreenHeight();
    currentScreenWidth = GetScreenWidth();

    if (currentScreenHeight == 0 || currentScreenWidth == 0)
    {
        currentScreenHeight = screenHeight;
        currentScreenWidth = screenWidth;
    }
    InitWindow(currentScreenWidth, currentScreenHeight, "raylib game template");

    // ----------------------------------------
    // - BOARD
    // ----------------------------------------
    for (size_t i = 0; i < 9; i++)
    {
        Rectangle r = {
            20 + squareWidth*(i % 3),
            20 + squareWidth*(i / 3),
            squareWidth - squareShrink,
            squareWidth - squareShrink,
        };
        squares[i] =  r;
    }
    
    // ----------------------------------------
    // - WebSocket
    // ----------------------------------------
    WebsocketConnect(&onmessage);

    // ----------------------------------------
    // - Main Loop
    // ----------------------------------------
    emscripten_set_main_loop(UpdateScreen, 60, 1);

    // ----------------------------------------
    // - Destruct
    // ----------------------------------------
    CloseWindow();
    WebsocketClose();
    return 0;
}
Color c;

void UpdateScreen(void)
{
    int btnState = 0;
    bool btnAction = false;

    Vector2 mousePoint = {0.0f, 0.0f};
    mousePoint = GetMousePosition();
    if (state == LOBBY)
    {
       UpdateScreenLobby();
    }
    if (state == MYTURN)
    {
        btnAction = false;
        btnState = -1;
        for (size_t i = 0; i < 9; i++)
        {
            if (CheckCollisionPointRec(mousePoint, squares[i]))
            {
                btnState = i;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    printf("press %d", i);
                    if (board[i] == EMPTY)
                    {
                        if(selected >= 0)
                        {
                            sprintf(openmsg, "{\"event\": \"move\", \"square\": %d, \"from\": %d}", i, selected);
                            emscripten_websocket_send_utf8_text(ws, openmsg);
                            board[i] = EMPTY;
                            state = STARTED;
                            selected = -1;
                        } else if(placed < 3)
                        {
                            sprintf(openmsg, "{\"event\": \"move\", \"square\": %d}", i);
                            emscripten_websocket_send_utf8_text(ws, openmsg);
                            placed++;
                            state = STARTED;
                            selected = -1;
                        }

                    }
                    else if (board[i] == MINE)
                    {
                        selected = i;
                    }
                    else
                    {
                        selected = -1;
                    }
                }
                break;
            }
        }
    }
    BeginDrawing();
    if (state == MYTURN)
    {
        ClearBackground(RAYWHITE);
    }
    else
    {
        ClearBackground(BLACK);
    }

    if(state == LOBBY)
    {
        DrawLobby();
    }

    if (state == STARTED || state == MYTURN)
    {
        for (size_t i = 0; i < 9; i++)
        {
            if (btnState == i && state == MYTURN && board[i] == EMPTY)
                DrawRectangleRec(squares[i], YELLOW);
            else
            {
                if (selected == i)
                    c = GREEN;
                else if (board[i] == MINE)
                    c = RED;
                else if (board[i] == THERIS)
                    c = BLUE;
                else
                    c = LIGHTGRAY;
                DrawRectangleRec(squares[i], c);
            }
        }
    }

    EndDrawing();
}