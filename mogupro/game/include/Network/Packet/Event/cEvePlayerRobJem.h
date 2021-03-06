#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayerRobJem : public cPacketBase<cEvePlayerRobJem, PacketId::EVE_PLAYER_ROB_JEM>
{
public:
    cEvePlayerRobJem( );
    cEvePlayerRobJem( std::string const& playerName );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    // 奪った相手の名前。
    // そのうちIDとかになりそう。
    std::string mPlayerName;
};
}
}
}