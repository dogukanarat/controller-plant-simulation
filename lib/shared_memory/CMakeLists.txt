
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(
    ${CMAKE_PROJECT_NAME}_lib_shared_memory ${SOURCES}
    )

target_include_directories(
    ${CMAKE_PROJECT_NAME}_lib_shared_memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    )