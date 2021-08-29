file(
    GLOB
    SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/xm/xm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xm/xm_event.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xm/xm_state.c
)

set(
    XM_SRC_FILES
    ${SRC_FILES}
    PARENT_SCOPE
)

set(
    XM_INC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)

set(
    XM_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)

function( build_xm TARGET )
    add_library(
        ${TARGET}
        STATIC
        ${XM_SRC_FILES}
    )

    target_include_directories(
        ${TARGET}
        PUBLIC
        ${ARGV}
        ${XM_INC_DIRS}
    )

    target_compile_options(
        ${TARGET}
        PRIVATE
        -Werror -Wall -Wextra  -g -O0
    )
endfunction()
