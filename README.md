# C-onway's Game of Life

### Matteo Golin

A real-time Conway's Game of Life simulation written in C for high performance. Includes theme selection, speed
controls, different cell types, real-time analytics, interactive simulation setup and more.

https://github.com/linguini1/C-onway/assets/62850091/0c45270b-0b4e-4be8-9484-daa16b1e5e10

## Usage

Run the executable and interact with the live simulation windows with the controls.

### Controls

- Toggle pause/play with `space`.
- Left click to toggle cells on the grid.
- Press `c` to clear.
- Press `esc` or `q` to quit.
- Increase or decrease the simulation speed using the `+`/`-` keys.
  - Press `m` to increase to max speed.
- Switch cell types using the number keys.
- Zoom with the mouse wheel.
  - Use arrow keys to move around the zoomed in simulation grid.

**Appearance:**

- Press `t` to cycle through themes.
- Press `d` to toggle dark mode.
- Press `a` to hide/show live analytics.

## Building

**Requires:**

- SDL2 library (2.0)
- SDL_ttf library (2.20.2)

Building has currently only been tested on (Arch) Linux. Simply run `make all` to compile the entire program and link it
with the SDL2 libraries.

On Windows, the same applies, but you will need to provide the paths to your SDL2 `lib` and `include` folders. The build
command may look something like this:

```console
make all \
SDL_INC=C:/path/to/SDL2/x86_64/include \
SDL_LINK=C:/path/to/SDL2/x86_64/lib
SDLTTF_INC=C:/path/to/SDL2_ttf/x86_64/include \
SDLTTF_LINK=C:/path/to/SDL2_ttf/x86_64/lib
```

### Fonts

The default TTF font used for this program is included in the `src/` directory. However, you may also provide your own
TTF font if you prefer.

Compiling by hand, you must simply define `FONT_PATH` as the path to your desired TTF font:

```console
gcc <cflags> -o main.o -c main.c -DFONT_PATH=/path/to/font.ttf
```

Compiling without `FONT_PATH` defined will provide a compile error. It is already defined with the default font for you
if you are compiling with Make.

To change the path to something other than the default TTF font, you may call:

```console
make all FONT_PATH=path/to/font
```
