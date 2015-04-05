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
import javax.swing.JOptionPane;
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
      String extension = filename.substring(filename.lastIndexOf('.'));

      if (!extension.equals(".jar")) {
        dest.setText("-classpath " + parentDir + " " + filenameStem);
        javaSelected = parentDir;
        javaJar = false;
        javaJarExecutable = false;
      } else {
        try {
          javaJar = true;
          javaSelected = f.getAbsolutePath();
          JarFile jf = new JarFile(f);

          // It would seem that
          // jf.getManifest().getMainAttributes().containsKey("Main-Class")
          // should work, but for some reason it did not, and comparing the
          // Main-Class to the empty string does work BUG
          if (jf.getManifest() != null &&
              jf.getManifest().getMainAttributes().getValue("Main-Class") != "") {
            javaJarExecutable = true;
            dest.setText("-jar " + f.getAbsolutePath());
          } else {
            javaJarExecutable = false;
            dest.setText("-classpath " + f.getAbsolutePath() + " " +
                         filenameStem);
          }
        } catch (IOException e) {
          javaJarExecutable = false;
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

  public ArrayList<String> splitOffLeadingFile(String in) {
    // Looks for a valid file left to right
    int i = 1;
    for (; i < in.length(); ++i) {
      if ((new File(in.substring(0,i))).isFile())
        break;
    }
    ArrayList<String> split = new ArrayList<>();
    split.add(in.substring(0, i));
    if (i < in.length())
      split.add(in.substring(i));

    return split;
  }

  public ArrayList<String> splitOffLeadingDirectory(String in) {
    // Looks for a valid directory right to left
    int i = in.length();
    for(; i > 0; --i) {
      if ((new File(in.substring(0, i))).isDirectory())
        break;
    }
    ArrayList<String> split = new ArrayList<>();
    split.add(in.substring(0, i));
    if (i < in.length())
      split.add(in.substring(i));

    return split;
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
        // The selected file was not modified
        args.add(binFile);
        TFString = TFString.substring(binFile.length());
      } else {
        // Try to find a valid file
        ArrayList<String> split = splitOffLeadingFile(TFString);
        args.add(split.get(0));
        if (split.size() > 1)
          TFString = split.get(1);
        else
          TFString = "";
      }
      String[] tokens = TFString.split(" ");
      for (String s : tokens) {
        if (s.length() > 0)
          args.add(s);
      }

      return args;
    }

    // Java case

    // Add java executable
    String separator = System.getProperty("file.separator");
    String path = System.getProperty("java.home")
      + separator + "bin" + separator + "java";

    if (!(new File(path)).isFile() && (new File(path + ".exe")).isFile())
      path += ".exe";

    if (!(new File(path)).isFile()) {
      // Had trouble detecting java executable
      JOptionPane.showMessageDialog(
          null, "Attemped to find java executable as:\n" + path +
                    "\nHowever it was not found",
          "Unable to find Java executable", JOptionPane.ERROR_MESSAGE);
    }
    args.add(path);

    // Handle rest
    String rawTF = javaTF.getText();

    if (rawTF.startsWith("-classpath " + javaSelected)) {
      // Either a .class or a nonexecutable jar
      args.add("-classpath");
      args.add(javaSelected);
      rawTF = rawTF.substring(("-classpath " + javaSelected).length());
    } else if (javaJarExecutable && rawTF.startsWith("-jar " + javaSelected)) {
      args.add("-jar");
      args.add(javaSelected);
      rawTF = rawTF.substring(("-jar " + javaSelected).length());
    } else if (javaJar && !javaJarExecutable && rawTF.startsWith("-classpath ")) {
      args.add("-classpath");
      rawTF = rawTF.substring(("-classpath").length()).trim();
      ArrayList<String> split = splitOffLeadingFile(rawTF);
      args.add(split.get(0));
      if (split.size() > 1)
        rawTF = split.get(1);
      else
        rawTF = "";
    } else if (rawTF.startsWith("-classpath ")) {
      args.add("-classpath");
      rawTF = rawTF.substring(("-classpath").length()).trim();
      ArrayList<String> split = splitOffLeadingDirectory(rawTF);
      args.add(split.get(0));
      if (split.size() > 1)
        rawTF = split.get(1);
      else
        rawTF = "";
    } else if (rawTF.startsWith("-jar ")) {
      args.add("-jar");
      rawTF = rawTF.substring(("-jar").length()).trim();
      ArrayList<String> split = splitOffLeadingFile(rawTF);
      args.add(split.get(0));
      if (split.size() > 1)
        rawTF = split.get(1);
      else
        rawTF = "";
    }

    // Split remainder
    String[] tokens = rawTF.split(" ");
    for (String s : tokens) {
      if (s.length() > 0)
        args.add(s);
    }

    return args;
  }

  public void setEnable(boolean enabled) {
    binTF.setEditable(enabled);
    binButton.setEnabled(enabled);
    javaTF.setEditable(enabled);
    javaButton.setEnabled(enabled);
    humanTF.setEditable(enabled);
    mode.setEnabled(enabled);
  }

  public void swap(ProgramSelectorPanel other) {
    // Swap contents of all private variables

    // Binary
    String tempString = binTF.getText();
    binTF.setText(other.binTF.getText());
    other.binTF.setText(tempString);

    tempString = binFile;
    binFile = other.binFile;
    other.binFile = tempString;

    // Java
    tempString = javaTF.getText();
    javaTF.setText(other.javaTF.getText());
    other.javaTF.setText(tempString);

    tempString = javaSelected;
    javaSelected = other.javaSelected;
    other.javaSelected = tempString;

    boolean tempBool = javaJar;
    javaJar = other.javaJar;
    other.javaJar = tempBool;

    tempBool = javaJarExecutable;
    javaJarExecutable = other.javaJarExecutable;
    other.javaJarExecutable = tempBool;

    // Human
    tempString = humanTF.getText();
    humanTF.setText(other.humanTF.getText());
    other.humanTF.setText(tempString);

    // GUI Status
    int tempInt = mode.getSelectedIndex();
    mode.setSelectedIndex(other.mode.getSelectedIndex());
    other.mode.setSelectedIndex(tempInt);

    tempBool = binTF.isEditable();
    setEnable(other.binTF.isEditable());
    other.setEnable(tempBool);
  }

  final static String BINARYPANEL = "Binary";
  final static String JAVAPANEL = "Java";
  final static String HUMANPANEL = "Human";

  private JTextField binTF = new JTextField();
  private JButton binButton = new JButton("Browse");
  private String binFile = "";

  private JTextField javaTF = new JTextField();
  private JButton javaButton = new JButton("Browse");
  private String javaSelected = "";
  private boolean javaJar = false;
  private boolean javaJarExecutable = false;

  private JTextField humanTF = new JTextField();

  private JPanel cards = new JPanel(new CardLayout());

  private String currentCard;
  private JComboBox<String> mode = new JComboBox<String>();
}
