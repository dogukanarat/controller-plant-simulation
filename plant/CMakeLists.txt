set(PROJECTNAME ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${PROJECTNAME}_plant ${SOURCES})

target_link_libraries(
    ${PROJECTNAME}_plant LINK_PUBLIC ${PROJECTNAME}_lib_shared_memory
    )
