/**
* Contains operations for basic life operations, like cell state calculations.
 * @author Matteo Golin
 * @version 1.0
*/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "life.h"

// Constants
const Coordinate NEIGHBOURS[8] = {
        {0,  -1}, // Up
        {0,  1}, // Down
        {1,  0}, // Right
        {-1, 0}, // Left
        {1,  -1}, // Upper right
        {1,  1}, // Lower right
        {-1,  -1}, // Upper left
        {-1, 1} // Lower left
};

/**
 * Create the Environment (grid) for cell growth to occur in, starting with all dead cells.
 * @param width The width of the environment
 * @param height The height of the environment
 * @return a flattened 2D array of booleans representing the environment
 */
Environment *init_environment(int width, int height) {

    int size = width * height;

    // Create environment
    Environment *env = malloc(sizeof(Environment));
    assert(env != NULL);

    env->grid = (bool*) malloc(sizeof(bool) * size);
    assert(env->grid != NULL);
    env->height = height;
    env->width = width;

    // Create simulation grid
    for (int i = 0; i < size; i++) {
        env->grid[i] = false; // All values initialized to false
    }

    return env;
}

/**
 * Destroys an Environment struct.
 * @param env the environment to be freed.
 */
void destroy_env(Environment *env) {
    free(&(env->grid));
    free(env);
}

/**
 * Prints the simulation grid as 1s and 0s.
 * @param env The environment to be printed.
 */
void _debug_print_env(Environment *env) {
    for (int x = 0; x < env->width; x++) {
        for (int y = 0; y < env->height; y++) {
            printf("%d", access(env, x, y));
        }
        printf("\n");
    }
}

/**
 * Allows indexing of the flattened 2D array environment
 * @param env The environment to be accessed
 * @param x The x coordinate of the desired cell
 * @param y The y coordinate of the desired cell
 * @return
 */
bool access(Environment *env, int x, int y) {
    int i = ((env->width) * y) + x; // Calculate index
    return env->grid[i];
}

/**
 * Writes a value to the (x, y) coordinate in the simulation grid
 * @param env The environment to be modified
 * @param x The x coordinate of the location to be modified
 * @param y The y coordinate of the location to be modified
 * @param value The value to be written to the (x, y) location
 */
void write(Environment *env, int x, int y, bool value) {
    int i = ((env->width) * y) + x; // Calculate index
    env->grid[i] = value;
}


/**
 * If the coordinate is out of the environment boundaries, wrap it around to the opposite side.
 * @param coord
 * @return The wrapped coordinate
 */
Coordinate wrap(Environment *env, Coordinate coord) {
    if (coord.x < 0) {
        coord.x += env->width;
    }
    if (coord.x >= env->width) {
        coord.x -= env->width;
    }
    if (coord.y < 0) {
        coord.y += env->height;
    }
    if (coord.y >= env->height) {
        coord.y -= env->height;
    }
    return coord;
}

/**
 * Calculates the number of living neighbours surrounding the cell. Cells on the environment
 * border will look past the borders as though wrapping to the other side..
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The number of living neighbours around the current cell
 */
int num_neighbours(Environment *env, int x, int y) {

    int neighbours = 0;
    for (int i = 0; i < 8; i++) {

        // Calculate position of current neighbour in each of the 8 surrounding cells
        Coordinate position = NEIGHBOURS[i];
        Coordinate neighbour = {x + position.x, y + position.y};

        // If neighbour out of bounds, wrap around
        neighbour = wrap(env, neighbour);

        // Check if alive to increment total
        if (access(env, neighbour.x, neighbour.y)) {
            neighbours++;
        }
    }
    return neighbours;
}

/**
 * Calculates the next state for the cell at (x, y)
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool next_state(Environment *env, int x, int y) {
    int neighbours = num_neighbours(env, x, y);
    bool cell_state = access(env, x, y);

    // If a cell is alive:
    if (cell_state) {

        // If 1 or fewer neighbours, it dies
        if (neighbours <= 1) {
            return false;
        }

            // If 4 or more neighbours, it dies
        else if (neighbours >= 4) {
            return false;
        }

        // If it has 2-3 neighbours, it stays alive
        else {
            return true;
        }
    }

    // If a cell is dead
    else {
        // And it has exactly 3 neighbours, it becomes alive
        return neighbours == 3;
    }
}

/**
 * Steps through one generation of the simulation, calculating the next one.
 * @param env The environment to update with the next generation
 */
void next_generation(Environment *env) {

    // Update the copy with all the new states
    bool *copy = (bool*) malloc(sizeof(bool) * env->width * env->height);
    for (int x = 0; x < env->width; x++) {
        for (int y = 0; y < env->height; y++) {
            copy[(env->width * y) + x] = next_state(env, x, y); // Write next state onto the copy
        }
    }
    free(env->grid);
    env->grid = copy;
}

/**
 * Places a seed in the environment.
 * @param env The environment containing the simulation grid
 * @param seed The seed to be placed in the simulation
 */
void place_seed(Environment *env, Seed *seed) {
    for (int i = 0; i < seed->cells; i++) {
        Coordinate coord = seed->points[i];
        write(env, coord.x, coord.y, true);
    }
}

/**
 * Initializes a seed with space for the specified number of cells.
 * @param cells The number of cells the seed will have
 * @return a seed with space for the specified number of cells.
 */
Seed *init_seed(int cells) {
    Seed *seed = (Seed *) malloc(sizeof(Seed) + sizeof(Coordinate) * cells);
    assert(seed != NULL);
    seed->cells = cells;
    return seed;
}


/**
 * Creates a shoebox seed at the origin.
 * @param x The x coordinate at which to place the seed
 * @param y The y coordinate at which to place the seed
 * @return The shoebox seed
 */
Seed *ShoeBoxSeed(int x, int y) {
    Seed *seed = init_seed(9);

    Coordinate points[9] = {
            {-1 + x, -1 + y},
            {0 + x,  -1 + y},
            {1 + x,  -1 + y},
            {2 + x,  -1 + y},
            {-1 + x, 0 + y},
            {2 + x,  0 + y},
            {-1 + x, 1 + y},
            {1 + x,  1 + y},
            {2 + x,  1 + y}
    };
    memcpy(seed->points, points, sizeof(points));

    return seed;
}
