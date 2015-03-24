set(TEST_LINK_COMPONENTS
  ChineseCheckers
  )

set(ChineseCheckersSources
  State.cpp
  )

add_unittest(ChineseCheckers_tests
  ${ChineseCheckersSources}
  )
