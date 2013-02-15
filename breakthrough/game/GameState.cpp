#include <iostream>
using std::cerr;
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
            case board_open:
                os << ".";
                break;
            case board_closed:
                os << "X";
                break;
            case board_player1:
                os << "1";
                break;
            case board_player2:
                os << "2";
                break;
        }
    }

    int p1_count = 0;
    int p2_count = 0;
    for (auto i : s.pieces)
        if (i.player == player1)
            ++p1_count;
        else
            ++p2_count;

    os << "There are " << s.pieces.size() << " pieces still in play. "
       << "Of them, " << p1_count << " are Player 1's "
       << "and " << p2_count << " Player 2's.\n";

    vector<Move> p1moves = s.get_moves(player1);
    os << "Player 1 has " << p1moves.size() << " moves: ";
    for (auto m : p1moves)
        os << s.pretty_print_move(m) << ", ";
    os << "\n";

    vector<Move> p2moves = s.get_moves(player2);
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
    {
        if (p.player == player && player == player1)
        {
            // Player 1
            size_t down_left = p.location + board_size + 1;
            if (board[down_left] == board_open ||
                board[down_left] == board_player2)
                moves.push_back(Move(p.location, down_left));

            size_t down = p.location + board_size + 2;
            if (board[down] == board_open)
                moves.push_back(Move(p.location, down));

            size_t down_right = p.location + board_size + 3;
            if (board[down_right] == board_open ||
                board[down_right] == board_player2)
                moves.push_back(Move(p.location, down_right));
        }
        else if (p.player == player && player == player2)
        {
            // Player 2
            size_t up_left = p.location - board_size - 3;
            if (board[up_left] == board_open ||
                board[up_left] == board_player1)
                moves.push_back(Move(p.location, up_left));

            size_t up = p.location - board_size - 2;
            if (board[up] == board_open)
                moves.push_back(Move(p.location, up));

            size_t up_right = p.location - board_size -1;
            if (board[up_right] == board_open ||
                board[up_right] == board_player1)
                moves.push_back(Move(p.location, up_right));
        }
    }

    return moves;
}

GameState& GameState::apply_move(const Move m)
{
    // Store the move on the history stack
    move_history.push_back(m);

    // Only move if an actual move
    if (!m.isNull())
    {
        // Find the pieces involved in the move
        size_t mover = pieces.size();
        size_t captured = pieces.size();
        for (size_t i = 0; i < pieces.size(); ++i)
        {
            if (pieces[i].location == m.from)
                mover = i;
            else if (pieces[i].location == m.to)
                captured = i;
        }

        // Move the piece
        board[m.from] = board_open;
        if (pieces[mover].player == player1)
            board[m.to] = board_player1;
        else
            board[m.to] = board_player2;
        pieces[mover].location = m.to;

        // Remove the captured piece if applicable
        if (captured != pieces.size())
            pieces.erase(pieces.begin() + captured);
    }

    // Change whose turn it is
    turn = (turn == player1) ? player2 : player1;

    return *this;
}

GameState& GameState::undo_move()
{
    // Not needed for server/random player so not implemented
    return *this;
}

bool GameState::game_over() const
{
    if (move_history.size() > 1 && move_history.back().isNull())
        return true;

    int p1_count = 0;
    int p2_count = 0;

    for (auto p : pieces)
    {
        // Check if piece is in last row
        if ((p.player == player1 && p.location > (board_size + 2) * board_size)
            || (p.player == player2 && p.location < 2 * (board_size + 2)))
            return true;
        if (p.player == player1) ++p1_count;
        if (p.player == player2) ++p2_count;
    }

    // Game is over if one player has no pieces
    if (p1_count == 0 || p2_count == 0)
        return true;

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
    return "MOVE " + pretty_print_location(m.from) + " TO " + pretty_print_location(m.to);
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
                board.push_back(board_closed);
            else
                board.push_back(board_open);


    pieces.clear();
    pieces.reserve(4 * board_size);
    // Player 1 pieces
    for (size_t i = board_size + 3; i < 3 * (board_size + 2); ++i)
        if (i % (board_size + 2) !=  0 && i % (board_size + 2) != board_size + 1)
        {
            pieces.push_back(Piece(i, player1));
            board[i] = board_player1;
        }

    // Player 2 pieces
    for (size_t i = (board_size + 2) * (board_size - 1);
         i < (board_size + 2) * (board_size + 1); ++i)
        if (i % (board_size + 2) !=  0 && i % (board_size + 2) != board_size + 1)
        {
            pieces.push_back(Piece(i, player2));
            board[i] = board_player2;
        }

    // Everything else
    turn = player1;
}

Move GameState::translate_to_local(const vector<string> message) const
{
    size_t from = 0;
    size_t to = 0;

    if (message[0] == "MOVE")
    {
        // Message source is a client
        from = message[1].at(0) - 'a' + 1 + stoi(message[2]) * (board_size + 2);
        to = message[4].at(0) - 'a' + 1 + stoi(message[5]) * (board_size + 2);
    }
    else
    {
        // Message source is the server
        from = message[2].at(0) - 'a' + 1 + stoi(message[3]) * (board_size + 2);
        to = message[5].at(0) - 'a' + 1 + stoi(message[6]) * (board_size + 2);
    }

    return Move(from, to);
}

bool GameState::valid_move(const Move m) const
{
    return valid_move(m, turn);
}

bool GameState::valid_move(const Move m, const Players player) const
{
    // Can't make a move if game is over
    if (game_over())
        return false;

    // Validate the locations are in plausible ranges
    // Only need to check upper bounds because location type is unsigned
    if (m.from > (board_size + 2) * (board_size + 2)
        || m.to > (board_size + 2) * (board_size + 2))
    {
        cerr << "Move " << m.from << " to " << m.to << " is outside plausible board range" << endl;
        return false;
    }

    // Find the pieces involved in the move
    size_t mover = pieces.size();
    for (size_t i = 0; i < pieces.size(); ++i)
    {
        if (pieces[i].location == m.from)
        {
            mover = i;
            break;
        }
    }

    // There needs to be a piece at the from location
    if (mover == pieces.size())
    {
        cerr << "Move has no piece at from location" << endl;
        return false;
    }

    // The moved piece needs to be owned by the current player
    if (pieces[mover].player != player)
    {
        cerr << "Move trying to move opponents piece" << endl;
        return false;
    }

    // The to location either needs to be open, or occupied by an opposing piece
    if (board[m.to] == board_closed)
    {
        cerr << "Move results in piece being off the board" << endl;
        return false;
    }
    if ((board[m.from] == board_player1 && board[m.to] == board_player1)
        || (board[m.from] == board_player2 && board[m.to] == board_player2))
    {
        cerr << "Move tries to capture own piece" << endl;
        return false;
    }

    // The move needs to be a valid for the current player
    size_t diff = 0;
    if (player == player1)
        diff = m.to - m.from;
    else
        diff = m.from - m.to;

    // If up or down, can't capture
    if (diff == board_size + 2 && board[m.to] != board_open)
    {
        cerr << "Move tries to capture, but it is not a diagonal move" << endl;
        return false;
    }

    if (board_size + 1 <= diff && diff <= board_size +3)
        return true;

    // Not a valid move
    return false;
}
