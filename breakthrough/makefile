CXXFLAGS=-O3 -std=c++11 -stdlib=libc++
# Linux users, remove '-stdlib=libc++' in the above line if you get an error

GAMESTATE = game/GameState.o game/Move.o
CLIENTSHARED = Client.o Math.o Timer.o
CLIENTRANDOM = RandomPlayer.o $(CLIENTSHARED) $(GAMESTATE)
CLIENTMASTER = ServerMain.o Server.o $(CLIENTSHARED) $(GAMESTATE)

.PHONY: all
all: breakthrough_master breakthrough_random

breakthrough_random: $(CLIENTRANDOM)
	$(CXX) -o breakthrough_random $(CLIENTRANDOM) $(CXXFLAGS)

breakthrough_master: $(CLIENTMASTER)
	$(CXX) -o breakthrough_master $(CLIENTMASTER) $(CXXFLAGS)

game/GameState.o: game/GameState.h game/Move.h game/Piece.h game/Players.h
game/Move.o: game/Move.h
Client.o: Client.h String.h
Math.o: Math.h
RandomPlayer.o: game/GameState.h game/Move.h Client.h Math.h game/Players.h
Server.o: Server.h Client.h game/GameState.h
ServerMain.o: Server.h
Timer.o:

.PHONY: clean
clean:
	rm -f breakthrough_master breakthrough_random $(GAMESTATE) $(CLIENTSHARED) $(CLIENTRANDOM) $(CLIENTMASTER)

