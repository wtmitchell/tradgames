import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.concurrent.LinkedBlockingQueue;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumnModel;

public class GameMaster<BoardPanelType extends AbstractBoardPanel> {
  public static void main(String[] args) {
    GameMaster<HexGameBoard> g = new GameMaster<>(HexGameBoard.class);
  }

  public GameMaster(Class<BoardPanelType> typeClass) {
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

    // Instantiate the BoardPanel. This nonsense is needed since Java's generics
    // erases the type of BoardPanelType so there is no way to do something like:
    // BoardPanelType boardPanel = new BoardPanelType();
    try {
      boardPanel = typeClass.newInstance();
    } catch (InstantiationException ex) {
      ex.printStackTrace();
    } catch (IllegalAccessException ex) {
      ex.printStackTrace();
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
      changeState(State.STOPPING);
  }

  private void createProgramSelectorPanel(Container pane) {
    // Labels
    JPanel labels =
        new JPanel(new GridLayout(3, 1, 5, 5)); // row, col, hgap, vgap
    JLabel p1label = new JLabel("Player1:");
    labels.add(p1label);
    JLabel p2label = new JLabel("Player2:");
    labels.add(p2label);
    JLabel modlabel = new JLabel("Moderator:");
    labels.add(modlabel);
    pane.add(labels, BorderLayout.WEST);

    // Add center panel that has selectors
    JPanel center =
        new JPanel(new GridLayout(3, 1, 5, 5)); // row, col, hgap, vgap
    center.add(p1ps);
    center.add(p2ps);
    center.add(modps);
    pane.add(center, BorderLayout.CENTER);

    pane.add(mainButton, BorderLayout.LINE_END);
  }

  private void createTabbedPanel(Container pane) {
    JTabbedPane tabbed = new JTabbedPane();

    moveTable.setFillsViewportHeight(true);

    TableColumnModel tcm = moveTable.getColumnModel();
    tcm.getColumn(0).setMinWidth(50); // Turn
    tcm.getColumn(0).setMaxWidth(50); // Turn
    tcm.getColumn(0).setResizable(false); // Turn
    tcm.getColumn(1).setMinWidth(20); // #
    tcm.getColumn(1).setMaxWidth(20); // #
    tcm.getColumn(1).setResizable(false); // #
    // The other columns are left alone since their width could vary

    moveTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

    JScrollPane moveTableScroll = new JScrollPane(moveTable);
    moveTableScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    tabbed.add(moveTableScroll, "Moves");


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

  private void createMenu(JFrame main) {
    JMenuBar menuBar = new JMenuBar();

    JMenu menu = new JMenu("File");
    menu.setMnemonic(KeyEvent.VK_F);
    menu.getAccessibleContext().setAccessibleDescription("Open/Save a game");
    menuBar.add(menu);

    // Open
    openItem = new JMenuItem("Open", KeyEvent.VK_O);
    openItem.setAccelerator(
        KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
    openItem.getAccessibleContext().setAccessibleDescription("Opens a game");
    menu.add(openItem);

    // Save
    saveItem = new JMenuItem("Save", KeyEvent.VK_S);
    saveItem.setAccelerator(
        KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
    saveItem.getAccessibleContext().setAccessibleDescription("Saves a game");
    menu.add(saveItem);

    menu.addSeparator();

    // Exit
    exitItem = new JMenuItem("Exit", KeyEvent.VK_X);
    exitItem.getAccessibleContext().setAccessibleDescription(
        "Quits the program");
    menu.add(exitItem);

    main.setJMenuBar(menuBar);
  }

  private void createAndShowGUI() {
    // Create and set up the window.
    //JFrame frame = new JFrame("GameMaster");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    // Setup layout
    frame.setLayout(new BorderLayout());

    // Add things
    JPanel top = new JPanel(new BorderLayout());
    createProgramSelectorPanel(top);
    frame.add(top, BorderLayout.PAGE_START);

    JPanel center = new JPanel(new BorderLayout());
    createTabbedPanel(center);
    frame.add(center, BorderLayout.CENTER);

    frame.add(boardPanel, BorderLayout.WEST);

    createMenu(frame);

    // Make the GUI functional
    addAllListeners();

    // Begin waiting
    changeState(State.WAITING);

    // Display the window.
    frame.pack();
    frame.setVisible(true);
  }

  private void addAllListeners() {
    mainButton.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          mainListener();
        }
      });

    // Have the board GUI update when a row is selected in the JTable
    moveTable.getSelectionModel().addListSelectionListener(new ListSelectionListener() {
        @SuppressWarnings("unchecked")
        public void valueChanged(ListSelectionEvent e) {
          //Ignore extra messages.
          if (e.getValueIsAdjusting()) return;

          ListSelectionModel lsm = (ListSelectionModel)e.getSource();
          if (!lsm.isSelectionEmpty()) {
            int selectedRow = lsm.getMinSelectionIndex();

            // This cast really is safe since we constructed the table with
            // this specific model
            Move m = ((MoveTableModel)moveTable.getModel()).getRow(selectedRow);
            boardPanel.updateData(m.turn, m.state, m.nextMoves);
          }
        }
      });

    // Add hook to auto-scroll the table, and auto-select last row, which in turn
    // updates the GUI as the game is played.
    moveTable.getModel().addTableModelListener(new TableModelListener() {
        @Override
        public void tableChanged(TableModelEvent e) {
          if (e.getType() == TableModelEvent.INSERT) {
            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                  int last = moveTable.getModel().getRowCount() - 1;
                  Rectangle r = moveTable.getCellRect(last, 0, true);
                  moveTable.setRowSelectionInterval(last, last);
                  moveTable.scrollRectToVisible(r);
                }
              });
          }
        }
      });

    // Menu items
    openItem.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent evt) {
          System.out.println("Called open item");
        }
      });

    saveItem.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent evt) {
          System.out.println("Called save item");
        }
      });

    exitItem.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent evt) {
          frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));
        }
      });

  }

  private void changeEnable(boolean enable) {
    p1ps.setEnable(enable);
    p2ps.setEnable(enable);
    modps.setEnable(enable);
  }

  @SuppressWarnings("unchecked")
  private void resetForNewGame() {
    for (JTextArea i : textAreas)
      i.setText("");

    // This cast really is safe since we constructed the table with
    // this specific model
    ((MoveTableModel)moveTable.getModel()).clearAll();
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
      resetForNewGame();
      startGame();
    } else if (newState == State.STOPPING) {
      changeEnable(false);
      mainButton.setEnabled(false);
      mainButton.setText("Stopping. Please wait");
      currentState = State.STOPPING;
      gi.stopRunning();
    }
  }

  private void processQueue(int id) {
    // Read from the queue until it is empty
    for (String msg = updateQueues.get(id).poll(); msg != null;
         msg = updateQueues.get(id).poll())
      textAreas.get(id).append(msg + "\n");
  }

  @SuppressWarnings("unchecked")
  private void processGUIUpdate(String msg) {
    // These casts really are safe since we constructed the table with
    // this specific model
    if (msg.startsWith("MOVE")) {
      ((MoveTableModel)moveTable.getModel()).addMoveMsg(msg);
    } else if (msg.startsWith("GUI")) {
      ((MoveTableModel)moveTable.getModel()).addGUIState(msg);
    }
  }

  private void startGame() {
    String modCmd = modps.getProgram();
    String p1Cmd = p1ps.getProgram();
    boolean p1Human = p1ps.isHuman();
    String p2Cmd = p2ps.getProgram();
    boolean p2Human = p2ps.isHuman();

    System.out.println("modCmd: '" + modCmd + "'");
    System.out.println("p1Cmd: '" + p1Cmd + "' p1Human = " + p1Human);
    System.out.println("p2Cmd: '" + p2Cmd + "' p2Human = " + p2Human);
    // Ensure the user selected at least something
    if (modCmd.equals("")) {
      JOptionPane.showMessageDialog(null,
                                    "Moderator cannot be empty.",
                                    "Cannot start game",
                                    JOptionPane.ERROR_MESSAGE);
      changeState(State.WAITING);
      return;
    }
    if (p1Cmd.equals("")) {
      JOptionPane.showMessageDialog(null,
                                    "Player 1 cannot be empty.",
                                    "Cannot start game",
                                    JOptionPane.ERROR_MESSAGE);
      changeState(State.WAITING);
      return;
    }
    if (p2Cmd.equals("")) {
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
        new ProcessControlBlock(modCmd, true,
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
                                        processGUIUpdate(msg);
                                      }
                                    });
                                  }
                                }));
    // Create a PCB for player1
    if (p1Human) {
      pcbs.add(new ProcessControlBlock());
    } else {
      pcbs.add(new ProcessControlBlock(
          p1Cmd, false,
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
    }
    // Create a PCB for player2
    if (p2Human) {
      pcbs.add(new ProcessControlBlock());
    } else {
      pcbs.add(new ProcessControlBlock(
          p2Cmd, false,
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
    }
    // Start the actual game
    gi = new GameInstance(pcbs,
                          new UpdateHook() {
                            @Override
                            public void update() {
                              SwingUtilities.invokeLater(new Runnable() {
                                @Override
                                public void run() {
                                  changeState(State.WAITING);
                                }
                              });
                            }
                          },
                          new UpdateMsgHook() {
                            @Override
                            public void update(String msg) {
                              SwingUtilities.invokeLater(new Runnable() {
                                @Override
                                public void run() {
                                  JOptionPane.showMessageDialog(
                                      null, msg, "Error",
                                      JOptionPane.ERROR_MESSAGE);
                                }
                              });
                            }
                          });

    // Add appropriate hooks for human players and have them report their
    // names
    if (p1Human) {
      boardPanel.setP1Hook(new UpdateMsgHook() {
          @Override
          public void update(String msg) {
            gi.getMessageQueue().add(new Message(StreamType.stdout, false, msg, 1));
          }
        });
    }
    if (p2Human) {
      boardPanel.setP2Hook(new UpdateMsgHook() {
          @Override
          public void update(String msg) {
            gi.getMessageQueue().add(new Message(StreamType.stdout, false, msg, 2));
          }
        });
    }

    gi.start();

    if (p1Human) {
      gi.getMessageQueue().add(new Message(StreamType.stdout, false, "#name " + p1Cmd, 1));
    }
    if (p2Human) {
      gi.getMessageQueue().add(new Message(StreamType.stdout, false, "#name " + p2Cmd, 2));
    }


  }

  private GameInstance gi;

  private JButton mainButton = new JButton();

  private ProgramSelectorPanel p1ps = new ProgramSelectorPanel(true);
  private ProgramSelectorPanel p2ps = new ProgramSelectorPanel(true);
  private ProgramSelectorPanel modps = new ProgramSelectorPanel();

  private JMenuItem openItem;
  private JMenuItem saveItem;
  private JMenuItem exitItem;

  private JFrame frame = new JFrame("GameMaster");

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

  private JTable moveTable = new JTable(new MoveTableModel());

  private BoardPanelType boardPanel; // initialized in ctor

  private class Move {
    public Integer turn;
    public Integer playerNum;
    public String playerName;
    public String move;
    public String elapsed;
    public String state;
    public String nextMoves;

    public Object get(int i) {
      switch (i) {
      case 0:
        return turn;
      case 1:
        return playerNum;
      case 2:
        return playerName;
      case 3:
        return move;
      case 4:
        return elapsed;
      case 5:
        return state;
      case 6:
        return nextMoves;
      }
      return "?"; // Should never happen
    }
  }

  private class MoveTableModel extends AbstractTableModel {
    private String[] columnNames = {"Turn", "#", "Name", "Move", "Time elapsed",
                                    "Resulting Board State", "Next Moves"};


    private ArrayList<Move> data = new ArrayList<>();

    private String moveCache = "";

    @Override
    public int getColumnCount() {
      return columnNames.length;
    }

    @Override
    public int getRowCount() {
      return data.size();
    }

    @Override
    public String getColumnName(int col) {
      return columnNames[col];
    }

    @Override
    public Object getValueAt(int row, int col) {
      return data.get(row).get(col);
      //return "?";
    }

    @Override
    public Class getColumnClass(int c) {
      return getValueAt(0, c).getClass();
    }

    @Override
    public boolean isCellEditable(int row, int col) {
      return false;
    }

    public void addMoveMsg(String msg) {
      moveCache = msg;
    }

    public void addGUIState(String msg) {
      // Only valid to call if moveCache is not empty
      if (moveCache.equals(""))
        return;

      String[] p1 = moveCache.split("\\|");
      String[] p2 = msg.split("\\|");

      Move m = new Move();

      /*
        Example moderator lines:
        MOVE | Turn: 25 | Player 1: Random | Move: 1 MOVE FROM 27 TO 19 | Elapsed:  0h  0m  0s  21ms
        GUI | 2 1 1 1 1 0 0 0 0 0 1 0 1 1 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 2 0 0 0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2 | 28, 10; 28, 20; 28, 27; 28, 29; 28, 36; 28, 37; 61, 52; 61, 53; 61, 60; 62, 53; 62, 60; 69, 53; 69, 60; 69, 68; 70, 52; 70, 68; 71, 53; 77, 68; 77, 76; 78, 60; 78, 76;

        Example moderator lines after splitting:
        p1[0] = 'MOVE '
        p1[1] = ' Turn: 4 '
        p1[2] = ' Player 2: MyName '
        p1[3] = ' Move: 2 MOVE FROM 52 TO 51 '
        p1[4] = ' Elapsed:  0h  0m  0s  20ms'
        p2[0] = 'GUI '
        p2[1] = ' 1 1 1 1 0 1 1 0 0 0 1 1 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 0 0 2 2 0 0 0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2 '
        p2[2] = ' 1, 3; 1, 19; 2, 3; 2, 20; 4, 3; 4, 6; 4, 12; 4, 13; 5, 3; 5, 6; 5, 13; 5, 14; 5, 19; 10, 12; 10, 19; 11, 3; 11, 12; 11, 19; 11, 20; 18, 19; 18, 36; 27, 19; 27, 28; 27, 36; '
      */

      try {
        m.turn = Integer.parseInt(p1[1].substring(6).trim());
      } catch (NumberFormatException e) {
        m.turn = -1;
      }
      m.playerNum = p1[2].charAt(8) - '0';
      m.playerName = p1[2].substring(11).trim();
      m.move = p1[3].substring(9).trim();
      m.elapsed = p1[4].substring(9).trim();
      m.state = p2[1].trim();
      m.nextMoves = p2[2].trim();

      data.add(m);
      moveCache = "";

      fireTableRowsInserted(data.size(), data.size());
    }

    public void clearAll() {
      data.clear();
      fireTableDataChanged();
    }

    public Move getRow(int i) {
      return data.get(i);
    }
  }
}
