#include <Network/cUDPServerManager.h>
#include <Network/cUDPManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <cinder/app/App.h>
#include <limits>
#include <Node/action.hpp>
#include <Utility/MessageBox.h>
namespace Network
{
cUDPServerManager::cUDPServerManager( )
    : mRoot( Node::node::create( ) )
{
    mRoot->set_schedule_update( );
}
void cUDPServerManager::close( )
{
    mSocket.close( );
}
void cUDPServerManager::open( )
{
    mSocket.open( 25565 );
}
void cUDPServerManager::update( float delta )
{
    updateRecv( );
    updateSend( );
    mRoot->entry_update( delta );
}
void cUDPServerManager::updateSend( )
{
    // 送信するものがあればバッファーから取り出して送る。
    for ( auto& handle : mHandle )
    {
        // 余ってたらパケットを送ります。
        if ( !handle.second.buffer.empty( ) )
        {
            mSocket.write( handle.first, handle.second.buffer.size( ), handle.second.buffer.data( ) );
            handle.second.buffer.clear( );
            handle.second.buffer.shrink_to_fit( );
        }
    }
}
void cUDPServerManager::updateRecv( )
{
    // 受信したものがあればバッファーから取り出してパケットの分別を行う。
    while ( !mSocket.emptyChunk( ) )
    {
        auto chunk = mSocket.popChunk( );
        if ( cUDPManager::getInstance( )->isConnectPacket( chunk ) || 
            ( mHandle.find( chunk.networkHandle ) != mHandle.end( ) ) )
        {
            cUDPManager::getInstance( )->onReceive( chunk );
        }
        else
        {
            // コネクションを確立しないまま送信してきた場合。
        }
    }

    connection( );
    ping( );
}
void cUDPServerManager::sendDataBufferAdd( cNetworkHandle const & networkHandle, cPacketBuffer const & packetBuffer )
{
    auto& buf = mHandle[networkHandle].buffer;

    // パケットが大きくなりそうなら先に送ってしまいます。
    if ( 1024 < buf.size( ) )
    {
        mSocket.write( networkHandle, buf.size( ), buf.data( ) );
        buf.clear( );
        buf.shrink_to_fit( );
    }

    ubyte2 const& byte = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

    buf.resize( buf.size( ) + byte );
    memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
}
void cUDPServerManager::connection( )
{
    while ( auto p = cRequestManager::getInstance( )->getReqConnect( ) )
    {
        auto itr = mHandle.insert( std::make_pair( p->mNetworkHandle, std::move( cClientInfo( ) ) ) );
        send( p->mNetworkHandle, new Packet::Response::cResConnect( ) );

        using namespace Node::Action;
        auto networkHandle = p->mNetworkHandle;
        auto act = repeat_forever::create( sequence::create( delay::create( 1.5F ), call_func::create( [ networkHandle, this ]
        {
            send( networkHandle, new Packet::Event::cEvePing( ) );
        } ) ) );
        act->set_tag( itr.first->second.id );
        mRoot->run_action( act );
    }
}
void cUDPServerManager::ping( )
{
    while ( auto p = cDeliverManager::getInstance( )->getDliPing( ) )
    {
        mHandle[p->mNetworkHandle].closeSecond = cinder::app::getElapsedSeconds( ) + 5.0F;
    }
    for ( auto itr = mHandle.begin( ); itr != mHandle.end( ); )
    {
        if ( itr->second.closeSecond < cinder::app::getElapsedSeconds( ) )
        {
            mRoot->remove_action_by_tag( itr->second.id );
            mHandle.erase( itr );
            continue;
        }
        itr++;
    }
}
cUDPServerManager::cClientInfo::cClientInfo( )
    : closeSecond( cinder::app::getElapsedSeconds( ) + 5.0F )
{
    idCount += 1;

    // オーバーフローしたらエラーにします。
    if ( idCount == 0U )
    {
        Utility::MessageBoxOk( "クライアントの数が上限に達しました。",
                               [ ] { exit( 0 ); } );
    }
    else
    {
        id = idCount;
    }
}
ubyte2 cUDPServerManager::cClientInfo::idCount = 0U;
}