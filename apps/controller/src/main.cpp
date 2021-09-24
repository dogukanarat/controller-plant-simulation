#include "main.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define RECEIVE_PORT_NO 5002
#define TRANSMIT_PORT_NO 5001
#define SOCKET_DELAY_US 100000

Needmon::Message controllerOutMessage;
Needmon::Message controllerInMessage;

OS::Mutex printMutex;
OS::Mutex controllerInQueueMutex;
OS::Mutex controllerOutQueueMutex;
OS::Mutex connectionMutex;

void *client(void *arg)
{
    UNUSED(arg);

    Needmon::Buffer messageBuffer;
    Needmon::ErrorNo errorNo = true;
    Needmon::Ethernet *insProtocol = nullptr;
    Needmon::Communication *insClient = nullptr;
    UInt32 receiveCounter = 0;
    UInt32 transmitCounter = 0;

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
        errorNo = insClient->Read(messageBuffer);

        if (errorNo == true)
        {
            controllerInQueueMutex.Lock();
            controllerInMessage.Parse(messageBuffer);
            controllerInQueueMutex.Unlock();

            // printMutex.Lock();
            // OS::print("[SERVER] Message is received | Message: %d\n", receiveCounter++);
            // printMutex.Unlock();
        }
        else
        {
        }

        controllerOutQueueMutex.Lock();
        controllerOutMessage.Serialize(messageBuffer);
        controllerOutQueueMutex.Unlock();

        errorNo = insClient->Write(messageBuffer);

        if (errorNo == true)
        {
            // printMutex.Lock();
            // OS::print("[SERVER] Message is transmitted | Message: %d\n", transmitCounter++);
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

    Messages::PlantOut plantOutPacket;
    Messages::ControllerOut controllerOutPacket;

    Control::Decimal noisyValue = 0.0f;
    Control::Decimal filteredValue = 0.0f;

    while (true)
    {
        currentStartTime = OS::Now();

        controllerInQueueMutex.Lock();
        controllerInMessage.Decode(plantOutPacket);
        controllerInQueueMutex.Unlock();

        noisyValue = plantOutPacket.noisySignal;

        filteredValue = noisyValue / 2;

        controllerOutPacket.filteredSignal = filteredValue;

        controllerOutQueueMutex.Lock();
        controllerOutMessage.Encode(controllerOutPacket);
        controllerOutQueueMutex.Unlock();

        printMutex.Lock();
        OS::print("[CONTROLLER] Timestamp: %d \t Noisy Value: %.2f \t Filtered Value: %.2f \t\n", timestamp, noisyValue, filteredValue);
        printMutex.Unlock();

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
    OS::thread_t thread_controller;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create(&thread_client, client);
    OS::create(&thread_controller, controller);

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join(thread_client);
    OS::join(thread_controller);

    return 0;
}
