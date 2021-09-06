namespace Needmon
{
    class Packet
    {
        public:
        Packet() {};
        ~Packet() {};

        virtual void Encode() {};
        virtual void Decode() {};
    };
}