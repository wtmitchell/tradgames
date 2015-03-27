public class Move {
  public int from;
  public int to;

  public Move(int from, int to) {
    this.from = from;
    this.to = to;
  }

  public String toString() {
    return "MOVE FROM " + from + " TO " + to;
  }

  public boolean equals(Object other) {
    if ( this == other ) return true;
    if ( !(other instanceof Move) ) return false;
    Move m = (Move) other;

    return from == m.from && to == m.to;
  }
}
