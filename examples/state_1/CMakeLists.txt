set( EXAMPLE_TARGET state_1 )

set ( XM_TARGET xm_state_1 )
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
    ${CMAKE_CURRENT_SOURCE_DIR}/state_1.c
)
target_link_libraries( ${EXAMPLE_TARGET} PUBLIC ${XM_TARGET})
target_compile_options( ${EXAMPLE_TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
