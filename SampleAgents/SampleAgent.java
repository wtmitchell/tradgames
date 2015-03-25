import java.util.Scanner;

public class SampleAgent {
  public static void main(String[] args) {
    String name = "DefaultName";
    if (args.length > 2)
      name = args[1];
    SampleAgent sa = new SampleAgent(name);
    sa.playGame();
  }

  public enum Players { player1, player2 }

  public SampleAgent(String myName) {
    this.myName = myName;
    stdin = new Scanner(System.in);
  }

  // You may wish to alter this method to return a Move class (which you create)
  // instead of String.
  private String nextMove() {
    // Determine the next move somehow
    return "MOVE a 2 TO b 3";
  }

  public void playGame() {
    // Identify myself
    System.out.println("#name " + myName);
    System.out.flush();

    // Wait for start of game
    waitForStart();

    // Main game loop
    while (true) {
      if (currentPlayer == myPlayer) {
        // My turn

        // Check if game is over

        // Determine next move
        String m = nextMove();

        // Apply it to local state
        // gs.applyMove(m);

        // Tell the world
        printAndReceiveEcho(m);

        // It is the opponents turn
        currentPlayer = (currentPlayer == Players.player1) ? Players.player2
                                                           : Players.player1;
      } else {
        // Wait for move from other player
        // Get server's next instruction
        String serverMsg = readMessage();
        String[] tokens = tokenize(serverMsg);

        if (tokens.length == 6 && tokens[0].equals("MOVE")) {
          // Translate to local coordinates and update our local state
          // Move m = gs.translateToLocal(tokens);
          // gs.applyMove(m);

          // It is now my turn
          currentPlayer = (currentPlayer == Players.player1) ? Players.player2
                                                             : Players.player1;
        } else if (tokens.length == 4 && tokens[0].equals("FINAL") &&
                   tokens[2].equals("BEATS")) {
          // Game over
          if (tokens[1].equals(myName) && tokens[3].equals(opponentName)) {
            System.err.println("I, " + myName + ", have won!");
            System.err.flush();
          } else if (tokens[3].equals(myName) &&
                     tokens[1].equals(opponentName)) {
            System.err.println("I, " + myName + ", have lost.");
            System.err.flush();
          } else {
            System.err.println(
                "Did not find expected players in FINAL command.");
            System.err.println(
                "Found '" + tokens[1] + "' and '" + tokens[3] + "'. "
                + "Expected '" + myName + "' and '" + opponentName + "'.");
            System.err.println("Received message '" + serverMsg + "'");
            System.err.flush();
          }
          break;
        } else {
          // Unknown command
          System.err.println("Unknown command of '" + serverMsg +
                             "' from the server");
          System.err.flush();
        }
      }
    }
  }

  private void printAndReceiveEcho(String msg) {
    System.out.println(msg);
    System.out.flush();
    String echo = readMessage();
    if (!echo.equals(msg)) {
      System.err.println("Expected echo of '" + msg + "'. Received '" + echo +
                         "'");
      System.err.flush();
    }
  }

  private String readMessage() { return stdin.nextLine().trim(); }

  private String[] tokenize(String s) { return s.split(" "); }

  private void waitForStart() {
    while (true) {
      String response = readMessage();
      String[] tokens = tokenize(response);

      if (tokens.length == 4 && tokens[0].equals("BEGIN") &&
          tokens[1].equals("CHINESECHECKERS")) {
        // Found BEGIN GAME message, determine if we play first
        if (tokens[2].equals(myName)) {
          // We go first!
          opponentName = tokens[3];
          myPlayer = Players.player1;
          break;
        } else if (tokens[3].equals(myName)) {
          // They go first
          opponentName = tokens[2];
          myPlayer = Players.player2;
          break;
        } else {
          System.err.println(
              "Did not find '" + myName + "', my name, in the BEGIN command.\n"
              + "# Found '" + tokens[2] + "' and '" + tokens[3] + "'"
              + " as player names. Received message '" + response + "'");
          System.err.flush();
          System.out.println("#quit");
          System.out.flush();
        }
      } else if (response.equals("DUMPSTATE")) {
        // System.out.println(gs.dumpState());
        // System.out.flush();
      } else if (tokens[0].equals("LOADSTATE")) {
        // String newState = response.substring(10);
        // System.out.println(gs.loadState(newState));
        // System.out.flush();
      } else if (tokens[0].equals("MOVE")) {
        // Move m = gs.translateToLocal(tokens);
        // gs.applyMove(m);
      } else {
        System.err.println("Unexpected message '" + response + "'");
        System.err.flush();
      }
    }

    // Game is about to begin, restore to start state in case DUMPSTATE/LOADSTATE/LISTMOVES
    // were used
    //gs.reset();

    // Player 1 goes first
    currentPlayer = Players.player1;
  }

  private Scanner stdin;
  private Players currentPlayer;
  private Players myPlayer;
  private String myName;
  private String opponentName;

  // You will want to implement this class yourself
  /*
  private GameState gs;
  */
}
