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
  // in general
  expected.push_back({2, 20});
  expected.push_back({2, 4});
  expected.push_back({3, 12});
  expected.push_back({3, 4});
  expected.push_back({10, 28});
  expected.push_back({10, 12});
  expected.push_back({11, 20});
  expected.push_back({11, 12});
  expected.push_back({18, 36});
  expected.push_back({18, 20});
  expected.push_back({19, 28});
  expected.push_back({19, 20});
  expected.push_back({27, 36});
  expected.push_back({27, 28});

  s.getMoves(moves);

  EXPECT_EQ(expected.size(), moves.size());
  for (size_t i = 0, e = expected.size(); i != e; ++i)
    EXPECT_EQ(expected[i], moves[i]) << "i = " << i;
}
