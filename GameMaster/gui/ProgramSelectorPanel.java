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
          browseBinListener(binTF);
        }
      });
    javaButton.addActionListener(new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
          browseJavaListener(javaTF);
        }
      });
  }

  private void browseBinListener(JTextField dest) {
    final JFileChooser fc = new JFileChooser();
    fc.setDialogTitle("Select an executable");
    if (fc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
      binFile = fc.getSelectedFile().getAbsolutePath();
      dest.setText(binFile);
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

          // It would seem that
          // jf.getManifest().getMainAttributes().containsKey("Main-Class")
          // should work, but for some reason it did not, and comparing the
          // Main-Class to the empty string does work BUG
          if (jf.getManifest() != null &&
              jf.getManifest().getMainAttributes().getValue("Main-Class") != "")
            dest.setText("-jar " + f.getAbsolutePath());
          else
            dest.setText("-classpath " + f.getAbsolutePath() + " " +
                         filenameStem);
        } catch (IOException e) {
          dest.setText("-classpath " + f.getAbsolutePath() + " " +
                       filenameStem);
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

  public ArrayList<String> getProgram() {
    ArrayList<String> args = new ArrayList<>();
    if (currentCard.equals(HUMANPANEL)) {
      args.add(humanTF.getText());
      return args;
    }

    if (currentCard.equals(BINARYPANEL)) {
      String TFString = binTF.getText();

      if (TFString.startsWith(binFile)) {
        args.add(binFile);
        TFString = TFString.substring(binFile.length());
      }
      String[] tokens = TFString.split(" ");
      for (String s : tokens) {
        if (s.length() > 0)
          args.add(s);
      }

      System.out.print("Split '" + binTF.getText() + "' as ");
      for (String s : args)
        System.out.print("'" + s + "' ");
      System.out.print("\n");

      return args;
    }

    String separator = System.getProperty("file.separator");
    String path = System.getProperty("java.home")
      + separator + "bin" + separator + "java";

    if (!(new File(path)).isFile() && (new File(path + ".exe")).isFile())
      path += ".exe";

    args.add(path);
    args.add(javaTF.getText());

    System.out.print("Split '" + javaTF.getText() + "' as ");
    for (String s : args)
      System.out.print("'" + s + "' ");
    System.out.print("\n");

    return args;
  }

  public void setEnable(boolean enabled) {
    binTF.setEditable(enabled);
    binButton.setEnabled(enabled);
    javaTF.setEditable(enabled);
    javaButton.setEnabled(enabled);
    humanTF.setEditable(enabled);
  }

  final static String BINARYPANEL = "Binary";
  final static String JAVAPANEL = "Java";
  final static String HUMANPANEL = "Human";

  private JTextField binTF = new JTextField();
  private JButton binButton = new JButton("Browse");
  private String binFile = "";

  private JTextField javaTF = new JTextField();
  private JButton javaButton = new JButton("Browse");

  private JTextField humanTF = new JTextField();

  private JPanel cards = new JPanel(new CardLayout());

  private String currentCard;
  private JComboBox<String> mode = new JComboBox<String>();
}
