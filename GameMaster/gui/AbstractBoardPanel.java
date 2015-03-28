import javax.swing.JPanel;

public abstract class AbstractBoardPanel extends JPanel {
  public abstract void updateData(int turn, String state, String nextMoves);
}
