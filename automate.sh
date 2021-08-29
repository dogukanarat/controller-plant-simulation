#!/bin/sh

if [ $1 = 'build' ]
    then
    #build binaries
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    rm -f -r build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G Ninja && ninja && cd ..
    
fi

if [ $1 = 'run' ]
    then
    #run project
    if [ $2 = 'plant' ]
        then
        cd build && cd plant && ./plant && cd .. / cd ..
    fi
    
    if [ $2 = 'controller' ]
        then
        cd build && cd controller && ./controller && cd .. / cd ..
    fi
fi

if [ $1 = 'clean' ]
    then
    #clean build
    rm -f -r build
fi





