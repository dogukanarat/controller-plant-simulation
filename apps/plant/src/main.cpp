#include "main.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define RECEIVE_PORT_NO 5001
#define TRANSMIT_PORT_NO 5002
#define SOCKET_DELAY_US 100000


static struct 
{
    Needmon::Message plantOutMessage;
    Needmon::Message plantInMessage;
    OS::Mutex plantInMessageMutex;
    OS::Mutex plantOutMessageMutex;
    OS::Mutex printMutex;
    Needmon::Topic testTopic;
} GL;




void TestCallBack(Needmon::Message &message)
{
    GL.printMutex.Lock();
    OS::print("[PLANT] Callback is called! \n");
    GL.printMutex.Unlock();
}

void *server(void *arg)
{
    UNUSED(arg);

    Needmon::Buffer messageBuffer;
    Messages::PlantOut plantOutPacket;
    Needmon::ErrorNo errorNo = true;
    Needmon::Ethernet *insProtocol = nullptr;
    Needmon::Communication *insServer = nullptr;
    UInt32 receiveCounter = 0;
    UInt32 transmitCounter = 0;

    OS::display("[SERVER] Plant server has been started! ");

    insProtocol = new Needmon::TCP("127.0.0.1", RECEIVE_PORT_NO);
    insServer = new Needmon::Server(insProtocol);

    GL.testTopic.Publish(GL.plantOutMessage);

    do
    {
        errorNo = insServer->Connect();
    } while (errorNo == false);

    errorNo = insServer->Process();

    OS::display("[SERVER] Connected!");

    while (true)
    {
        GL.plantOutMessageMutex.Lock();
        GL.plantOutMessage.Serialize(messageBuffer);
        GL.plantOutMessageMutex.Unlock();

        errorNo = insServer->Write(messageBuffer);

        if (errorNo == true)
        {
            // printMutex.Lock();
            // OS::print("[SERVER] Message is transmitted | Message: %d\n", transmitCounter++);
            // printMutex.Unlock();
        }
        else
        {
        }

        errorNo = insServer->Read(messageBuffer);

        if (errorNo == true)
        {
            GL.plantInMessageMutex.Lock();
            GL.plantInMessage.Parse(messageBuffer);
            GL.plantInMessageMutex.Unlock();

            // printMutex.Lock();
            // OS::print("[SERVER] Message is received | Message: %d\n", receiveCounter++);
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

    GL.testTopic.Subscribe(TestCallBack);

    OS::waitUs(SOCKET_DELAY_US);

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{1000};

    uint32_t timestamp = 0;

    Control::GaussianDistribution noiseGenerator(0, 1.0f);

    Messages::PlantOut plantOutPacket;
    Messages::ControllerOut controllerOutPacket;

    Control::Decimal noisyValue = 0.0f;
    Control::Decimal filteredValue = 0.0f;

    while (true)
    {
        currentStartTime = OS::Now();

        Control::Decimal cosOut = 5.0f * cos((2.0f * M_PI / 180.0f) * (timestamp * 5));
        noisyValue = noiseGenerator.Random(static_cast<Control::Decimal>(cosOut));
        plantOutPacket.noisySignal = static_cast<float>(noisyValue);

        GL.plantOutMessageMutex.Lock();
        GL.plantOutMessage.Encode(plantOutPacket);
        GL.plantOutMessageMutex.Unlock();

        GL.plantInMessageMutex.Lock();
        GL.plantInMessage.Decode(controllerOutPacket);
        GL.plantInMessageMutex.Unlock();

        filteredValue = controllerOutPacket.filteredSignal;

        GL.printMutex.Lock();
        OS::print("[PLANT] Timestamp: %d \t Clear Signal: %.2f \t Noisy Value: %.2f \t Filtered Value: %.2f \t\n", timestamp, cosOut, noisyValue, filteredValue);
        GL.printMutex.Unlock();

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
    OS::thread_t thread_plant;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create(&thread_server, server);
    OS::create(&thread_plant, plant);

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join(thread_server);
    OS::join(thread_plant);

    return 0;
}
