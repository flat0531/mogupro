#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>
#include <Utility/cString.h>
#include <Game/cClientAdapter.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{

cBlock none_block;
cChunk none_chunk;
cUnderGround::cUnderGround() :
    mChunkHolder( this )
{
}

cUnderGround::~cUnderGround()
{
}

void cUnderGround::setup()
{
    TEX->set( "dirt", "dirt.jpg" );

    cTimeMeasurement::getInstance()->make();

    for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
    {
        for ( int x = 0; x < CHUNK_RANGE_X; x++ )
        {
            mChunkHolder.setChunk( x, 0, z );
        }
    }
    cTimeMeasurement::getInstance()->make();

    //console() << "Chunk set time : " << cTimeMeasurement::getInstance()->deltaTime() << std::endl;

    for ( size_t i = 0; i < 1; i++ )
    {
        mChunkLoadThreads.emplace_back( [&]
        {
            calcChunks();
        }
        );
        mChunkLoadThreads.emplace_back( [&]
        {
            chunkMeshReLoaded();
        }
        );
    }
}

void cUnderGround::update()
{
    std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
    {
        chunk.second->update();
        chunk.second->createMainCall();
    }
}

void cUnderGround::draw()
{
    std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );

    auto texture = TEX->get( "dirt" );
    if ( !texture )
        return;

    using namespace ci::gl;
    auto ctx = context();

    Rectf texRect = texture->getAreaTexCoords( Area( vec2( 0 ), texture->getSize() ) );

    ScopedVao vaoScp( ctx->getDrawTextureVao() );
    ScopedBuffer vboScp( ctx->getDrawTextureVbo() );
    ScopedTextureBind texBindScope( texture );
    ci::gl::ScopedGlslProg glsl( ci::gl::getStockShader( ci::gl::ShaderDef().texture() ) );

    //auto glsl = getStockShader( ShaderDef().uniformBasedPosAndTexCoord().color().texture( texture ) );
    //ScopedGlslProg glslScp( glsl );
    //glsl->uniform( "uTex0", 0 );
    //glsl->uniform( "uPositionOffset", vec2( 0 ) );
    //glsl->uniform( "uPositionScale", vec2( 1 ) );
    //glsl->uniform( "uTexCoordOffset", texRect.getUpperLeft() );
    //glsl->uniform( "uTexCoordScale", texRect.getSize() );

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
        chunk.second->draw();
}

void cUnderGround::shutdown()
{
    mIsRunning = false;
    for ( auto& thread : mChunkLoadThreads )
    {
        thread.join();
    }
    mChunkHolder.clear();
}

bool cUnderGround::chunkMeshReLoaded()
{
    while ( mIsRunning )
    {
        for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        {
            for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto chunk = mChunkHolder.getChunk( x, z );
                chunk->reBuildMesh();
            }
        }
    }

    return true;
}

bool cUnderGround::calcChunks()
{
    while ( mIsRunning )
    {
        for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        {
            for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto chunk = mChunkHolder.getChunk( x, z );
                mChunkHolder.createChunk( chunk );
            }
        }
        break;
    }

    while ( mIsRunning )
    {
        for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        {
            for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto chunk = mChunkHolder.getChunk( x, z );
                mChunkHolder.createChunkMesh( chunk );
            }
        }
        return true;
    }
    return true;
}

ci::ivec3 cUnderGround::getChunkCellFromPosition( const ci::vec3 & position )
{
    return ivec3( position ) / CHUNK_SIZE;
}

ci::ivec3 cUnderGround::getBlockCellFromPosition( const ci::vec3 & position )
{
    return ivec3( position ) % CHUNK_SIZE;
}

cBlock* cUnderGround::getBlock( const ci::ivec3& position )
{
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );

    if ( mChunkHolder.isExistsChunk( chunk_cell ) )
        return &none_block;
    if ( mChunkHolder.cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return &none_block;

    return mChunkHolder.getChunk( chunk_cell )->getBlock( block_cell );
}

