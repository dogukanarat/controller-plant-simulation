file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(controller ${SOURCES})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

target_link_libraries(
    controller LINK_PUBLIC needmon
)

target_link_libraries(
    controller LINK_PUBLIC osal
    )

target_link_libraries(
    controller LINK_PUBLIC Eigen3::Eigen
)