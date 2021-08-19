#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <shared_memory.hpp>

using namespace std;

data_frame_t *frame_st = NULL;

void *server( void *arg )
{
	cout << "Plant server has been started!" << endl;
    flush(cout);

    while (1)
    {
        usleep(10);

        char *block = attach_memory_block(FILENAME, BLOCK_SIZE);

        if( block != NULL )
        {
            memcpy(block, &frame_st, BLOCK_SIZE);

            detach_memory_block( block );

            cout << "System Time: " << frame_st->plant.system_time_u32 << endl;
            cout << "Temperature: " << frame_st->plant.temperature_s16 << endl;
            cout << "---------------------------------------------------" << endl;
            flush(cout);

            
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
        usleep(1000);
        if( frame_st != NULL )
        {
            frame_st->plant.temperature_s16 += 1;
        } else {}
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