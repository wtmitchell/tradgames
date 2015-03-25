#include <gtest/gtest.h>

#include <cstddef>
#include <string>

#include "ChineseCheckers/State.h"

TEST(State, CtorDump) {
  ChineseCheckers::State s;

  std::string state = s.dumpState();
  EXPECT_EQ("1 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
            "0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2",
            state);
}

TEST(State, getMoves) {
  ChineseCheckers::State s;

  std::vector<ChineseCheckers::Move> moves;
  std::vector<ChineseCheckers::Move> expected;

  // This assumes the order of returned moves which is not a safe assumption
  // in general. We know the order in this specific case is lexicographic
  expected.push_back({2, 4});
  expected.push_back({2, 20});
  expected.push_back({3, 4});
  expected.push_back({3, 12});
  expected.push_back({10, 12});
  expected.push_back({10, 28});
  expected.push_back({11, 12});
  expected.push_back({11, 20});
  expected.push_back({18, 20});
  expected.push_back({18, 36});
  expected.push_back({19, 20});
  expected.push_back({19, 28});
  expected.push_back({27, 28});
  expected.push_back({27, 36});

  s.getMoves(moves);

  EXPECT_EQ(expected, moves);

  // Configuration that leads to duplicated moves arrived at by different paths
  EXPECT_TRUE(s.loadState("1 "
                          "1 2 0 0 0 0 0 0 0 "
                          "2 2 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0"));
  expected.clear();

  s.getMoves(moves);
  expected.push_back({0, 2});
  expected.push_back({0, 18});

  EXPECT_EQ(expected, moves);
}

void DepthLimitedDFS(ChineseCheckers::State &s, int depth);

void DepthLimitedDFS(ChineseCheckers::State &s, int depth) {
  if (depth == 0)
    return;

  std::vector<ChineseCheckers::Move> moves;
  s.getMoves(moves);

  for (const auto m : moves) {
    EXPECT_TRUE(s.applyMove(m)) << "depth = " << depth << " move = " << m
                                << " state = " << s.dumpState();
    DepthLimitedDFS(s, depth - 1);
    EXPECT_TRUE(s.undoMove(m)) << "depth = " << depth << " move = " << m
                               << " state = " << s.dumpState();
  }
}

TEST(State, DepthLimitedDFS3) {
  ChineseCheckers::State s;

  std::string original = s.dumpState();

  DepthLimitedDFS(s, 3);

  EXPECT_EQ(original, s.dumpState());
}

TEST(State, LoadDumpState) {
  ChineseCheckers::State s;

  auto starting = s.dumpState();

  EXPECT_TRUE(s.loadState(starting));
  EXPECT_EQ(starting, s.dumpState());

  // Invalid player
  std::string invalid1 =
      "0 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
      "0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2";
  EXPECT_FALSE(s.loadState(invalid1));

  // Invalid player
  std::string invalid2 =
      "3 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
      "0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2";
  EXPECT_FALSE(s.loadState(invalid2));

  // Too few entries
  std::string invalid3 =
      "1 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
      "0 0 0 0 2 2 2 0 0 0 0 0 2 2 2";
  EXPECT_FALSE(s.loadState(invalid3));

  // Too many entries
  std::string invalid4 =
      "1 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
      "0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2 0";
  EXPECT_FALSE(s.loadState(invalid4));

  s.reset();
  EXPECT_EQ(starting, s.dumpState());
}

TEST(State, GameOver) {
  ChineseCheckers::State s;

  EXPECT_FALSE(s.gameOver());

  std::string p1wins =
      "1 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
      "0 0 0 0 2 2 2 0 0 0 0 0 2 1 2 2";

  EXPECT_TRUE(s.loadState(p1wins));
  EXPECT_TRUE(s.gameOver());
  EXPECT_EQ(1, s.winner());

  std::string p2wins =
      "1 2 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1 0 0 0 0 "
      "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 2 2 0 0 "
      "0 0 0 0 2 2 2 0 0 0 0 0 2 2 2 2";

  EXPECT_TRUE(s.loadState(p2wins));
  EXPECT_TRUE(s.gameOver());
  EXPECT_EQ(2, s.winner());
}
