#ifndef LOBBY_H
#define LOBBY_H
#include <emscripten/websocket.h>
#include "raylib.h"
extern EMSCRIPTEN_WEBSOCKET_T ws;
void InitLobby(void);
void UpdateScreenLobby(void);
void DrawLobby(void);

#endif /* LOBBY_H */
