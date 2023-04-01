/**
 * Contains types and function headers for running the simulation logic.
 * @author Matteo Golin
 * @version 1.0
 */
#ifndef CONWAY_LIFE_H
#define CONWAY_LIFE_H

#include <stdbool.h>
#include <SDL.h>

// Custom types
typedef struct sim_data {
    long unsigned int total_cells;
    unsigned int initial_cells;
    long long unsigned int generations;
    unsigned int generation_speed;
} SimulationAnalytics;

typedef struct environment {
    unsigned int width;
    unsigned int height;
    SimulationAnalytics data;
    bool *grid;
} Environment;

typedef struct coord {
    int x;
    int y;
} Coordinate;

typedef struct seed {
    unsigned int cells;
    Coordinate *points;
} Seed;

// State calculation functions
typedef bool (*StateCalculator)(Environment const *, unsigned int, unsigned int);

typedef struct cell_type{
    char *name;
    StateCalculator calculator;
} CellType;

#define ConwayCell {"conway cell", conway_next_state}
#define MazeCell {"maze cell", maze_next_state}
#define NoiseCell {"noise cell", noise_next_state}
#define StableCell {"stable cell", stable_next_state}
#define BridgeCell {"bridge cell", bridge_next_state};

/* FUNCTION HEADERS */

/* SIMULATION ANALYTICS */
void populate_analytics_string(char **string, Environment const *env, CellType *cell_type);

/* COORDINATE MANIPULATION */
Coordinate translate(Coordinate coord, int x, int y);

void translate_coordinates(Coordinate *coords, unsigned int len, int x, int y);

Coordinate wrap(Environment const *env, Coordinate coord);

/* SIMULATION ENVIRONMENT */
Environment *init_environment(unsigned int width, unsigned int height, unsigned int generation_speed);

void destroy_env(Environment *env);

void debug_print_environment(Environment const *env);

void clear_env(Environment *env);

bool in_bounds(Environment const *env, unsigned int x, unsigned int y);

bool access(Environment const *env, unsigned int x, unsigned int y);

void write(Environment *env, unsigned int x, unsigned int y, bool value);

int num_neighbours(Environment const *env, unsigned int x, unsigned int y, unsigned int consider);

void next_generation(Environment *env, CellType *cell_type);

/* STATE CALCULATORS */
bool conway_next_state(Environment const *env, unsigned int x, unsigned int y);

bool maze_next_state(Environment const *env, unsigned int x, unsigned int y);

bool noise_next_state(Environment const *env, unsigned int x, unsigned int y);

bool stable_next_state(Environment const *env, unsigned int x, unsigned int y);

bool bridge_next_state(Environment const *env, unsigned int x, unsigned int y);

/* CELL TYPES */
void change_cell_type(CellType *cell_type, SDL_KeyCode key);

/* SEEDS */
Seed *init_seed(unsigned int cells);

void destroy_seed(Seed *seed);

void place_seed(Environment *env, Seed const *seed);

// Custom seeds
Seed *ShoeBoxSeed(unsigned int x, unsigned int y);

#endif