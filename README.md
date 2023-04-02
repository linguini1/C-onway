# C-onway's Game of Life
### Matteo Golin

A real-time Conway's Game of Life simulation written in C for better performance. The simulation comes with
variable colours, starting seeds and real-time simulation analytics.

## Installation
No installation required, just run `conway.exe`.

## Usage
### Controls
- Pause and play with the space bar.
- Click to place cells.
- Press `c` to clear.
- Press `esc` to quit.
- Zoom with the mouse wheel.
- Use arrow keys to move around the simulation grid.
- Increase or decrease the simulation speed using the `+`/`-` keys.
  - Toggle max speed with the `m` key.
- Switch cell types using the number keys.
- Cycle through themes using the `t` key.
- Toggle dark mode with the `d` key.
- Toggle live analytics with the `a` key.

## Development
Development requires:
- A Windows operating system
- MinGW compiler
- SDL2 library
- SDL_ttf library

### Installing libraries
SDL_ttf can be installed [here](https://github.com/libsdl-org/SDL_ttf/releases). Use the Windows development version 
for 64bit (mingw.tar.gz).
SDL2 can be installed [here](https://wiki.libsdl.org/SDL2/Installation).

Place the following files in `/cmake-build-debug`:
- `SDL2.dll`
- `SDL2_ttf.dll`

### Coming features
- Analytics CSV file
