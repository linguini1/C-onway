/**
*
*/

#ifndef CONWAY_RULES_H
#define CONWAY_RULES_H

#include "environment.h"
#include "neighbourhoods.h"
#include <SDL.h>

/* CUSTOM TYPES */

typedef bool (*StateCalculator)(Environment const *, unsigned int, unsigned int);

typedef struct cell_type {
    char *name;
    StateCalculator calculator;
} CellType;

/* FUNCTION HEADERS */

/* STATE CALCULATORS */
bool conway_next_state(Environment const *env, unsigned int x, unsigned int y);

bool maze_next_state(Environment const *env, unsigned int x, unsigned int y);

bool noise_next_state(Environment const *env, unsigned int x, unsigned int y);

bool fractal_next_state(Environment const *env, unsigned int x, unsigned int y);

bool triple_moore_conway_next_state(Environment const *env, unsigned int x, unsigned int y);

bool organic_maze_next_state(Environment const *env, unsigned int x, unsigned int y);

bool von_neumann_r2_conway_next_state(Environment const *env, unsigned int x, unsigned int y);

/* CELL TYPES */
#define ConwayCell {"conway cell", conway_next_state}
#define MazeCell {"maze cell", maze_next_state}
#define NoiseCell {"noise cell", noise_next_state}
#define FractalCell {"fractal cell", fractal_next_state}
#define FractalCornerCell {"fractal corner cell", fractal_corner_next_state}
#define TripleMooreConwayCell {"triple moore conway cell", triple_moore_conway_next_state}
#define OrganicMazeCell {"organic maze cell", organic_maze_next_state}
#define VonNeumannR2ConwayCell {"von neumann r2 conway cell", von_neumann_r2_conway_next_state}

void change_cell_type(CellType *cell_type, SDL_KeyCode key);

/* SIMULATION ANALYTICS */
void populate_analytics_string(char **string, Environment const *env, CellType *cell_type);

/* SIMULATION ENVIRONMENT */

void next_generation(Environment *env, CellType *cell_type);

#endif