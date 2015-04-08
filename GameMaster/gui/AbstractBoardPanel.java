import java.awt.LayoutManager;
import javax.swing.JPanel;

public abstract class AbstractBoardPanel extends JPanel {
  public AbstractBoardPanel(LayoutManager layout) {
    super(layout);
  }
  public abstract void updateData(int turn, String state, String nextMoves);
  public abstract void updateData(int turn, String state, String nextMoves, String eval);
  public abstract void setP1Hook(UpdateMsgHook hook);
  public abstract void setP2Hook(UpdateMsgHook hook);
}
