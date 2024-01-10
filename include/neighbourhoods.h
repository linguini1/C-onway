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

/* CUSTOM TYPES */
typedef struct coord {
    int x;
    int y;
} Coordinate;

typedef struct neighbourhood {
    unsigned short int size;
    Coordinate neighbours[];
} Neighbourhood;

/* NEIGHBOURHOODS */
#define VonNeumann                                                                                                     \
    {0, -1}, {0, 1}, {1, 0}, { -1, 0 }
#define VonNeumannCorners                                                                                              \
    {1, -1}, {1, 1}, {-1, -1}, { -1, 1 }
#define Lesse                                                                                                          \
    VonNeumannCorners, {0, -2}, {0, 2}, {2, 0}, { -2, 0 }
#define Moore VonNeumann, VonNeumannCorners
#define VonNeumannR2                                                                                                   \
    Moore, {0, -2}, {0, 2}, {2, 0}, { -2, 0 }
#define TripleMoore                                                                                                    \
    VonNeumannR2, {-1, -2}, {1, -2}, {-1, 2}, {1, 2}, {-2, -1}, {-2, 1}, {2, -1}, { 2, 1 }
#define TripleMooreCorner                                                                                              \
    TripleMoore, {-2, -2}, {-2, 2}, {2, -2}, { 2, 2 }

/* EXTERNAL NEIGHBOURHOOD CONSTANTS */
extern const Neighbourhood VON_NEUMANN;
extern const Neighbourhood VON_NEUMANN_CORNERS;
extern const Neighbourhood LESSE;
extern const Neighbourhood MOORE;
extern const Neighbourhood VON_NEUMANN_R2;
extern const Neighbourhood TRIPLE_MOORE;
extern const Neighbourhood TRIPLE_MOORE_CORNER;

/* COORDINATE MANIPULATION */
Coordinate translate(Coordinate coord, int x, int y);
void translate_coordinates(Coordinate *coords, unsigned int len, int x, int y);
Coordinate wrap(Environment const *env, Coordinate coord);

/* NEIGHBOUR LOGIC */
bool *neighbours(Environment const *env, unsigned int x, unsigned int y, Neighbourhood const *neighbourhood,
                 bool *neighbour_states);
unsigned int num_neighbours(Environment const *env, unsigned int x, unsigned int y, Neighbourhood const *neighbourhood);

#endif // CONWAY_NEIGHBOURHOODS_H
