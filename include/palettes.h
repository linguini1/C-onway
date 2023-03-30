/**
* Contains colour palettes for the simulation.
 * @author Matteo Golin
 * @version 1.0
*/

// Custom types
typedef struct rgb {
    short unsigned int r;
    short unsigned int g;
    short unsigned int b;
} RGB;

typedef struct palette {
    RGB light;
    RGB dark;
} Palette;

// Some included palettes
#define MonitorGlow {{240, 246, 240}, {34, 35, 35}}
#define Nokia3310 {{199, 240, 216}, {67,  82,  61}}
#define EndGame {{220, 242, 157}, {27,  18,  51}}
#define PaperAndDust {{203, 158, 106}, {37,  41,  50}}
#define IBM8503 {{235, 229, 206}, {46,  48,  55}}

// Helper functions
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light);

void reset_palette(Palette *palette, SDL_KeyCode key);