import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class GameMaster {
  private static void browseListener(JTextField dest, String title) {
    final JFileChooser fc = new JFileChooser();
    int returnVal = fc.showOpenDialog(null);

    if (returnVal == JFileChooser.APPROVE_OPTION) {
      dest.setText(fc.getSelectedFile().getAbsolutePath());
    }
  }
  private static void createProgramSelectorPanel(Container pane) {
    // Add center panel that has selectors
    JPanel center =
        new JPanel(new GridLayout(3, 3, 5, 5)); // row, col, hgap, vgap
    JLabel p1label = new JLabel("Player1:");
    center.add(p1label);
    final JTextField p1text = new JTextField();
    p1text.setEditable(true);
    center.add(p1text);
    JButton p1browse = new JButton("Browse");
    p1browse.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        browseListener(p1text, "Player 1");
      }
    });
    center.add(p1browse);

    JLabel p2label = new JLabel("Player2:");
    center.add(p2label);
    final JTextField p2text = new JTextField();
    p2text.setEditable(true);
    center.add(p2text);
    JButton p2browse = new JButton("Browse");
    p2browse.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        browseListener(p2text, "Player 2");
      }
    });
    center.add(p2browse);

    JLabel modlabel = new JLabel("Moderator:");
    center.add(modlabel);
    final JTextField modtext = new JTextField();
    modtext.setEditable(true);
    center.add(modtext);
    JButton modbrowse = new JButton("Browse");
    modbrowse.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        browseListener(modtext, "Moderator");
      }
    });
    center.add(modbrowse);

    pane.add(center, BorderLayout.CENTER);
    // Add Go button
    JButton gobutton = new JButton("Begin Game");
    pane.add(gobutton, BorderLayout.LINE_END);
  }
  private static void createCenterPanel(Container pane) {
    JTabbedPane tabbed = new JTabbedPane();

    JPanel gameBoard = new JPanel();
    tabbed.add(gameBoard, "Board");

    JPanel modstdout = new JPanel();
    tabbed.add(modstdout, "Moderator stdout");

    JPanel modstderr = new JPanel();
    tabbed.add(modstderr, "Moderator stderr");

    JPanel p1stdout = new JPanel();
    tabbed.add(p1stdout, "Player 1 stdout");

    JPanel p1stderr = new JPanel();
    tabbed.add(p1stderr, "Player 1 stderr");

    JPanel p2stdout = new JPanel();
    tabbed.add(p2stdout, "Player 2 stdout");

    JPanel p2stderr = new JPanel();
    tabbed.add(p2stderr, "Player 2 stderr");

    pane.add(tabbed, BorderLayout.CENTER);

    JPanel movePanel = new JPanel(new BorderLayout());
    JLabel moveLabel = new JLabel("Moves:");
    movePanel.add(moveLabel, BorderLayout.PAGE_START);
    JTextArea movesArea = new JTextArea("P1: MOVE XX TO YY");
    movePanel.add(movesArea, BorderLayout.CENTER);
    pane.add(movePanel, BorderLayout.LINE_END);
  }
  private static void createAndShowGUI() {
    // Create and set up the window.
    JFrame frame = new JFrame("GameMaster");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    // Setup layout
    frame.setLayout(new BorderLayout());

    // Add things
    JPanel top = new JPanel(new BorderLayout());
    createProgramSelectorPanel(top);
    frame.add(top, BorderLayout.PAGE_START);

    JPanel center = new JPanel(new BorderLayout());
    createCenterPanel(center);
    frame.add(center, BorderLayout.CENTER);

    // Display the window.
    frame.pack();
    frame.setVisible(true);
  }

  public static void main(String[] args) {
    // Set the Look and Feel
    try {
      UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
    } catch (UnsupportedLookAndFeelException ex) {
      ex.printStackTrace();
    } catch (IllegalAccessException ex) {
      ex.printStackTrace();
    } catch (InstantiationException ex) {
      ex.printStackTrace();
    } catch (ClassNotFoundException ex) {
      ex.printStackTrace();
    }

    // Turn off metal's use bold fonts
    UIManager.put("swing.boldMetal", Boolean.FALSE);

    javax.swing.SwingUtilities.invokeLater(new Runnable() {
      public void run() { createAndShowGUI(); }
    });
  }
}
