#ifndef MOVE_H
#define MOVE_H

#include <cstdlib>

class Move
{
 public:
    Move(size_t from_, size_t to_) : from(from_), to(to_) {}
    bool isNull() const { return to == 0 && from == 0; }
    size_t from;
    size_t to;
};

bool operator==(const Move &a, const Move &b);

#endif
