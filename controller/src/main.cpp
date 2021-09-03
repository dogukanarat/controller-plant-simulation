#include "main.h"

using namespace OSAL;

void *server( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;
    Needmon::Frame messageFrame;
    Packets::Periodic periodicPacket;

    OS::display("[SERVER] Controller server has been started! ");

    const uint16_t port_number_u16     = 5001;
    int            server_socket_int   = Socket::_socketStream();
    int            client_socket_int   = 0;

    Socket::address_in_t* p_server_socket_address_in_st = Socket::address_in_init( port_number_u16 );
    Socket::address_in_t* p_client_socket_address_in_st = (Socket::address_in_t*)OS::memAllocate( sizeof(Socket::address_in_t) );

    Socket::length_t server_socklen_st = sizeof(*p_server_socket_address_in_st);
    Socket::length_t client_socklen_st = sizeof(*p_client_socket_address_in_st);

    
    if( Socket::_bind( server_socket_int, (Socket::address_t*)p_server_socket_address_in_st, server_socklen_st) < 0 )
    {
        OS::display("[ERROR] Bind error!");
    }

    if( Socket::_listen( server_socket_int) < 0 )
    {
        OS::display("[ERROR] Listen error!");
    }

    while( true )
    {
        OS::display("[SERVER] Server in the loop!");

        client_socket_int = Socket::_accept( server_socket_int, (Socket::address_t*)p_client_socket_address_in_st, &client_socklen_st );

        Socket::_read( client_socket_int, messageBuffer.GetBuffer(), Needmon::FRAME_SIZE);

        messageFrame.Parse( messageBuffer );
        periodicPacket.Decode( messageFrame );

        OS::print("[CLIENT] Received message: 0x%X \n", periodicPacket.Data.data1);
        

        Socket::_close( client_socket_int );

        OS::waitUs(1000000);

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

    OS::waitUs(5000000);

    OS::display("[CLIENT] Controller client has been started! ");

    const uint16_t port_number_u16     = 5001;

    Socket::address_in_t*   p_server_socket_address_in_st;
    Socket::host_t*         p_host_st             = (Socket::host_t*)(gethostbyname("127.0.0.1"));
    int                     socket_int            = Socket::_socketStream();

    OS::display("[CLIENT] Client socket is created!");

    if( p_host_st == NULL )
    {
        OS::display("[CLIENT] No such host!");
    }

    p_server_socket_address_in_st = Socket::address_in_init_by_name( p_host_st,  port_number_u16 );

    OS::display("[CLIENT] Client socket is initialized!");

    if( Socket::_connect( socket_int, p_server_socket_address_in_st) < 0 )
    {
        OS::display("[CLIENT] Connection error!");
    }

    while( true )
    {
        Socket::_write(socket_int, messageBuffer.GetBuffer(), Needmon::FRAME_SIZE );

        OS::waitUs(1000000);
       
    }
    
    return 0;
}

int main()
{
    OS::display("[PROCESS] Controller has been started!");

    /**
     * @brief 
     * 
     */
    OS::thread_t thread_server;
    OS::thread_t thread_client;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create( &thread_server, server );
    OS::create( &thread_client, client );

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_server );
    OS::join( thread_client );
  
    return 0;
}