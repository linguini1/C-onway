/**
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
} Environment;

/* SIMULATION ENVIRONMENT */
Environment *init_environment(unsigned int width, unsigned int height, unsigned int generation_speed);

void destroy_env(Environment *env);

void clear_env(Environment *env);

bool access(Environment const *env, unsigned int x, unsigned int y);

void write(Environment *env, unsigned int x, unsigned int y, bool value);

bool in_bounds(Environment const *env, unsigned int x, unsigned int y);

#endif