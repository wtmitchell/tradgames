import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;


public class AgentTester<BoardPanelType extends AbstractBoardPanel> {
  public static void main(String[] args) {
    AgentTester<HexGameBoard> a = new AgentTester<>(HexGameBoard.class);
  }

  public AgentTester(Class<BoardPanelType> typeClass) {
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
    //JFrame frame = new JFrame("GameMaster");
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
        public void actionPerformed(ActionEvent e) {
          startListener();
        }
      });
  }

  private void startListener() {
    if (currentState == State.WAITING)
      changeState(State.RUNNING);
    else if (currentState == State.RUNNING)
      changeState(State.STOPPING);
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
      //resetForNewGame();
      //startGame();
    } else if (newState == State.STOPPING) {
      ps.setEnable(false);
      startButton.setEnabled(false);
      startButton.setText("Please wait");
      currentState = State.STOPPING;
      //gi.stopRunning();
    }
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

}
