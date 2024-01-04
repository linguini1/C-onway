/**
 * Some basic functions for controlling simulation appearance.
 * @author Matteo Golin
 * @version 1.0
 */
#include "../include/palettes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

/**
 * Set the draw colour of the renderer using a palette.
 * @param renderer The SDL renderer being used.
 * @param palette The selected palette.
 * @param light Whether to use the light colour or the dark colour of the palette (true for light, false for dark).
 */
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light) {

    SDL_Color colour; // Select the colour to be used
    if (light) {
        colour = palette->light;
    } else {
        colour = palette->dark;
    }
    // Set draw colour
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
}
