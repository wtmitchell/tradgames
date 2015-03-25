include_directories(${PROJECT_SOURCE_DIR}/include)

set(ChineseCheckersRandomSources
  main.cpp
  )

add_executable(ChineseCheckersRandom
  ${ChineseCheckersRandomSources})
target_link_libraries(ChineseCheckersRandom
  Common
  ChineseCheckers
  )

