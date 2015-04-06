import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.HashMap;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.UIManager;

public class HexGameBoard extends AbstractBoardPanel {
  HexGameBoard() {
    super(new BorderLayout());
    // By default the hooks do nothing
    p1Hook = new UpdateMsgHook() {
      @Override
      public void update(String msg) {}
    };
    p2Hook = new UpdateMsgHook() {
      @Override
      public void update(String msg) {}
    };

    add(showNumsCheck, BorderLayout.NORTH);
    add(board, BorderLayout.CENTER);

    showNumsCheck.addItemListener(new ItemListener() {
        @Override
        public void itemStateChanged(ItemEvent e) {
          board.repaint();
        }
      });
  }

  @Override
  public void updateData(int turn, String boardState, String nextMoves) {
    this.turn = turn;

    // Split apart state
    String[] stateTokens = boardState.split(" ");
    try {
      nextPlayer = Integer.parseInt(stateTokens[0]);
      // Start at 1 since first digit is player's turn
      for (int i = 1; i < stateTokens.length; ++i) {
        Spot s = state.get(i - 1);
        // s.highlight = false;
        switch (Integer.parseInt(stateTokens[i])) {
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
      System.err.println("Invalid state string passed to HexGameBoard: " +
                         boardState);
      e.printStackTrace();
    }

    // Split apart moves
    String[] moveTokens = nextMoves.split(";");
    moves.clear();
    movesCount = 0;
    movesDupes = false;
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
          ++movesCount;
        } else {
          if (!dests.contains(to)) {
            dests.add(to);
          } else {
            movesDupes = true;
            System.out.println("Duplicate move " + from + ", " + to);
          }
          ++movesCount;
        }
      }
    } catch (NumberFormatException e) {
      System.err.println("Invalid move string passed to HexGameBoard: " +
                         nextMoves);
      e.printStackTrace();
    }

