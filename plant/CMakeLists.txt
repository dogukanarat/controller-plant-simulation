set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_plant ${SOURCES})

#add_library(${BINARY}_plant_lib STATIC ${SOURCES})