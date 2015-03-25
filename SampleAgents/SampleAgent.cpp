#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

// You probably want to implement a Move class and remove this typedef
typedef std::string Move;

class Client {
public:
  explicit Client(std::string name_) : name(name_) {}
  void play_game();

private:
  Move next_move() const;
  void print_and_recv_echo(const std::string &msg) const;
  std::string read_msg() const;
  std::vector<std::string> tokenize_msg(const std::string &msg) const;
  void wait_for_start();
  void switch_current_player();

  // The GameState class is not provided for you. Feel to adjust the calls
  // below using gs to suit how your GameState is designed.
  // GameState gs;

  enum Players { player1, player2 };
  Players current_player;
  Players my_player;
  std::string name;
  std::string opp_name;
};

int main(int argc, char *argv[]) {
  // Determine our name from command line
  // Note that name cannot contain spaces or be "moderator" or "observer"
  std::string name = "DefaultName";
  if (argc >= 2)
    name = argv[1];

  Client c(name);
  c.play_game();

  return EXIT_SUCCESS;
}

Move Client::next_move() const {
  // Somehow select your next move
  return "MOVE 0 TO 2";
}

void Client::play_game() {
  // Identify myself
  std::cout << "#name " << name << std::endl;

  // Wait for start of game
  wait_for_start();

  // Main game loop
  for (;;) {
    if (current_player == my_player) {
      // My turn

      // Check if game is over
      /*
      if (gs.game_over()) {
        std::cerr << "I, " << name << ", have lost" << std::endl;
        switch_current_player();
        continue;
      }
      */

      // Determine next move
      const Move m = next_move();

      // Apply it locally
      // gs.apply_move(m);

      // Tell the world
      print_and_recv_echo(m);

      // It is the opponents turn
      switch_current_player();
    } else {
      // Wait for move from other player
      // Get server's next instruction
      std::string server_msg = read_msg();
      const std::vector<std::string> tokens = tokenize_msg(server_msg);

      if (tokens.size() == 5 && tokens[0] == "MOVE") {
        // Translate to local coordinates and update our local state
        // const Move m = gs.translate_to_local(tokens);
        // gs.apply_move(m);

        // It is now my turn
        switch_current_player();
      } else if (tokens.size() == 4 && tokens[0] == "FINAL" &&
                 tokens[2] == "BEATS") {
        // Game over
        if (tokens[1] == name && tokens[3] == opp_name)
          std::cerr << "I, " << name << ", have won!" << std::endl;
        else if (tokens[3] == name && tokens[1] == opp_name)
          std::cerr << "I, " << name << ", have lost." << std::endl;
        else
          std::cerr << "Did not find expected players in FINAL command.\n"
                    << "Found '" << tokens[1] << "' and '" << tokens[3] << "'. "
                    << "Expected '" << name << "' and '" << opp_name << "'.\n"
                    << "Received message '" << server_msg << "'" << std::endl;
        break;
      } else {
        // Unknown command
        std::cerr << "Unknown command of '" << server_msg << "' from the server"
                  << std::endl;
      }
    }
  }
}

// Sends a msg to stdout and verifies that the next message to come in is it
// echoed back. This is how the server validates moves
void Client::print_and_recv_echo(const std::string &msg) const {
  // Note the endl flushes the stream, which is necessary
  std::cout << msg << std::endl;
  const std::string echo_recv = read_msg();
  if (msg != echo_recv)
    std::cerr << "Expected echo of '" << msg << "'. Received '" << echo_recv
              << "'" << std::endl;
}

/* Reads a line, up to a newline from the server */
std::string Client::read_msg() const {
  std::string msg;
  std::getline(std::cin, msg); // This is a blocking read

  // Trim white space from beginning of string
  const char *WhiteSpace = " \t\n\r\f\v";
  msg.erase(0, msg.find_first_not_of(WhiteSpace));
  // Trim white space from end of string
  msg.erase(msg.find_last_not_of(WhiteSpace) + 1);

  return msg;
}

// Tokenizes a message based upon whitespace
std::vector<std::string> Client::tokenize_msg(const std::string &msg) const {
  // Tokenize using whitespace as a delimiter
  std::stringstream ss(msg);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> tokens(begin, end);

  return tokens;
}

void Client::wait_for_start() {
  for (;;) {
    std::string response = read_msg();
    const std::vector<std::string> tokens = tokenize_msg(response);

    if (tokens.size() == 4 && tokens[0] == "BEGIN" &&
        tokens[1] == "CHINESECHECKERS") {
      // Found BEGIN GAME message, determine if we play first
      if (tokens[2] == name) {
        // We go first!
        opp_name = tokens[3];
        my_player = player1;
        break;
      } else if (tokens[3] == name) {
        // They go first
        opp_name = tokens[2];
        my_player = player2;
        break;
      } else {
        std::cerr << "Did not find '" << name
                  << "', my name, in the BEGIN BREAKTHROUGH command.\n"
                  << "# Found '" << tokens[2] << "' and '" << tokens[3] << "'"
                  << " as player names. Received message '" << response << "'"
                  << std::endl;
        std::cout << "#quit" << std::endl;
        std::exit(EXIT_FAILURE);
      }
    } else if (response == "DUMPSTATE") {
      // std::cout << gs.dumpState() << std::endl;
    } else if (tokens[0] == "LOADSTATE") {
      std::string new_state = response.substr(10);
      /*
      if (!gs.loadState(new_state))
        std::cerr << "Failed to load '" << new_state << "'\n";
      */
    } else if (response == "LISTMOVES") {
      /*
      std::vector<Move> moves;
      gs.getMoves(moves);
      for (const auto i : moves)
        std::cout << i.from << ", " << i.to << "; ";
      std::cout << std::endl;
      */
    } else if (tokens[0] == "MOVE") {
      // Just apply the move
      // const Move m = gs.translate_to_local(tokens);
      // gs.apply_move(m);
    } else {
      std::cerr << "Unexpected message " << response << "\n";
    }
  }

  // Game is about to begin, restore to start state in case DUMPSTATE/LOADSTATE/LISTMOVES
  // were used
  //gs.reset();

  // Player 1 goes first
  current_player = player1;
}

void Client::switch_current_player() {
  current_player = (current_player == player1) ? player2 : player1;
}
