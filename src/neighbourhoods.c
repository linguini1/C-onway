/**
* Contains logic for accessing different cell neighbourhoods.
 * @author Matteo Golin
 * @version 1.0
*/

#include "neighbourhoods.h"
#include <stdlib.h>

/* NEIGHBOURHOODS */
const Neighbourhood VON_NEUMANN = {4, {VonNeumann}};
const Neighbourhood VON_NEUMANN_CORNERS = {4, {VonNeumannCorners}};
const Neighbourhood MOORE = {8, {Moore}};
const Neighbourhood VON_NEUMANN_R2 = {12, {VonNeumannR2}};
const Neighbourhood TRIPLE_MOORE = {20, {TripleMoore}};
const Neighbourhood TRIPLE_MOORE_CORNER = {24, {TripleMooreCorner}};

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

/* NEIGHBOUR LOGIC */

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