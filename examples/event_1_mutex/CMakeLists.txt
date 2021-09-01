set( EXAMPLE_TARGET event_1_mutex )

set ( XM_TARGET xm_event_1_mutex )
build_xm ( ${XM_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(
    ${XM_TARGET}
    PUBLIC
)

add_executable(
    ${EXAMPLE_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)
target_link_libraries( ${EXAMPLE_TARGET} PUBLIC ${XM_TARGET} pthread)
target_compile_options( ${EXAMPLE_TARGET} PRIVATE -Werror -Wall -Wextra  -g -O0)
