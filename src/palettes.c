/**
* Some basic functions for controlling simulation appearance.
 * @author Matteo Golin
 * @version 1.0
*/
#include <SDL.h>
#include <stdbool.h>
#include "palettes.h"

/**
 * Set the draw colour of the renderer using a palette.
 * @param renderer The SDL renderer being used.
 * @param palette The selected palette.
 * @param light Whether to use the light colour or the dark colour of the palette (true for light, false for dark).
 */
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light) {

    RGB colour; // Select the colour to be used
    if (light) {
        colour = palette->light;
    } else {
        colour = palette->dark;
    }
    // Set draw colour
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, 255);
}

/**
 * Resets the palette to the specified palette.
 * @param palette
 */
void reset_palette(Palette *palette, SDL_KeyCode key) {
    switch (key) {
        case SDLK_1:
            *palette = (Palette) Nokia3310;
            break;
        default:
            *palette = (Palette) MonitorGlow; // Also handles key 0
            break;
    }
}