# Specify minimum required version of CMake.
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

option(ENABLE_COVERAGE "Provide Coverage via lcov" OFF)
set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Specify project and identify languages used.
project(random LANGUAGES CXX)

include(Sanitizers.cmake)

# Add program target called hello.

if (ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING 
	  "Set the build type." FORCE)
  include(CodeCoverage.cmake)
  APPEND_COVERAGE_COMPILER_FLAGS(-00)
  set(EXECUTABLE_OUTPUT_PATH "coverage")
  # SETUP_TARGET_FOR_COVERAGE_LCOV(NAME coverage EXECUTABLE run_tests coverage DEPENDENCIES random)
  # APPEND_COVERAGE_COMPILER_FLAGS(-00)
  # add_custom_target(coverage run_tests
  #  COMMAND "ctest -VV" )

endif()
# find_package(Catch2 REQUIRED)
# target_link_libraries(tests Catch2::Catch2)
include_directories(include app)
add_executable(test_random app/test_random.cpp lib/random.cpp include/ra/random.hpp)
add_executable(test_rational app/test_rational.cpp include/ra/rational.hpp)
target_compile_features(test_rational PRIVATE cxx_std_17)

# target_link_libraries(test_rational Catch2::Catch2)
IF (ENABLE_COVERAGE)
	setup_target_for_coverage_lcov(
		NAME coverage
		EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/run_tests coverage
		LCOV_ARGS -rc lcov_branch_coverage=1
		GENHTML_ARGS --legend --branch-coverage
		DEPENDENCIES test_random test_rational)
    file(COPY run_tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()