cmake_minimum_required( VERSION 3.0 )

project( xm LANGUAGES C VERSION 0.1.0 )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

enable_testing( )

set (CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -pedantic -g -O0 --coverage" )
set (CMAKE_C_FLAGS "-Werror -Wall -Wextra -pedantic -g -O0 --coverage" )
set (CMAKE_EXE_LINKER_FLAGS "--coverage" )

add_subdirectory( ${PROJECT_SOURCE_DIR}/src )
add_subdirectory( ${PROJECT_SOURCE_DIR}/tests )
add_subdirectory( ${PROJECT_SOURCE_DIR}/examples )
