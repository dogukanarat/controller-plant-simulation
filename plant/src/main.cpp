#include "main.h"

using namespace OSAL;

OS::Mutex printMutex;

Needmon::Queue plantInQueue(20);
Needmon::Queue plantOutQueue(20);

void *server( void *arg )
{
    UNUSED( arg );

    Needmon::Buffer messageBuffer;

    OS::display("[SERVER] Controller server has been started! ");

    Needmon::Ethernet* insTcp = new Needmon::TCP("127.0.0.1", 5001);
    Needmon::Communication* insServer = new Needmon::Server(insTcp);

    Needmon::ErrorNo errorNo = true;

    errorNo = insServer->Connect();

    if( errorNo == true )
    {
        errorNo = insServer->Process();
    } else {}

    uint32_t aliveCounter = 0;

    while( errorNo == true )
    {
        errorNo = insServer->Read(messageBuffer);

        if( errorNo == true )
        {
            std::shared_ptr<Needmon::Frame> messageFrame(new Needmon::Frame());

            // messageFrame->Parse(messageBuffer);
            // messageFrame->Decode(periodicPacket);

            // OS::print("[SERVER] Message is received | Message: %d\n", periodicPacket.Data.data1 );

            // messageQueue.Push(messageFrame);

        }

        std::shared_ptr<Needmon::Frame> messageFrame;

        plantOutQueue.Pop(messageFrame);

        if( messageFrame != nullptr )
        {

        }
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
    
    while( true )
    {
        currentStartTime = OS::Now();

        Control::Decimal cosOut = 5.0f * cos( ( 2.0f * M_PI / 180.0f)* (timestamp * 5) );

        noisyValue = noiseGenerator.Random(static_cast<Control::Decimal>( cosOut ));
        plantOutPacket.noisySignal = static_cast<float>(noisyValue);

        std::shared_ptr<Needmon::Frame> plantOutMessage(new Needmon::Frame());

        plantOutMessage->Encode( controllerOutPacket );

        plantOutQueue.Push(plantOutMessage);

        printMutex.Lock();
        OS::print("[PLANT] Timestamp: %d \t Clear Signal: %.2f \t Noisy Value: %.2f \n", timestamp, cosOut, noisyValue );
        printMutex.Unlock();

        timestamp++;

        nextStartTime = currentStartTime + intervalMillis;

        OS::SleepUntil( nextStartTime );

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
    OS::create( &thread_server, server );
    OS::create( &thread_plant, plant );

    /**
     * @brief Construct a new os thread join object
     * 
     */
    OS::join( thread_server );
    OS::join( thread_plant );
  
    return 0;
}