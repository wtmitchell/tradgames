import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.nio.file.NoSuchFileException;
import java.nio.file.Path;
import java.nio.file.Paths;
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
                } catch (NoSuchFileException e) {
                    System.err.println("Response file does not exist.");
                    System.err.println(e);
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
                          .isRequired()
                          .create("1"));
        options.addOption(OptionBuilder.withLongOpt("player2")
                          .withDescription("Command to run player 2")
                          .hasArg()
                          .withArgName("CMD")
                          .isRequired()
                          .create("2"));
        options.addOption(OptionBuilder.withLongOpt("master")
                          .withDescription("Command to run master which administers the game")
                          .hasArg()
                          .withArgName("CMD")
                          .isRequired()
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
        programs.add(cmd.getOptionValue("master"));
        programs.add(cmd.getOptionValue("player1"));
        programs.add(cmd.getOptionValue("player2"));

        GameInstance gi = new GameInstance(programs);
        gi.start();
    }

    private static List<String> readResponseFile(String filename)
        throws InvalidPathException, IOException {
        Path filePath = Paths.get(filename);
        return Files.readAllLines(filePath, Charset.defaultCharset());
    }
}

