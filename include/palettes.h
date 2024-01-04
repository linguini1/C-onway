/**
 * Contains colour palettes for the simulation.
 * @author Matteo Golin
 * @version 1.0
 */
#ifndef CONWAY_PALETTES_H
#define CONWAY_PALETTES_H
#include <SDL2/SDL.h>
#include <stdbool.h>

// Custom types
typedef struct palette {
    SDL_Color light;
    SDL_Color dark;
} Palette;

// Some included palettes in RGBA format
#define MonitorGlow                                                                                                    \
    {                                                                                                                  \
        {240, 246, 240, 255}, { 34, 35, 35, 255 }                                                                      \
    }
#define Nokia3310                                                                                                      \
    {                                                                                                                  \
        {199, 240, 216, 255}, { 67, 82, 61, 255 }                                                                      \
    }
#define EndGame                                                                                                        \
    {                                                                                                                  \
        {220, 242, 157, 255}, { 27, 18, 51, 255 }                                                                      \
    }
#define PaperAndDust                                                                                                   \
    {                                                                                                                  \
        {203, 158, 106, 255}, { 37, 41, 50, 255 }                                                                      \
    }
#define IBM8503                                                                                                        \
    {                                                                                                                  \
        {235, 229, 206, 255}, { 46, 48, 55, 255 }                                                                      \
    }
#define OngBit                                                                                                         \
    {                                                                                                                  \
        {237, 132, 99, 255}, { 21, 29, 36, 255 }                                                                       \
    }
#define PaperBack                                                                                                      \
    {                                                                                                                  \
        {184, 194, 185, 255}, { 56, 43, 38, 255 }                                                                      \
    }
#define IronBlues                                                                                                      \
    {                                                                                                                  \
        {228, 232, 209, 255}, { 89, 110, 121, 255 }                                                                    \
    }
#define SpriteZero                                                                                                     \
    {                                                                                                                  \
        {234, 230, 214, 255}, { 9, 121, 65, 255 }                                                                      \
    }
#define Casio                                                                                                          \
    {                                                                                                                  \
        {131, 176, 126, 255}, { 0, 0, 0, 255 }                                                                         \
    }

#define NUM_PALETTES 10

// Helper functions
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light);

#endif // CONWAY_PALETTES_H
