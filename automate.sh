#!/bin/sh

if [ $1 = 'build' ]
    then
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    rm -f -r build
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=./../install -G Ninja
    ninja
    cd ..
fi

if [ $1 = 'update' ]
    then
    git submodule update --remote --recursive
fi

if [ $1 = 'install' ]
    then
    rm -f -r install
    mkdir install
    cd build
    ninja install
    cd ..
fi

if [ $1 = 'run' ]
    then
    #run project
    if [ $2 = 'plant' ]
        then
        ./build/apps/plant/plant
    fi
    
    if [ $2 = 'controller' ]
        then
        ./build/apps/controller/controller
    fi
fi

if [ $1 = 'clean' ]
    then
    #clean build
    rm -f -r build
    rm -f -r install
fi





