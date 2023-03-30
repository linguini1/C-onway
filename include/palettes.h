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
#define OngBit {{237, 132, 99}, {21, 29, 36}}
#define PaperBack {{184, 194, 185}, {56, 43, 38}}
#define IronBlues {{228, 232, 209}, {89, 110, 121}}
#define SpriteZero {{234, 230, 214}, {9, 121, 65}}
#define Casio {{131, 176, 126}, {0, 0, 0}}

// Helper functions
void set_draw_colour(SDL_Renderer *renderer, Palette const *palette, bool light);

void reset_palette(Palette *palette, SDL_KeyCode key);