import java.util.ArrayList;

public class ProcessControlBlock {
  public ProcessControlBlock() {
    // No commandline means this program suffers from Humanitis
    isHuman = true;
  }
  public ProcessControlBlock(ArrayList<String> args, boolean broadcast,
                             UpdateMsgHook stdout, UpdateMsgHook stderr) {
    isHuman = false;
    this.args = args;
    this.broadcast = broadcast;
    this.stdout = stdout;
    this.stderr = stderr;
  }

  public String toString() {
    StringBuilder sb = new StringBuilder();
    for (String s : args)
      sb.append(s);
    return sb.toString();
  }

  public boolean isHuman;
  public ArrayList<String> args;
  public boolean broadcast;
  public UpdateMsgHook stdout;
  public UpdateMsgHook stderr;
}
