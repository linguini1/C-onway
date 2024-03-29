/**
 * Contains rules for determining cell state, various cell types and logic for updating the simulation
 * environment using those rules.
 * @author Matteo Golin
 * @version 1.0
 */
#include "../include/rules.h"
#include "../include/asprintf.h"
#include <stdlib.h>

/**
 * Calculates the next state for the cell at (x, y) based on Conway's original Game of Life rules
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(conway_next_state) {
    unsigned int neighbours = num_neighbours(env, x, y, &MOORE);
    bool alive = env_access(env, x, y);

    // If a cell is alive and has:
    // 1 or fewer neighbours, it dies
    // 4 or more neighbours, it dies
    // 2-3 neighbours, it stays alive
    if (alive) return !(neighbours <= 1 || neighbours >= 4);

    // If a cell is dead and it has exactly 3 neighbours, it becomes alive
    return neighbours == 3;
}

/**
 * Calculates the next state for the cell at (x, y) based on the rules for Maze cells
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(maze_next_state) {
    bool alive = env_access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &MOORE);
    if (alive) return 2 <= neighbours && neighbours <= 5; // If already alive must have 2-5 neighbours to survive
    return neighbours == 3;                               // If dead, must have exactly 3 neighbours to live
}

/**
 * Calculates the next state for the cell at (x, y) based on the rules for Pixel cells
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(noise_next_state) {
    bool alive = env_access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &MOORE);
    if (alive) return 4 <= neighbours && neighbours <= 5; // If already alive must have 4-5 neighbours to survive
    return neighbours == 2;                               // If dead, must have exactly 2 neighbours to live
}

/**
 * Calculates the next state for the cell at (x, y) based on a variation of the original CGOL rules using the Von
 * Neumann neighbourhood
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(fractal_next_state) {
    bool alive = env_access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &VON_NEUMANN);
    if (alive) return (neighbours >= 2); // If already alive, must have more than two neighbours to live
    return neighbours == 1;              // If dead, must have exactly 1 neighbour to be born
}

/**
 * Calculates the next state for the cell at (x, y) based on a variation of the original CGOL rules using the Von
 * Neumann diamond-shaped neighbourhood
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(fractal_corner_next_state) {
    bool alive = env_access(env, x, y);
    unsigned int neighbours = num_neighbours(env, x, y, &VON_NEUMANN_CORNERS);
    if (alive) return neighbours >= 2; // If already alive Must have more than two neighbours to live
    return neighbours == 1;            // If dead, must have exactly 1 neighbour to be born
}

/**
 * Calculates the next state for the cell at (x, y) based on Conway's original Game of Life rules in the Lesse
 * neighbourhood.
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(lesse_conway_next_state) {
    unsigned int neighbours = num_neighbours(env, x, y, &LESSE);
    bool alive = env_access(env, x, y);

    // If a cell is alive and it has:
    // 1 or fewer neighbours, it dies
    // 4 or more neighbours, it dies
    // 2-3 neighbours, it stays alive
    if (alive) return !(neighbours <= 1 || neighbours >= 4);

    // If a cell is dead and it has exactly 3 neighbours, it becomes alive
    return neighbours == 3;
}

/**
 * Calculates the next state for the cell at (x, y) based on the Triple Moore variation of the original CGOL rules
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(triple_moore_conway_next_state) {

    bool alive = env_access(env, x, y);
    bool neighbour_vector[TRIPLE_MOORE.size];
    neighbours(env, x, y, &TRIPLE_MOORE, &neighbour_vector[0]); // Determine neighbours

    // The first eight neighbours
    int neighbour_count = 0;
    for (unsigned int i = 0; i < 8; i++)
        neighbour_count += neighbour_vector[i];
    int closest_eight = neighbour_count;

    // Get remaining neighbours using custom loop to save computation
    for (unsigned int i = 8; i < TRIPLE_MOORE.size; i++)
        neighbour_count += neighbour_vector[i];

    // If a cell is alive and it has:
    // 1 or fewer neighbours, it dies
    // 4 or more neighbours, it dies
    // 2-3 neighbours, it stays alive
    if (alive) return !(neighbour_count <= 4 || neighbour_count >= 11 || closest_eight > 5);

    // If a cell is dead and it has exactly 3 neighbours, it becomes alive
    return (neighbour_count <= 10 && neighbour_count >= 7) && (closest_eight > 2 && closest_eight < 5);
}

/**
 * Calculates the next state for the cell at (x, y) based on a more complex variation of Conway's original GOL rules.
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(von_neumann_r2_conway_next_state) {

    bool alive = env_access(env, x, y);
    bool neighbour_vector[VON_NEUMANN_R2.size];
    neighbours(env, x, y, &VON_NEUMANN_R2, &neighbour_vector[0]); // Determine neighbours

    // The first four neighbours
    int closest_four = neighbour_vector[0] + neighbour_vector[1] + neighbour_vector[2] + neighbour_vector[3];

    // Get remaining neighbours using custom loop to save computation
    int neighbour_count = closest_four;
    for (unsigned int i = 4; i < VON_NEUMANN_R2.size; i++) {
        neighbour_count += neighbour_vector[i];
    }

    // If a cell is alive:
    if (alive) {
        if (neighbour_count <= 2 || neighbour_count >= 6 || closest_four == 4) {
            // If 2 or fewer neighbours, it dies
            // If 6 or more neighbours, it dies
            // If four closest neighbours, it dies
            return false;
        }
        return (closest_four > 0); // If it has 3-4 neighbours, it stays alive
    }

    // If a cell is dead and it has exactly 4 neighbours, it becomes alive
    return (neighbour_count == 4) && (closest_four > 0);
}

/**
 * Calculates the next state for the cell at (x, y) to create a more organic maze shape.
 * Note: This pattern seems to occur if the original COGL rules are scaled to a larger neighbourhood. Cells begin to
 * grow uncontrollably. All other COGL variations included in this simulation have an additional control factor added,
 * like also considering the number of neighbours in another smaller neighbourhood.
 * @param env The environment that holds the simulation
 * @param x The x coordinate of the current cell
 * @param y The y coordinate of the current cell
 * @return The next state of the cell (true for alive, false for dead)
 */
