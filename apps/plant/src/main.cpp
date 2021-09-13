#include "main.h"

using namespace OSAL;

#define QUEUE_SIZE 20
#define PORT_NO 5001

Needmon::Queue g_plantInQueue(QUEUE_SIZE);
Needmon::Queue g_plantOutQueue(QUEUE_SIZE);

Needmon::Ethernet*      insTcp;
Needmon::Communication* insServer;

OS::Mutex printMutex;
OS::Mutex serverMutex;
OS::Mutex plantInQueueMutex;
OS::Mutex plantOutQueueMutex;

void *serverRead( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer        messageBuffer;
    Packets::ControllerOut controllerOutPacket;
    Packets::PlantOut      plantOutPacket;

    Needmon::ErrorNo errorNo = true;
    uint32_t aliveCounter = 0;

    while( errorNo == true )
    {
        serverMutex.Lock();
        errorNo = insServer->Read(messageBuffer);
        serverMutex.Unlock();

        if( errorNo == true )
        {
            std::shared_ptr<Needmon::Frame> messageFrame(new Needmon::Frame());

            messageFrame->Parse(messageBuffer);
            messageFrame->Decode(controllerOutPacket);

            printMutex.Lock();
            OS::print("[SERVER_READ] Message is received | Message: %d\n");
            printMutex.Unlock();

            if( g_plantInQueue.Push(messageFrame).IsFailed() == false )
            {

            } else {}

        } else {}
        
    }

    return 0;
}

void *serverWrite( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer        messageBuffer;
    Packets::ControllerOut controllerOutPacket;
    Packets::PlantOut      plantOutPacket;

    Needmon::ErrorNo errorNo = true;
    uint32_t aliveCounter = 0;

    while( errorNo == true )
    {
        std::shared_ptr<Needmon::Frame> messageFrame;

        plantOutQueueMutex.Lock();
        uint8_t queueResult = g_plantOutQueue.Pop(messageFrame).IsFailed();
        plantOutQueueMutex.Unlock();

        if( queueResult == false )
        {
            messageFrame->Encode(plantOutPacket);
            messageFrame->Serialize(messageBuffer);

            serverMutex.Lock();
            errorNo = insServer->Write(messageBuffer);
            serverMutex.Unlock();

            printMutex.Lock();
            OS::print("[SERVER_WRITE] Message is transmitted | Message: \n");
            printMutex.Unlock();
        } else {}
        

        OS::waitUs(1000);
    }

    return 0;
}

void *plant( void *arg )
{
    UNUSED( arg );

    OS::TimePoint currentStartTime{};
    OS::TimePoint nextStartTime{};

    const OS::MilliSecond intervalMillis{100};

    uint32_t timestamp = 0;
    
    Control::GaussianDistribution noiseGenerator(0, 1.0f);

    Packets::PlantOut plantOutPacket;
    Packets::ControllerOut controllerOutPacket;

    Control::Decimal noisyValue = 0.0f;

    int returnMutex = 0;
    
    while( true )
    {
        currentStartTime = OS::Now();

        Control::Decimal cosOut = 5.0f * cos( ( 2.0f * M_PI / 180.0f)* (timestamp * 5) );
        noisyValue = noiseGenerator.Random(static_cast<Control::Decimal>(cosOut));
        plantOutPacket.noisySignal = static_cast<float>(noisyValue);

        std::shared_ptr<Needmon::Frame> plantOutMessage(new Needmon::Frame());

        plantOutMessage->Encode(controllerOutPacket);

        plantOutQueueMutex.Lock();
        if( g_plantOutQueue.Push(plantOutMessage).IsFailed() == true )
        {
            while( false == printMutex.TryLock() );
            OS::print("[PLANT] Queue is full\n");
            printMutex.Unlock();
        }
        else
        {
            while( false == printMutex.TryLock() );
            OS::print("[PLANT] Queue size is %d\n", g_plantOutQueue.Length());
            printMutex.Unlock();
        }
        plantOutQueueMutex.Unlock();

        while( false == printMutex.TryLock() );
        OS::print("[PLANT] Timestamp: %d \t Clear Signal: %.2f \t Noisy Value: %.2f \t\n", timestamp, cosOut, noisyValue );
        printMutex.Unlock();

        timestamp++;

        nextStartTime = currentStartTime + intervalMillis;
        OS::SleepUntil( nextStartTime );

    }

    return 0;
}

int createServer()
{
    OS::display("[SERVER] Controller server has been started! ");

    insTcp = new Needmon::TCP("127.0.0.1", PORT_NO);
    insServer = new Needmon::Server(insTcp);

    Needmon::ErrorNo errorNo = true;

    errorNo = insServer->Connect();

    if( errorNo == true )
    {
        errorNo = insServer->Process();
    } else {}

    return true;
}

int main()
{
    OS::display("[PROCESS] Controller has been started!");

    /**
     * @brief Create a Server object
     * 
     */
    createServer();

    /**
     * @brief 
     * 
     */
    OS::thread_t thread_serverRead;
    OS::thread_t thread_serverWrite;
    OS::thread_t thread_plant;

    /**
     * @brief Construct a new os thread create object
     * 
     */
    OS::create( &thread_serverRead, serverRead );
    OS::create( &thread_serverWrite, serverWrite );
    OS::create( &thread_plant, plant );

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_serverRead );
    OS::join( thread_serverWrite );
    OS::join( thread_plant );
  
    return 0;
}