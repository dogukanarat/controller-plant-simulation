#!/bin/sh

if [ $1 = 'build' ]
    then
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    rm -f -r build
    rm -f -r install
    mkdir build
    mkdir install
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=./../install -G Ninja
    ninja
    ninja install
    cd ..

fi

if [ $1 = 'run' ]
    then
    #run project
    if [ $2 = 'plant' ]
        then
        ./install/bin/plant
    fi
    
    if [ $2 = 'controller' ]
        then
        ./install/bin/controller
    fi
fi

if [ $1 = 'clean' ]
    then
    #clean build
    rm -f -r build
    rm -f -r install
fi