state_calculator(conway_cancer_next_state) {
    unsigned int neighbours = num_neighbours(env, x, y, &VON_NEUMANN_R2);
    bool alive = env_access(env, x, y);

    // If a cell is alive and it has:
    // 2 or fewer neighbours, it dies
    // 5 or more neighbours, it dies
    // 3-4 neighbours, it stays alive
    if (alive) return !(neighbours <= 2 || neighbours > 6);

    // If a cell is dead and it has exactly 4 neighbours, it becomes alive
    return neighbours == 4;
}

/**
 * Populates a string with the most recent simulation analytics.
 * @param string Pointer to the string that will contain the analytics
 * @param env The environment to gather analytics on
 * @param cell_type The cell type currently being used in the simulation
 */
void populate_analytics_string(char **string, Environment const *env, CellType const *cell_type) {

    SimulationAnalytics data = env->data;
    double percent_alive = ((double)(data.total_cells) / (double)(env->width * env->height)) * 100.0;
    double initial_cells = data.initial_cells == 0 ? 1.0 : (double)data.initial_cells;
    double growth = ((double)data.total_cells / initial_cells) * 100.0;

    asprintf(string,
             "cell type: %s\ngenerations: %llu\ninitial cells: %u\ncells: %u\npercentage alive: %.3f%%\ngrowth: "
             "%.1f%%\ngeneration length: %ums",
             cell_type->name, data.generations, data.initial_cells, data.total_cells, percent_alive, growth,
             data.generation_speed);
}

/**
 * Steps through one generation of the simulation, calculating the next one.
 * @param env The environment to update with the next generation
 * @param cell_type The type of cell to calculate the next generation for
 */
void next_generation(Environment *env, CellType const *cell_type) {

    env->data.total_cells = 0; // Reset cell total
    env->data.generations++;   // Increase generations

    // Update the next generation with all the new states
    for (uint32_t x = 0; x < env->width; x++) {
        for (uint32_t y = 0; y < env->height; y++) {
            bool state = cell_type->calculator(env, x, y);
            env->data.total_cells += state;                      // Increase cell total
            env->_next_generation[(env->width * y) + x] = state; // Write next state onto the next generation grid
        }
    }

    // Swap current simulation grid for the next generation
    bool *temp = env->grid;
    env->grid = env->_next_generation;
    env->_next_generation = temp;
}
