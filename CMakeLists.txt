cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wunused-parameter -Wextra -pedantic -Werror -target x86_64-apple-darwin20.3.0 -v")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wunused-parameter -Wextra -pedantic -Werror -target x86_64-apple-darwin20.3.0 -v")

project(web_server_project)

include_directories(controller)
include_directories(plant)

add_subdirectory(controller)
add_subdirectory(plant)

add_subdirectory(lib/googletest)
add_subdirectory(lib/osal)
add_subdirectory(lib/needmon)





