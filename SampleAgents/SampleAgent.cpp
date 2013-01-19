#include <algorithm>
using std::find_if;
#include <cstdlib>
#include <functional>
using std::not1;
using std::ptr_fun;
#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include <iterator>
using std::istream_iterator;
#include <locale>
using std::isspace;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
using std::getline;
#include <vector>
using std::vector;

enum Players { player1, player2 };

// You probably want to implement a Move class and remove this typedef
typedef string Move;

class Client {
 public:
  explicit Client(string name_) : name(name_) {}
  void play_game();

 private:
  Move next_move() const;
  void print_and_recv_echo(string msg) const;
  string read_msg() const;
  vector<string> read_msg_and_tokenize(string *response) const;
  void wait_for_start();

  // The GameState class is not provided for you. Feel to adjust the calls
  // below using gs to suit how your GameState is designed.
  // GameState* gs;
  Players current_player;
  Players my_player;
  string name;
  string opp_name;
};


int main(int argc, char* argv[]) {
  // Determine our name from command line
  // Note that name cannot contain spaces or be "server" or "observer"
  string name = "DefaultName";
  if (argc >= 2)
    name = argv[1];

  Client c(name);
  c.play_game();

  return EXIT_SUCCESS;
}

Move Client::next_move() const {
  // Somehow select your next move
  return "MOVE a 2 TO b 2";
}


void Client::play_game() {
  // Identify myself
  cout << "#name " << name << endl;

  // Wait for start of game
  wait_for_start();

  // Main game loop
  for (;;) {
    if (current_player == my_player) {
      // My turn

      // Check if game is over
      /*
      if (gs->game_over()) {
        cerr << "I, " << name << ", have lost" << endl;
        current_player = (current_player == player1) ? player2 : player1;
        continue;
      }
      */

      // Determine next move
      const Move m = next_move();

      // Apply it locally
      // gs->apply_move(m);

      // Tell the world
      print_and_recv_echo(m);

      // It is the opponents turn
      current_player = (current_player == player1) ? player2 : ::player1;
    } else {
      // Wait for move from other player
      // Get server's next instruction
      string server_msg;
      const vector<string> tokens = read_msg_and_tokenize(&server_msg);

      if (tokens.size() == 6 && tokens[0] == "MOVE") {
        // Translate to local coordinates and update our local state
        // const Move m = gs->translate_to_local(tokens);
        // gs->apply_move(m);

        // It is now my turn
        current_player = (current_player == player1) ? player2 : player1;
      } else if (tokens.size() == 4 && tokens[0] == "FINAL" &&
                 tokens[2] == "BEATS") {
        // Game over
        if (tokens[1] == name && tokens[3] == opp_name)
          cerr << "I, " << name << ", have won!" << endl;
        else if (tokens[3] == name && tokens[1] == opp_name)
          cerr << "I, " << name << ", have lost." << endl;
        else
          cerr << "Did not find expected players in FINAL command.\n"
               << "Found '"<< tokens[1] <<"' and '" << tokens[3] << "'. "
               << "Expected '" << name << "' and '" << opp_name <<"'.\n"
               << "Received message '" << server_msg << "'" << endl;
        break;
      } else {
        // Unknown command
        cerr << "Unknown command of '" << server_msg
             << "' from the server" << endl;
      }
    }
  }
}

/* Sends a msg to stdout and verifies that the next message to come in
   is it echoed back. This is how the server validates moves */
void Client::print_and_recv_echo(const string msg) const {
  cout << msg << endl;  // Note the endl flushes the stream, which is necessary
  const string echo_recv = read_msg();
  if (msg != echo_recv)
    cerr << "Expected echo of '" << msg << "'. Received '" << echo_recv << "'";
}

/* Reads a line, up to a newline from the server */
string Client::read_msg() const {
  string msg;
  getline(cin, msg);  // This is a blocking read

  // Trim white space from end of string
  msg.erase(find_if(msg.rbegin(), msg.rend(),
                    not1(ptr_fun<int, int>(isspace))).base(), msg.end());
  return msg;
}

/* Reads a line from the server and tokenizes the results based on a delimiter
   of a space. If the actual server message is requested via a non-null argument,
   it is recorded */
vector<string> Client::read_msg_and_tokenize(string *response) const {
  const string msg = read_msg();

  // Tokenize using whitespace as a delimiter
  stringstream ss(msg);
  istream_iterator<string> begin(ss);
  istream_iterator<string> end;
  vector<string> const tokens(begin, end);

  if (response != NULL)
    *response = msg;

  return tokens;
}

void Client::wait_for_start() {
  for (;;) {
    string response;
    const vector<string> tokens = read_msg_and_tokenize(&response);

    if (tokens.size() == 4 && tokens[0] == "BEGIN" &&
        tokens[1] == "BREAKTHROUGH") {
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
        cerr << "Did not find '" << name
             << "', my name, in the BEGIN BREAKTHROUGH command.\n"
             << "# Found '"<< tokens[2] <<"' and '" << tokens[3] << "'"
             << " as player names. Received message '" << response << "'";
        cout << "#quit";
        exit(EXIT_FAILURE);
      }
    }
  }

  // Player 1 goes first
  current_player = player1;
}

