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

// Constants
const int WIDTH = 1200;
const int HEIGHT = 700;
const short unsigned int SCALE = 5;
const unsigned int GAME_WIDTH = WIDTH / SCALE;
const unsigned int GAME_HEIGHT = HEIGHT / SCALE;
const char WINDOW_NAME[] = "Conway's Game of Life Analyzer";

// Simulation parameters
Palette game_palette = MonitorGlow;
const unsigned int DEFAULT_FRAME_DELAY = 200;
const unsigned int MAX_FRAME_DELAY = 2000;
const unsigned int FRAME_DELAY_STEP = 25;
const int FONT_SIZE = 12;

int main(int argc, char **argv) {

    // OpenGL params
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Start SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Start SDL_ttf
    if (TTF_Init() < 0) {
        printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
        return EXIT_FAILURE;
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

    // Load font
    TTF_Font *font = TTF_OpenFont("../src/uni0553.ttf", FONT_SIZE); // TODO fix path
    if (font == NULL){
        printf("Font could not be loaded.");
        return EXIT_FAILURE;
    }

    // Runtime variables
    bool running = true; // For quitting the animation
    bool playing = false; // For play and pause
    bool dark_mode = true; // Simulation runs in dark mode
    bool analytics_on = true; // Shows analytics by default
    char *analytics_string; // String for analytics text
    SDL_Event event; // For capturing events

    // Simulation assets
    Environment *environment = init_environment(GAME_WIDTH, GAME_HEIGHT, DEFAULT_FRAME_DELAY);

    while (running) {

        // Handle events
        while (SDL_PollEvent(&event)) {

            // Quit program
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
                if (key == SDLK_DOWN && environment->data.frame_speed <= MAX_FRAME_DELAY - FRAME_DELAY_STEP) {
                    environment->data.frame_speed += FRAME_DELAY_STEP; // Slow down
                } else if (key == SDLK_UP && environment->data.frame_speed >= FRAME_DELAY_STEP) {
                    environment->data.frame_speed -= FRAME_DELAY_STEP; // Speed up
                } else if (key == SDLK_m) {
                    environment->data.frame_speed = 0;  // Max speed
                }
                // Switch theme (keys between 0-9)
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
                // Clear simulation
                if (key == SDLK_c){
                    clear_env(environment);
                }
            }
            // Mouse click or click and drag
            if (event.type == SDL_MOUSEBUTTONDOWN || (event.type == SDL_MOUSEMOTION && event.motion.state)) {
                unsigned int x = event.motion.x / SCALE;
                unsigned int y = event.motion.y / SCALE;
                bool new_state = !access(environment, x, y);
                if (new_state){
                    environment->data.initial_cells++; // These are not natural cells, so they should be registered
                } else {
                    environment->data.initial_cells--; // A cell has been removed
                }
                write(environment, x, y, new_state); // Toggle cell state at click
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

        // Create analytics
        populate_analytics_string(&analytics_string, environment); // Create analytics string
        SDL_Surface *analytics_surface = TTF_RenderText_Solid_Wrapped(
                font,
                analytics_string,
                dark_mode ? game_palette.light : game_palette.dark, // Switch colour with toggle
                WIDTH  // Wrap on \n or when width is larger than window width
        );
        SDL_Texture *analytics_texture = SDL_CreateTextureFromSurface(renderer, analytics_surface);
        SDL_Rect analytics_rect = {5, 0, analytics_surface->w, analytics_surface->h}; // Top left corner
        SDL_FreeSurface(analytics_surface); // No longer needed after being added to texture

        // If analytics on, draw them
        if (analytics_on) {
            SDL_RenderSetScale(renderer, 1.0f, 1.0f); // Text scale back to 1
            SDL_RenderCopy(renderer, analytics_texture, NULL, &analytics_rect);
        }
        SDL_DestroyTexture(analytics_texture);

        // Calculate the next generation
        if (playing) {
            next_generation(environment);
        }

        // Show what was drawn
        SDL_Delay(environment->data.frame_speed);
        SDL_RenderPresent(renderer);

        // Clean up
        free(analytics_string);
    }

    // Release simulation assets
    destroy_env(environment);
    TTF_CloseFont(font);

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
