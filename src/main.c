/**
 * Main file for running Conway's Game of Life simulation in real time with simulation analytics.
 * @author Matteo Golin
 * @version 1.1
 */
#include "../include/palettes.h"
#include "../include/rules.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    DRAW_STATE_NONE = 0,
    DRAW_STATE_CELL = 1,
    DRAW_STATE_UNSET,
} DrawState;

typedef struct {
    int x_offset;
    int y_offset;
    unsigned int zoom;
    bool running;
    bool playing;
    bool dark_mode;
    bool analytics_on;
    DrawState draw_state;
    CellType cell_type;
    char *analytics_string;
    short unsigned int palette;
} GameState;

#ifndef FONT_PATH
#error "FONT_PATH not defined with the path to a ttf font. See README."
#endif // FONT_PATH

#define FONT_SIZE 12
#define FONT_SCALE 1.8f
#define DEFAULT_SCALE 6
#define MAX_SCALE 14
#define ZOOM_STEP 1
#define MOVEMENT_STEP 5
#define DEFAULT_FRAME_DELAY 100
#define MAX_FRAME_DELAY 1000
#define FRAME_DELAY_STEP 10

const char WINDOW_NAME[] = "Conway's Game of Life Analyzer";

/** All of the possible colour palettes. */
const Palette GAME_PALETTES[] = {Casio,   MonitorGlow, Nokia3310, EndGame,   PaperAndDust,
                                 IBM8503, OngBit,      PaperBack, IronBlues, SpriteZero};
/** Maps digit keys to cell types. */
const CellType CELL_MAP[10] = {
    ConwayCell,        ConwayCell,  LesseConwayCell, VonNeumannR2ConwayCell, TripleMooreConwayCell, MazeCell,
    FractalCornerCell, FractalCell, NoiseCell,       ConwayCancerCell,
};

static GameState game_state = {
    .x_offset = 0,
    .y_offset = 0,
    .zoom = 0,                      // No zoom by default
    .running = true,                // For quitting the animation
    .playing = false,               // For play and pause
    .dark_mode = true,              // Simulation runs in dark mode
    .analytics_on = true,           // Shows analytics by default
    .draw_state = DRAW_STATE_UNSET, // For drawing a cohesive line on drag
    .cell_type = ConwayCell,        // Initialize starting game cell to classic Conway cell
    .analytics_string = NULL,       // String for analytics text
    .palette = 0,                   // Controls game palette
};

// Helper functions
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light);