void cUnderGround::setBlock( const ci::ivec3& position, cBlock& block )
{
    if ( position.y < 0 )
        return;
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );

    if ( mChunkHolder.isExistsChunk( chunk_cell ) )
        return;
    if ( mChunkHolder.cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return;

    mChunkHolder.getChunk( chunk_cell )->setBlock( block_cell, block );
}

cChunk* cUnderGround::getChunk( const ci::ivec3 & position )
{
    auto chunk_cell = getChunkCellFromPosition( position );

    if ( mChunkHolder.isExistsChunk( chunk_cell ) )
        return &none_chunk;

    return mChunkHolder.getChunk( chunk_cell );
}

bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    cClientAdapter::getInstance()->sendBreakBlock( position, radius );
    return true;
}

bool cUnderGround::blockBreakNetwork( const ci::vec3 & position, const float & radius )
{
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );

    if ( mChunkHolder.isExistsChunk( chunk_cell.x, chunk_cell.y, chunk_cell.z ) )
        return false;
    if ( mChunkHolder.cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return false;

    auto break_chunk = mChunkHolder.getChunk( chunk_cell );
    std::vector<cChunk*> chunks;

    // 必ず一マスは掘る
    auto first_chunk = break_chunk->breakBlock( block_cell );
    chunks.push_back( first_chunk );

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    // 掘られたチャンクを登録する
    for ( int z = s.z; z < e.z; z++ )
        for ( int y = s.y; y < e.y; y++ )
            for ( int x = s.x; x < e.x; x++ )
            {
                auto  c = break_chunk->breakBlock( ivec3( x, y, z ) );
                if ( std::any_of( chunks.begin(), chunks.end(),
                                  [&]( cChunk* t ) { return t == c; } ) )
                    continue;
                chunks.push_back( c );
            }

    // 掘られたチャンクの周りも登録する
    std::vector<cChunk*> temp_chunks;
    for ( auto& break_chunk : chunks )
    {
        for ( int i = 0; i < 4; i++ )
        {
            auto& cell = break_chunk->getCell();
            switch ( i )
            {
                case 0: cell.x += 1; break;
                case 1: cell.x -= 1; break;
                case 2: cell.z += 1; break;
                case 3: cell.z -= 1; break;
            }

            if ( mChunkHolder.isExistsChunk( cell ) )
                continue;
            if ( std::any_of( chunks.begin(), chunks.end(),
                              [&]( cChunk* t ) { return t->getCell() == cell; } ) )
                continue;

            auto temp_chunk = mChunkHolder.getChunk( cell );
            temp_chunk->mIsBlockBroken = true;
            temp_chunks.push_back( temp_chunk );
        }
    }

    std::copy( temp_chunks.begin(), temp_chunks.end(), std::back_inserter( chunks ) );
    for ( auto& c : chunks )
    {
        c->reBuildStart();
    }
    return true;
}

ci::vec3 cUnderGround::getBlockTopPosition( const ci::vec3 & target_position )
{
    auto chunk_cell = getChunkCellFromPosition( target_position );
    auto block_cell = getBlockCellFromPosition( target_position );

    auto p = ( block_cell * BLOCK_SIZE ) + ( chunk_cell * CHUNK_SIZE );
    p.y = CHUNK_SIZE + (int) ( BLOCK_SIZE / 2.0f );

    return p;
}

ci::vec3 cUnderGround::getBlockHighestPosition( const ci::vec3 & target_position )
{
    auto chunk_cell = getChunkCellFromPosition( target_position );
    auto block_cell = getBlockCellFromPosition( target_position );
    block_cell.y = CHUNK_SIZE - 1;

    if ( mChunkHolder.isExistsChunk( chunk_cell.x, chunk_cell.y, chunk_cell.z ) )
        return ivec3( 0 );

    auto  chunk = mChunkHolder.getChunk( chunk_cell );
    auto block_pos = chunk->getBlock( block_cell )->mPosition;
    block_pos.y += ( BLOCK_SIZE / 2.0f );
    return block_pos;
}

ci::ivec3 cUnderGround::getBlockMaxCell()
{
    return ivec3( CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE );
}

}
}