    board.repaint();
  }

  @Override
  public void setP1Hook(UpdateMsgHook hook) {
    p1Hook = hook;
  }

  @Override
  public void setP2Hook(UpdateMsgHook hook) {
    p2Hook = hook;
  }

  private class Move {
    public int from;
    public int to;

    public Move(int from, int to) {
      this.from = from;
      this.to = to;
    }
  }

  private enum SpotState { EMPTY, PLAYER1, PLAYER2 }

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
  private final static int COL_WIDTH = 20;     // FLAT_TO_FLAT / 2
  private final static double HEX_SIDE = 23.1; // FLAT_TO_FLAT / sqrt(3)
  private final static double HALF_HEX_SIDE = 11.55;
  private final static double ROW_HEIGHT = 34.65; // 3 * HALF_HEX_SIDE
  private final static int DIAMETER = 30;

  // Constant shifts applied inside the TheBoard panel
  private final static int X_OFF = 10;
  private final static int Y_OFF = 5;

  private final static int BOARD_DIM = 9;

  private static class UIColor {
    private final static Color EMPTY = Color.WHITE;
    private final static Color P1 = Color.RED;
    private final static Color P2 = Color.CYAN;
    private final static Color FROM = Color.YELLOW;
    private final static Color TO = Color.MAGENTA;
    private final static Color BORDER = Color.BLACK;
    private final static Color LOCATION_NUMBER = Color.BLACK;
  }

  private int turn = -1;
  private int hover = -1;
  private int from = -1;
  private int nextPlayer = -1;
  private ArrayList<Spot> state = new ArrayList<>();
  private HashMap<Integer, ArrayList<Integer>> moves = new HashMap<>();
  private int movesCount = -1;
  private boolean movesDupes = false;
  private UpdateMsgHook p1Hook;
  private UpdateMsgHook p2Hook;
  private JCheckBox showNumsCheck = new JCheckBox("Show location numbers");
  private TheBoard board = new TheBoard();

  private class TheBoard
      extends JPanel implements MouseListener, MouseMotionListener {
    public TheBoard() {
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

      Graphics2D g2d = (Graphics2D)g;
      g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                           RenderingHints.VALUE_ANTIALIAS_ON);

      // Clear existing
      g2d.setColor(UIManager.getColor("Panel.background"));
      g2d.fillRect(0, 0, getWidth(), getHeight());

      // Draw information if sensible
      if (turn >= 0) {
        g2d.setColor(UIManager.getColor("Panel.foreground"));
        g2d.drawString("Turn: " + turn, 0, 20 + Y_OFF);
      }

      if (nextPlayer != -1) {
        String nextMove = "Next Move: ";
        g2d.setColor(UIManager.getColor("Panel.foreground"));
        g2d.drawString(nextMove, 0, 40 + Y_OFF);

        g2d.setPaint(nextPlayer == 1 ? UIColor.P1 : UIColor.P2);
        Rectangle2D nextMoveBounds =
            g2d.getFont().getStringBounds(nextMove, g2d.getFontRenderContext());
        int nextMoveDiam = 10;
        g2d.fillOval((int)nextMoveBounds.getWidth(),
                     40 + Y_OFF - (int)nextMoveBounds.getHeight() / 2 -
                         nextMoveDiam / 2,
                     nextMoveDiam, nextMoveDiam);
      }
      if (movesCount != -1) {
        g2d.setColor(UIManager.getColor("Panel.foreground"));
        g2d.drawString("# moves: " + movesCount, 0, 60 + Y_OFF);
      }

      if (movesDupes) {
        g2d.setColor(UIManager.getColor("Panel.foreground"));
        g2d.drawString("Has duplicates", 0, 80 + Y_OFF);
      }

      // Draw mouse over hover
      if (from != -1 || hover != -1) {
        int source = from != -1 ? from : hover;

        // Highlight source
        Spot sourceSpot = state.get(source);
        g2d.setPaint(UIColor.FROM);
        g2d.fillOval(sourceSpot.xx - 5, sourceSpot.yy - 5, DIAMETER + 10,
                     DIAMETER + 10);

        // Highlight all dests
        ArrayList<Integer> dests = moves.get(source);
        if (dests != null) {
          for (Integer to : dests) {
            Spot destSpot = state.get(to);
            g2d.setPaint(UIColor.TO);
            g2d.fillOval(destSpot.xx - 5, destSpot.yy - 5, DIAMETER + 10,
                         DIAMETER + 10);
          }
        }
      }

      // Draw the actual spots
      for (Spot i : state) {
        switch (i.state) {
        case EMPTY:
          g2d.setPaint(UIColor.EMPTY);
          g2d.fill(i);
          g2d.setStroke(new BasicStroke(3.0f));
          g2d.setPaint(UIColor.BORDER);
          g2d.drawOval(i.xx, i.yy, DIAMETER, DIAMETER);
          break;
        case PLAYER1:
          g2d.setPaint(UIColor.P1);
          g2d.fill(i);
          g2d.setStroke(new BasicStroke(3.0f));
          g2d.setPaint(UIColor.BORDER);
          g2d.drawOval(i.xx, i.yy, DIAMETER, DIAMETER);
          break;
        case PLAYER2:
          g2d.setPaint(UIColor.P2);
          g2d.fill(i);
          g2d.setStroke(new BasicStroke(3.0f));
          g2d.setPaint(UIColor.BORDER);
          g2d.drawOval(i.xx, i.yy, DIAMETER, DIAMETER);
          break;
        }

        // Overlay location numbers
        if (showNumsCheck.isSelected()) {
          g2d.setColor(UIColor.LOCATION_NUMBER);
          g2d.drawString("" + i.idx, i.xx + DIAMETER / 3, i.yy + 3 * DIAMETER / 4);
        }
      }

      g2d.dispose();
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
    public void mouseReleased(MouseEvent e) {}

    @Override
    public void mouseClicked(MouseEvent e) {
      // New from of a move
      if (hover != -1 && from == -1) {
        from = hover;
        return;
      }

      // Or maybe not
      if (!moves.containsKey(from))
        return;

      // Clicked on a valid destination
      int idx = mouseToIndex(e.getPoint());
      if (moves.get(from).contains(idx)) {
        String move = "MOVE FROM " + from + " TO " + idx;
        if (turn % 2 == 0)
          p1Hook.update(move);
        else
          p2Hook.update(move);
      }

      // Since we clicked, reset to non-clicked state
      hover = -1;
      from = -1;
      repaint();
    }

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {
      if (hover != -1) {
        hover = -1;
        from = -1;
        repaint();
      }
    }

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseDragged(MouseEvent e) {}

    @Override
    public void mouseMoved(MouseEvent e) {
      int idx = mouseToIndex(e.getPoint());

      // Outside of a spot
      if (idx == -1 || !state.get(idx).contains(e.getPoint())) {
        if (hover != -1) {
          hover = -1;
          repaint();
        }
        return;
      }

      // If we already have a from or if this is not a valid source of
      // a move, do nothing
      if (from != -1 || !moves.containsKey(idx))
        return;

      // Start highlighting if we aren't already
      if (hover != idx) {
        hover = idx;
        repaint();
      }
    }
  }
}
