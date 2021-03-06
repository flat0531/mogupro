#include <Scene/Member/cTitle.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Scene/Member/cGameMain.h>
#include <Scene/Member/cMatching.h>
#include <Scene/Member/cMatchingServer.h>
#include <Scene/cSceneManager.h>
#include <Network.hpp>
using namespace cinder;
namespace Scene
{
namespace Member
{
cTitle::cTitle( )
{

}
cTitle::~cTitle( )
{

}
void cTitle::setup( )
{
	CAMERA->setup( );

	mBackGround = Node::Renderer::sprite::create( "titleBack.png" );
	mBackGround->set_scale( vec2( app::getWindowSize( ) ) / mBackGround->get_content_size( ) * vec2( 1, -1 ) );

	mContentsRoot = Node::node::create( );
	mContentsRoot->set_content_size( app::getWindowSize( ) - ivec2( 100, 100 ) );
	mContentsRoot->set_schedule_update( );
	mContentsRoot->set_scale( vec2( 1, -1 ) );
	mContentsRoot->set_position( mContentsRoot->get_content_size( ) / 2.0F * vec2( -1, 1 ) );

	auto logo = mContentsRoot->add_child( Node::Renderer::sprite::create( "titleLogo.png" ) );
	logo->set_anchor_point( vec2( 0.0F ) );
	logo->set_position( vec2( 0.0F ) );

	auto pushanybutton = mContentsRoot->add_child( Node::Renderer::sprite::create( "pushanybutton.png" ) );
	pushanybutton->set_anchor_point( vec2( 1, 1 ) );
	pushanybutton->set_position( mContentsRoot->get_content_size( ) );

	auto version = mContentsRoot->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
	version->set_anchor_point( vec2( 0, 1 ) );
	version->set_position( mContentsRoot->get_content_size( ) * vec2( 0, 1 ) );
	version->set_text( "ver0.1.0" );
}
void cTitle::shutDown( )
{
}
void cTitle::resize( )
{
}
void cTitle::update( float deltaTime )
{
	mContentsRoot->entry_update( deltaTime );

	if ( ENV->pressKey( cinder::app::KeyEvent::KEY_LCTRL ) )
	{
		if ( ENV->pushKey( cinder::app::KeyEvent::KEY_F1 ) )
		{
			cSceneManager::getInstance( )->shift<Scene::Member::cMatching>( );
		}
		else if ( ENV->pushKey( cinder::app::KeyEvent::KEY_F2 ) )
		{
			cSceneManager::getInstance( )->shift<Scene::Member::cMatchingServer>( );
		}
	}
	else
	{
		if ( Utility::cInputAll::getInstance( )->anyKey( ) )
		{
			Network::cUDPClientManager::getInstance( )->open( );
			Network::cUDPServerManager::getInstance( )->open( );
			Network::cUDPClientManager::getInstance( )->connectOfflineServer( );
			cSceneManager::getInstance( )->shift<Scene::Member::cGameMain>( );
		}
	}
}
void cTitle::draw( )
{
}
void cTitle::draw2D( )
{
	mBackGround->entry_render( cinder::mat4( ) );
	mContentsRoot->entry_render( cinder::mat4( ) );
}
}
}
