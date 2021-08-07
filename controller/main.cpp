#include<iostream>
#include "pthread.h"
#include "string.h"
#include <unistd.h>
#include "string.h"
#include "../lib/shared_memory/shared_memory.hpp"

using namespace std;

void *server( void *arg )
{
	cout << "Controller server has been started!" << endl;
    flush(cout);

    while (1)
    {
        char *block = attach_memory_block(FILENAME, BLOCK_SIZE);

        if( block != NULL )
        {
            sleep(1);
            cout << "Controller  is reading: ";
            printf("%d\n", (int)(*block));
            flush(cout);

            detach_memory_block( block );
        } else {}
    }
    
    return 0;
}

void *client( void *arg )
{
    cout << "Controller client has been started!" << endl;
    flush(cout);

    while (1)
    {
    }
    
    return 0;
}

int main()
{
    cout << "Controller has been started!" << endl;

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