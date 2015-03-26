//===------------------------------------------------------------*- C++ -*-===//
#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED

#include <string>
#include <vector>

#include "ChineseCheckersState.h"

class Agent {
public:
  Agent();
  void playGame();

private:
  Move nextMove();
  void print_and_recv_echo(const std::string &msg) const;
  std::string readMsg() const;
  std::vector<std::string> tokenizeMsg(const std::string &msg) const;
  void waitForStart();
  void switchCurrentPlayer();

  bool isValidStartGameMessage(const std::vector<std::string> &tokens) const;
  bool isValidMoveMessage(const std::vector<std::string> &tokens) const;


  ChineseCheckersState state;
  enum Players { player1, player2 };
  Players current_player;
  Players my_player;
  std::string name;
  std::string opp_name;
};


#endif
