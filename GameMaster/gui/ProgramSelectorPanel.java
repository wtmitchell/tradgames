import java.awt.BorderLayout;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ProgramSelectorPanel extends JPanel {
  public ProgramSelectorPanel() {
    this(false);
  }
  public ProgramSelectorPanel(boolean includeHuman) {
    super(new BorderLayout());

    mode = new JComboBox<String>();

    mode.addItem("App");
    mode.addItem("Java");
    if (includeHuman)
      mode.addItem("Human");

    switchState(State.App);
  }

  private void switchState(State newState) {
    if (currentState == newState)
      return;

    if (newState == State.Human) {
      this.remove(tf);
      this.remove(click);
      currentState = newState;
      return;
    }

    if (currentState == State.Human) {
      this.add(click, BorderLayout.WEST);
      this.add(tf, BorderLayout.CENTER);
    }
  }

  private JTextField tf = new JTextField();
  private JButton click = new JButton("Browse");

  private enum State { App, Java, Human };

  private State currentState = State.Human;
  private JComboBox<String> mode;
}
