#include "lobby.h"
#include <emscripten/websocket.h>
#include <stdio.h>

bool btnAction = false;
int btnHover = 0;
Vector2 mousePoint = {0.0f, 0.0f};
Rectangle button = {150, 10, 70, 40};


void UpdateScreenLobby(void)
{
    mousePoint = GetMousePosition();
    btnAction = false;
    btnHover = 0;
    if (CheckCollisionPointRec(mousePoint, button))
    {
        btnHover = 1;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            btnAction = true;
    }

    if (btnAction)
    {
        printf("press\n");
        emscripten_websocket_send_utf8_text(ws, "{\"event\": \"start\"}");
    }
    
}

void DrawLobby(void)
{
    DrawRectangleRec(button, LIGHTGRAY);
    DrawText("start", button.x + 10, button.y + 10, 20, DARKGRAY);
}