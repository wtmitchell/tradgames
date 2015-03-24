#ifndef PIECE_H
#define PIECE_H

#include "Players.h"

class Piece
{
public:
    Piece(size_t location_, Players player_) : location(location_), player(player_) {}
    size_t location;
    Players player;
};

#endif
