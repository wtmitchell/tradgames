#include <gtest/gtest.h>

#include <string>

#include "Common/String.h"

TEST(String, ltrim) {
  std::string orig = "  x x x  ";
  std::string expected = "x x x  ";

  EXPECT_EQ(expected, Common::ltrim(orig));
}

TEST(String, rtrim) {
  std::string orig = "  x x x  ";
  std::string expected = "  x x x";

  EXPECT_EQ(expected, Common::rtrim(orig));
}

TEST(String, trim) {
  std::string orig = "  x x x  ";
  std::string expected = "x x x";

  EXPECT_EQ(expected, Common::trim(orig));
}

TEST(String, split) {
  std::string src = "a b c d e f g h";
  std::vector<std::string> expected = {"a", "b", "c", "d", "e", "f", "g", "h"};

  std::vector<std::string> tokenized = Common::split(src);

  EXPECT_EQ(expected.size(), tokenized.size());
  for (size_t i = 0, e = expected.size(); i != e; ++i)
    EXPECT_EQ(expected[i], tokenized[i]) << "i = " << i;
}
