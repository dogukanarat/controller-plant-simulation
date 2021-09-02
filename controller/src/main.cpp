#include "main.h"

void *server( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;
    Packets::Periodic periodicPacket;

    OS_display("[SERVER] Controller server has been started! ");

    const uint16_t port_number_u16     = 5001;
    int            server_socket_int   = OS_socket_tcp();
    int            client_socket_int   = 0;

    OS_socket_address_in_t *p_server_socket_address_in_st = OS_socket_address_in_init( port_number_u16 );
    OS_socket_address_in_t *p_client_socket_address_in_st = (OS_socket_address_in_t*)OS_mem_allocate( sizeof(OS_socket_address_in_t) );

    OS_socket_length_t server_socklen_st = sizeof(*p_server_socket_address_in_st);
    OS_socket_length_t client_socklen_st = sizeof(*p_client_socket_address_in_st);

    
    if( OS_bind( server_socket_int, (OS_socket_address_t*)p_server_socket_address_in_st, server_socklen_st) < 0 )
    {
        OS_display("[ERROR] Bind error!");
    }

    if( OS_listen( server_socket_int) < 0 )
    {
        OS_display("[ERROR] Listen error!");
    }

    while( true )
    {
        OS_display("[SERVER] Server in the loop!");

        client_socket_int = OS_accept( server_socket_int, (OS_socket_address_t*)p_client_socket_address_in_st, &client_socklen_st );

        OS_read( client_socket_int, messageBuffer.GetBuffer(), Needmon::FRAME_SIZE);

        messageFrame.Parse( messageBuffer );
        periodicPacket.Decode( messageFrame );

        OS_print("[CLIENT] Received message: 0x%X \n", periodicPacket.Data.data1);
        

        OS_close( client_socket_int );

        OS_wait_us(1000000);

    }
    
    return 0;
}

void *client( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;
    Packets::Periodic periodicPacket;

    periodicPacket.Data.data1 = 0x23;
    periodicPacket.Encode( messageFrame );
    messageFrame.Serialize( messageBuffer );

    OS_wait_us(5000000);

    OS_display("[CLIENT] Controller client has been started! ");

    const uint16_t port_number_u16     = 5001;

    OS_socket_address_in_t *p_server_socket_address_in_st;
    OS_host_t              *p_host_st           = (OS_host_t*)(gethostbyname("127.0.0.1"));
    int                     socket_int          = OS_socket_tcp();

    OS_display("[CLIENT] Client socket is created!");

    if( p_host_st == NULL )
    {
        OS_display("[CLIENT] No such host!");
    }

    p_server_socket_address_in_st = OS_socket_address_in_init_by_name( p_host_st,  port_number_u16 );

    OS_display("[CLIENT] Client socket is initialized!");

    if( OS_connect( socket_int, p_server_socket_address_in_st) < 0 )
    {
        OS_display("[CLIENT] Connection error!");
    }

    while( true )
    {
        OS_write(socket_int, messageBuffer.GetBuffer(), Needmon::FRAME_SIZE );

        OS_wait_us(1000000);
       
    }
    
    return 0;
}

int main()
{
    OS_display("[PROCESS] Controller has been started!");

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