/**
 * Contains logic for creating and manipulating the simulation environment along
 * with simulation analytics.
 * @author Matteo Golin
 * @version 1.0
 */

#ifndef CONWAY_ENVIRONMENT_H
#define CONWAY_ENVIRONMENT_H

#include <stdbool.h>

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
    bool *_next_generation;
} Environment;

/* ENVIRONMENT MANAGEMENT */
Environment *env_init(unsigned int width, unsigned int height, unsigned int generation_speed);

void env_destroy(Environment *env);

void env_clear(Environment *env);

/* ENVIRONMENT ACCESS & MANIPULATION */
bool env_access(Environment const *env, unsigned int x, unsigned int y);

void env_write(Environment *env, unsigned int x, unsigned int y, bool value);

bool env_in_bounds(Environment const *env, unsigned int x, unsigned int y);

#endif // CONWAY_ENVIRONMENT_H
