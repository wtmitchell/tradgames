# AgentTester

AgentTester is a Java Swing GUI to make debugging you agent easier.

It requires Java 8 to run.

To use:
1. Select the type agent you have from the combo box in the upper left corner. Use
   "Binary" if you are using C++, and "Java" if you are using Java.
2. Press the Browse and navigate to your executable, or main class (Main.class if
   you used the starting Java code).
3. Press the "Start" button. This will launch a new instance of your agent.

At this point you can send commands by typing in the textfield at the bottom of the
window and either pressing enter or the "Send" button.

If you wish to view the state of the board, press the "Refresh Board" button. This
will send the DUMPSTATE and LISTMOVES commands to your agent and graphically show
the results on the gameboard.

When there is a board displayed, hovering over the pieces of the player whose turn
it is will highlight the moves reported by the LISTMOVES command. If you click on
the source piece and then its destination, the input box at the bottom of the screen
will be populated by the corresponding MOVE command for that move.

Finally, when AgentTester launches an agent, it does this in a new process each time
(and separate VM for Java). Because of this, you should be able to leave AgentTester
open, recompile in your IDE, then stop and restart the agent in AgentTester and get
the new version.

If there are any problems running this program, contact Will.
