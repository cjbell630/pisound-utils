#!/usr/bin/python3

# based on: https://github.com/Greg209/patchbox-modep-stuff/blob/master/midi_cmd_server.py

import time
import asyncio
import websockets
import sys, getopt


# Websocket class. Makes (and restores) a single connection for multiple socket requests
class MODEPWebSocket:
    __ws = None
    __url = "ws://patchbox.local/websocket"

    def __init__(self):
        self.retryTime = 0
        self.retryRepeat = 30
        self.__create_connect()

    async def __create_connect(self):
        if self.__ws is None:
            if (time.time() - self.retryTime) > self.retryRepeat:
                try:
                    self.__ws = await websockets.connect(self.__url)
                    self.retryTime = 0
                except ConnectionRefusedError:
                    self.retryTime = time.time()

    def connect(self):
        if self.__ws is None:
            asyncio.get_event_loop().run_until_complete(self.__create_connect())

    def send(self, msg):
        if self.__ws is not None:
            try:
                asyncio.get_event_loop().run_until_complete(self.__async_send(msg))
            except ConnectionRefusedError:
                self.__create_connect()
        else:
            asyncio.get_event_loop().run_until_complete(self.__create_connect())

    async def __async_send(self, message):
        await self.__ws.send(message)

    def set_fluid_drums_program(self, program_num: int):
        self.send(f"param_set /graph/FluidPlug_FluidDrums/program {program_num}")

    def set_fluid_keyboard_program(self, program_num: int):
        self.send(f"param_set /graph/FluidPlug_FluidChromPerc/program {program_num}")


if __name__ == "__main__":

    # Establish a websocket connection
    # NOTE: needed to call .connect manually as it doesn't seem to connect through the __init__
    socket = MODEPWebSocket()
    socket.connect()
    print("socket connected")

    try:
        opts, args = getopt.getopt(sys.argv[1:], "d:k:", ["drums=", "keyboard="])

        for opt, arg in opts:
            print(f"opt: {opt}, arg: {arg}")
            if opt in ("-d", "--drums"):
                print(f"setting fluid drums program to {arg}")
                socket.set_fluid_drums_program(int(arg))
            elif opt in ("-k", "--keyboard"):
                print(f"setting fluid keyboard program to {arg}")
                socket.set_fluid_keyboard_program(int(arg))
    except getopt.GetoptError:
        print("Usage: parameter_change.py -d <program_num> -k <program_num>")
        sys.exit(2)
