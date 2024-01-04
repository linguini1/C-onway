/**
 * Contains logic for creating and manipulating the simulation environment,
 * along with simulation analytics.
 * @author Matteo Golin
 * @version 1.0
 */

#include "../include/environment.h"
#include <assert.h>
#include <stdlib.h>

/* ENVIRONMENT MANAGEMENT */

/**
 * Create the Environment (grid) for cell growth to occur in, starting with all
 * dead cells.
 * @param width The width of the environment
 * @param height The height of the environment
 * @return a flattened 2D array of booleans representing the environment
 */
Environment *env_init(unsigned int width, unsigned int height, unsigned int generation_speed) {

    unsigned int size = width * height;

    // Create environment
    Environment *env = (Environment *)malloc(sizeof(Environment));
    assert(env != NULL);

    // Create simulation grid and next generation grid
    env->grid = (bool *)malloc(sizeof(bool) * size);
    assert(env->grid != NULL);
    env->_next_generation = (bool *)malloc(sizeof(bool) * size);
    assert(env->_next_generation != NULL);

    env->height = height;
    env->width = width;

    // Create simulation grid
    for (unsigned int i = 0; i < size; i++) {
        env->grid[i] = false; // All values initialized to false
    }

    // Simulation data
    env->data.total_cells = 0;
    env->data.initial_cells = 0;
    env->data.generations = 0;
    env->data.generation_speed = generation_speed;

    return env;
}

/**
 * Destroys an Environment struct.
 * @param env the environment to be freed.
 */
void env_destroy(Environment *env) {
    free(env->grid);
    free(env->_next_generation);
    free(env);
}

/**
 * Clear all cells from the simulation grid.
 * @param env The simulation environment to be cleared.
 */
void env_clear(Environment *env) {
    unsigned int size = env->width * env->height;
    for (unsigned int i = 0; i < size; i++) {
        env->grid[i] = false;
    }

    // Reset totals
    env->data.initial_cells = 0;
    env->data.total_cells = 0;
    env->data.generations = 0;
}

/* ENVIRONMENT ACCESS & MANIPULATION */

/**
 * Allows indexing of the flattened 2D array environment
 * @param env The environment to be accessed
 * @param x The x coordinate of the desired cell
 * @param y The y coordinate of the desired cell
 * @return
 */
bool env_access(Environment const *env, unsigned int x, unsigned int y) {
    assert(env_in_bounds(env, x, y));        // Assert x, y in bounds
    unsigned int i = ((env->width) * y) + x; // Calculate index
    return env->grid[i];
}

/**
 * Writes a value to the (x, y) coordinate in the simulation grid
 * @param env The environment to be modified
 * @param x The x coordinate of the location to be modified
 * @param y The y coordinate of the location to be modified
 * @param value The value to be written to the (x, y) location
 */
void env_write(Environment *env, unsigned int x, unsigned int y, bool value) {
    assert(env_in_bounds(env, x, y));        // Assert x, y in bounds
    unsigned int i = ((env->width) * y) + x; // Calculate index
    env->grid[i] = value;
}

/**
 * Checks if given (x, y) coordinates are within the bounds of the simulation
 * @param env The environment which sets the simulation bounds
 * @param x The x coordinate to be checked
 * @param y The y coordinate to be checked
 * @return true if (x, y) are within bounds, false otherwise
 */
bool env_in_bounds(Environment const *env, unsigned int x, unsigned int y) {
    unsigned int index = env->width * y + x;
    if (index < env->width * env->height) {
        return true;
    }
    return false;
}
