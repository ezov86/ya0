cmake_minimum_required(VERSION 3.22)
project(ya C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-g -std=c99 -pedantic-errors -DDEBUG")
# set(CMAKE_C_FLAGS "-O3 -m32 -std=c99")

add_executable(ya main.c log.c mem.c vec.c lex.c test.c)



# cmake_minimum_required(VERSION 3.5)
# project(ya C)

# # Set the compiler options
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
# set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# # Create OBJECT_DIR variable
# set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/ya.dir)
# message("-- Object files will be output to: ${OBJECT_DIR}")

# # Set the sources
# set(SOURCES main.c lexer.c buf.c log.c mem.c)

# # Create the executable
# add_executable(ya ${SOURCES})

# # Create the gcov target. Run coverage tests with 'make gcov'
# add_custom_target(gcov
#     COMMAND mkdir -p coverage
#     COMMAND ${CMAKE_MAKE_PROGRAM} test
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     )
# add_custom_command(TARGET gcov
#     COMMAND echo "=================== GCOV ===================="
#     COMMAND gcov -b ${CMAKE_SOURCE_DIR}/*.cpp -o ${OBJECT_DIR} > CoverageSummary.tmp
#     | grep -A 5 "main.c" > CoverageSummary.tmp
#     COMMAND cat CoverageSummary.tmp
#     COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/coverage"
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
#     )

# add_dependencies(gcov ya)
# # Make sure to clean up the coverage folder
# set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES coverage)

# # Create the gcov-clean target. This cleans the build as well as generated 
# # .gcda and .gcno files.
# add_custom_target(scrub
# COMMAND ${CMAKE_MAKE_PROGRAM} clean
# COMMAND rm -f ${OBJECT_DIR}/*.gcno
# COMMAND rm -f ${OBJECT_DIR}/*.gcda
# WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )

# # Testing
# enable_testing()

# add_test(output_test ${CMAKE_CURRENT_BINARY_DIR}/ya)
# # set_tests_properties(output_test PROPERTIES PASS_REGULAR_EXPRESSION "0;5;10")

############
# WORKS!!! #
############

# cmake_minimum_required(VERSION 3.5)
# project(ya C)

# # Set the compiler options
# set(CMAKE_C_STANDARD 99)
# SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
# set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
# set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)

# # Create OBJECT_DIR variable
# set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/ya.dir)
# message("-- Object files will be output to: ${OBJECT_DIR}")

# # Set the sources
# set(SOURCES main.c buf.c mem.c log.c)

# # Create the executable
# add_executable(ya ${SOURCES})

# # Create the gcov target. Run coverage tests with 'make gcov'
# add_custom_target(gcov
#     COMMAND mkdir -p coverage
#     COMMAND ${CMAKE_MAKE_PROGRAM} test
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     )
# add_custom_command(TARGET gcov
#     COMMAND echo "=================== GCOV ===================="
#     COMMAND gcov -b ${CMAKE_SOURCE_DIR}/*.c -o ${OBJECT_DIR}
#         | grep -A 5 "main.c" > CoverageSummary.tmp
#     COMMAND cat CoverageSummary.tmp
#     COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/coverage"
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage  # Need separate command for this line
#     )
# add_dependencies(gcov ya)
# # Make sure to clean up the coverage folder
# set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES coverage)

# # Create the gcov-clean target. This cleans the build as well as generated 
# # .gcda and .gcno files.
# add_custom_target(scrub
#     COMMAND ${CMAKE_MAKE_PROGRAM} clean
#     COMMAND rm -f ${OBJECT_DIR}/*.gcno
#     COMMAND rm -f ${OBJECT_DIR}/*.gcda
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     )

# # Testing
# enable_testing()

# add_test(output_test ${CMAKE_CURRENT_BINARY_DIR}/ya)
# set_tests_properties(output_test PROPERTIES PASS_REGULAR_EXPRESSION "0;5;10")
