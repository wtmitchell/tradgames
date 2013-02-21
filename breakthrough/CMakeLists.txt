project(BREAKTHROUGH)
cmake_minimum_required(VERSION 2.8)

# Set up the compiler flags used
if("${CMAKE_C_COMPILER}" MATCHES "gcc")
  # gcc flags
  set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion -Wfloat-equal -Wredundant-decls -Wshadow -Wwrite-strings -Wno-long-long")

  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else()
  if("${CMAKE_C_COMPILER}" MATCHES "clang")
    # clang flags
    #set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "-std=c++11 -Weverything")

    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  else()
    # Generic flags
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  endif()
endif()

# Catch most of the source files
file(GLOB_RECURSE game . game/*.cpp)
set(shared Client.cpp Math.cpp Timer.cpp)

add_executable(breakthrough_master ${game} ${shared} Server.cpp ServerMain.cpp)
add_executable(breakthrough_random ${game} ${shared} RandomPlayer.cpp)

# The random statistics program uses Boost to do calculations
# Build it only if we find Boost
find_package(Boost)

if(Boost_FOUND)
  add_executable(breakthrough_statistics ${game} ${shared} RandomStatistics.cpp)
endif()
