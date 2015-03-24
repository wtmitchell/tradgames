# Build the googletest library
include_directories(googletest/include)

set(googletest_src
  googletest/gtest.cc
  googletest/gtest-death-test.cc
  googletest/gtest-filepath.cc
  googletest/gtest-port.cc
  googletest/gtest-printers.cc
  googletest/gtest-test-part.cc
  googletest/gtest-typed-test.cc
  )

# The gtest library will always get the same compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CXX_VERSION} -O3 -Wno-variadic-macros")
else()
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CXX_VERSION} -O3")
    if(USE_LIBCXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
  endif()
endif()

add_library(gtest_main ${googletest_src} googletest/gtest_main.cc)
