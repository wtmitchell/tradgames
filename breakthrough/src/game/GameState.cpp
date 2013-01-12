#include <iostream>
using std::cout;
using std::endl;
#include <ostream>
using std::ostream;
#include <sstream>
using std::ostringstream;

#include "GameState.h"

ostream& operator<<(ostream& os, GameState& s)
{
    os << "--------------------------------------------------------------\n";

    // Add column labels
    os << "   ";
    for (char i = 'a'; i - 'a' < (char) s.board_size; ++i)
        os << i;
    os << "\n";

    // Don't start i at 0 to skip first row since it is out of bounds
    for (size_t i = s.board_size + 2; i < s.board.size() - (s.board_size + 2); ++i)
    {
        // Add row labels
        if (i % (s.board_size + 2) == 0)
        {
            os << " " << i / (s.board_size + 2) << " ";
            // Also they correspond to out of play positions
            continue;
        }

        // Skip if edge of border
        if (i % (s.board_size + 2) == s.board_size + 1)
        {
            os << "\n";
            continue;
        }

        switch(s.board[i])
        {
            case Board::open:
                os << ".";
                break;
            case Board::closed:
                os << "X";
                break;
            case Board::player1:
                os << "1";
                break;
            case Board::player2:
                os << "2";
                break;
        }
    }

    int p1_count = 0;
    int p2_count = 0;
    for (auto i : s.pieces)
        if (i.player == Players::player1)
            ++p1_count;
        else
            ++p2_count;

    os << "There are " << s.pieces.size() << " pieces still in play. "
       << "Of them, " << p1_count << " are Player 1's "
       << "and " << p2_count << " Player 2's.\n";

    vector<Move> p1moves = s.get_moves(Players::player1);
    os << "Player 1 has " << p1moves.size() << " moves: ";
    for (auto m : p1moves)
        os << s.pretty_print_move(m) << ", ";
    os << "\n";

    vector<Move> p2moves = s.get_moves(Players::player2);
    os << "Player 2 has " << p2moves.size() << " moves: ";
    for (auto m : p2moves)
        os << s.pretty_print_move(m) << ", ";
    os << "\n";

    os << "--------------------------------------------------------------"
       << endl;

    return os;
}

GameState::GameState(size_t board_size_) : board_size(board_size_)
{
    reset();
}

vector<Move> GameState::get_moves() const
{
    return get_moves(get_whose_turn());
}


vector<Move> GameState::get_moves(const Players player) const
{
    vector<Move> moves;

    for(auto p : pieces)
        if (p.player == player && player == Players::player1)
        {
            // Player 1
            size_t down_left = p.location + board_size + 1;
            if (board[down_left] == Board::open ||
                board[down_left] == Board::player2)
                moves.push_back(Move(p.location, down_left));

            size_t down = p.location + board_size + 2;
            if (board[down] == Board::open ||
                board[down] == Board::player2)
                moves.push_back(Move(p.location, down));

            size_t down_right = p.location + board_size + 3;
            if (board[down_right] == Board::open ||
                board[down_right] == Board::player2)
                moves.push_back(Move(p.location, down_right));
        }
        else if (p.player == player && player == Players::player2)
        {
            // Player 2
            size_t up_left = p.location - board_size - 3;
            if (board[up_left] == Board::open ||
                board[up_left] == Board::player1)
                moves.push_back(Move(p.location, up_left));

            size_t up = p.location - board_size - 2;
            if (board[up] == Board::open ||
                board[up] == Board::player1)
                moves.push_back(Move(p.location, up));

            size_t up_right = p.location - board_size -1;
            if (board[up_right] == Board::open ||
                board[up_right] == Board::player1)
                moves.push_back(Move(p.location, up_right));
        }

    return moves;
}

bool GameState::game_over() const
{
    return false;
}

Players GameState::get_whose_turn() const
{
    return turn;
}

string GameState::pretty_print_location(const size_t location) const
{
    char column = static_cast<char>(location % (board_size + 2)) - 1 + 'a';
    size_t row = location / (board_size + 2);
    ostringstream oss;
    oss << column << " " << row;
    return oss.str();
}

string GameState::pretty_print_move(const Move m) const
{
    return "move " + pretty_print_location(m.from) + " to " + pretty_print_location(m.to);
}


void GameState::reset()
{
    // Sets the board up for a new game
    board.clear();
    board.reserve((board_size + 2) * (board_size + 2));

    // Make the board open except for the borders
    for (size_t i = 0; i < board_size + 2; ++i)
        for (size_t j = 0; j < board_size + 2; ++j)
            if (i == 0 || i == (board_size + 1) || j == 0 || j == (board_size + 1))
                board.push_back(Board::closed);
            else
                board.push_back(Board::open);


    pieces.clear();
    pieces.reserve(4 * board_size);
    // Player 1 pieces
    for (size_t i = board_size + 3; i < 3 * (board_size + 2); ++i)
        if (i % (board_size + 2) !=  0 && i % (board_size + 2) != board_size + 1)
        {
            pieces.push_back(Piece(i, Players::player1));
            board[i] = Board::player1;
        }

    // Player 2 pieces
    for (size_t i = (board_size + 2) * (board_size - 1);
         i < (board_size + 2) * (board_size + 1); ++i)
        if (i % (board_size + 2) !=  0 && i % (board_size + 2) != board_size + 1)
        {
            pieces.push_back(Piece(i, Players::player2));
            board[i] = Board::player2;
        }

    // Everything else
    turn = Players::player1;
}

bool GameState::valid_move(const Move m) const
{
    return false;
}
