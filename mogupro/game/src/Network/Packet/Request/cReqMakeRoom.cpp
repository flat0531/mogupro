#include <Network/Packet/Request/cReqMakeRoom.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqMakeRoom::cReqMakeRoom( ) : mRoomID(-1), mNetworkHandle("", 0)
{
}
{

}

void cReqMakeRoom::packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data)
{
}
ubyte2 cReqMakeRoom::packetExport( char* const data )
{
    return offset;
}
}
}
}