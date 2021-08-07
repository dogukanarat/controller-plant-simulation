#!/bin/sh

if [ $1 = 'build' ]
    then
    #build binaries
    mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make && cd ..
    
fi

if [ $1 = 'run' ]
    then
    #run project
    cd build && cd src && ./web_server_project_run && cd .. / cd ..

fi

if [ $1 = 'clean' ]
    then
    #clean build
    rm -r build
fi





