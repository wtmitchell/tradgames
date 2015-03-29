import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.HashMap;
import javax.swing.JPanel;
import javax.swing.UIManager;

public class HexGameBoard extends AbstractBoardPanel implements MouseListener, MouseMotionListener {
  HexGameBoard() {
    setPreferredSize(
        new Dimension((int)(9 * FLAT_TO_FLAT + 2 * X_OFF),
                      (int)(52 * HALF_HEX_SIDE + 2 * Y_OFF))); // w, h
    addMouseListener(this);
    addMouseMotionListener(this);

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

    // Draw mouse over hover
    if (hover != -1) {
      Spot s = state.get(hover);
      g2d.setPaint(Color.YELLOW);
      g2d.fillOval(s.xx - 5, s.yy - 5, DIAMETER + 10, DIAMETER + 10);
    }

    if (hover != -1) {
      ArrayList<Integer> dests = moves.get(hover);
      if (dests != null) {
        for (Integer to : dests) {
          Spot s = state.get(to);
          g2d.setPaint(Color.MAGENTA);
          g2d.fillOval(s.xx - 5, s.yy - 5, DIAMETER + 10, DIAMETER + 10);
        }
      }
    }

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
        Spot s = state.get(i - 1);
        //s.highlight = false;
        switch(Integer.parseInt(stateTokens[i])) {
        case 0:
          s.state = SpotState.EMPTY;
          break;
        case 1:
          s.state = SpotState.PLAYER1;
          break;
        case 2:
          s.state = SpotState.PLAYER2;
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
        int from = Integer.parseInt(tok[0].trim());
        int to = Integer.parseInt(tok[1].trim());
        ArrayList<Integer> dests = moves.get(from);
        if (dests == null) {
          dests = new ArrayList<Integer>();
          dests.add(to);
          moves.put(from, dests);
        } else {
          dests.add(to);
        }
        //moves.add(new Move(Integer.parseInt(tok[0].trim()), Integer.parseInt(tok[1].trim())));
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

  private int mouseToIndex(Point p) {
    int x = p.x - X_OFF;
    int y = p.y - Y_OFF;
    int dispCol = x / COL_WIDTH;
    int dispRow = (int)(y / ROW_HEIGHT);
    int col = (dispCol + dispRow - BOARD_DIM + 1) / 2;
    int row = dispRow - col;

    // Outside of board
    if (row < 0 || row >= BOARD_DIM || col < 0 || col >= BOARD_DIM)
      return -1;

    int idx = row * BOARD_DIM + col;

    return idx;
  }

  @Override
  public void mouseReleased(MouseEvent e) {
  }

  @Override
  public void mouseClicked(MouseEvent e) {
    System.out.println("Clicked on " + hover);
  }

  @Override
  public void mouseEntered(MouseEvent e) {
  }

  @Override
  public void mouseExited(MouseEvent e) {
    if (hover != -1) {
      hover = -1;
      repaint();
    }
  }

  @Override
  public void mousePressed(MouseEvent e) {
  }

  @Override
  public void mouseDragged(MouseEvent e) {
  }

  @Override
  public void mouseMoved(MouseEvent e) {
    // Quick no change
    if (hover != -1 && state.get(hover).contains(e.getPoint()))
      return;

    int idx = mouseToIndex(e.getPoint());

    // Outside of a spot
    if (idx == -1 || !state.get(idx).contains(e.getPoint())) {
      if (hover != -1) {
        hover = -1;
        repaint();
      }
      return;
    }

    // Start highlighting if we aren't already
    if (hover != idx) {
      hover = idx;
      repaint();
    }
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
      xx = x;
      yy = y;
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
  private int hover = -1;
  private ArrayList<Spot> state = new ArrayList<>();
  private HashMap<Integer, ArrayList<Integer>> moves = new HashMap<>();
}
