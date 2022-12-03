#ifndef NETWORK_H
#define NETWORK_H
#include <emscripten/websocket.h>

#define SERVER_URL "ws://92.33.203.234:8001"
extern EMSCRIPTEN_WEBSOCKET_T ws;
static EMSCRIPTEN_RESULT result;
extern char player_id[10];
extern char openmsg[100];

void WebsocketConnect(EM_BOOL (*onmessage)(int , const EmscriptenWebSocketMessageEvent*, void *));
void WebsocketClose();
EM_BOOL onopen(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData);
EM_BOOL onerror(int eventType, const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData);
EM_BOOL onclose(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData);
#endif /* NETWORK_H */