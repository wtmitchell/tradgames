import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import javax.swing.JPanel;
import javax.swing.UIManager;

// Leads to 24 * R = 320 * sqrt(3) = 554.25 height

public class HexGameBoard extends AbstractBoardPanel {
  HexGameBoard() {
    setPreferredSize(
        new Dimension((int)(9 * FLAT_TO_FLAT + 2 * X_OFF),
                      (int)(52 * HALF_HEX_SIDE + 2 * Y_OFF))); // w, h
  }

  @Override
  public void paintComponent(Graphics g) {
    super.paintComponent(g);

    // All the drawing is on a BufferedImage, so we just need to draw the image
    if (emptyBoard == null)
      createBufferedImages();

    g.drawImage(emptyBoard, 0, 0, null);
    g.drawImage(piecesBoardOverlay, 0, 0, null);

  }

  @Override
  public void updateData(int turn, String boardState, String nextMoves) {
    this.turn = turn;

    // Split apart state
    String[] stateTokens = boardState.split(" ");
    state.clear();
    try {
      // Start at 1 since first digit is player's turn
      for (int i = 1; i < stateTokens.length; ++i) {
        state.add(Integer.parseInt(stateTokens[i]));
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

    drawBoard(piecesBoardOverlay);
  }

  private void drawBoard(BufferedImage bi) {
    Graphics2D g2d = (Graphics2D) bi.getGraphics();
    g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

    // Clear existing
    g2d.setBackground(new Color(0, true));
    g2d.clearRect(0, 0, bi.getWidth(), bi.getHeight());

    // Draw information
    g2d.setColor(UIManager.getColor("Panel.foreground"));
    g2d.drawString("Turn: " + turn, 0, 20);

    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
      int row = i / BOARD_DIM;
      int col = i % BOARD_DIM;
      int dispRow = row + col;
      int dispCol = col - row + BOARD_DIM - 1;

      int currentVal = state.get(i);
      if (currentVal == 0) {
        // Empty spot
        // Do nothing, underlying empty board takes care of it
      } else if (currentVal == 1) {
        // Player 1 piece
        drawCircle(g2d, dispCol * COL_WIDTH, (int) (dispRow * ROW_HEIGHT), Color.RED);
      } else if (currentVal == 2) {
        // Player 2 piece
        drawCircle(g2d, dispCol * COL_WIDTH, (int) (dispRow * ROW_HEIGHT), Color.BLUE);
      }
    }

    g2d.dispose();

    // Repaint to update
    repaint();
  }

  private void drawCircle(Graphics2D g, int x, int y, Color c) {
    g.setPaint(c);
    g.fillOval(x + X_OFF, y + Y_OFF, DIAMETER, DIAMETER);
    g.setStroke(new BasicStroke(3.0f));
    g.setPaint(Color.BLACK);
    g.drawOval(x + X_OFF, y + Y_OFF, DIAMETER, DIAMETER);
  }

  private void createBufferedImages() {
    emptyBoard = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
    Graphics2D g2d = (Graphics2D) emptyBoard.getGraphics();
    g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

    // Start with a clean slate
    g2d.setColor(UIManager.getColor("Panel.background"));
    g2d.fillRect(0, 0, getWidth(), getHeight());

    // Draw all the pieces
    for (int i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
      int row = i / BOARD_DIM;
      int col = i % BOARD_DIM;
      int dispRow = row + col;
      int dispCol = col - row + BOARD_DIM - 1;

      drawCircle(g2d, dispCol * COL_WIDTH, (int) (dispRow * ROW_HEIGHT), Color.WHITE);
    }

    g2d.dispose();

    piecesBoardOverlay = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
  }

  private class Move {
    public int from;
    public int to;

    public Move(int from, int to) {
      this.from = from;
      this.to = to;
    }
  }

  private final static int FLAT_TO_FLAT = 40;
  private final static int COL_WIDTH = 20; // FLAT_TO_FLAT / 2
  private final static double HEX_SIDE = 23.1; // FLAT_TO_FLAT / sqrt(3)
  private final static double HALF_HEX_SIDE = 11.55;
  private final static double ROW_HEIGHT = 34.65; // 3 * HALF_HEX_SIDE
  private final static int DIAMETER = 30;
  private final static int X_OFF = 5;
  private final static int Y_OFF = 5;
  private final static int BOARD_DIM = 9;

  private BufferedImage emptyBoard;
  private BufferedImage piecesBoardOverlay;

  private int turn = 0;
  private ArrayList<Integer> state = new ArrayList<>();
  private ArrayList<Move> moves = new ArrayList<>();
}
