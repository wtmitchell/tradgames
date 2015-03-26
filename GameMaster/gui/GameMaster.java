import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

public class GameMaster {
  public static void main(String[] args) { GameMaster g = new GameMaster(); }

  public GameMaster() {
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

  private void browseListener(JTextField dest, String title) {
    final JFileChooser fc = new JFileChooser();
    fc.setDialogTitle(title);
    if (fc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
      dest.setText(fc.getSelectedFile().getAbsolutePath());
    }
  }

  private void mainListener() {
    if (currentState == State.WAITING)
      changeState(State.RUNNING);
    else if (currentState == State.RUNNING)
      changeState(State.WAITING);
  }

  private void createProgramSelectorPanel(Container pane) {
    // Add center panel that has selectors
    JPanel center =
        new JPanel(new GridLayout(3, 3, 5, 5)); // row, col, hgap, vgap
    JLabel p1label = new JLabel("Player1:");
    center.add(p1label);
    center.add(p1Browse);
    center.add(p1cmd);

    JLabel p2label = new JLabel("Player2:");
    center.add(p2label);
    center.add(p2Browse);
    center.add(p2cmd);

    JLabel modlabel = new JLabel("Moderator:");
    center.add(modlabel);
    center.add(modBrowse);
    center.add(modcmd);

    pane.add(center, BorderLayout.CENTER);

    pane.add(mainButton, BorderLayout.LINE_END);
  }

  private void createCenterPanel(Container pane) {
    JTabbedPane tabbed = new JTabbedPane();

    JPanel gameBoardTab = new JPanel(new BorderLayout());
    gameBoardTab.add(new JLabel("Not yet implemented"), BorderLayout.CENTER);
    JPanel movePanel = new JPanel(new BorderLayout());
    movePanel.add(new JLabel("Moves:"), BorderLayout.PAGE_START);
    JTextArea movesArea = new JTextArea(
        "Not yet implemented.\n Will be list"
        + " of moves that can\n be clicked to show the board\n at that state");
    movePanel.add(movesArea, BorderLayout.CENTER);
    gameBoardTab.add(movePanel, BorderLayout.LINE_END);
    tabbed.add(gameBoardTab, "Board");

    JTextArea modstdout = new JTextArea();
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


  }

  private void createAndShowGUI() {
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

    // Make the GUI functional
    addAllListeners();

    // Begin waiting
    changeState(State.WAITING);

    // Display the window.
    frame.pack();
    frame.setVisible(true);
  }

  private void addAllListeners() {
    p1Browse.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          browseListener(p1cmd, "Player 1");
        }
      });

    p2Browse.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          browseListener(p2cmd, "Player 2");
        }
      });

    modBrowse.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        browseListener(modcmd, "Moderator");
      }
    });

    mainButton.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          mainListener();
        }
      });
  }

  private void changeState(State newState) {
    // No change
    if (currentState == newState)
      return;

    if (newState == State.WAITING) {
      p1cmd.setEditable(true);
      p2cmd.setEditable(true);
      modcmd.setEditable(true);
      p1Browse.setEnabled(true);
      p2Browse.setEnabled(true);
      modBrowse.setEnabled(true);
      mainButton.setEnabled(true);
      mainButton.setText("Begin Game");
      currentState = State.WAITING;
    } else if (newState == State.RUNNING) {
      p1cmd.setEditable(false);
      p2cmd.setEditable(false);
      modcmd.setEditable(false);
      p1Browse.setEnabled(false);
      p2Browse.setEnabled(false);
      modBrowse.setEnabled(false);
      mainButton.setEnabled(true);
      mainButton.setText("Stop everything");
      currentState = State.RUNNING;
      startGameInstance();
    } else if (newState == State.STOPPING) {
      p1cmd.setEditable(false);
      p2cmd.setEditable(false);
      modcmd.setEditable(false);
      p1Browse.setEnabled(false);
      p2Browse.setEnabled(false);
      modBrowse.setEnabled(false);
      mainButton.setEnabled(false);
      mainButton.setText("Stopping. Please wait");
      currentState = State.STOPPING;
    }
  }

  private void startGameInstance() {
    if (modcmd.getText().equals("")) {
      JOptionPane.showMessageDialog(null,
                                    "Moderator cannot be empty.",
                                    "Cannot start game",
                                    JOptionPane.ERROR_MESSAGE);
      changeState(State.WAITING);
      return;
    }
    if (p1cmd.getText().equals("")) {
      JOptionPane.showMessageDialog(null,
                                    "Player 1 cannot be empty.",
                                    "Cannot start game",
                                    JOptionPane.ERROR_MESSAGE);
      changeState(State.WAITING);
      return;
    }
    if (p2cmd.getText().equals("")) {
      JOptionPane.showMessageDialog(null,
                                    "Player 2 cannot be empty.",
                                    "Cannot start game",
                                    JOptionPane.ERROR_MESSAGE);
      changeState(State.WAITING);
      return;
    }
    ArrayList<String> programs = new ArrayList<String>();
    programs.add(modcmd.getText());
    programs.add(p1cmd.getText());
    programs.add(p2cmd.getText());

    GameInstance gi = new GameInstance(programs);
    gi.start();
  }

  private JTextField p1cmd = new JTextField();
  private JTextField p2cmd = new JTextField();
  private JTextField modcmd = new JTextField();
  private JButton mainButton = new JButton();
  private JButton p1Browse = new JButton("Browse");
  private JButton p2Browse = new JButton("Browse");
  private JButton modBrowse = new JButton("Browse");
  private enum State { WAITING, RUNNING, STOPPING }
  private State currentState = State.STOPPING;
}
