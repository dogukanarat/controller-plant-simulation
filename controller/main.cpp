#include "os_stream.hpp"
#include "os_thread.hpp"
#include "os_timer.hpp"
#include "os_memory.hpp"
#include "os_socket.hpp"

void *server( void *arg )
{
    OS_display("[INFO] Controller server has been started! ");

    const uint16_t port_number_u16     = 5001;
    int            server_socket_int   = OS_socket_tcp();

    OS_socket_address_in_t *p_server_socket_address_in_st = OS_socket_address_in_init( port_number_u16 );
    OS_socket_address_in_t *p_client_sockaddr_st          = (OS_socket_address_in_t*)OS_mem_allocate( sizeof(OS_socket_address_in_t) );

    OS_socket_length_t server_socklen_st = sizeof(*p_server_socket_address_in_st);
    OS_socket_length_t client_socklen_st = sizeof(*p_client_sockaddr_st);


    while( true )
    {
        OS_wait_us(1000);

    }
    
    return 0;
}

void *client( void *arg )
{
    OS_display("[INFO] Controller client has been started! ");

    while( true )
    {
        OS_wait_us(1000);
       
    }
    
    return 0;
}

int main()
{
    OS_display( "Controller has been started!" );

    /**
     * @brief 
     * 
     */
    OS_thread_t thread_server;
    OS_thread_t thread_client;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS_thread_create( &thread_server, server );
    OS_thread_create( &thread_client, client );

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS_thread_join( thread_server );
    OS_thread_join( thread_client );
  
    return 0;
}