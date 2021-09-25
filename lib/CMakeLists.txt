if (NOT TARGET gtest_main)
    add_subdirectory(googletest)
endif() 

if (NOT TARGET needmon)
    add_subdirectory(needmon)
endif() 

if (NOT TARGET osal)
    add_subdirectory(osal)
endif() 

add_subdirectory(common)