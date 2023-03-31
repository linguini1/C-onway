/**
 * Contains types and function headers for running the simulation logic.
 * @author Matteo Golin
 * @version 1.0
 */
#ifndef CONWAY_LIFE_H
#define CONWAY_LIFE_H

#include <stdbool.h>

// Custom types
typedef struct sim_data {
    long unsigned int total_cells;
    unsigned int initial_cells;
    long long unsigned int generations;
    unsigned int frame_speed;
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

// Function headers
void populate_analytics_string(char **string, Environment const *env);

Environment *init_environment(unsigned int width, unsigned int height, unsigned int frame_speed);

void clear_env(Environment *env);

void destroy_env(Environment *env);

void _debug_print_env(Environment const *env);

bool access(Environment const *env, unsigned int x, unsigned int y);

void write(Environment *env, unsigned int x, unsigned int y, bool value);

Coordinate wrap(Environment const *env, Coordinate coord);

int num_neighbours(Environment const *env, int x, int y);

bool next_state(Environment const *env, int x, int y);

void next_generation(Environment *env);

Seed *init_seed(unsigned int cells);

void destroy_seed(Seed *seed);

Coordinate translate(Coordinate coord, int x, int y);

void translate_coordinates(Coordinate *coords, unsigned int len, int x, int y);

void place_seed(Environment *env, Seed const *seed);

Seed *ShoeBoxSeed(unsigned int x, unsigned int y);

#endif