import { WebSocketServer, WebSocket } from 'ws'
import { IJoin, ILeave, IMessage, IMove} from './IMessages'
import _ from 'lodash'

interface WS extends WebSocket{
    name: string
}
const wss = new WebSocketServer({ host: '0.0.0.0', port: 8001 })

let players: {[name: string]: WS} = {}

function join(message: IJoin, ws: WS) {
    ws.name = message.name
    players[ws.name] = ws
}

function leave(ws: WS) {
    console.log(ws.name, 'left');
    delete players[ws.name]
}

function start(starter: WS) {
    for(const [name, ws] of Object.entries(players)) {
        ws.send(0)
        ws.send(starter.name)
    }
}

function broadcast(message: string) {
    for(const [name, ws] of Object.entries(players)) {
        ws.send(message)
    }
}

function next(ws: WS){
    for(const [name, _ws] of Object.entries(players)){
        if(name != ws.name) return name
    }
    return ws.name
}

function move(message: IMove, ws: WS){
    let {square, from} = message
    if(from == undefined)
        from = -1
    broadcast(`move ${ws.name} ${square} ${from}`)
    broadcast(next(ws))
}
wss.on('connection', function connection(ws) {
    ws.on('message', function onmessage(data) {
        console.log('received: %s', data)
        const message: IMessage = JSON.parse(data.toString())
        switch (message.event) {
            case "join":
                join(message, ws as WS)
                for(const [name, ws] of Object.entries(players)) {
                    console.log(name);
                }
                break;
            case "start":
                start(ws as WS)
                break;
            case "move":
                move(message, ws as WS)
                break
            case "win":
                broadcast("win")
                break
            default:
                break;
        }
    })

    ws.on('close', function onclose() {
        leave(ws as WS)
    })

})
