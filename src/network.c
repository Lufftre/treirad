#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "network.h"
char player_id[10];
char openmsg[100];
EMSCRIPTEN_WEBSOCKET_T ws;

void WebsocketConnect(EM_BOOL (*onmessage)(int , const EmscriptenWebSocketMessageEvent*, void *))
{
    EmscriptenWebSocketCreateAttributes ws_attrs = {
        SERVER_URL,
        NULL,
        EM_TRUE
    };
    ws = emscripten_websocket_new(&ws_attrs);

    emscripten_websocket_set_onopen_callback(ws, NULL, onopen);
    emscripten_websocket_set_onerror_callback(ws, NULL, onerror);
    emscripten_websocket_set_onclose_callback(ws, NULL, onclose);
    emscripten_websocket_set_onmessage_callback(ws, NULL, onmessage);

}

void WebsocketClose()
{
    result = emscripten_websocket_close(ws, 1000, "no reason");
    if (result)
        printf("Failed to emscripten_websocket_close(): %d\n", result);
}

EM_BOOL onopen(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData)
{
    puts("onopen");
    srand(time(NULL));
    int r = rand();
    sprintf(player_id, "%d", r);
    sprintf(openmsg, "{\"event\": \"join\", \"name\": \"%s\"}", player_id);
    result = emscripten_websocket_send_utf8_text(websocketEvent->socket, openmsg);
    if (result)
        printf("Failed to emscripten_websocket_send_utf8_text(): %d\n", result);
    return EM_TRUE;
}

EM_BOOL onerror(int eventType, const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData)
{
    puts("onerror");
    return EM_TRUE;
}
EM_BOOL onclose(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData)
{
    puts("onclose");

    return EM_TRUE;
}