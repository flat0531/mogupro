#include <Game/Field/cChunkHolder.h>
#include <Game/Field/cUnderGround.h>
#include <Game/Field/cChunkMeshBuilder.h>
#include <Utility/cString.h>

using namespace ci;
using namespace ci::app;
namespace Game
{
namespace Field
{
cChunkHolder::cChunkHolder( cUnderGround* under_ground ) :
    mUnderGround( under_ground )
    , mChunkIdOffset( 0 )
{
}

cChunkHolder::~cChunkHolder()
{

}

cChunk* cChunkHolder::getChunk( const int& x, const int& z )
{
    return getChunk( ci::ivec3( x, 0, z ) );
}

cChunk* cChunkHolder::getChunk( const ci::ivec3& c )
{
    return mChunks.at( ivec3( c.x, 0, c.z ) ).get();
}

cChunkLayer* cChunkHolder::getChunkLayer( const ci::ivec3 & cell )
{
    return getChunk( ivec3( cell.x, 0, cell.z ) )->getChunkLayer( cell.y );
}

ChunkMap& cChunkHolder::getChunks()
{
    return mChunks;
}

void cChunkHolder::setChunk( cChunkRef&  chunk )
{
    auto cell = chunk->getCell();
    mChunks[cell] = chunk;
}

void cChunkHolder::setChunk( const int& x, const int& y, const int& z )
{
    if ( isExistsChunk( x, y, z ) == false )
        return;
    int id = ( CHUNK_RANGE_Y + 1 ) * mChunkIdOffset;
    mChunkIdOffset += 1;
    mChunks[ivec3( x, y, z )] = std::make_shared<cChunk>( x, z, id, mUnderGround );
}

bool isPointToSphere( const ci::vec3& point, const ci::vec3& sphere_pos, const float& radius )
{
    float x = ( sphere_pos.x - point.x ) *( sphere_pos.x - point.x );
    float y = ( sphere_pos.y - point.y ) *( sphere_pos.y - point.y );
    float z = ( sphere_pos.z - point.z ) *( sphere_pos.z - point.z );
    return ( x + y + z ) <= radius * radius;
}

bool cChunkHolder::breakBlock( const ci::ivec3 & chunk_cell,
                               const ci::ivec3& block_cell,
                               const ci::vec3& sphere_pos,
                               const float & radius,
                               const cBreakBlockType& type )
{
    if ( isExistsChunk( chunk_cell ) )
        return false;
    if ( cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return false;

    auto break_chunk_layer = getChunkLayer( chunk_cell );

    // 必ず一マスは掘る
    auto first_layer = break_chunk_layer->breakBlock( block_cell );

    if ( radius < 0 )
        return false;

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    // 掘られたチャンクを登録する
    std::vector<cChunkLayer*> build_chunk_layers;
    if ( first_layer != nullptr )
        build_chunk_layers.push_back( first_layer );

    for ( int z = s.z; z <= e.z; z++ )
        for ( int y = s.y; y <= e.y; y++ )
            for ( int x = s.x; x <= e.x; x++ )
            {
                auto block = break_chunk_layer->getBlock( ivec3( x, y, z ) );
                if ( block == nullptr )
                    continue;

                if ( block->isActive() )
                    if ( isPointToSphere( block->getPosition(), sphere_pos, radius ) == false )
                        continue;

                if ( type.find( block->mType ) == false )
                    continue;

                auto layer = break_chunk_layer->getChunkLayer( ivec3( x, y, z ) );
                layer = break_chunk_layer->breakBlock( block, layer );
                if ( layer == nullptr )
                    continue;

                if ( std::any_of( build_chunk_layers.begin(), build_chunk_layers.end(),
                                  [&]( cChunkLayer* t ) { return t == layer; } ) )
                    continue;

                build_chunk_layers.emplace_back( layer );
            }

    // 掘られたチャンクの周りも登録する
    std::vector<cChunkLayer*> temp_layers;
    for ( auto break_layer : build_chunk_layers )
    {
        for ( int i = 0; i < 6; i++ )
        {
            auto cell = break_layer->getChunkCell();
            switch ( i )
            {
                case 0: cell.x += 1; break;
                case 1: cell.x -= 1; break;
                case 2: cell.y += 1; break;
                case 3: cell.y -= 1; break;
                case 4: cell.z += 1; break;
                case 5: cell.z -= 1; break;
            }

            if ( isExistsChunk( cell.x, 0, cell.z ) )
                continue;
            if ( cell.y > getHighestCell() || cell.y < 0 )
                continue;
            if ( std::any_of( build_chunk_layers.begin(), build_chunk_layers.end(),
                              [&]( cChunkLayer* t ) { return t->getChunkCell() == cell; } ) )
                continue;

            auto temp_layer = getChunkLayer( cell );
            temp_layer->mIsRebuildMesh = true;
            temp_layers.emplace_back( temp_layer );
        }
    }

    std::copy( temp_layers.begin(), temp_layers.end(), std::back_inserter( build_chunk_layers ) );

    for ( auto c : build_chunk_layers )
        c->reBuildStart();
    return true;
}

bool cChunkHolder::isBreakBlock( const ci::ivec3 & chunk_cell,
                                 const ci::ivec3 & block_cell,
                                 const ci::vec3 & sphere_pos,
                                 const float & radius )
{

    if ( isExistsChunk( chunk_cell ) )
        return false;
    if ( cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return false;

    if ( radius < 0 )
        return false;

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    auto break_chunk_layer = getChunkLayer( chunk_cell );
    for ( int z = s.z; z <= e.z; z++ )
        for ( int y = s.y; y <= e.y; y++ )
            for ( int x = s.x; x <= e.x; x++ )
            {
                auto block = break_chunk_layer->getBlock( ivec3( x, y, z ) );
                if ( block == nullptr )
                    continue;
                if ( block->isActive() == false )
                    continue;
                if ( isPointToSphere( block->getPosition(), sphere_pos, radius ) == false )
                    continue;

                return true;
            }

    return false;
}

std::vector<int> cChunkHolder::getChunkId( const ci::ivec3 & chunk_cell,
                                           const ci::ivec3& block_cell,
                                           const float & radius )
{
    std::vector<int> chunks_id;
    if ( isExistsChunk( chunk_cell ) )
        return chunks_id;
    if ( cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return chunks_id;

    // positionの位置のチャンクを最初に登録する
    auto chunk_layer = getChunkLayer( chunk_cell );
    chunks_id.push_back( chunk_layer->getChunkLayerId() );

    if(radius < BLOCK_SIZE )
        return chunks_id;

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    for ( int z = s.z; z <= e.z; z += r.z  * 2)
        for ( int y = s.y; y <= e.y; y += r.y * 2 )
            for ( int x = s.x; x <= e.x; x += r.x * 2 )
            {
                auto layer = chunk_layer->getChunkLayer( ivec3( x, y, z ) );
                if ( layer == nullptr )
                    continue;

                int id = layer->getChunkLayerId();
                if ( std::any_of( chunks_id.begin(), chunks_id.end(),
                                  [&]( int t ) { return t == id; } ) )
                    continue;

                chunks_id.emplace_back( id );
            }

    return chunks_id;
}

bool cChunkHolder::createChunk( cChunk* chunk )
{
    chunk->createBlocks();
    return true;
}

bool cChunkHolder::createChunkMesh( cChunk* chunk )
{
    chunk->buildMesh();
    return true;
}

bool cChunkHolder::isExistsChunk( const ci::ivec3 & cell )
{
    if ( cell.y < 0 || cell.y > CHUNK_RANGE_Y + 1 )
        return true;
    return mChunks.find( ivec3( cell.x, 0, cell.z ) ) == mChunks.end();
}

bool cChunkHolder::isExistsChunk( const int& x, const int& y, const int& z )
{
    return isExistsChunk( ivec3( x, y, z ) );
}

bool cChunkHolder::cellIsOutOfBounds( const int & x, const int & y, const int & z )
{
    if ( x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE )
        return true;
    if ( x < 0 || y < 0 || z < 0 )
        return true;
    return false;
}

int cChunkHolder::getHighestCell()
{
    return CHUNK_RANGE_Y;
}

void cChunkHolder::clear()
{
    mChunks.clear();
}
}
}
