import java.util.ArrayList;


public class ChineseCheckersState {
  // Initialize with the starting state for a 2 player game
  public ChineseCheckersState() {
    reset();
  }

  // Put all valid moves into the vector of moves passed in by reference
  public void getMoves(ArrayList<Move> moves) {
    // WARNING: This function must not return duplicate moves
    moves.clear();

    for (int i = 0; i < 81; ++i) {
      if (board[i] == currentPlayer) {
        getMovesSingleStep(moves, i);
        // Need to add jump moves
      }
    }
  }

  // Apply the move m, returning true if m is a valid move, false if not
  public boolean applyMove(Move m) {
    // Ensure the from and to are reasonable
    if (m.from > 80 || m.to > 80 || m.from == m.to)
      return false;

    // Check the move
    // FIXME: This should be uncommented once you have getMoves working!!
    if (!isValidMove(m))
      return false;

    // Apply the move
    int temp = board[m.from];
    board[m.from] = board[m.to];
    board[m.to] = temp;

    // Update whose turn it is
    swapTurn();

    return true;
  }

  // Undo the move m, returning true if m is a move that can be undone, false if not
  public boolean undoMove(Move m) {
    // Ensure the from and to are reasonable
    if (m.from > 80 || m.to > 80 || m.from == m.to)
      return false;

    // Undo the move
    int temp = board[m.from];
    board[m.from] = board[m.to];
    board[m.to] = temp;
    swapTurn();

    // Check the move is valid from this state that is back one step
    if (!isValidMove(m)) {
      // Woops, it was not valid, undo our changes
      swapTurn();
      int temp2 = board[m.from];
      board[m.from] = board[m.to];
      board[m.to] = temp2;

      return false;
    }

    return true;
  }

  // Returns true iff the move m is valid
  public boolean isValidMove(Move m) {
    // Ensure from and to make sense
    if (board[m.from] != currentPlayer || board[m.to] != 0)
      return false;

    // NOTE: Checking validity in this way is inefficient

    // Get current available moves
    ArrayList<Move> moves = new ArrayList<>();
    getMoves(moves);

    // Find the move among the set of available moves
    boolean found = moves.contains(m);

    return found;
  }

  // Returns true iff the game is over
  public boolean gameOver() {
    return player1Wins() || player2Wins();
  }

  // Return the player who won, assuming the game is over
  public int winner() {
    if (player1Wins())
      return 1;
    if (player2Wins())
      return 2;
    return -1; // No one has won
  }

  // Reset the board to the initial state
  public void reset() {
  board = new int[]{1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
            0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2};
  currentPlayer = 1;
  }

  // Loads the state stored in the string, returning true if it is a valid state, false if not
  public boolean loadState(String newState) {
    // Tokenize newState using whitespace as delimiter
    String[] tokenized = newState.split(" ");

    // Ensure the length
    if (tokenized.length != 82)
      return false;

    // Validate first item, whose turn it is
    if (!tokenized[0].equals("1") && !tokenized[0].equals("2"))
      return false;

    try {
      currentPlayer = Integer.parseInt(tokenized[0]);
    } catch (NumberFormatException e) {
      return false;
    }

    // Ensure rest of tokens are valid
    for (int i = 1, e = tokenized.length; i != e; ++i) {
      try {
        int val = Integer.parseInt(tokenized[i]);
        if (0 <= val && val <= 2)
          board[i - 1] = val;
        else
          return false;
      } catch (NumberFormatException ex) {
        return false;
      }
    }
    return true;
  }

  // Dump out the current state, usable with loadState
  public String dumpState() {
    StringBuilder out = new StringBuilder();
    out.append(currentPlayer);
    for (int i = 0; i < board.length; ++i)
      out.append(" " + board[i]);

    return out.toString();
  }

  // Translates a sequence of tokens from the move format used to the local move type
  public Move translateToLocal(String[] tokens) {
    // The numbers in the MOVE command sent by the moderator is already in the
    // format we need
    try {
      Move m = new Move(0, 0);
      m.from = Integer.parseInt(tokens[2]);
      m.to = Integer.parseInt(tokens[4]);
      return m;
    } catch (NumberFormatException e) {
      return new Move(0, 0);
    }
  }

  private int[]  board;
  private int currentPlayer = 0;

  private void getMovesSingleStep(ArrayList<Move> moves, int from) {
  int row = from / 9;
  int col = from % 9;

  // Up Left
  if (col > 0 && board[from - 1] == 0)
    moves.add(new Move(from, from - 1));

  // Up Right
  if (row > 0 && board[from - 9] == 0)
    moves.add(new Move(from, from - 9));

  // Left
  if (col > 0 && row < 8 && board[from + 8] == 0)
    moves.add(new Move(from, from + 8));

  // Right
  if (col < 8 && row > 0 && board[from - 8] == 0)
    moves.add(new Move(from, from - 8));

  // Down Left
  if (row < 8 && board[from + 9] == 0)
    moves.add(new Move(from, from + 9));

  // Down Right
  if (col < 8 && board[from + 1] == 0)
    moves.add(new Move(from, from + 1));
  }

  private void swapTurn() {
    currentPlayer = currentPlayer == 1 ? 2 : 1;
  }

  private boolean player1Wins() {
  // Wins by having all the bottom triangle filled and at least one is from the
  // first player

  boolean p1inTriangle = false;
  int target[] = new int[]{53, 61, 62, 69, 70, 71, 77, 78, 79, 80};
  for (int i : target) {
    if (board[i] == 0)
      return false;
    if (board[i] == 1)
      p1inTriangle = true;
  }

  return p1inTriangle;
  }
  private boolean player2Wins() {
  // Wins by having all of top triangle filled and at least one is from the
  // second player

  boolean p2inTriangle = false;
  int target[] = new int[]{0, 1, 2, 3, 9, 10, 11, 18, 19, 27};
  for (int i : target) {
    if (board[i] == 0)
      return false;
    if (board[i] == 2)
      p2inTriangle = true;
  }

  return p2inTriangle;
  }
}

