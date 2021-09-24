file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true src/*.cpp)
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES true inc/*.h)

add_executable(test ${SOURCES})