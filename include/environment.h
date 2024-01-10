/**
 * Contains logic for creating and manipulating the simulation environment along
 * with simulation analytics.
 * @author Matteo Golin
 * @version 1.0
 */
#ifndef CONWAY_ENVIRONMENT_H
#define CONWAY_ENVIRONMENT_H

#include <stdbool.h>
#include <stdint.h>

/** Bundle of simulation analytics data. */
typedef struct {
    uint32_t total_cells;      /**< The total number of cells in the simulation grid at a given time. */
    uint32_t initial_cells;    /**< The number of initial cells (user drawn) in the simulation. */
    uint64_t generations;      /**< The number of generations that have passed. */
    uint16_t generation_speed; /**< The speed of each generation in milliseconds. */
} SimulationAnalytics;

/** Represents the simulation environment. */
typedef struct environment {
    uint32_t width;           /**< The width of the simulation grade. */
    uint32_t height;          /**< The height of the simulation grid. */
    SimulationAnalytics data; /**< The simulation analytics corresponding to this environment. */
    bool *grid;               /**< The current cell grid. */
    bool *_next_generation;   /**< The cell grid for placing the next calculated grid. */
} Environment;

Environment *env_init(uint32_t width, uint32_t height, uint16_t generation_speed);
void env_destroy(Environment *env);
void env_clear(Environment *env);
bool env_access(Environment const *env, uint32_t x, uint32_t y);
void env_write(Environment *env, uint32_t x, uint32_t y, bool value);
bool env_in_bounds(Environment const *env, uint32_t x, uint32_t y);
bool env_toggle_cell(Environment *env, uint32_t x, uint32_t y);

#endif // CONWAY_ENVIRONMENT_H
