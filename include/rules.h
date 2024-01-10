/**
 * Contains rules for determining cell state, various cell types and logic for updating the simulation
 * environment using those rules.
 * @author Matteo Golin
 * @version 1.0
 */

#ifndef CONWAY_RULES_H
#define CONWAY_RULES_H

#include "environment.h"
#include "neighbourhoods.h"
#include <SDL2/SDL.h>

typedef bool (*StateCalculator)(Environment const *, uint32_t, uint32_t);

/** Represents a type of cell. */
typedef struct cell_type {
    const char *name;           /**< The name of the cell type. */
    StateCalculator calculator; /**< The function to use for calculating the next state of a cell of this type. */
} CellType;

#define state_calculator(name) bool name(Environment const *env, uint32_t x, uint32_t y)
state_calculator(conway_next_state);
state_calculator(maze_next_state);
state_calculator(noise_next_state);
state_calculator(fractal_next_state);
state_calculator(fractal_corner_next_state);
state_calculator(lesse_conway_next_state);
state_calculator(triple_moore_conway_next_state);
state_calculator(conway_cancer_next_state);
state_calculator(von_neumann_r2_conway_next_state);

#define ConwayCell                                                                                                     \
    { "conway cell", conway_next_state }
#define MazeCell                                                                                                       \
    { "maze cell", maze_next_state }
#define NoiseCell                                                                                                      \
    { "noise cell", noise_next_state }
#define FractalCell                                                                                                    \
    { "fractal cell", fractal_next_state }
#define FractalCornerCell                                                                                              \
    { "fractal corner cell", fractal_corner_next_state }
#define LesseConwayCell                                                                                                \
    { "lesse conway cell", lesse_conway_next_state }
#define TripleMooreConwayCell                                                                                          \
    { "triple moore conway cell", triple_moore_conway_next_state }
#define VonNeumannR2ConwayCell                                                                                         \
    { "von neumann r2 conway cell", von_neumann_r2_conway_next_state }
#define ConwayCancerCell                                                                                               \
    { "conway cancer cell", conway_cancer_next_state }

void populate_analytics_string(char **string, Environment const *env, CellType const *cell_type);
void next_generation(Environment *env, CellType const *cell_type);

#endif // CONWAY_RULES_H
