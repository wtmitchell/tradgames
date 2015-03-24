#include "Move.h"

bool operator==(const Move &a, const Move &b)
{
    return a.from == b.from && a.to == b.to;
}
