#include <Network/Packet/Event/cEveLightBomb.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveLightBomb::cEveLightBomb( )
{

}
void cEveLightBomb::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
	imp >> playerId >> position >> speed;
}
ubyte2 cEveLightBomb::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
	exp << playerId << position << speed;
    return exp;
}
}
}
}