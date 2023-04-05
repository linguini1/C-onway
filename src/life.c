/**
* Contains operations for basic life operations, like cell state calculations.
 * @author Matteo Golin
 * @version 1.0
*/
#include "life.h"
#include "asprintf.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* CONSTANTS */

/* NEIGHBOURHOODS */
const Neighbourhood VON_NEUMANN = {4, {VonNeumann}};
const Neighbourhood VON_NEUMAN_CORNERS = {4, {VonNeumannCorners}};
const Neighbourhood MOORE = {8, {Moore}};
const Neighbourhood VON_NEUMANN_R2 = {12, {VonNeumannR2}};
const Neighbourhood TRIPLE_MOORE = {20, {TripleMoore}};
const Neighbourhood TRIPLE_MOORE_CORNER = {24, {TripleMooreCorner}};

/* SIMULATION ANALYTICS */

void populate_analytics_string(char **string, Environment const *env, CellType *cell_type) {

    SimulationAnalytics data = env->data;
    float percent_alive = ((float) (data.total_cells) / (float) (env->width * env->height)) * 100.0f;
    float initial_cells = data.initial_cells == 0 ? 1.0f : (float) data.initial_cells;
    float growth = ((float) data.total_cells / initial_cells) * 100.0f;

    asprintf(
            string,
            "cell type: %s\ngenerations: %llu\ninitial cells: %u\ncells: %lu\npercentage alive: %.3f%%\ngrowth: %.1f%%\ngeneration length: %ums",
            cell_type->name,
            data.generations,
            data.initial_cells,
            data.total_cells,
            percent_alive,
            growth,
            data.generation_speed
    );
}

/* COORDINATE MANIPULATION */

/**
 * Translates the coordinate.
 * @param coord The coordinate to be translated
 * @param x The x amount to translate the coordinate by
 * @param y The y amount to translate the coordinate by
 * @return A new coordinate which is the translated version of the passed coordinate.
 */
Coordinate translate(Coordinate coord, int x, int y) {
    return (Coordinate) {coord.x + x, coord.y + y};
}

/**
 * Translates all coordinates in an array.
 * @param coords The array of coordinates to be translated.
 * @param len The length of the array
 * @param x The x amount to translate all coordinates by
 * @param y The y amount to translate all coordinates by
 */
void translate_coordinates(Coordinate *coords, unsigned int len, int x, int y) {
    for (int i = 0; i < len; i++) {
        coords[i] = translate(coords[i], x, y);
    }
}

/**
 * If the coordinate is out of the environment boundaries, wrap it around to the opposite side.
 * @param coord
 * @return The wrapped coordinate
 */
Coordinate wrap(Environment const *env, Coordinate coord) {
    if (coord.x < 0) {
        coord.x += (int) env->width;
    }
    if (coord.x >= env->width) {
        coord.x -= (int) env->width;
    }
    if (coord.y < 0) {
        coord.y += (int) env->height;
    }
    if (coord.y >= env->height) {
        coord.y -= (int) env->height;
    }
    return coord;
}

/* SIMULATION ENVIRONMENT */

/**
 * Create the Environment (grid) for cell growth to occur in, starting with all dead cells.
 * @param width The width of the environment
 * @param height The height of the environment
 * @return a flattened 2D array of booleans representing the environment
 */
