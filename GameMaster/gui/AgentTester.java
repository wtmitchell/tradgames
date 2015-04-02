import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import javax.swing.JButton;
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

public class AgentTester<BoardPanelType extends AbstractBoardPanel> {
  public static void main(String[] args) {
    // If using Windows turn off Direct3d rendering because it quite broken
    // with Java 8 and some unknown combination of Windows and video drivers
    // This is equivalent to putting -Dsun.java2d.d3d=false on the command
    // line
    if (System.getProperty("os.name").contains("Windows")) {
      System.setProperty("sun.java2d.d3d", "false");
    }

    AgentTester<HexGameBoard> a = new AgentTester<>(HexGameBoard.class);
  }

  public AgentTester(Class<BoardPanelType> typeClass) {
    // Set the Look and Feel
    /* Use the default Look and Feel since this exact code triggers a bug
       in Java 8 on OSX
    try {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
   //UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
    } catch (UnsupportedLookAndFeelException ex) {
      ex.printStackTrace();
    } catch (IllegalAccessException ex) {
      ex.printStackTrace();
    } catch (InstantiationException ex) {
      ex.printStackTrace();
    } catch (ClassNotFoundException ex) {
      ex.printStackTrace();
    }
    */

    // Turn off metal's use bold fonts
    //UIManager.put("swing.boldMetal", Boolean.FALSE);

    // Instantiate the BoardPanel. This nonsense is needed since Java's
    // generics
    // erases the type of BoardPanelType so there is no way to do something
    // like:
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

  private void createAndShowGUI() {
    // Create and set up the window.
    // JFrame frame = new JFrame("GameMaster");
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    // Setup layout
    frame.setLayout(new BorderLayout());

    // Add things
    JPanel top = new JPanel(new BorderLayout());
    top.add(new JLabel("Agent: "), BorderLayout.WEST);
    top.add(ps, BorderLayout.CENTER);
    top.add(startButton, BorderLayout.EAST);
    frame.add(top, BorderLayout.PAGE_START);

    frame.add(boardPanel, BorderLayout.WEST);

    JPanel center = new JPanel(new BorderLayout());
    JTabbedPane tabbed = new JTabbedPane();
    JScrollPane inoutScroll = new JScrollPane(inout);
    tabbed.add(inoutScroll, "Stdin/Stdout");

    JScrollPane errScroll = new JScrollPane(err);
    tabbed.add(errScroll, "Stderr");

    center.add(tabbed, BorderLayout.CENTER);
    frame.add(center, BorderLayout.CENTER);

    JPanel controls = new JPanel(new BorderLayout());
    controls.add(refreshButton, BorderLayout.WEST);
    controls.add(input, BorderLayout.CENTER);
    controls.add(sendButton, BorderLayout.EAST);

    frame.add(controls, BorderLayout.SOUTH);

    // Make the GUI functional
    addAllListeners();

    // Begin waiting
    changeState(State.WAITING);

    // Display the window.
    frame.pack();
    frame.setVisible(true);
  }

  private void addAllListeners() {
    startButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        startListener();
      }
    });

    sendButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent e) {
        sendListener();
      }
    });

    input.addKeyListener(new KeyListener() {
        @Override
        public void keyPressed(KeyEvent e) {
          if (e.getKeyCode() ==  KeyEvent.VK_ENTER) {
            sendListener();
          }
        }
        @Override
        public void keyReleased(KeyEvent e) {
        }
        @Override
        public void keyTyped(KeyEvent e) {
        }
      });

    refreshButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          refreshListener();
        }
      });

    boardPanel.setP1Hook(new UpdateMsgHook() {
        @Override
        public void update(String msg) {
          input.setText(msg);
        }
      });

    boardPanel.setP2Hook(new UpdateMsgHook() {
        @Override
        public void update(String msg) {
          input.setText(msg);
        }
      });
  }

  private void startListener() {
    if (currentState == State.WAITING)
      changeState(State.RUNNING);
    else if (currentState == State.RUNNING)
      changeState(State.STOPPING);
  }

  private void sendListener() {
    try {
      messages.put(new Message(input.getText()));
    } catch (InterruptedException e) {
      JOptionPane.showMessageDialog(null, e,
                                    "InterrupedException writing to stdin",
                                    JOptionPane.ERROR_MESSAGE);
    }
    input.setText("");
  }

  private void refreshListener() {
    try {
      expectingGUIUpdate = true;
      lastBoardState = "";
      lastMoves = "";
      messages.put(new Message("DUMPSTATE"));
      messages.put(new Message("LISTMOVES"));
    } catch (InterruptedException e) {
      JOptionPane.showMessageDialog(null, e,
                                    "InterrupedException writing to stdin",
                                    JOptionPane.ERROR_MESSAGE);
    }
  }

  private void changeState(State newState) {
    // No change
    if (currentState == newState)
      return;

    if (newState == State.WAITING) {
      ps.setEnable(true);
      startButton.setEnabled(true);
      startButton.setText("Start");
      currentState = State.WAITING;
    } else if (newState == State.RUNNING) {
      ps.setEnable(false);
      startButton.setEnabled(true);
      startButton.setText("Stop");
      currentState = State.RUNNING;
      resetForRun();
      (new MessageThread()).start();
    } else if (newState == State.STOPPING) {
      ps.setEnable(false);
      startButton.setEnabled(false);
      startButton.setText("Please wait");
      currentState = State.STOPPING;
      running.set(false);
    }
  }

  private void resetForRun() {
    inout.setText("");
    err.setText("");
  }

  private void checkGUIUpdate(String msg) {
    // Checks if return is from a GUI update and if so, handle it
    if (!expectingGUIUpdate)
      return;

    String[] tokens = msg.split(" ");
    if (tokens.length == 82 && lastBoardState.equals("")) {
      System.out.println("It is");
      lastBoardState = msg;
      return;
    }

    lastMoves = msg;

    boardPanel.updateData(-1, lastBoardState, lastMoves);
    expectingGUIUpdate = false;
  }

  private JFrame frame = new JFrame("Agent Tester");
  private BoardPanelType boardPanel;
  private ProgramSelectorPanel ps = new ProgramSelectorPanel();
  private JButton startButton = new JButton("Start");
  private JButton refreshButton = new JButton("Refresh Board");
  private JButton sendButton = new JButton("Send");
  private JTextArea inout = new JTextArea();
  private JTextArea err = new JTextArea();
  private JTextField input = new JTextField();

  private enum State { WAITING, RUNNING, STOPPING }
  private State currentState = State.STOPPING;

  private LinkedBlockingQueue<Message> messages = new LinkedBlockingQueue<>();
  private AtomicBoolean running = new AtomicBoolean(false);

  private String lastBoardState = "";
  private String lastMoves = "";
  private boolean expectingGUIUpdate = false;

  private class MessageThread extends Thread {
    @Override
    public void run() {
      messages.clear();
      if (startProcess()) {
        processMessages();

        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
              changeState(AgentTester.State.WAITING);
            }
          });
      }
    }

    private boolean startProcess() {
      try {
        ArrayList<String> args = ps.getProgram();

        ProcessBuilder pb = new ProcessBuilder(args);
        proc = pb.start();

        StreamConsumer stdout = new StreamConsumer(
            proc.getInputStream(), StreamType.stdout, false, messages, 0);
        StreamConsumer stderr = new StreamConsumer(
            proc.getErrorStream(), StreamType.stderr, false, messages, 0);
        stdout.start();
        stderr.start();

        stdin = new PrintWriter(new OutputStreamWriter(new BufferedOutputStream(
                                    proc.getOutputStream())),
                                true);

        if (!proc.isAlive()) {
          JOptionPane.showMessageDialog(
              null, "The process " + args +
                        " immediately terminated. Ensure it is a valid program",
              "Execution error", JOptionPane.ERROR_MESSAGE);
          return false;
        }
        return true;
      } catch (IOException e) {
        JOptionPane.showMessageDialog(null, e, "IOException",
                                      JOptionPane.ERROR_MESSAGE);
        changeState(AgentTester.State.WAITING);
      }
      return false;
    }

    private void processMessages() {
      Message m;
      running.set(true);
    main:
      while (running.get()) {
        try {
          Thread.sleep(5);
        } catch (InterruptedException e) {
        }

        // Check for new message
        m = messages.poll();
        if (m != null)
          handleMessage(m);

        // Check if client exited
        try {
          proc.exitValue();
          break main;
        } catch (IllegalThreadStateException e) {
        }
      }

      // Handle any remaining messages
      while ((m = messages.poll()) != null) {
        handleMessage(m);
      }

      // Forcibly kill the process if needed
      try {
        proc.exitValue();
      } catch (IllegalThreadStateException e) {
        proc.destroy();
      }
    }

    private void handleMessage(Message m) {
      m.message = m.message.trim();

      SwingUtilities.invokeLater(new Runnable() {
        @Override
        public void run() {
          if (m.type == StreamType.stdout) {
            inout.append(m.message + "\n");
            checkGUIUpdate(m.message);
          } else if (m.type == StreamType.stdin) {
            stdin.println(m.message);
            inout.append(m.message + "\n");
          } else if (m.type == StreamType.stderr) {
            err.append(m.message + "\n");
          }
        }
      });
    }

    private Process proc;
    private PrintWriter stdin;
  }
}
