/**
 * Main file for running Conway's Game of Life simulation in real time with simulation analytics.
 * @author Matteo Golin
 * @version 1.0
 */
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "life.h"
#include "palettes.h"

// Constants
const int WIDTH = 700;
const int HEIGHT = 700;
const int SCALE = 3;
const int GAME_WIDTH = WIDTH / SCALE;
const int GAME_HEIGHT = HEIGHT / SCALE;
const char WINDOW_NAME[] = "Geometry Visualizer";
const Coordinate MAP_CENTER = {GAME_WIDTH / 2, GAME_HEIGHT / 2};

// Simulation parameters
Palette game_palette = IBM8503;

int main(int argc, char **argv) {

    // OpenGL params
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Start SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow(
            WINDOW_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_OPENGL
    );

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Accelerated and in sync with monitor refresh rate
    );
    SDL_RenderSetScale(renderer, (float) SCALE, (float) SCALE);

    // Runtime variables
    bool running = true; // For quitting the animation
    bool playing = true; // For play and pause
    SDL_Event event; // For capturing events

    // Simulation assets
    Environment *environment = init_environment(GAME_WIDTH, GAME_HEIGHT);
    Seed *seed = ShoeBoxSeed(MAP_CENTER.x, MAP_CENTER.y);
    place_seed(environment, seed);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Keypress events
            if (event.type == SDL_KEYDOWN){
                // Pause & play
                if (event.key.keysym.sym == SDLK_SPACE){
                    playing = !playing;
                }
            }
        }

        // Clear screen
        set_draw_colour(renderer, &game_palette, false); // Dead cell colour
        SDL_RenderClear(renderer);
        set_draw_colour(renderer, &game_palette, true); // Living cell colour

        // Draw cells
        for (int x = 0; x < GAME_WIDTH; x++){
            for (int y = 0; y < GAME_HEIGHT; y++){
                if (access(environment, x, y) == true){
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }

        // Calculate the next generation
        if (playing){
            next_generation(environment);
        }

        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destroy_env(environment);

    return 0;
}
