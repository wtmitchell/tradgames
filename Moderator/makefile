CXX = clang++
CFLAGS = -O3 -std=c++11

default: ChineseCheckersModerator ChineseCheckersRandom

ChineseCheckersModerator: apps/ChineseCheckersModerator/main.cpp lib/Common/Client.cpp lib/Common/Timer.cpp lib/ChineseCheckers/Client.cpp lib/ChineseCheckers/State.cpp
	$(CXX) $(CFLAGS) -o ChineseCheckersModerator -I include apps/ChineseCheckersModerator/main.cpp lib/Common/Client.cpp lib/Common/Timer.cpp lib/ChineseCheckers/Client.cpp lib/ChineseCheckers/State.cpp

ChineseCheckersRandom: apps/ChineseCheckersRandom/main.cpp lib/Common/Client.cpp lib/Common/Timer.cpp lib/ChineseCheckers/Client.cpp lib/ChineseCheckers/State.cpp
	$(CXX) $(CFLAGS) -o ChineseCheckersRandom -I include apps/ChineseCheckersRandom/main.cpp lib/Common/Client.cpp lib/Common/Timer.cpp lib/ChineseCheckers/Client.cpp lib/ChineseCheckers/State.cpp

