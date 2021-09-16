#include "stdint.h"
#include "os_socket.h"
#include "buffer.h"
#include "os_stream.h"

namespace Needmon
{
    typedef int ErrorNo;
    
    class Communication
    {
        public:

        virtual ErrorNo SetHandler(void (*receiveHandler)(uint8_t*,uint32_t)) { m_receiveHandler = receiveHandler; return true; };
        virtual ErrorNo Connect() = 0;
        virtual ErrorNo Process() = 0;
        virtual ErrorNo Read(Buffer& buffer) = 0;
        virtual ErrorNo Write(Buffer& buffer) = 0;

        protected:
        Communication() {};
        virtual ~Communication() {};

        Buffer m_buffer;
        void (*m_receiveHandler)(uint8_t*,uint32_t) = nullptr;
    };

    typedef enum
    {
        ETHERNET_UDP,
        ETHERNET_TCP,
    } EthernetProtocol;

    class Ethernet
    {
        public:
        Ethernet(const char* ipAddress, int portNo);

        uint16_t m_portNo;
        char* m_ipAddress;
        int m_serverSocket;
        EthernetProtocol m_protocol;
        OSAL::Socket::host_t* m_host;
        OSAL::Socket::address_in_t* m_serverSocketAddress;
        OSAL::Socket::length_t m_serverLen;
        
    };

    class TCP : public Ethernet
    {
        public:
        TCP(const char * ipAddress, int portNo) 
        : Ethernet{ ipAddress, portNo}
        {
            m_serverSocket = OSAL::Socket::_socketStream();
            m_protocol = ETHERNET_TCP;
        };

        ~TCP() {};
    };

    class UDP : public Ethernet
    {
        public:
        UDP(const char * ipAddress, int portNo) 
        : Ethernet{ ipAddress, portNo}
        {
            m_serverSocket = OSAL::Socket::_socketDgram();
            m_protocol = ETHERNET_UDP;
        };

        ~UDP() {};
    };

    class Server : public Communication
    {
        public:
        Server(Ethernet* ethernet);
        ~Server();

        OSAL::Socket::address_in_t* m_clientSocketAddress = nullptr;
        OSAL::Socket::length_t m_clientSocketLen;
        int m_clientSocket;
        Ethernet* m_ethernet;

        ErrorNo Connect() override;
        ErrorNo Process() override;
        ErrorNo Read(Buffer& buffer) override;
        ErrorNo Write(Buffer& buffer) override;
    };

    class Client : public Communication
    {
        public:
        Client(Ethernet* ethernet);
        ~Client() {};

        Ethernet* m_ethernet;

        ErrorNo Connect() override;
        ErrorNo Process() override;
        ErrorNo Read(Buffer& buffer) override;
        ErrorNo Write(Buffer& buffer) override;
    };


}
