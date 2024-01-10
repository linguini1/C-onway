/**
 * Contains logic for accessing different cell neighbourhoods, as well as some included cell neighbourhoods from
 * established cellular automata.
 * @author Matteo Golin
 * @version 1.0
 */
#ifndef CONWAY_NEIGHBOURHOODS_H
#define CONWAY_NEIGHBOURHOODS_H

#include "environment.h"
#include <stdbool.h>

/** Represents a coordinate in the 2D plane. */
typedef struct coord {
    int32_t x; /**< The x component of the coordinate. */
    int32_t y; /**< The y component of the coordinate. */
} Coordinate;

/** Represents a cell's neighbourhood. */
typedef struct neighbourhood {
    uint8_t size;            /**< The number of grid cells in the neighbourhood. */
    Coordinate neighbours[]; /**< The locations of each neighbour in relative vectors from the cell being considered. */
} Neighbourhood;

/** Von Neumann neighbourhood. */
#define VonNeumann                                                                                                     \
    {0, -1}, {0, 1}, {1, 0}, { -1, 0 }
/** Von Neumann neighbourhood (just the corners). */
#define VonNeumannCorners                                                                                              \
    {1, -1}, {1, 1}, {-1, -1}, { -1, 1 }
/** Lesse neighbourhood. */
#define Lesse                                                                                                          \
    VonNeumannCorners, {0, -2}, {0, 2}, {2, 0}, { -2, 0 }
/** Moore neighbourhood. */
#define Moore VonNeumann, VonNeumannCorners
/** Von Neumann R2 neighbourhood. */
#define VonNeumannR2                                                                                                   \
    Moore, {0, -2}, {0, 2}, {2, 0}, { -2, 0 }
/** Triple Moore neighbourhood. */
#define TripleMoore                                                                                                    \
    VonNeumannR2, {-1, -2}, {1, -2}, {-1, 2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, { 2, 1 }
/** Triple Moore corners neighbourhood. */
#define TripleMooreCorner                                                                                              \
    TripleMoore, {-2, -2}, {-2, 2}, {2, -2}, { 2, 2 }

extern const Neighbourhood VON_NEUMANN;
extern const Neighbourhood VON_NEUMANN_CORNERS;
extern const Neighbourhood LESSE;
extern const Neighbourhood MOORE;
extern const Neighbourhood VON_NEUMANN_R2;
extern const Neighbourhood TRIPLE_MOORE;
extern const Neighbourhood TRIPLE_MOORE_CORNER;

Coordinate translate(Coordinate coord, int32_t x, int32_t y);
void translate_coordinates(Coordinate *coords, size_t len, int32_t x, int32_t y);
Coordinate wrap(Environment const *env, Coordinate coord);
bool *neighbours(Environment const *env, uint32_t x, uint32_t y, Neighbourhood const *neighbourhood,
                 bool *neighbour_states);
unsigned int num_neighbours(Environment const *env, uint32_t x, uint32_t y, Neighbourhood const *neighbourhood);

#endif // CONWAY_NEIGHBOURHOODS_H
