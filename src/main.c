/**
 * Main file for running Conway's Game of Life simulation in real time with simulation analytics.
 * @author Matteo Golin
 * @version 1.1
 */
#include "palettes.h"
#include "rules.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>

// Constants
const unsigned int DEFAULT_SCALE = 6;
const unsigned int MAX_SCALE = 14;
const unsigned int ZOOM_STEP = 1;
const int MOVEMENT_STEP = 5;
const float FONT_SCALE = 1.8f;
const char WINDOW_NAME[] = "Conway's Game of Life Analyzer";

// Simulation parameters
Palette const GAME_PALETTES[] = PALETTES;
const unsigned int DEFAULT_FRAME_DELAY = 100;
const unsigned int MAX_FRAME_DELAY = 1000;
const unsigned int FRAME_DELAY_STEP = 10;
const unsigned int FONT_SIZE = 12;

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

    // Determine screen size
    SDL_DisplayMode initial_display_mode;
    SDL_GetCurrentDisplayMode(0, &initial_display_mode); // Get the current window size

    // Create window
    SDL_Window *window = SDL_CreateWindow(
            WINDOW_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            initial_display_mode.w,
            initial_display_mode.h,
            SDL_WINDOW_OPENGL
    );
    SDL_SetWindowResizable(window, true); // Window should be resizable

    // Determine simulation size from window size
    unsigned int game_width = initial_display_mode.w / (unsigned int) DEFAULT_SCALE;
    unsigned int game_height = initial_display_mode.h / (unsigned int) DEFAULT_SCALE;
    unsigned int zoom = 0; // No zoom by default
    int x_offset = 0;
    int y_offset = 0;

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Accelerated and in sync with monitor refresh rate
    );

    // Load font
    TTF_Font *font = TTF_OpenFont("../src/uni0553.ttf", (int) FONT_SIZE); // TODO fix path
    if (font == NULL) {
        printf("Font could not be loaded.");
        return EXIT_FAILURE;
    }

    // Runtime variables
    unsigned int generation_timer = SDL_GetTicks(); // Slow generations without slowing animation
    bool running = true; // For quitting the animation
    bool playing = false; // For play and pause
    bool dark_mode = true; // Simulation runs in dark mode
    bool analytics_on = true; // Shows analytics by default
    bool selected_state = false; // For drawing a cohesive line on drag
    SDL_Event event; // For capturing events

    // Simulation assets
    Environment *environment = init_environment(game_width, game_height, DEFAULT_FRAME_DELAY);
    CellType cell_type = ConwayCell;
    char *analytics_string; // String for analytics text
    short unsigned int palette = 0; // Controls game palette

    while (running) {

        // Handle events
        while (SDL_PollEvent(&event)) {

            // Quit program (esc)
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }

            // Keypress events
            if (event.type == SDL_KEYDOWN) {
                SDL_KeyCode key = event.key.keysym.sym;
                unsigned int current_width = initial_display_mode.w / (DEFAULT_SCALE + zoom);
                unsigned int current_height = initial_display_mode.h / (DEFAULT_SCALE + zoom);

                // Pause & play
                if (key == SDLK_SPACE) {
                    playing = !playing;
                }
                    // Speed controls
                else if ((key == SDLK_MINUS || key == SDLK_KP_MINUS) &&
                         environment->data.generation_speed <= MAX_FRAME_DELAY - FRAME_DELAY_STEP) {
                    environment->data.generation_speed += FRAME_DELAY_STEP; // Slow down
                } else if ((key == SDLK_PLUS || key == SDLK_EQUALS || key == SDLK_KP_PLUS) &&
                           environment->data.generation_speed >= FRAME_DELAY_STEP) {
                    environment->data.generation_speed -= FRAME_DELAY_STEP; // Speed up
                } else if (key == SDLK_m) {
                    environment->data.generation_speed = 0;  // Max speed
                }
                    // Switch cell types (keys between 0-9)
                else if (0x30 <= key && key <= 0x39) {
                    change_cell_type(&cell_type, key);
                }
                    // Toggle dark mode
                else if (key == SDLK_d) {
                    dark_mode = !dark_mode;
                }
                    // Toggle analytics
                else if (key == SDLK_a) {
                    analytics_on = !analytics_on;
                }
                    // Clear simulation
                else if (key == SDLK_c) {
                    clear_env(environment);
                }
                    // Cycle through themes
                else if (key == SDLK_t) {
                    palette = (palette + 1) % NUM_PALETTES;
                }
            }

            // Mouse click or click and drag
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                unsigned int x = event.motion.x / (unsigned int) (DEFAULT_SCALE + zoom) - x_offset;
                unsigned int y = event.motion.y / (unsigned int) (DEFAULT_SCALE + zoom) - y_offset;

                // Coordinates must be within boundaries, otherwise the action will be ignored
                if (in_bounds(environment, x, y)) {
                    selected_state = !access(environment, x, y);
                    if (selected_state) {
                        environment->data.initial_cells++; // These are not natural cells, so they should be registered
                    } else if (environment->data.initial_cells - 1 > 0) {
                        environment->data.initial_cells--; // A cell has been removed
                    }
                    write(environment, x, y, selected_state); // Toggle cell state at click
                }
            }

            if (event.type == SDL_MOUSEMOTION && event.motion.state) {
                unsigned int x = event.motion.x / (unsigned int) (DEFAULT_SCALE + zoom) - x_offset;
                unsigned int y = event.motion.y / (unsigned int) (DEFAULT_SCALE + zoom) - y_offset;

                // Coordinates must be within boundaries, otherwise the action will be ignored
                if (in_bounds(environment, x, y)) {
                    if (selected_state) {
                        environment->data.initial_cells++; // These are not natural cells, so they should be registered
                    } else if (environment->data.initial_cells - 1 > 0) {
                        environment->data.initial_cells--; // A cell has been removed
                    }
                    write(environment, x, y, selected_state); // Toggle cell state at click
                }
            }

            // Scroll to zoom
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0 && DEFAULT_SCALE + zoom + ZOOM_STEP <= MAX_SCALE) {
                    zoom += ZOOM_STEP;
                } else if (event.wheel.y < 0 && DEFAULT_SCALE + zoom - ZOOM_STEP >= DEFAULT_SCALE) {
                    zoom -= ZOOM_STEP;
                }
            }
        }

        // Clear screen
        SDL_RenderSetScale(renderer, (float) (DEFAULT_SCALE + zoom), (float) (DEFAULT_SCALE + zoom)); // Scale for cells
        set_draw_colour(renderer, &GAME_PALETTES[palette], !dark_mode); // Dead cell colour
        SDL_RenderClear(renderer);
        set_draw_colour(renderer, &GAME_PALETTES[palette], dark_mode); // Living cell colour

        // Draw cells
        for (int x = 0; x < game_width; x++) {
            for (int y = 0; y < game_height; y++) {
                if (access(environment, x, y) == true) {
                    SDL_RenderDrawPoint(renderer, x + x_offset, y + y_offset); // Allows arrow key movement
                }
            }
        }

        // Create analytics
        populate_analytics_string(&analytics_string, environment, &cell_type); // Create analytics string
        SDL_DisplayMode display_mode;
        SDL_GetCurrentDisplayMode(0, &display_mode); // Get current width and height for text wrapping
        SDL_Surface *analytics_surface = TTF_RenderText_Solid_Wrapped(
                font,
                analytics_string,
                dark_mode ? GAME_PALETTES[palette].light : GAME_PALETTES[palette].dark, // Switch colour with toggle
                display_mode.w  // Wrap on \n or when width is larger than window width
        );
        // Display text in top left corner
        SDL_Texture *analytics_texture = SDL_CreateTextureFromSurface(renderer, analytics_surface);
        SDL_Rect analytics_rect = {5, 0, analytics_surface->w, analytics_surface->h};
        SDL_FreeSurface(analytics_surface); // No longer needed after being added to texture

        // If analytics are turned on, draw them
        if (analytics_on) {
            SDL_RenderSetScale(renderer, FONT_SCALE, FONT_SCALE); // Text scale back to 1
            SDL_RenderCopy(renderer, analytics_texture, NULL, &analytics_rect);
        }
        SDL_DestroyTexture(analytics_texture);

        // Calculate the next generation if playing and enough time has passed since last generation
        if (playing && (SDL_GetTicks() - generation_timer) >= environment->data.generation_speed) {
            next_generation(environment, &cell_type);
            generation_timer = SDL_GetTicks();
        }

        // Show what was drawn
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