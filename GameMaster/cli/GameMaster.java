import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;



import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;

public class GameMaster {

    public static void main(String[] args) {
        // Check for response file and load it
        ArrayList<String> arguments = new ArrayList<String>();

        for (String s : args) {
            if (s.charAt(0) == '@') {
                try {
                    arguments.addAll(readResponseFile(s.substring(1)));
                } catch (IOException e) {
                    System.err.println("IO Error reading response file");
                    System.err.println(e);
                }
            } else {
                arguments.add(s);
            }
        }

        // Process the command line arguments
        Options options = new Options();
        options.addOption("h", "help", false, "Shows this help message");
        options.addOption(OptionBuilder.withLongOpt("player1")
                          .withDescription("Command to run player 1")
                          .hasArg()
                          .withArgName("CMD")
                          .create("1"));
        options.addOption(OptionBuilder.withLongOpt("player2")
                          .withDescription("Command to run player 2")
                          .hasArg()
                          .withArgName("CMD")
                          .create("2"));
        options.addOption(OptionBuilder.withLongOpt("moderator")
                          .withDescription("Command to run moderator which administers the game")
                          .hasArg()
                          .withArgName("CMD")
                          .create("m"));

        boolean printUsage = false;
        CommandLine cmd = null;
        try {
            CommandLineParser parser = new PosixParser();
            cmd = parser.parse(options, arguments.toArray(new String[]{}));
        } catch (ParseException e) {
            printUsage = true;
            System.err.println("Error: " + e.getMessage() );
        }

        if (printUsage || cmd.hasOption("help")) {
            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp("GameMaster [OPTIONS] [@RESPONSE_FILE]", options);
            System.out.println("Options may be placed in a response file "
                               + "(text file with one option per line).");
            System.exit(1);
        }

        // Start things up
        ArrayList<String> programs = new ArrayList<String>();
        programs.add(cmd.getOptionValue("moderator"));
        programs.add(cmd.getOptionValue("player1"));
        programs.add(cmd.getOptionValue("player2"));

        GameInstance gi = new GameInstance(programs);
        gi.start();
    }

    private static List<String> readResponseFile(String filename)
        throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new FileReader(filename));
        List<String> lines = new ArrayList<String>();
        String line = null;
        while ((line = bufferedReader.readLine()) != null) {
            lines.add(line);
        }
        bufferedReader.close();
        return lines;
    }
}

