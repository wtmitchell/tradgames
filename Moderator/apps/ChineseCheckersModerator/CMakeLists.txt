include_directories(${PROJECT_SOURCE_DIR}/include)

set(ChineseCheckersModeratorSources
  main.cpp
  )

add_executable(ChineseCheckersModerator
  ${ChineseCheckersModeratorSources})
target_link_libraries(ChineseCheckersModerator
  Common
  ChineseCheckers
  )

