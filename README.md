Code for AI for Traditional Games
=================================

There are two programs in this repository:

1. GameMaster - A Java program that controls the interactions between agents and a master running the game.

2. breakthrough - A C++ program that implements a server to play the game breakthrough.


GameMaster
----------
GameMaster is a Java programs that controls the interactions between agents and a master running the game.

The following commands are server commands:
<dl>
    <dt>#getname ID</dt>
    <dd>Returns the name of player with ID.</dd>
    <dt>#master</dt>
    <dd>Tells the server that the programs should receive all messages and sends broadcasts.</dd>
    <dt>#name NAME</dt>
    <dd>Declares the current agent to be named NAME. No spaces are allowed. The names server and observer are reserved.</dd>
    <dt>#players</dt>
    <dd>Returns the number of connected players.</dd>
    <dt>#quit</dt>
    <dd>Immediately terminates the game. This message is always broadcast to all clients.</dd>
</dl>

Any other command sent with a # prefix will be ignored and not sent on to the other clients.

breakthrough
------------
breakthrough is a C++ program that plays the game [breakthrough][btwiki]

During the game, the following commands will be issued:
<dl>
    <dt>BEGIN BREAKTHROUGH player1 player2</dt>
    <dd>Starts a game using with player1 starting first. The names player1 and player2 will be replaced with the names provided to the server with the #name command.</dd>
    <dt>FINAL winner BEATS loser</dt>
    <dd>Declares the game over with the winner and loser specified. As with BEGIN, the names declared by the #name command will be used in player of winner and loser.</dd>
</d>

During the game, there is only one valid command to send:
<dl>
    <dt>MOVE from_location TO to_location</dt>
    <dd>Moves the piece at from_location to to_location. Locations are specified with a letter and number, separated by a space. The letter designates the column and the number the row. The locations are the same as <a href="http://en.wikipedia.org/wiki/Algebraic_chess_notation">algebraic chess notation</a> except since all pieces are the same, no notation of the pieces are made. An example move is "MOVE a 2 TO a 3".</dd>
</dl>

At the start of the game, player1 has their pieces in rows 1 and 2, whereas player2 has their pieces in rows 7 and 8.

[btwiki]: http://en.wikipedia.org/wiki/Breakthrough_%28board_game%29

Your Program
------------
To make your program interact, it will communicate over stdin and stdout (System.in and System.out in Java). You may freely write to stderr (System.err) if you wish to see diagnostic information during game play.

See the source breakthrough/src/RandomPlayer.cpp to see a simple implementation of an agent that selects moves randomly.

Compiling Everything
--------------------
Since GameMaster is Java, a simple "javac GameMaster.java" in the appropriate directory is enough.

breakthrough is using cmake to configure the build. On Linux, run build.sh.

If you are using Visual Studio 2012, load the solution file breakthrough.sln and build the two projects contained within.

Running a Game
--------------
To start a game between two random players type the following:

    java GameMaster -1 'path/to/breakthrough_random player1' -2 'path/to/breakthrough_random player2' -m 'path/to/breakthrough'

Alternatively, the game setup can be stored in a response file like

    --player1=path/to/breakthrough_random player1
    --player2=path/to/breakthrough_random player2
    --master=path/to/breakthrough

It can then be loaded like:

    java GameMaster @response_file_name

A sample response file is included named Random.vs.Random.txt.

Running GameMaster with no arguments, or the argument --help, will display the full set of options that the program supports.

Sample Game Output
------------------
An example of what a game looks like between two random players may look like:

   $ java -cp GameMaster/ GameMaster @Random.vs.Random.txt
   Will start game run by: 'breakthrough/build/breakthrough'
   Player1: 'breakthrough/build/breakthrough_random Random1'
   Player2: 'breakthrough/build/breakthrough_random Random2'
   0:stdout all: #name server
   0:stdout all: #master
   0:stdout all: #players
   Received #players query from server; answer: 1
   1:stdout: #name Random1
   2:stdout: #name Random2
   0:stdout all: #players
   Received #players query from server; answer: 3
   0:stdout all: #getname 0
   Received #getname query from server; answer: server
   0:stdout all: #getname 1
   Received #getname query from server; answer: Random1
   0:stdout all: #getname 2
   Received #getname query from server; answer: Random2
   0:stdout all: BEGIN BREAKTHROUGH Random1 Random2
   1:stdout: MOVE f 2 TO e 3
   0:stderr: Turn by 1:Random1 took  0h  0m  0s  11ms
   0:stdout all: MOVE f 2 TO e 3
   2:stdout: MOVE f 7 TO e 6
   0:stderr: Turn by 2:Random2 took  0h  0m  0s  16ms
   0:stdout all: MOVE f 7 TO e 6
   1:stdout: MOVE c 2 TO c 3
   0:stderr: Turn by 1:Random1 took  0h  0m  0s  16ms
   0:stdout all: MOVE c 2 TO c 3
   2:stdout: MOVE d 7 TO d 6
   ...
   1:stdout: MOVE c 7 TO c 8
   0:stdout all: MOVE c 7 TO c 8
   0:stderr: Turn by 1:Random1 took  0h  0m  0s  16ms
   0:stdout all: FINAL Random1 BEATS Random2
   0:stdout all: #quit
   2:stderr: I, Random2, have lost
   $
