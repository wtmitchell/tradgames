import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;

public class HexGameBoard extends AbstractBoardPanel {
  HexGameBoard() {
    setPreferredSize(new Dimension(400, 400));
  }

  @Override
  public void paintComponent(Graphics g) {
    super.paintComponent(g);

    if (image == null)
      createEmptyImage();

    g.drawImage(image, 0, 0, null);

    //g.drawString("Turn: " + turn, 0, 20);
    //g.drawString(boardState, 20, 20);
    //g.drawRect(200, 200, 200, 200);
  }

  @Override
  public void updateData(int turn, String boardState, String nextMoves) {
    this.turn = turn;
    this.boardState = boardState;
    this.nextMoves = nextMoves;

    System.out.println("Turn: " + this.turn);
    System.out.println("State: " + this.boardState);
    System.out.println("nextMoves: " + this.nextMoves);

    drawBoard();
  }

  private void drawBoard() {
    g2d.setColor(new Color(0, 0, 0, 0)); // FIXME Should be pulled from the JPanel
    g2d.clearRect(0, 0, getWidth(), getHeight());
    g2d.drawString("Turn: " + turn, 0, 20);

    repaint();
  }

  private void createEmptyImage() {
    image = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
    g2d = (Graphics2D)image.getGraphics();
    /*g2d.setColor(Color.BLACK);
    g2d.drawString("Add a rectangle by doing mouse press, drag and release!", 40, 15);*/
  }

  private BufferedImage image;
  private Graphics2D g2d;
  private int turn = 0;
  private String boardState = "";
  private String nextMoves = "";
}
