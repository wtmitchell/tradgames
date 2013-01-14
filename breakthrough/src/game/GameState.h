#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <ostream>
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "Move.h"
#include "Piece.h"
#include "Players.h"

enum class Board {closed, open, player1, player2};

inline size_t row_major(size_t x, size_t y, size_t row_size)
{
    return x * row_size + y;
}


class GameState
{
 public:
    friend ostream& operator<<(ostream& os, GameState& s);

    explicit GameState(size_t board_size = 8); // In theory 4-26 supported, but only 8 has been tested

    vector<Move> get_moves() const; // for current player's move
    vector<Move> get_moves(const Players player) const;
    GameState& apply_move(const Move m); // return self-reference
    GameState& undo_move();
    /*
    int get_winner() const;
    */

    bool game_over() const;
    Players get_whose_turn() const;
    string pretty_print_location(const size_t location) const;
    string pretty_print_move(const Move m) const;
    void reset();
    bool valid_move(const Move m) const;

 private:
    const size_t board_size;
    vector<Board> board;
    vector<Piece> pieces;
    Players turn;
};

#endif
