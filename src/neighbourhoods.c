/**
* Contains logic pertaining to cell neighbourhoods.
 * @author Matteo Golin
 * @version 1.0
*/

#include "neighbourhoods.h"

/* CONSTANTS */
const Neighbourhood VON_NEUMANN = {4, {VonNeumann}};
const Neighbourhood VON_NEUMANN_CORNERS = {4, {VonNeumannCorners}};
const Neighbourhood MOORE = {8, {Moore}};
const Neighbourhood VON_NEUMANN_R2 = {12, {VonNeumannR2}};
const Neighbourhood TRIPLE_MOORE = {20, {TripleMoore}};
const Neighbourhood TRIPLE_MOORE_CORNER = {24, {TripleMooreCorner}};

/* FUNCTION HEADERS */

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