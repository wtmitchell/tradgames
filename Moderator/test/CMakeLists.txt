include_directories(${PROJECT_SOURCE_DIR}/include)

# googletest uses pthreads
find_package(Threads)

# Function to collect a libraries tests together into a distinct
# googletest executable per library
function(add_unittest test_name)
  # Include as SYSTEM to ignore warnings in these files
  include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/util/googletest/include)

  add_executable(${test_name} ${ARGN})
  target_link_libraries(${test_name}
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT}
    ${TEST_LINK_COMPONENTS}
    )

  add_test(${test_name} ${CMAKE_BINARY_DIR}/${test_name})

  if (VALGRIND_FOUND)
    add_custom_target(memcheck-${test_name}
      COMMAND ${VALGRIND_MEMCHECK} ${CMAKE_BINARY_DIR}/${test_name})
    add_dependencies(memcheck-all memcheck-${test_name})
  endif (VALGRIND_FOUND)

endfunction()

# googletest triggers some warnings via macro includes, suppress them
if("${CMAKE_C_COMPILER}" MATCHES "clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors -Wno-missing-variable-declarations")
endif()

add_subdirectory(ChineseCheckers)
