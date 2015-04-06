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
  for (auto i : std::array<ChineseCheckers::Move, 14>{{{2, 4},
                                                       {2, 20},
                                                       {3, 4},
                                                       {3, 12},
                                                       {10, 12},
                                                       {10, 28},
                                                       {11, 12},
                                                       {11, 20},
                                                       {18, 20},
                                                       {18, 36},
                                                       {19, 20},
                                                       {19, 28},
                                                       {27, 28},
                                                       {27, 36}}})
    expected.push_back(i);

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

  for (auto i : std::array<ChineseCheckers::Move, 2>{{{0, 2}, {0, 18}}})
    expected.push_back(i);

  s.getMoves(moves);
  EXPECT_EQ(expected, moves);

  // Another
  EXPECT_TRUE(s.loadState("1 "
                          "1 1 0 1 0 2 0 1 0 "
                          "1 1 1 1 1 1 2 2 2 "
                          "0 2 0 0 0 0 0 0 0 "
                          "2 2 0 2 0 0 0 0 0 "
                          "0 0 2 0 0 0 0 0 0 "
                          "0 0 2 1 0 2 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0"));
  expected.clear();

  for (auto i : std::array<ChineseCheckers::Move, 42>{{{0, 2},
                                                       {0, 4},
                                                       {0, 6},
                                                       {0, 8},
                                                       {0, 18},
                                                       {0, 20},
                                                       {0, 22},
                                                       {0, 24},
                                                       {0, 26},
                                                       {0, 36},
                                                       {1, 2},
                                                       {3, 2},
                                                       {3, 4},
                                                       {3, 21},
                                                       {3, 37},
                                                       {3, 39},
                                                       {3, 55},
                                                       {3, 57},
                                                       {7, 6},
                                                       {7, 8},
                                                       {7, 23},
                                                       {7, 25},
                                                       {9, 18},
                                                       {10, 2},
                                                       {10, 18},
                                                       {11, 2},
                                                       {11, 20},
                                                       {12, 4},
                                                       {12, 20},
                                                       {12, 21},
                                                       {13, 4},
                                                       {13, 21},
                                                       {13, 22},
                                                       {14, 6},
                                                       {14, 22},
                                                       {14, 23},
                                                       {48, 39},
                                                       {48, 40},
                                                       {48, 46},
                                                       {48, 49},
                                                       {48, 56},
                                                       {48, 57}}})
    expected.push_back(i);

  s.getMoves(moves);
  EXPECT_EQ(expected, moves);

  // Another
  EXPECT_TRUE(s.loadState("1 "
                          "1 0 0 1 0 1 0 0 0 "
                          "0 1 1 0 0 0 0 0 0 "
                          "1 0 1 1 0 0 0 0 0 "
                          "1 0 0 0 0 0 0 0 0 "
                          "0 1 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 2 2 "
                          "0 0 0 0 0 2 2 2 2 "
                          "0 0 0 0 0 2 2 2 2"));

  expected.clear();
  for (auto i : std::array<ChineseCheckers::Move, 58>{{{0, 1},
                                                       {0, 9},
                                                       {3, 1},
                                                       {3, 2},
                                                       {3, 4},
                                                       {3, 12},
                                                       {3, 19},
                                                       {5, 4},
                                                       {5, 6},
                                                       {5, 13},
                                                       {5, 14},
                                                       {10, 1},
                                                       {10, 2},
                                                       {10, 9},
                                                       {10, 12},
                                                       {10, 19},
                                                       {10, 28},
                                                       {10, 30},
                                                       {10, 46},
                                                       {11, 2},
                                                       {11, 9},
                                                       {11, 12},
                                                       {11, 13},
                                                       {11, 19},
                                                       {11, 29},
                                                       {11, 45},
                                                       {18, 2},
                                                       {18, 4},
                                                       {18, 6},
                                                       {18, 9},
                                                       {18, 19},
                                                       {18, 36},
                                                       {18, 38},
                                                       {20, 2},
                                                       {20, 4},
                                                       {20, 6},
                                                       {20, 12},
                                                       {20, 19},
                                                       {20, 22},
                                                       {20, 28},
                                                       {20, 29},
                                                       {21, 1},
                                                       {21, 12},
                                                       {21, 13},
                                                       {21, 19},
                                                       {21, 22},
                                                       {21, 29},
                                                       {21, 30},
                                                       {27, 9},
                                                       {27, 19},
                                                       {27, 28},
                                                       {27, 36},
                                                       {37, 28},
                                                       {37, 29},
                                                       {37, 36},
                                                       {37, 38},
                                                       {37, 45},
                                                       {37, 46}}})
    expected.push_back(i);

  s.getMoves(moves);
  EXPECT_EQ(expected, moves);

  // Another
  EXPECT_TRUE(s.loadState("2 "
                          "0 1 0 0 0 0 0 1 1 "
                          "1 0 0 0 0 0 0 0 2 "
                          "0 0 1 0 0 0 0 0 2 "
                          "0 0 1 0 0 0 0 0 0 "
                          "0 1 2 2 0 0 0 0 0 "
                          "1 0 2 1 0 2 0 0 0 "
                          "1 0 0 0 0 0 0 0 0 "
                          "0 0 0 0 0 0 0 0 2 "
                          "0 0 0 0 0 2 2 2 0"));

  expected.clear();
  for (auto i : std::array<ChineseCheckers::Move, 41>{{{17, 16},
                                                       {17, 25},
                                                       {17, 35},
                                                       {26, 25},
                                                       {26, 34},
                                                       {26, 35},
                                                       {38, 30},
                                                       {38, 36},
                                                       {38, 40},
                                                       {38, 46},
                                                       {38, 56},
                                                       {39, 30},
                                                       {39, 31},
                                                       {39, 40},
                                                       {39, 55},
                                                       {39, 57},
                                                       {47, 31},
                                                       {47, 46},
                                                       {47, 49},
                                                       {47, 51},
                                                       {47, 55},
                                                       {47, 56},
                                                       {50, 41},
                                                       {50, 42},
                                                       {50, 49},
                                                       {50, 51},
                                                       {50, 58},
                                                       {50, 59},
                                                       {71, 62},
                                                       {71, 70},
                                                       {71, 80},
                                                       {77, 68},
                                                       {77, 69},
                                                       {77, 76},
                                                       {78, 62},
                                                       {78, 69},
                                                       {78, 70},
                                                       {78, 76},
                                                       {78, 80},
                                                       {79, 70},
                                                       {79, 80}}})
    expected.push_back(i);

  s.getMoves(moves);
  EXPECT_EQ(expected, moves);

  // Pathological case with many duplicate potentials
  EXPECT_TRUE(s.loadState("1 "
                          "1 1 0 1 0 1 0 1 0 "
                          "1 1 1 1 1 1 1 1 1 "
                          "0 1 0 1 0 1 0 1 0 "
                          "1 1 1 1 1 1 1 1 1 "
                          "0 1 0 1 0 1 0 1 0 "
                          "1 1 1 1 1 1 1 1 1 "
                          "0 1 0 1 0 1 0 1 0 "
                          "1 1 1 1 1 1 1 1 1 "
                          "0 1 0 1 0 1 0 1 1"));

  expected.clear();
  for (auto i : std::array<ChineseCheckers::Move, 154>{{{0, 2},
                                                        {0, 4},
                                                        {0, 6},
                                                        {0, 8},
                                                        {0, 18},
                                                        {0, 20},
                                                        {0, 22},
                                                        {0, 24},
                                                        {0, 26},
                                                        {0, 36},
                                                        {0, 38},
                                                        {0, 40},
                                                        {0, 42},
                                                        {0, 44},
                                                        {0, 54},
                                                        {0, 56},
                                                        {0, 58},
                                                        {0, 60},
                                                        {0, 62},
                                                        {0, 72},
                                                        {0, 74},
                                                        {0, 76},
                                                        {0, 78},
                                                        {1, 2},
                                                        {3, 2},
                                                        {3, 4},
                                                        {5, 4},
                                                        {5, 6},
                                                        {7, 6},
                                                        {7, 8},
                                                        {9, 18},
                                                        {10, 2},
                                                        {10, 18},
                                                        {11, 2},
                                                        {11, 20},
                                                        {12, 4},
                                                        {12, 20},
                                                        {13, 4},
                                                        {13, 22},
                                                        {14, 6},
                                                        {14, 22},
                                                        {15, 6},
                                                        {15, 24},
                                                        {16, 8},
                                                        {16, 24},
                                                        {17, 8},
                                                        {17, 26},
                                                        {19, 18},
                                                        {19, 20},
                                                        {21, 20},
                                                        {21, 22},
                                                        {23, 22},
                                                        {23, 24},
                                                        {25, 24},
                                                        {25, 26},
                                                        {27, 18},
                                                        {27, 36},
                                                        {28, 20},
                                                        {28, 36},
                                                        {29, 20},
                                                        {29, 38},
                                                        {30, 22},
                                                        {30, 38},
                                                        {31, 22},
                                                        {31, 40},
                                                        {32, 24},
                                                        {32, 40},
                                                        {33, 24},
                                                        {33, 42},
                                                        {34, 26},
                                                        {34, 42},
                                                        {35, 26},
                                                        {35, 44},
                                                        {37, 36},
                                                        {37, 38},
                                                        {39, 38},
                                                        {39, 40},
                                                        {41, 40},
                                                        {41, 42},
                                                        {43, 42},
                                                        {43, 44},
                                                        {45, 36},
                                                        {45, 54},
                                                        {46, 38},
                                                        {46, 54},
                                                        {47, 38},
                                                        {47, 56},
                                                        {48, 40},
                                                        {48, 56},
                                                        {49, 40},
                                                        {49, 58},
                                                        {50, 42},
                                                        {50, 58},
                                                        {51, 42},
                                                        {51, 60},
                                                        {52, 44},
                                                        {52, 60},
                                                        {53, 44},
                                                        {53, 62},
                                                        {55, 54},
                                                        {55, 56},
                                                        {57, 56},
                                                        {57, 58},
                                                        {59, 58},
                                                        {59, 60},
                                                        {61, 60},
                                                        {61, 62},
                                                        {63, 54},
                                                        {63, 72},
                                                        {64, 56},
                                                        {64, 72},
                                                        {65, 56},
                                                        {65, 74},
                                                        {66, 58},
                                                        {66, 74},
                                                        {67, 58},
                                                        {67, 76},
                                                        {68, 60},
                                                        {68, 76},
                                                        {69, 60},
                                                        {69, 78},
                                                        {70, 62},
                                                        {70, 78},
                                                        {71, 62},
                                                        {73, 72},
                                                        {73, 74},
                                                        {75, 74},
                                                        {75, 76},
                                                        {77, 76},
                                                        {77, 78},
                                                        {79, 78},
                                                        {80, 2},
                                                        {80, 4},
                                                        {80, 6},
                                                        {80, 8},
                                                        {80, 18},
                                                        {80, 20},
                                                        {80, 22},
                                                        {80, 24},
                                                        {80, 26},
                                                        {80, 36},
                                                        {80, 38},
                                                        {80, 40},
                                                        {80, 42},
                                                        {80, 44},
                                                        {80, 54},
                                                        {80, 56},
                                                        {80, 58},
                                                        {80, 60},
                                                        {80, 62},
                                                        {80, 72},
                                                        {80, 74},
                                                        {80, 76},
                                                        {80, 78}}})
    expected.push_back(i);

  s.getMoves(moves);
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

