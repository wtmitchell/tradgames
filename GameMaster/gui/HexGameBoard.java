import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import javax.swing.JPanel;
import javax.swing.UIManager;

public class HexGameBoard extends AbstractBoardPanel implements MouseListener, MouseMotionListener {
  HexGameBoard() {
    setPreferredSize(
        new Dimension((int)(9 * FLAT_TO_FLAT + 2 * X_OFF),
                      (int)(52 * HALF_HEX_SIDE + 2 * Y_OFF))); // w, h
    addMouseListener(this);

    createEmptyBoard();
  }

  @Override
  public void paintComponent(Graphics g) {
    super.paintComponent(g);

    Graphics2D g2d = (Graphics2D) g;
    g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

    // Clear existing
    g2d.setColor(UIManager.getColor("Panel.background"));
    g2d.fillRect(0, 0, getWidth(), getHeight());

    // Draw information
    g2d.setColor(UIManager.getColor("Panel.foreground"));
    g2d.drawString("Turn: " + turn, 0, 20);


    for (Spot i : state) {
      switch(i.state) {
      case EMPTY:
        g2d.setPaint(Color.WHITE);
        g2d.fill(i);
        g2d.setStroke(new BasicStroke(3.0f));
        g2d.setPaint(Color.BLACK);
        g2d.drawOval(i.xx, i.yy, DIAMETER, DIAMETER);
        break;
      case PLAYER1:
        g2d.setPaint(Color.RED);
        g2d.fill(i);
        g2d.setStroke(new BasicStroke(3.0f));
        g2d.setPaint(Color.BLACK);
        g2d.drawOval(i.xx, i.yy, DIAMETER, DIAMETER);
        break;
      case PLAYER2:
        g2d.setPaint(Color.BLUE);
        g2d.fill(i);
        g2d.setStroke(new BasicStroke(3.0f));
        g2d.setPaint(Color.BLACK);
        g2d.drawOval(i.xx, i.yy, DIAMETER, DIAMETER);
        break;
      }
    }

    g2d.dispose();
  }

  @Override
  public void updateData(int turn, String boardState, String nextMoves) {
    this.turn = turn;

    // Split apart state
    String[] stateTokens = boardState.split(" ");
    try {
      // Start at 1 since first digit is player's turn
      for (int i = 1; i < stateTokens.length; ++i) {
        switch(Integer.parseInt(stateTokens[i])) {
        case 0:
          state.get(i - 1).state = SpotState.EMPTY;
          break;
        case 1:
          state.get(i - 1).state = SpotState.PLAYER1;
          break;
        case 2:
          state.get(i - 1).state = SpotState.PLAYER2;
          break;
        }
      }
    } catch (NumberFormatException e) {
      System.err.println("Invalid state string passed to HexGameBoard: " + boardState);
      e.printStackTrace();
    }

    // Split apart moves
    String [] moveTokens = nextMoves.split(";");
    moves.clear();
    try {
      for (int i = 0; i < moveTokens.length; ++i) {
        String[] tok = moveTokens[i].split(",");
        moves.add(new Move(Integer.parseInt(tok[0].trim()), Integer.parseInt(tok[1].trim())));
      }
    } catch (NumberFormatException e) {
      System.err.println("Invalid move string passed to HexGameBoard: " + nextMoves);
      e.printStackTrace();
    }

    repaint();
  }

  private void createEmptyBoard() {
    state.clear();

    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
      int row = i / BOARD_DIM;
      int col = i % BOARD_DIM;
      int dispRow = row + col;
      int dispCol = col - row + BOARD_DIM - 1;

      Spot s = new Spot(i, X_OFF + dispCol * COL_WIDTH,
                        Y_OFF + (int)(dispRow * ROW_HEIGHT), SpotState.EMPTY);
      state.add(s);
    }
  }

  @Override
  public void mouseReleased(MouseEvent e) {
  }

  @Override
  public void mouseClicked(MouseEvent e) {
    int x = e.getX() - X_OFF;
    int y = e.getY() - Y_OFF;
    int dispCol = x / COL_WIDTH;
    int dispRow = (int)(y / ROW_HEIGHT);
    int col = (dispCol + dispRow - BOARD_DIM + 1) / 2;
    int row = dispRow - col;

    // Outside of board
    if (row < 0 || row >= BOARD_DIM || col < 0 || col >= BOARD_DIM)
      return;

    int idx = row * BOARD_DIM + col;

    // Check actual click
    if (!state.get(idx).contains(e.getPoint()))
      return;

    System.out.println("Clicked on " + idx);
  }

  @Override
  public void mouseEntered(MouseEvent e) {
  }

  @Override
  public void mouseExited(MouseEvent e) {
  }

  @Override
  public void mousePressed(MouseEvent e) {
  }

  @Override
  public void mouseDragged(MouseEvent e) {
  }

  @Override
  public void mouseMoved(MouseEvent e) {
  }

  private class Move {
    public int from;
    public int to;

    public Move(int from, int to) {
      this.from = from;
      this.to = to;
    }
  }

  private enum SpotState {
    EMPTY, PLAYER1, P1_MOVE_SOURCE, P1_MOVE_DEST,
    PLAYER2, P2_MOVE_SOURCE, P2_MOVE_DEST }

  private class Spot extends Ellipse2D.Double {
    public int idx;
    public int xx;
    public int yy;
    public SpotState state;
    public Spot(int idx, int x, int y, SpotState state) {
      super(x, y, DIAMETER, DIAMETER);
      this.idx = idx;
      this.xx = x;
      this.yy = y;
      this.state = state;
    }
  }

  private final static int FLAT_TO_FLAT = 40;
  private final static int COL_WIDTH = 20; // FLAT_TO_FLAT / 2
  private final static double HEX_SIDE = 23.1; // FLAT_TO_FLAT / sqrt(3)
  private final static double HALF_HEX_SIDE = 11.55;
  private final static double ROW_HEIGHT = 34.65; // 3 * HALF_HEX_SIDE
  private final static int DIAMETER = 30;
  private final static int X_OFF = 10;
  private final static int Y_OFF = 5;
  private final static int BOARD_DIM = 9;

  private int turn = 0;
  private ArrayList<Move> moves = new ArrayList<>();
  private ArrayList<Spot> state = new ArrayList<>();
}
