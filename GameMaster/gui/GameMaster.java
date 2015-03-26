import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.concurrent.LinkedBlockingQueue;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
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

    // Instantiate the update queues
    for (int i = 0; i < StreamIDs.count; ++i) {
      updateQueues.add(new LinkedBlockingQueue<String>());
      textAreas.add(new JTextArea());
    }

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

    /* Hide until functional
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
    */

    JScrollPane modstdoutScroll = new JScrollPane(textAreas.get(StreamIDs.modstdout));
    modstdoutScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(modstdoutScroll, "Moderator stdout");

    JScrollPane modstderrScroll = new JScrollPane(textAreas.get(StreamIDs.modstderr));
    modstderrScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(modstderrScroll, "Moderator stderr");

    JScrollPane p1stdoutScroll = new JScrollPane(textAreas.get(StreamIDs.p1stdout));
    p1stdoutScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(p1stdoutScroll, "Player 1 stdout");

    JScrollPane p1stderrScroll = new JScrollPane(textAreas.get(StreamIDs.p1stderr));
    p1stderrScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(p1stderrScroll, "Player 1 stderr");

    JScrollPane p2stdoutScroll = new JScrollPane(textAreas.get(StreamIDs.p2stdout));
    p2stdoutScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(p2stdoutScroll, "Player 2 stdout");

    JScrollPane p2stderrScroll = new JScrollPane(textAreas.get(StreamIDs.p2stderr));
    p2stderrScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(p2stderrScroll, "Player 2 stderr");

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

  private void changeEnable(boolean enable) {
    p1cmd.setEditable(enable);
    p2cmd.setEditable(enable);
    modcmd.setEditable(enable);
    p1Browse.setEnabled(enable);
    p2Browse.setEnabled(enable);
    modBrowse.setEnabled(enable);
  }

  private void changeState(State newState) {
    // No change
    if (currentState == newState)
      return;

    if (newState == State.WAITING) {
      changeEnable(true);
      mainButton.setEnabled(true);
      mainButton.setText("Begin Game");
      currentState = State.WAITING;
    } else if (newState == State.RUNNING) {
      changeEnable(false);
      mainButton.setEnabled(true);
      mainButton.setText("Stop everything");
      currentState = State.RUNNING;
      startGame();
    } else if (newState == State.STOPPING) {
      changeEnable(false);
      mainButton.setEnabled(false);
      mainButton.setText("Stopping. Please wait");
      currentState = State.STOPPING;
    }
  }

  private void processQueue(int id) {
    // Read from the queue until it is empty
    for (String msg = updateQueues.get(id).poll(); msg != null;
         msg = updateQueues.get(id).poll())
      textAreas.get(id).append(msg + "\n");
  }

  private void startGame() {
    // Ensure the user selected at least something
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

    ArrayList<ProcessControlBlock> pcbs = new ArrayList<>();
    // Create a PCB for the moderator
    pcbs.add(
        new ProcessControlBlock(modcmd.getText(), true,
                                new UpdateMsgHook() {
                                  @Override
                                  public void update(String msg) {
                                    updateQueues.get(StreamIDs.modstdout).add(msg);
                                    SwingUtilities.invokeLater(new Runnable() {
                                      @Override
                                      public void run() {
                                        processQueue(StreamIDs.modstdout);
                                      }
                                    });
                                  }
                                },
                                new UpdateMsgHook() {
                                  @Override
                                  public void update(String msg) {
                                    updateQueues.get(StreamIDs.modstderr).add(msg);
                                    SwingUtilities.invokeLater(new Runnable() {
                                      @Override
                                      public void run() {
                                        processQueue(StreamIDs.modstderr);
                                      }
                                    });
                                  }
                                }));
    // Create a PCB for player1
    pcbs.add(
        new ProcessControlBlock(p1cmd.getText(), false,
                                new UpdateMsgHook() {
                                  @Override
                                  public void update(String msg) {
                                    updateQueues.get(StreamIDs.p1stdout).add(msg);
                                    SwingUtilities.invokeLater(new Runnable() {
                                      @Override
                                      public void run() {
                                        processQueue(StreamIDs.p1stdout);
                                      }
                                    });
                                  }
                                },
                                new UpdateMsgHook() {
                                  @Override
                                  public void update(String msg) {
                                    updateQueues.get(StreamIDs.p1stderr).add(msg);
                                    SwingUtilities.invokeLater(new Runnable() {
                                      @Override
                                      public void run() {
                                        processQueue(StreamIDs.p1stderr);
                                      }
                                    });
                                  }
                                }));
    // Create a PCB for player2
    pcbs.add(
        new ProcessControlBlock(p2cmd.getText(), false,
                                new UpdateMsgHook() {
                                  @Override
                                  public void update(String msg) {
                                    updateQueues.get(StreamIDs.p2stdout).add(msg);
                                    SwingUtilities.invokeLater(new Runnable() {
                                      @Override
                                      public void run() {
                                        processQueue(StreamIDs.p2stdout);
                                      }
                                    });
                                  }
                                },
                                new UpdateMsgHook() {
                                  @Override
                                  public void update(String msg) {
                                    updateQueues.get(StreamIDs.p2stderr).add(msg);
                                    SwingUtilities.invokeLater(new Runnable() {
                                      @Override
                                      public void run() {
                                        processQueue(StreamIDs.p2stderr);
                                      }
                                    });
                                  }
                                }));

    // Start the actual game
    GameInstance gi = new GameInstance(pcbs);
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

  // Note that the elements of StreamIDs must start at zero
  // and increase by 1 between constants. These are used as
  // named constants to access into the updateQueues ArrayList
  private class StreamIDs {
    public final static int modstdout = 0;
    public final static int modstderr = 1;
    public final static int p1stdout = 2;
    public final static int p1stderr = 3;
    public final static int p2stdout = 4;
    public final static int p2stderr = 5;
    public final static int count = 6;
  }
  private ArrayList<LinkedBlockingQueue<String>> updateQueues =
      new ArrayList<LinkedBlockingQueue<String>>();
  private ArrayList<JTextArea> textAreas = new ArrayList<JTextArea>();
}
