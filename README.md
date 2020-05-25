# Super Mario Bros

Made for educational porpuses. 
No game-engine, only C++ and SDL2.

I've used an ECS architecture for this project, I took some cues from: https://github.com/redxdev/ECS although my implementation is probably less efficent, I'm not as confident with C++ as I'd like to be.

Author: Fernando Raviola
Twitter: @fernandoraviola

Contributions are welcomed!

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

![Game screenshot](https://github.com/feresr/super-mario-bros/blob/master/readme/game.png)
![Loading screenshot](https://github.com/feresr/super-mario-bros/blob/master/readme/loading.png)
![Editor screenshot](https://github.com/feresr/super-mario-bros/blob/master/readme/editor.png)

## Level editor

![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/editor%20build.gif)
![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/editor%20tiles.gif)


