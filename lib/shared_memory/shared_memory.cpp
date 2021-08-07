
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "unistd.h"
#include "shared_memory.hpp"

#define IPC_RESULT_ERROR (-1)

static int get_shared_block( char *filename, int size )
{
    key_t key;

    if( access( filename, F_OK ) == 0 ) {
        // file exists
    } else {
        FILE * fPtr;
        fPtr = fopen(filename, "w");
        fclose(fPtr);
    }

    key = ftok(filename, 0);
    
    if( key == IPC_RESULT_ERROR)
    {
        return IPC_RESULT_ERROR;
    } else {}

    return shmget(key, size, 0644 | IPC_CREAT);
}

char *attach_memory_block( char *filename, int size )
{
    int shared_block_id = get_shared_block( filename, size );
    char *result;

    if( shared_block_id == IPC_RESULT_ERROR )
    {
        return NULL;
    } else {}

    result = (char *)shmat(shared_block_id, NULL, 0);

    if( result == (char *)( IPC_RESULT_ERROR ) )
    {
        return NULL;
    } else {}

    return result;
}

bool detach_memory_block( char *block )
{
    return ( shmdt( block ) != IPC_RESULT_ERROR );
}

bool destroy_memory_block( char *filename )
{
    int shared_block_id = get_shared_block( filename, 0 );

    if( shared_block_id == IPC_RESULT_ERROR )
    {
        return NULL;
    } else {}

    return ( shmctl( shared_block_id, IPC_RMID, NULL ) != IPC_RESULT_ERROR );


} 