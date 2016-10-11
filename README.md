# gpu-gameoflife
Conway's game of life on a GPU using OpenGL.

### Compiling
```
mkdir build && cd build
cmake ..
make
```

### Running
```
./gameoflife <world_width> <world_height> <time_per_turn>
```
world_width and world_height default to 1024. time_per_turn defaults to 0.1.
