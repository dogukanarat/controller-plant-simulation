
#define BLOCK_SIZE 4096
#define FILENAME "./../shared_memory.dat"

static int get_shared_block( char *filename, int size );
char *attach_memory_block( char *filename, int size );
bool detach_memory_block( char *block );
bool destroy_memory_block( char *filename );