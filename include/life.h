/**
 * Contains types and function headers for running the simulation logic.
 * @author Matteo Golin
 * @version 1.0
 */
#ifndef CONWAY_LIFE_H
#define CONWAY_LIFE_H

#include "neighbourhoods.h"
#include "environment.h"
#include <stdbool.h>
#include <SDL.h>

/* NEIGHBOURHOODS */
#define VonNeumann {0,-1}, {0,1}, {1,0}, {-1,0}
#define VonNeumannCorners {1,-1}, {1,1}, {-1,-1}, {-1,1}
#define Moore VonNeumann, VonNeumannCorners
#define VonNeumannR2 Moore, {0,-2}, {0,2}, {2,0}, {-2,0}
#define TripleMoore VonNeumannR2, {-1,-2}, {1,-2}, {-1,2}, {1, 2}, {-2,-1}, {-2,1}, {2,-1}, {2,1}
#define TripleMooreCorner TripleMoore, {-2,-2}, {-2,2}, {2,-2}, {2,2}

// State calculation functions
typedef bool (*StateCalculator)(Environment const *, unsigned int, unsigned int);

typedef struct cell_type {
    char *name;
    StateCalculator calculator;
} CellType;

#define ConwayCell {"conway cell", conway_next_state}
#define MazeCell {"maze cell", maze_next_state}
#define NoiseCell {"noise cell", noise_next_state}
#define FractalCell {"fractal cell", fractal_next_state}
#define FractalCornerCell {"fractal corner cell", fractal_corner_next_state}
#define TripleMooreConwayCell {"triple moore conway cell", triple_moore_conway_next_state}
#define OrganicMazeCell {"organic maze cell", organic_maze_next_state}
#define VonNeumannR2ConwayCell {"von neumann r2 conway cell", von_neumann_r2_conway_next_state}

/* FUNCTION HEADERS */

/* SIMULATION ANALYTICS */
void populate_analytics_string(char **string, Environment const *env, CellType *cell_type);

/* COORDINATE MANIPULATION */
Coordinate wrap(Environment const *env, Coordinate coord);

/* SIMULATION ENVIRONMENT */

bool *neighbours(Environment const *env, unsigned int x, unsigned int y, Neighbourhood const *neighbourhood);

unsigned int num_neighbours(Environment const *env, unsigned int x, unsigned int y, Neighbourhood const *neighbourhood);

void next_generation(Environment *env, CellType *cell_type);

/* STATE CALCULATORS */
bool conway_next_state(Environment const *env, unsigned int x, unsigned int y);

bool maze_next_state(Environment const *env, unsigned int x, unsigned int y);

bool noise_next_state(Environment const *env, unsigned int x, unsigned int y);

bool fractal_next_state(Environment const *env, unsigned int x, unsigned int y);

bool triple_moore_conway_next_state(Environment const *env, unsigned int x, unsigned int y);

bool organic_maze_next_state(Environment const *env, unsigned int x, unsigned int y);

bool von_neumann_r2_conway_next_state(Environment const *env, unsigned int x, unsigned int y);

/* CELL TYPES */
void change_cell_type(CellType *cell_type, SDL_KeyCode key);

#endif