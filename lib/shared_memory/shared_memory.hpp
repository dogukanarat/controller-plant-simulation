
#include <cstdint>

#define BLOCK_SIZE 4096
#define FILENAME "./../shared_memory.dat"

typedef struct 
{
    struct
    {
        uint32_t system_time_u32;
        int16_t  temperature_s16;
    } plant;
    
    struct
    {
        int16_t  response_s16;
    } controller;
    
} data_frame_t;

static int get_shared_block( char *filename, int size );
char *attach_memory_block( char *filename, int size );
bool detach_memory_block( char *block );
bool destroy_memory_block( char *filename );