int main(int argc, char *argv[]) {

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
    SDL_Window *window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          initial_display_mode.w, initial_display_mode.h, SDL_WINDOW_OPENGL);
    SDL_SetWindowResizable(window, true); // Window should be resizable

    // Determine simulation size from window size
    unsigned int game_width = initial_display_mode.w / DEFAULT_SCALE;
    unsigned int game_height = initial_display_mode.h / DEFAULT_SCALE;
    SDL_Point *points = malloc(sizeof(SDL_Point) * game_width * game_height);

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Accelerated and in sync with monitor refresh rate
    );

    // Load font
    TTF_Font *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (font == NULL) {
        printf("Font could not be loaded.");
        return EXIT_FAILURE;
    }

    // Runtime variables
    unsigned int generation_timer = SDL_GetTicks(); // Slow generations without slowing animation
    SDL_Event event;                                // For capturing events

    // Simulation assets
    Environment *environment = env_init(game_width, game_height, DEFAULT_FRAME_DELAY);

    while (game_state.running) {

        // Handle events
        while (SDL_PollEvent(&event)) {

            // Quit program (esc)
            if (event.type == SDL_QUIT) game_state.running = false;

            // Keypress events
            if (event.type == SDL_KEYDOWN) {
                SDL_KeyCode key = event.key.keysym.sym;

                switch (key) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    game_state.running = false;
                    break;
                case SDLK_SPACE:
                    game_state.playing = !game_state.playing;
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    if (environment->data.generation_speed <= MAX_FRAME_DELAY - FRAME_DELAY_STEP)
                        environment->data.generation_speed += FRAME_DELAY_STEP; // Speed up
                    break;
                case SDLK_PLUS:
                case SDLK_EQUALS:
                case SDLK_KP_PLUS:
                    if (environment->data.generation_speed >= FRAME_DELAY_STEP)
                        environment->data.generation_speed -= FRAME_DELAY_STEP; // Speed up
                    break;
                case SDLK_m:
                    environment->data.generation_speed = 0; // Max speed
                    break;
                case SDLK_d:
                    game_state.dark_mode = !game_state.dark_mode;
                    break;
                case SDLK_a:
                    game_state.analytics_on = !game_state.analytics_on;
                    break;
                case SDLK_c:
                    env_clear(environment);
                    break;
                case SDLK_t:
                    game_state.palette = (game_state.palette + 1) % NUM_PALETTES;
                    break;
                default:
                    if (0x30 <= key && key <= 0x39) game_state.cell_type = CELL_MAP[key - 48];
                    break;
                }
            }

            // Mouse click or click and drag
            if ((event.type == SDL_MOUSEBUTTONDOWN && event.button.state == SDL_PRESSED) ||
                (event.type == SDL_MOUSEMOTION && event.motion.state)) {
                unsigned int x = event.button.x / (DEFAULT_SCALE + game_state.zoom) - game_state.x_offset;
                unsigned int y = event.button.y / (DEFAULT_SCALE + game_state.zoom) - game_state.y_offset;
                if (env_in_bounds(environment, x, y)) {
                    if (game_state.draw_state == DRAW_STATE_UNSET) {
                        game_state.draw_state = env_toggle_cell(environment, x, y) ? DRAW_STATE_CELL : DRAW_STATE_NONE;
                    } else {
                        env_write(environment, x, y, game_state.draw_state);
                    }
                }
            } else if (event.button.state == SDL_RELEASED) {
                game_state.draw_state = DRAW_STATE_UNSET;
            }

            // Scroll to zoom
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0 && DEFAULT_SCALE + game_state.zoom + ZOOM_STEP <= MAX_SCALE) {
                    game_state.zoom += ZOOM_STEP;
                } else if (event.wheel.y < 0 && DEFAULT_SCALE + game_state.zoom - ZOOM_STEP >= DEFAULT_SCALE) {
                    game_state.zoom -= ZOOM_STEP;
                }
            }
        }

        // Clear screen
        SDL_RenderSetScale(renderer, (DEFAULT_SCALE + game_state.zoom), (DEFAULT_SCALE + game_state.zoom));
        set_draw_colour(renderer, &GAME_PALETTES[game_state.palette], !game_state.dark_mode); // Dead cell colour
        SDL_RenderClear(renderer);
        set_draw_colour(renderer, &GAME_PALETTES[game_state.palette], game_state.dark_mode); // Living cell colour

        // Draw cells
        unsigned int count = 0;
        for (unsigned int x = 0; x < game_width; x++) {
            for (unsigned int y = 0; y < game_height; y++) {
                if (env_access(environment, x, y) == true) {
                    points[count].x = x + game_state.x_offset;
                    points[count].y = y + game_state.y_offset;
                    count++;
                }
            }
        }
        SDL_RenderDrawPoints(renderer, points, count);

        // Create analytics
        populate_analytics_string(&game_state.analytics_string, environment,
                                  &game_state.cell_type); // Create analytics string
        SDL_DisplayMode display_mode;
        SDL_GetCurrentDisplayMode(0, &display_mode); // Get current width and height for text wrapping
        SDL_Surface *analytics_surface = TTF_RenderText_Solid_Wrapped(
            font, game_state.analytics_string,
            game_state.dark_mode ? GAME_PALETTES[game_state.palette].light
                                 : GAME_PALETTES[game_state.palette].dark, // Switch colour with toggle
            display_mode.w // Wrap on \n or when width is larger than window width
        );
        // Display text in top left corner
        SDL_Texture *analytics_texture = SDL_CreateTextureFromSurface(renderer, analytics_surface);
        SDL_Rect analytics_rect = {5, 0, analytics_surface->w, analytics_surface->h};
        SDL_FreeSurface(analytics_surface); // No longer needed after being added to texture

        // If analytics are turned on, draw them
        if (game_state.analytics_on) {
            SDL_RenderSetScale(renderer, FONT_SCALE, FONT_SCALE); // Text scale back to 1
            SDL_RenderCopy(renderer, analytics_texture, NULL, &analytics_rect);
        }
        SDL_DestroyTexture(analytics_texture);

        // Calculate the next generation if playing and enough time has passed since last generation
        if (game_state.playing && (SDL_GetTicks() - generation_timer) >= environment->data.generation_speed) {
            next_generation(environment, &game_state.cell_type);
            generation_timer = SDL_GetTicks();
        }

        // Show what was drawn
        SDL_RenderPresent(renderer);

        // Clean up
        free(game_state.analytics_string);
    }

    // Release simulation assets
    env_destroy(environment);
    free(points);
    TTF_CloseFont(font);

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

/**
 * Set the draw colour of the renderer using a palette.
 * @param renderer The SDL renderer being used.
 * @param palette The selected palette.
 * @param light Whether to use the light colour or the dark colour of the palette (true for light, false for dark).
 */
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light) {

    SDL_Color colour;
    if (light)
        colour = palette->light;
    else
        colour = palette->dark;
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
}
