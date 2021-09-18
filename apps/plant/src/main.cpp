#include "main.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define RECEIVE_PORT_NO 5001
#define TRANSMIT_PORT_NO 5002
#define SOCKET_DELAY_US 100000

Needmon::Queue g_plantInQueue(QUEUE_SIZE);
Needmon::Queue g_plantOutQueue(QUEUE_SIZE);

Needmon::Frame plantOutMessage;
Needmon::Frame plantInMessage;

OS::Mutex printMutex;
OS::Mutex plantInQueueMutex;
OS::Mutex plantOutQueueMutex;
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

    OS::display("[CLIENT] Plant client has been started! ");

    insProtocol = new Needmon::TCP("127.0.0.1", TRANSMIT_PORT_NO);
    insClient = new Needmon::Client(insProtocol);

    do
    {
        errorNo = insClient->Connect();
        OS::waitUs(SOCKET_DELAY_US);
    } while (errorNo == false);

    OS::display("[CLIENT] Connected!");

    connectionMutex.Unlock();

    errorNo = insClient->Process();

    while (true)
    {

        plantOutQueueMutex.Lock();
        plantOutMessage.Serialize(messageBuffer);
        plantOutQueueMutex.Unlock();

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
    Packets::PlantOut plantOutPacket;
    Needmon::ErrorNo errorNo = true;
    uint32_t aliveCounter = 0;
    Needmon::Ethernet *insProtocol = nullptr;
    Needmon::Communication *insServer = nullptr;

    OS::display("[SERVER] Plant server has been started! ");

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
            plantInQueueMutex.Lock();
            plantInMessage.Parse(messageBuffer);
            plantInQueueMutex.Unlock();

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

void *plant(void *arg)
{
    UNUSED(arg);

    OS::waitUs(SOCKET_DELAY_US);

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{1000};

    uint32_t timestamp = 0;

    Control::GaussianDistribution noiseGenerator(0, 1.0f);

    Packets::PlantOut plantOutPacket;
    Packets::ControllerOut controllerOutPacket;

    Control::Decimal noisyValue = 0.0f;

    connectionMutex.Lock();

    while (true)
    {
        currentStartTime = OS::Now();

        Control::Decimal cosOut = 5.0f * cos((2.0f * M_PI / 180.0f) * (timestamp * 5));
        noisyValue = noiseGenerator.Random(static_cast<Control::Decimal>(cosOut));
        plantOutPacket.noisySignal = static_cast<float>(noisyValue);

        plantOutQueueMutex.Lock();
        plantOutMessage.Encode(plantOutPacket);
        plantOutQueueMutex.Unlock();

        plantInQueueMutex.Lock();
        plantInMessage.Decode(controllerOutPacket);
        plantInQueueMutex.Unlock();

        // printMutex.Lock();
        // OS::print("[PLANT] Timestamp: %d \t Clear Signal: %.2f \t Noisy Value: %.2f \t\n", timestamp, cosOut, noisyValue );
        // printMutex.Unlock();

        // printMutex.Lock();
        // OS::print("[CONTROLLER] Timestamp: %d TEST \t\n");
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
    OS::thread_t thread_server;
    OS::thread_t thread_client;
    OS::thread_t thread_plant;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create(&thread_server, server);
    OS::create(&thread_client, client);
    OS::create(&thread_plant, plant);

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join(thread_server);
    OS::join(thread_client);
    OS::join(thread_plant);

    return 0;
}
