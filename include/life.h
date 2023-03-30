/**
 * Contains types and function headers for running the simulation logic.
 * @author Matteo Golin
 * @version 1.0
 */
#include <stdbool.h>

// Custom types
typedef struct environment {
    int width;
    int height;
    bool *grid;
} Environment;

typedef struct coord {
    int x;
    int y;
} Coordinate;

typedef struct seed {
    int cells;
    Coordinate points[];
} Seed;

// Function headers
Environment *init_environment(int width, int height);

void destroy_env(Environment *env);

void _debug_print_env(Environment const *env);

bool access(Environment const *env, int x, int y);

void write(Environment *env, int x, int y, bool value);

Coordinate wrap(Environment *env, Coordinate coord);

int num_neighbours(Environment const *env, int x, int y);

bool next_state(Environment const *env, int x, int y);

void next_generation(Environment *env);

Seed *init_seed(int cells);

void destroy_seed(Seed *seed);

Coordinate translate(Coordinate coord, int x, int y);

void translate_coordinates(Coordinate *coords, int len, int x, int y);

void place_seed(Environment *env, Seed *seed);

Seed *ShoeBoxSeed(int x, int y);