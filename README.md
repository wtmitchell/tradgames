Code for AI for Traditional Games taught by Nathan Sturtevant
==========================================

There are two programs in this repository:

1. GameMaster - A Java program that controls the interactions between agents and a master running the game.

2. breakthrough - A C++ program that implements a server to play the game breakthrough.


GameMaster
----------
GameMaster is a Java programs that controls the interactions between agents and a master running the game.

The following commands are server commands:
* #getname ID - Returns the name of player with ID.
* #master - Tells the server that the programs should receive all messages and sends broadcasts
* #name NAME - Declares the current agent to be named NAME. No spaces are allowed.
* #players - Returns the number of connected players.
* #quit - Immediately terminates the game. This message is always broadcast to all clients.

Any other command sent with a # prefix will be ignored and not sent on to the other clients.

breakthrough
------------
breakthrough is a C++ program that plays the game [breakthrough](http://en.wikipedia.org/wiki/Breakthrough_%28board_game%29).

During the game, the following commands will be issued:
* BEGIN BREAKTHROUGH player1 player2 - Starts a game using with player1 starting first. The names player1 and player2 will be replaced with the names provided to the server with the #name command.
* FINAL winner loser - Declares the game over with the winner and loser specified. As with BEGIN, the names declared by the #name command will be used in player of winner and loser.

During the game, there is only one valid command to send:
* MOVE from to - Moves pieces at location from to location to. Locations are specified as in chess with a letter and number, separated by a space. The letter designates the column and the number the row. The locations are the same as [algebraic chess notation](http://en.wikipedia.org/wiki/Algebraic_chess_notation) except since all pieces are the same, no notation of the pieces are made. An example move is "MOVE a 2 a 3".

At the start of the game, player1 has their pieces in rows 1 and 2, whereas player2 has their pieces in rows 7 and 8.

Your Program
------------
To make your program interact, it will communicate over stdin and stdout (System.in and System.out in Java). You may freely write to stderr (System.err) if you wish to see diagnostic information during game play.

See the source breakthrough/src/RandomPlayer.cpp to see a simple implementation of an agent that selects moves randomly.

Compiling Everything
-------------------
Since GameMaster is Java, a simple "javac GameMaster.java" in the appropriate directory is enough.

breakthrough is using cmake to configure the build.

Running a Game
--------------
To start a game between two random players type the following:

java GameMaster -1 'path/to/breakthrough_random player1' -2 'path/to/breakthrough_random player2' -m 'path/to/breakthrough'
