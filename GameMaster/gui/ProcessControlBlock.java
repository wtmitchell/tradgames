import java.util.ArrayList;

public class ProcessControlBlock {
  public ProcessControlBlock(String commandline, boolean broadcast,
                             UpdateMsgHook stdout, UpdateMsgHook stderr) {
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
  private String commandline;
  public ArrayList<String> args = new ArrayList<String>();
  public boolean broadcast;
  public UpdateMsgHook stdout;
  public UpdateMsgHook stderr;
}
