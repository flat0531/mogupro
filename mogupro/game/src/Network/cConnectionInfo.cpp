#include <Network/cConnectionInfo.h>
#include <Utility/MessageBox.h>
#include <cinder/app/App.h>
namespace Network
{
cConnectionInfo::cConnectionInfo( ubyte1 idCount )
	: closeSecond( cinder::app::getElapsedSeconds( ) + PING_HOLD_SECOND )
{
	if ( idCount == 255 )
	{
		MES_ERR( "接続相手の数が上限に達しました。",
				 [ ] { exit( 0 ); } );
	}
	else
	{
		id = idCount;
	}
}
}
