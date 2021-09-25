file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true src/*.cpp)
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES true inc/*.h)

add_executable(controller ${SOURCES})
target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

target_link_libraries(controller PRIVATE common osal Eigen3::Eigen)

install(TARGETS controller)