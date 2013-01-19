#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Note this file uses C99

typedef enum { player1, player2 } Players;

// These constants are adequate for Breakthrough, but may
// need adjustment for other games
#define MAX_LINE_LEN 255
#define MAX_TOKENS 10

static Players current_player;
static Players my_player;
static char name[MAX_LINE_LEN];
static char opp_name[MAX_LINE_LEN];
static int tokens_size = 0;
static char tokens[MAX_TOKENS][MAX_LINE_LEN];
static char message[MAX_LINE_LEN] = "";
static char next_move_cache[MAX_LINE_LEN] = "";

void next_move(void);
void play_game(void);
void print_and_recv_echo(const char *msg);
void read_msg(void);
void read_msg_and_tokenize(void);
void wait_for_start(void);

int main(int argc, char * argv[]) {
    // Determine our name from command line
    //Note that name cannot contain spaces or be "server" or "observer"
    strcpy(name, "DefaultName");
    if (argc >= 2)
        strcpy(name, argv[1]);

    play_game();

    return EXIT_SUCCESS;
}

void next_move(void) {
    // Somehow select your next move
    strcpy(next_move_cache, "MOVE a 2 TO b 2");
}

void play_game(void) {
    // Identify myself
    fprintf(stdout, "#name %s\n", name);
    fflush(stdout);

    // Wait for start of game
    wait_for_start();

    // Main game loop
    for (;;) {
        if (current_player == my_player) {
            // My turn
            // Check if game is over (optional)

            // Determine next move
            next_move();

            // Apply it to local state

            // Tell the world
            print_and_recv_echo(next_move_cache);

            // It is the opponents turn
            current_player = (current_player == player1) ? player2 : player1;
        } else {
            // Wait for move from other player
            // Get server's next instruction
            read_msg_and_tokenize();

            if (tokens_size == 6 && strcmp(tokens[0], "MOVE") == 0) {
                // Translate to local coordinates and update our local state

                // It is now my turn
                current_player = (current_player == player1) ? player2 : player1;
            } else if (tokens_size == 4 && strcmp(tokens[0], "FINAL") == 0 &&
                       strcmp(tokens[2], "BEATS") == 0) {
                // Game over
                if (strcmp(tokens[1], name) == 0 && strcmp(tokens[3], opp_name) == 0) {
                    fprintf(stderr, "I, %s, have won!\n", name);
                    fflush(stderr);
                } else if (strcmp(tokens[3], name) == 0&& strcmp(tokens[1], opp_name) == 0) {
                    fprintf(stderr, "I, %s, have lost.\n", name);
                    fflush(stderr);
                } else {
                    fprintf(stderr, "Did not find expected players in FINAL command.\n");
                    fprintf(stderr, "Found '%s' and '%s'.\n", tokens[1], tokens[3]);
                    fprintf(stderr, "Expected '%s' and '%s'.\n", name, opp_name);
                    fprintf(stderr, "Received message '%s'\n", message);
                    fflush(stderr);
                }
                break;
            } else {
                // Unknown command
                fprintf(stderr, "Unknown command of '%s' from the server\n", message);
                fflush(stderr);
            }
        }
    }
}

// Sends msg to stdout. Verifies it is echoed back. This is how the server
// validates moves.
void print_and_recv_echo(const char *msg) {
    fprintf(stdout, "%s\n", msg);
    fflush(stdout);

    read_msg();
    if (strcmp(msg, message) != 0) {
        fprintf(stderr, "Expected echo of '%s'. Received '%s'\n", msg, message);
        fflush(stderr);
    }
}

// Read a line, up to a newline, from the server
void read_msg(void) {
    // fgets is a blocking call, it also NUL-terminates
    fgets(message, 255, stdin);

    // Trim white space from end of string
    char *end = message + strlen(message) - 1;
    while(end > message && isspace(*end)) end--;
    *(end + 1) = '\0';
}

// Reads a line from the server and tokenizes the result based on a space
// delimiter
void read_msg_and_tokenize(void) {
    read_msg();

    // Copy message to tokens[0] to not destroy it with strtok
    char buffer[MAX_LINE_LEN];
    strcpy(buffer, message);

    // Tokenize based on space
    char *token = strtok(buffer, " ");
    tokens_size = 0;
    while (token != NULL && tokens_size < MAX_TOKENS) {
        strcpy(tokens[tokens_size], token);
        tokens_size++;
        token = strtok(NULL, " ");
    }
}

void wait_for_start(void) {
    for (;;) {
        // Read message
        read_msg_and_tokenize();

        if (tokens_size == 4 && strcmp(tokens[0], "BEGIN") == 0 &&
            strcmp(tokens[1], "BREAKTHROUGH") == 0) {
            // Found BEGIN GAME message, determine if we play first
            if (strcmp(tokens[2], name) == 0) {
                // We go first!
                strcpy(opp_name, tokens[3]);
                my_player = player1;
                break;
            } else if (strcmp(tokens[3], name) == 0) {
                // They go first
                strcpy(opp_name, tokens[2]);
                my_player = player2;
                break;
            } else {
                fprintf(stderr, "Did not find '%s', my name, in the BEGIN BREAKTHROUGH command.\n",
                        name);
                fprintf(stderr, "Found '%s' and '%s' as player names.\n", tokens[2], tokens[3]);
                fprintf(stderr, "Received message '%s'\n", message);
                fflush(stderr);
                fprintf(stdout, "#quit\n");
                fflush(stdout);
                exit(EXIT_FAILURE);
            }
        }
    }

    // Player 1 goes first
    current_player = player1;
}
