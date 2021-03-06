#include <Network/Packet/Deliver/cDliBreakBlocks.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Deliver
{
void cDliBreakBlocks::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter imp( data );
    for ( int i = 0; i < transferredBytes / ( sizeof( ubyte2 ) * 4 + sizeof( ubyte1 ) ); ++i )
    {
        float p[3];
        for ( int n = 0; n < 3; ++n )
        {
            ubyte2 src;
            imp >> src;
            p[n] = convertFloat( src );
        }
        ubyte2 radius;
        imp >> radius;
        ubyte1 type;
        imp >> type;
        mBreakFormats.emplace_back( cinder::vec3( p[0], p[1], p[2] ), convertFloat( radius ), type );
    }
}
ubyte2 cDliBreakBlocks::packetExport( char* const data )
{
    cExporter exp( data );
    for ( auto& o : mBreakFormats )
    {
        for ( int n = 0; n < 3; ++n )
        {
            exp << convertFixedpoint( o.position[n] );
        }
        exp << convertFixedpoint( o.radius );
        exp << o.type;
    }
    return exp;
}
}
}
}