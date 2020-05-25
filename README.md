# Super Mario Bros

Made for educational porpuses with no game engine only c++ and SDL2.

DEMO: https://youtu.be/HkLZ9ESYxCU

## Controls

- `AWSD` to move
- `LSHIFT` to sprint
- `E` to enter the level editor

## Build

I've only tested this on macOS Catalina but should work in Windows and Linux as well.
### Prerequisites:

```
$ brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer 
```

```
$ make build run

# or

$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ../bin/  
$ ./smb-
```

## Screenshots

![alt text](https://raw.githubusercontent.com/feresr/super-mario-bros/master/readme/game.png?token=ABGBITFVQBBDPSBXSIQVUVK6ZMPS4)
![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/loading.png)
![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/editor.png)

## Level editor

![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/editor%20build.gif)
![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/editor%20tiles.gif)


