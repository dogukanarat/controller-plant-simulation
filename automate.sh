#!/bin/sh

if [ $1 = 'build' ]
    then
    #build binaries
    rm -f -r build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make && cd ..
    
fi

if [ $1 = 'run' ]
    then
    #run project
    if [ $2 = 'plant' ]
        then
        cd build && cd plant && ./web_server_project_plant && cd .. / cd ..
    fi
    
    if [ $2 = 'controller' ]
        then
        cd build && cd controller && ./web_server_project_controller && cd .. / cd ..
    fi
fi

if [ $1 = 'clean' ]
    then
    #clean build
    rm -f -r build
fi





