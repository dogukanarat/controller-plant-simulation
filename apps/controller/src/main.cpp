#include "main.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define RECEIVE_PORT_NO 5002
#define TRANSMIT_PORT_NO 5001
#define SOCKET_DELAY_US 100000

Needmon::Queue g_controllerInQueue(QUEUE_SIZE);
Needmon::Queue g_controllerOutQueue(QUEUE_SIZE);

Needmon::Frame controllerOutMessage;
Needmon::Frame controllerInMessage;

OS::Mutex printMutex;
OS::Mutex controllerInQueueMutex;
OS::Mutex controllerOutQueueMutex;
OS::Mutex connectionMutex;

void *client(void *arg)
{
    UNUSED(arg);

    connectionMutex.Lock();

    Needmon::Buffer messageBuffer;
    Needmon::ErrorNo errorNo = true;
    uint32_t aliveCounter = 0;
    Needmon::Ethernet *insProtocol = nullptr;
    Needmon::Communication *insClient = nullptr;

    OS::display("[CLIENT] Controller client has been started! ");

    insProtocol = new Needmon::TCP("127.0.0.1", TRANSMIT_PORT_NO);
    insClient = new Needmon::Client(insProtocol);

    do
    {
        errorNo = insClient->Connect();
        OS::waitUs(SOCKET_DELAY_US);
    } while (errorNo == false);

    OS::display("[CLIENT] Connected!");

    errorNo = insClient->Process();

    while (true)
    {

        controllerOutQueueMutex.Lock();
        controllerOutMessage.Serialize(messageBuffer); 
        controllerOutQueueMutex.Unlock();
        
        errorNo = insClient->Write(messageBuffer);

        // printMutex.Lock();
        // OS::print("[CLIENT] Message is transmitted | Message: %d\n", aliveCounter++);
        // printMutex.Unlock();


        OS::waitUs(SOCKET_DELAY_US);
    }

    return 0;
}

void *server(void *arg)
{
    UNUSED(arg);

    Needmon::Buffer messageBuffer;
    Needmon::ErrorNo errorNo = true;
    uint32_t aliveCounter = 0;
    Needmon::Ethernet *insProtocol = nullptr;
    Needmon::Communication *insServer = nullptr;

    OS::display("[SERVER] Controller server has been started! ");

    insProtocol = new Needmon::TCP("127.0.0.1", RECEIVE_PORT_NO);
    insServer = new Needmon::Server(insProtocol);

    do
    {
        errorNo = insServer->Connect();
    } while (errorNo == false);

    errorNo = insServer->Process();

    OS::display("[SERVER] Connected!");

    while (true)
    {
        errorNo = insServer->Read(messageBuffer);

        if (errorNo == true)
        {
            controllerInQueueMutex.Lock();
            controllerInMessage.Parse(messageBuffer);
            controllerInQueueMutex.Unlock();

            // printMutex.Lock();
            // OS::print("[SERVER] Message is received | Message: %d\n", aliveCounter++);
            // printMutex.Unlock();
        }
        else
        {
        }

        OS::waitUs(SOCKET_DELAY_US);
    }

    return 0;
}

void *controller(void *arg)
{
    UNUSED(arg);

    OS::waitUs(SOCKET_DELAY_US);

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{1000};

    uint32_t timestamp = 0;

    Packets::PlantOut plantOutPacket;
    Packets::ControllerOut controllerOutPacket;

    Control::Decimal noisyValue = 0.0f;

    connectionMutex.Lock();

    while (true)
    {
        currentStartTime = OS::Now();

        printMutex.Lock();
        OS::print("[CONTROLLER] Timestamp : %d\n", timestamp);
        printMutex.Unlock();


        controllerInQueueMutex.Lock();
        controllerInMessage.Encode(plantOutPacket);
        controllerInQueueMutex.Unlock();

        controllerOutQueueMutex.Lock();
        controllerOutMessage.Decode(controllerOutPacket);
        controllerOutQueueMutex.Unlock();

        noisyValue = plantOutPacket.noisySignal;

        // printMutex.Lock();
        // OS::print("[PLANT] Timestamp: %d \t Noisy Value: %.2f \t\n", timestamp, noisyValue );
        // printMutex.Unlock();

        // printMutex.Lock();
        // OS::print("[CONTROLLER] Timestamp: %d TEST t\n");
        // printMutex.Unlock();

        timestamp++;

        nextStartTime = currentStartTime + intervalMillis;
        OS::SleepUntil(nextStartTime);
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
    OS::thread_t thread_client;
    OS::thread_t thread_server;
    OS::thread_t thread_controller;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create(&thread_client, client);
    OS::create(&thread_server, server);
    OS::create(&thread_controller, controller);

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join(thread_client);
    OS::join(thread_server);
    OS::join(thread_controller);

    return 0;
}