Environment *init_environment(unsigned int width, unsigned int height, unsigned int generation_speed) {

    unsigned int size = width * height;

    // Create environment
    Environment *env = (Environment *) malloc(sizeof(Environment));
    assert(env != NULL);

    env->grid = (bool *) malloc(sizeof(bool) * size);
    assert(env->grid != NULL);
    env->height = height;
    env->width = width;

    // Create simulation grid
    for (int i = 0; i < size; i++) {
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
void destroy_env(Environment *env) {
    free(env->grid);
    free(env);
}

/**
 * Prints the simulation grid as 1s and 0s.
 * @param env The environment to be printed.
 */
void debug_print_environment(Environment const *env) {
    for (int x = 0; x < env->width; x++) {
        for (int y = 0; y < env->height; y++) {
            printf("%d", access(env, x, y));
        }
        printf("\n");
    }
}

/**
 * Clear all cells from the simulation grid.
 * @param env The simulation environment to be cleared.
 */
void clear_env(Environment *env) {
    unsigned int size = env->width * env->height;
    for (unsigned int i = 0; i < size; i++) {
        env->grid[i] = false;
    }

    // Reset totals
    env->data.initial_cells = 0;
    env->data.total_cells = 0;
    env->data.generations = 0;
}

/**
 * Allows indexing of the flattened 2D array environment
 * @param env The environment to be accessed
 * @param x The x coordinate of the desired cell
 * @param y The y coordinate of the desired cell
 * @return
 */
bool access(Environment const *env, unsigned int x, unsigned int y) {
    assert(in_bounds(env, x, y)); // Assert x, y in bounds
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
void write(Environment *env, unsigned int x, unsigned int y, bool value) {
    assert(in_bounds(env, x, y)); // Assert x, y in bounds
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
bool in_bounds(Environment const *env, unsigned int x, unsigned int y) {
    unsigned int index = env->width * y + x;
    if (index < env->width * env->height) {
        return true;
    }
    return false;
}

/**
 * Returns an array of 8 booleans representing the state of each of a cells 8 neighbours (in order of the NEIGHBOURS
 * constant)
 * @param env The environment where the cell lives
 * @param x The x coordinate of the cell being examined
 * @param y The y coordinate of the cell being examined
 * @param consider The number of neighbours to consider from the NEIGHBOURS constant, in the order they appear
 * @return An array of 8 booleans corresponding to the cell's 8 neighbours' states
 */
bool *neighbours(Environment const *env, unsigned int x, unsigned int y, Neighbourhood const *neighbourhood) {

    // Create the array to hold neighbour states
    bool *neighbour_states = malloc(sizeof(bool) * neighbourhood->size);

    for (unsigned int i = 0; i < neighbourhood->size; i++) {
        // Calculate position of current neighbour in each of the 8 surrounding cells
        Coordinate position = neighbourhood->neighbours[i];
        Coordinate neighbour = {(int) x + position.x, (int) y + position.y};
        neighbour = wrap(env, neighbour);  // If neighbour out of bounds, wrap around

        // Store states
        neighbour_states[i] = access(env, neighbour.x, neighbour.y);
    }
    return neighbour_states;
}

/**
 * Calculates the number of living neighbours surrounding the cell. Cells on the environment
 * border will look past the borders as though wrapping to the other side..
 * @param env The environment where the cell lives
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @param consider The number of neighbours to consider from the NEIGHBOURS constant, in the order they appear
 * @return The number of living neighbours around the current cell
 */
unsigned int
num_neighbours(Environment const *env, unsigned int x, unsigned int y, Neighbourhood const *neighbourhood) {

    int neighbour_count = 0;
    bool *neighbour_states = neighbours(env, x, y, neighbourhood); // Get neighbour states

    for (int i = 0; i < neighbourhood->size; i++) {
        neighbour_count += neighbour_states[i]; // 1 if true, 0 if false, so total will be number of alive states
    }
    free(neighbour_states); // No longer used
    return neighbour_count;
}

/**
 * Steps through one generation of the simulation, calculating the next one.
 * @param env The environment to update with the next generation
 */
void next_generation(Environment *env, CellType *cell_type) {

    env->data.total_cells = 0; // Reset cell total
    env->data.generations++; // Increase generations

    // Update the copy with all the new states
    bool *copy = (bool *) malloc(sizeof(bool) * env->width * env->height);
    for (int x = 0; x < env->width; x++) {
        for (int y = 0; y < env->height; y++) {
            bool state = cell_type->calculator(env, x, y);

            // Increase cell total
            if (state) {
                env->data.total_cells++;
            }
            copy[(env->width * y) + x] = state; // Write next state onto the copy
        }
    }
    free(env->grid);
    env->grid = copy;
}

/* STATE CALCULATORS */

/**
 * Calculates the next state for the cell at (x, y) based on Conway's original Game of Life rules
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool conway_next_state(Environment const *env, unsigned int x, unsigned int y) {
    unsigned int neighbours = num_neighbours(env, x, y, &MOORE);
    bool alive = access(env, x, y);

    // If a cell is alive:
    if (alive) {
        if (neighbours <= 1 || neighbours >= 4) {
            // If 1 or fewer neighbours, it dies
            // If 4 or more neighbours, it dies
            return false;
        } else {
            return true;  // If it has 2-3 neighbours, it stays alive
        }
    }

        // If a cell is dead
    else {
        // And it has exactly 3 neighbours, it becomes alive
        return neighbours == 3;
    }
}

/**
 * Calculates the next state for the cell at (x, y) based on the rules for Maze cells
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool maze_next_state(Environment const *env, unsigned int x, unsigned int y) {
    bool alive = access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &MOORE);

    // If already alive
    if (alive) {
        if (2 <= neighbours && neighbours <= 5) {
            return true; // must have 2-5 neighbours to survive
        }
        return false;
    }
        // If dead
    else {
        return neighbours == 3; // Exactly 3 neighbours to live
    }
}

/**
 * Calculates the next state for the cell at (x, y) based on the rules for Pixel cells
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool noise_next_state(Environment const *env, unsigned int x, unsigned int y) {
    bool alive = access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &MOORE);

    // If already alive
    if (alive) {
        if (4 <= neighbours && neighbours <= 5) {
            return true; // must have 4-5 neighbours to survive
        }
        return false;
    }
        // If dead
    else {
        return neighbours == 2; // Exactly 2 neighbours to live
    }
}

/**
 * Calculates the next state for the cell at (x, y) based on a variation of the original CGOL rules using the Von
 * Neumann neighbourhood
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool fractal_next_state(Environment const *env, unsigned int x, unsigned int y) {
    bool alive = access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &VON_NEUMANN);

    // If already alive
    if (alive) {
        if (neighbours >= 2) {
            return true; // Must have more than two neighbours to live
        }
        return false;
    }
        // If dead
    else {
        return neighbours == 1; // Must have exactly 1 neighbour to be born
    }
}

/**
 * Calculates the next state for the cell at (x, y) based on a variation of the original CGOL rules using the Von
 * Neumann diamond-shaped neighbourhood
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool fractal_corner_next_state(Environment const *env, unsigned int x, unsigned int y) {
    bool alive = access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &VON_NEUMAN_CORNERS);

    // If already alive
    if (alive) {
        if (neighbours >= 2) {
            return true; // Must have more than two neighbours to live
        }
        return false;
    }
        // If dead
    else {
        return neighbours == 1; // Must have exactly 1 neighbour to be born
    }
}

/**
 * Calculates the next state for the cell at (x, y) based on the Triple Moore variation of the original CGOL rules
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool triple_moore_conway_next_state(Environment const *env, unsigned int x, unsigned int y) {

    bool alive = access(env, x, y);
    bool *neighbour_vector = neighbours(env, x, y, &TRIPLE_MOORE); // Determine neighbours

    // The first eight neighbours
    int neighbour_count = 0;
    for (unsigned int i = 0; i < 8; i++) {
        neighbour_count += neighbour_vector[i];
    }
    int closest_eight = neighbour_count;

    // Get remaining neighbours using custom loop to save computation
    for (unsigned int i = 8; i < TRIPLE_MOORE.size; i++) {
        neighbour_count += neighbour_vector[i];
    }
    free(neighbour_vector); // Done with vector

    // If a cell is alive:
    if (alive) {
        if (neighbour_count <= 4 || neighbour_count >= 11 || closest_eight > 5) {
            // If 1 or fewer neighbours, it dies
            // If 4 or more neighbours, it dies
            return false;
        } else {
            return true;  // If it has 2-3 neighbours, it stays alive
        }
    }

        // If a cell is dead
    else {
        // And it has exactly 3 neighbours, it becomes alive
        return (neighbour_count <= 10 && neighbour_count >= 7) && (closest_eight > 2 && closest_eight < 5);
    }
}

/**
 * Calculates the next state for the cell at (x, y) to create a more organic maze shape.
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool organic_maze_next_state(Environment const *env, unsigned int x, unsigned int y) {
    unsigned int neighbours = num_neighbours(env, x, y, &VON_NEUMANN_R2);
    bool alive = access(env, x, y);

    // If a cell is alive:
    if (alive) {
        if (neighbours <= 2 || neighbours > 6) {
            // If 2 or fewer neighbours, it dies
            // If 5 or more neighbours, it dies
            return false;
        } else {
            return true;  // If it has 3-4 neighbours, it stays alive
        }
    }

        // If a cell is dead
    else {
        // And it has exactly 4 neighbours, it becomes alive
        return neighbours == 4;
    }
}

/**
 * Calculates the next state for the cell at (x, y) based on a more complex variation of Conway's original GOL rules.
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
bool von_neumann_r2_conway_next_state(Environment const *env, unsigned int x, unsigned int y) {

    bool alive = access(env, x, y);
    bool *neighbour_vector = neighbours(env, x, y, &VON_NEUMANN_R2); // Determine neighbours

    // The first four neighbours
    int closest_four = neighbour_vector[0] + neighbour_vector[1] + neighbour_vector[2] + neighbour_vector[3];

    // Get remaining neighbours using custom loop to save computation
    int neighbour_count = closest_four;
    for (unsigned int i = 4; i < VON_NEUMANN_R2.size; i++) {
        neighbour_count += neighbour_vector[i];
    }
    free(neighbour_vector); // Done with vector

    // If a cell is alive:
    if (alive) {
        if (neighbour_count <= 2 || neighbour_count >= 6 || closest_four == 4) {
            // If 2 or fewer neighbours, it dies
            // If 6 or more neighbours, it dies
            // If four closest neighbours, it dies
            return false;
        } else {
            return (closest_four > 0);  // If it has 3-4 neighbours, it stays alive
        }
    }

        // If a cell is dead
    else {
        // And it has exactly 4 neighbours, it becomes alive
        return (neighbour_count == 4) && (closest_four > 0);
    }
}

/* CELL TYPES */
/**
 * Resets the cell type to the specified cell type.
 * @param cell_type A pointer to the current cell type variable being used by the game
 */
void change_cell_type(CellType *cell_type, SDL_KeyCode key) {
    switch (key) {
        case SDLK_2:
            *cell_type = (CellType) MazeCell;
            break;
        case SDLK_3:
            *cell_type = (CellType) NoiseCell;
            break;
        case SDLK_4:
            *cell_type = (CellType) FractalCell;
            break;
        case SDLK_5:
            *cell_type = (CellType) TripleMooreConwayCell;
            break;
        case SDLK_6:
            *cell_type = (CellType) VonNeumannR2ConwayCell;
            break;
        case SDLK_7:
            *cell_type = (CellType) OrganicMazeCell;
            break;
        default:
            *cell_type = (CellType) ConwayCell; // Also handles key 1
            break;
    }
}


/* SEEDS */

/**
 * Initializes a seed with space for the specified number of cells.
 * @param cells The number of cells the seed will have
 * @return a seed with space for the specified number of cells.
 */
Seed *init_seed(unsigned int cells) {
    Seed *seed = (Seed *) malloc(sizeof(Seed));
    assert(seed != NULL);
    seed->cells = cells;
    seed->points = (Coordinate *) malloc(sizeof(Coordinate) * cells);
    assert(seed->points != NULL);
    return seed;
}

/**
 * Frees a seed from memory.
 * @param seed The seed to be freed.
 */
void destroy_seed(Seed *seed) {
    free(seed->points);
    free(seed);
}

/**
 * Places a seed in the environment.
 * @param env The environment containing the simulation grid
 * @param seed The seed to be placed in the simulation
 */
void place_seed(Environment *env, Seed const *seed) {
    env->data.initial_cells = seed->cells; // Record initial cells
    for (int i = 0; i < seed->cells; i++) {
        Coordinate coord = seed->points[i];
        write(env, coord.x, coord.y, true);
    }
}

// Custom seeds

/**
 * Creates a shoebox seed at the origin.
 * @param x The x coordinate at which to place the seed
 * @param y The y coordinate at which to place the seed
 * @return The shoebox seed
 */
Seed *ShoeBoxSeed(unsigned int x, unsigned int y) {
    Seed *seed = init_seed(9);

    seed->points[0] = (Coordinate) {-1, -1};
    seed->points[1] = (Coordinate) {0, -1};
    seed->points[2] = (Coordinate) {1, -1};
    seed->points[3] = (Coordinate) {2, -1};
    seed->points[4] = (Coordinate) {-1, 0};
    seed->points[5] = (Coordinate) {2, 0};
    seed->points[6] = (Coordinate) {-1, 1};
    seed->points[7] = (Coordinate) {1, 1};
    seed->points[8] = (Coordinate) {2, 1};

    // Translate points and store in the seed
    translate_coordinates(seed->points, seed->cells, (int) x, (int) y);

    return seed;
}
