/**
 * Main file for running Conway's Game of Life simulation in real time with simulation analytics.
 * @author Matteo Golin
 * @version 1.0
 */
#include "palettes.h"
#include "life.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <Windows.h>

// Constants
const int WIDTH = 700;
const int HEIGHT = 700;
const int SCALE = 4;
const int GAME_WIDTH = WIDTH / SCALE;
const int GAME_HEIGHT = HEIGHT / SCALE;
const char WINDOW_NAME[] = "Conway's Game of Life Analyzer";
const Coordinate MAP_CENTER = {GAME_WIDTH / 2, GAME_HEIGHT / 2};

// Simulation parameters
Palette game_palette = MonitorGlow;
const int DEFAULT_FRAME_DELAY = 200;
const int MAX_FRAME_DELAY = 1000;
const int FRAME_DELAY_STEP = 25;
const int FONT_SIZE = 12;

int main(int argc, char **argv) {

    // OpenGL params
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Start SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
    }

    // Start SDL_ttf
    if (TTF_Init() < 0) {
        printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
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

    // Runtime variables
    bool running = true; // For quitting the animation
    bool playing = true; // For play and pause
    bool dark_mode = true; // Simulation runs in dark mode
    bool analytics_on = true; // Shows analytics by default
    int frame_delay = DEFAULT_FRAME_DELAY; // Length of each frame
    TTF_Font *font = TTF_OpenFont("..\\src\\uni0553.ttf", FONT_SIZE); // Analytics font and size TODO fix path
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
            if (event.type == SDL_KEYDOWN) {
                SDL_KeyCode key = event.key.keysym.sym;

                // Pause & play
                if (key == SDLK_SPACE) {
                    playing = !playing;
                }
                // Speed controls
                if (key == SDLK_UP && frame_delay <= MAX_FRAME_DELAY - FRAME_DELAY_STEP) {
                    frame_delay += FRAME_DELAY_STEP; // Speed up
                } else if (key == SDLK_DOWN && frame_delay >= FRAME_DELAY_STEP) {
                    frame_delay -= FRAME_DELAY_STEP; // Slow down
                } else if (key == SDLK_m) {
                    frame_delay = 0;  // Max speed
                }
                // Switch theme
                if (0x30 <= key && key <= 0x39) {
                    reset_palette(&game_palette, key);
                }
                // Toggle dark mode
                if (key == SDLK_d) {
                    dark_mode = !dark_mode;
                }
                // Toggle analytics
                if (key == SDLK_a) {
                    analytics_on = !analytics_on;
                }
            }
        }

        // Clear screen
        SDL_RenderSetScale(renderer, (float) SCALE, (float) SCALE); // Scale for cells
        set_draw_colour(renderer, &game_palette, !dark_mode); // Dead cell colour
        SDL_RenderClear(renderer);
        set_draw_colour(renderer, &game_palette, dark_mode); // Living cell colour

        // Draw cells
        for (int x = 0; x < GAME_WIDTH; x++) {
            for (int y = 0; y < GAME_HEIGHT; y++) {
                if (access(environment, x, y) == true) {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }

        // Draw analytics
        if (analytics_on) {
            SDL_RenderSetScale(renderer, 1.0f, 1.0f); // Text scale back to 1

            char *analytics_string;
            populate_analytics_string(&analytics_string, environment);

            // Wrap after width exceeded or on \n
            SDL_Surface *analytics_text = TTF_RenderText_Solid_Wrapped(
                    font, analytics_string, game_palette.light, WIDTH
            );
            SDL_Texture *analytics = SDL_CreateTextureFromSurface(renderer, analytics_text);
            SDL_Rect analytics_rect = {0, 0, analytics_text->w, analytics_text->h}; // Top left corner
            SDL_RenderCopy(renderer, analytics, NULL, &analytics_rect);
        }

        // Calculate the next generation
        if (playing) {
            next_generation(environment);
        }

        // Show what was drawn
        Sleep(frame_delay);
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Release simulation assets
    destroy_env(environment);
    destroy_seed(seed);

    return 0;
}
