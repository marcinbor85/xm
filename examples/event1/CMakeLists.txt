set( EXAMPLE_TARGET event1 )

set ( XM_TARGET xm_example )
build_xm ( ${XM_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(
    ${XM_TARGET}
    PUBLIC
    XM_CONFIG_PLATFORM_POSIX
)

add_executable(
    ${EXAMPLE_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/event1.c
)
target_link_libraries( ${EXAMPLE_TARGET} PUBLIC ${XM_TARGET})
target_compile_options( ${EXAMPLE_TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
