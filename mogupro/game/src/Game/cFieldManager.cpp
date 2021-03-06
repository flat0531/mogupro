#include <Game/cFieldManager.h>
#include <cinder/gl/gl.h>
using namespace ci;
namespace Game
{
cFieldManager::cFieldManager()
{
}
cFieldManager::~cFieldManager()
{
}
void cFieldManager::setup()
{
    mUnderGround = std::make_shared<Field::cUnderGround>();
    mUnderGround->setup();
}
void cFieldManager::update( const float& delta_time )
{
    mUnderGround->update( delta_time );
}
void cFieldManager::draw()
{
    auto ctx = gl::context();
    bool is_culling_change = ctx->getBoolState( GL_CULL_FACE );

    if ( is_culling_change == false )
        gl::enableFaceCulling( true );

    mUnderGround->draw();

    if ( is_culling_change == false )
        gl::enableFaceCulling( false );
}
bool cFieldManager::blockBreak( const ci::vec3& position, const float& radius, const Field::cBreakBlockType& type )
{
    return mUnderGround->blockBreak( position, radius, type );
}
bool cFieldManager::blockBreakNetwork( const ci::vec3 & position, const float & radius, const Field::cBreakBlockType& type )
{
    return mUnderGround->blockBreakNetwork( position, radius, type );
}
bool cFieldManager::isBreakBlock( const ci::vec3 & position, const float & radius )
{
    return mUnderGround->isBreakBlock( position, radius );
}
ci::vec3 cFieldManager::getBlockHighestPosition( const ci::vec3 & target_position )
{
    return mUnderGround->getBlockHighestPosition( target_position );
}
ci::vec3 cFieldManager::getBlockTopPosition( const ci::vec3 & target_position )
{
    return mUnderGround->getBlockTopPosition( target_position );
}
std::vector<int> cFieldManager::getChunkId( const ci::vec3 & position, const float & radius )
{
    return mUnderGround->getChunkId( position, radius );
}
void cFieldManager::shutdown()
{
    mUnderGround->shutdown();
}
}