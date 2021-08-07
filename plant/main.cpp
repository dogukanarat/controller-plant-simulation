#include<iostream>
#include "pthread.h"
#include <unistd.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "../lib/shared_memory/shared_memory.hpp"

using namespace std;

void *server( void *arg )
{
	cout << "Plant server has been started!" << endl;
    flush(cout);

    int counter = 0;
    char buffer[10];

    while (1)
    {
        sleep(1);

        char *block = attach_memory_block(FILENAME, BLOCK_SIZE);

        if( block != NULL )
        {
            cout << "Plant is writing: " << counter << endl;
            flush(cout);

            *block = (int)( counter );

            detach_memory_block( block );

            counter++;
        }
        else
        {
            cout << "Plant is telling: Block is not reachable" << endl;
            flush(cout);
        }

    }
    
    return 0;
}

void *client( void *arg )
{
    cout << "Plant client has been started!" << endl;
    flush(cout);
    
    while (1)
    {
    }
    
    return 0;
}

int main()
{
    cout << "Plant has been started!" << endl;

    /**
     * @brief 
     * 
     */
    pthread_t thread_server;
    pthread_t thread_client;

    pthread_create(&thread_server, NULL, server, NULL);
    pthread_create(&thread_client, NULL, client, NULL);

    pthread_join(thread_server, NULL);
    pthread_join(thread_client, NULL);
  
    return 0;
}