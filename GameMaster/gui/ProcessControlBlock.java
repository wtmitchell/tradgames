import java.util.ArrayList;

public class ProcessControlBlock {
  public ProcessControlBlock(String commandline, boolean broadcast, UpdateHook stdout, UpdateHook stderr) {
    for (String s : commandline.split(" "))
      args.add(s);
    this.broadcast = broadcast;
    this.stdout = stdout;
    this.stderr = stderr;
  }
  public ArrayList<String> args = new ArrayList<String>();
  public boolean broadcast;
  public UpdateHook stdout;
  public UpdateHook stderr;
}
