# Super Mario Bros

Made for educational purposes. 
No game-engine, only C++ and SDL2.

I've used an ECS architecture taking cues from: https://github.com/redxdev/ECS

- **Author**: Fernando Raviola
- **Twitter**: @fernandoraviola

Contributions are welcomed!

- [Article (medium.com)](https://medium.com/@FerRaviola/writing-super-mario-bros-in-c-e59dfc5743af)
- [Video Demo (youtube.com)](https://youtu.be/HkLZ9ESYxCU)

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

## Webassembly

Either install [emsdk](https://emscripten.org/docs/getting_started/downloads.html) and run
```
$ mkdir build
$ cd build
$ emcmake cmake ..
$ make
```
or build with [Docker](https://www.docker.com/)
```
$ docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk emcmake cmake -S . -B build
$ docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) emscripten/emsdk cmake --build build
```
Start a webserver in the bin folder
```
$ cd bin
$ python -m SimpleHTTPServer 8000
$ open http://localhost:8000/smb-.html
```

## Screenshots

![Game screenshot](https://github.com/feresr/super-mario-bros/blob/master/readme/game.png)
![Loading screenshot](https://github.com/feresr/super-mario-bros/blob/master/readme/loading.png)
![Editor screenshot](https://github.com/feresr/super-mario-bros/blob/master/readme/editor.png)

## Level editor

![alt text](https://github.com/feresr/super-mario-bros/blob/master/readme/editor%20build.gif)


