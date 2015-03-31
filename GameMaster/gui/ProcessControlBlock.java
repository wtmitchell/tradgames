import java.util.ArrayList;

public class ProcessControlBlock {
  public ProcessControlBlock() {
    // No commandline means this program suffers from Humanitis
    isHuman = true;
  }
  public ProcessControlBlock(String commandline, boolean broadcast,
                             UpdateMsgHook stdout, UpdateMsgHook stderr) {
    isHuman = false;
    this.commandline = commandline;
    for (String s : commandline.split(" "))
      args.add(s);
    this.broadcast = broadcast;
    this.stdout = stdout;
    this.stderr = stderr;
  }

  public String toString() {
    return commandline;
  }

  public boolean isHuman;
  private String commandline;
  public ArrayList<String> args = new ArrayList<String>();
  public boolean broadcast;
  public UpdateMsgHook stdout;
  public UpdateMsgHook stderr;
}
