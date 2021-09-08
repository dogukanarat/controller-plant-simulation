file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true src/*.cpp)
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES true inc/*.h)

add_library(common ${SOURCES})
set_target_properties(common PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

target_link_libraries(common PUBLIC needmon Eigen3::Eigen osal)

install(TARGETS common
        PUBLIC_HEADER DESTINATION include/common/
)
