export interface IMessage {
    event: "join" | "leave" | "start" | "move"| "win"
    name: string
    square: number
}

export interface IJoin extends IMessage {
    name: string
}

export interface ILeave extends IMessage {
    name: string
}

export interface IMove extends IMessage  {
    square: number
    from?: number
}
