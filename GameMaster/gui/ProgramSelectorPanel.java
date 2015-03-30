import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.jar.JarFile;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.filechooser.FileNameExtensionFilter;

public class ProgramSelectorPanel extends JPanel implements ItemListener {
  public ProgramSelectorPanel() {
    this(false);
  }
  public ProgramSelectorPanel(boolean includeHuman) {
    super(new BorderLayout());

    mode.addItem(BINARYPANEL);
    JPanel binPanel = new JPanel(new BorderLayout());
    binPanel.add(binButton, BorderLayout.WEST);
    binPanel.add(binTF, BorderLayout.CENTER);
    cards.add(binPanel, BINARYPANEL);
    currentCard = BINARYPANEL;

    mode.addItem(JAVAPANEL);
    JPanel javaPanel = new JPanel(new BorderLayout());
    javaPanel.add(javaButton, BorderLayout.WEST);
    javaPanel.add(javaTF, BorderLayout.CENTER);
    cards.add(javaPanel, JAVAPANEL);

    if (includeHuman) {
      mode.addItem(HUMANPANEL);
      JPanel humanPanel = new JPanel(new BorderLayout());
      humanPanel.add(new JLabel("Name: "), BorderLayout.WEST);
      humanTF.setText("Human");
      humanPanel.add(humanTF, BorderLayout.CENTER);
      cards.add(humanPanel, HUMANPANEL);
    }
    mode.setEditable(false);

    add(mode, BorderLayout.WEST);
    add(cards, BorderLayout.CENTER);

    mode.addItemListener(this);
    binButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          browseListener(binTF);
        }
      });
    javaButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          browseJavaListener(javaTF);
        }
      });
    //switchState(State.App);
  }

  private void browseListener(JTextField dest) {
    final JFileChooser fc = new JFileChooser();
    fc.setDialogTitle("Select an executable");
    if (fc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
      dest.setText(fc.getSelectedFile().getAbsolutePath());
    }
  }

  private void browseJavaListener(JTextField dest) {
    final JFileChooser fc = new JFileChooser();
    fc.setDialogTitle("Select a main class");

    FileNameExtensionFilter filter = new FileNameExtensionFilter(
        "Java Binary Files (.class, .jar)", "class", "jar");

    fc.setFileFilter(filter);
    if (fc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
      File f = fc.getSelectedFile();
      String parentDir = f.getParent();
      String filename = f.getName();
      String filenameStem = filename.substring(0, filename.lastIndexOf('.'));
      String extention = filename.substring(filename.lastIndexOf('.'));

      if (!extention.equals(".jar")) {
        dest.setText("-classpath " + parentDir + " " + filenameStem);
      } else {
        try {
        JarFile jf = new JarFile(f);
        if(jf.getManifest().getEntries().containsKey("Main-Class"))
          dest.setText("-jar " + f.getAbsolutePath());
        else
          dest.setText("-classpath " + f.getAbsolutePath() + " " + filenameStem);
        } catch (IOException e) {
          dest.setText("-classpath " + f.getAbsolutePath() + " " + filenameStem);
        }
      }
    }
  }

  public void itemStateChanged(ItemEvent e) {
    CardLayout cl = (CardLayout)(cards.getLayout());
    currentCard = (String) e.getItem();
    cl.show(cards, currentCard);
  }

  public boolean isHuman() {
    return currentCard.equals(HUMANPANEL);
  }

  public String getProgram() {
    if (currentCard.equals(HUMANPANEL))
      return humanTF.getText();

    if (currentCard.equals(BINARYPANEL))
      return binTF.getText();

    String separator = System.getProperty("file.separator");
    String path = System.getProperty("java.home")
      + separator + "bin" + separator + "java ";

    return path + javaTF.getText();
  }

  final static String BINARYPANEL = "Binary";
  final static String JAVAPANEL = "Java";
  final static String HUMANPANEL = "Human";

  private JTextField binTF = new JTextField();
  private JButton binButton = new JButton("Browse");

  private JTextField javaTF = new JTextField();
  private JButton javaButton = new JButton("Browse");

  private JTextField humanTF = new JTextField();

  private JPanel cards = new JPanel(new CardLayout());
  //private enum State { App, Java, Human };

  //private State currentState = State.Human;
  private String currentCard;
  private JComboBox<String> mode = new JComboBox<String>();
}
