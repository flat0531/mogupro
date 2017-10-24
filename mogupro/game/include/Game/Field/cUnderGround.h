#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/cChunk.h>
#include <Game/Field/cChunkHolder.h>
#include <Game/Field/FieldData.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <boost/lockfree/queue.hpp>

namespace Game
{
namespace Field
{
class cUnderGround
{
public:

    cUnderGround();
    ~cUnderGround();

    void setup();
    void update();
    void draw();

public:

    cBlock getBlock( ci::ivec3 position );
    void setBlock( ci::ivec3 position, cBlock block );

public: // Player,Strategy で使う関数

    bool blockBreak( const ci::vec3& position, const float& radius );
    ci::vec3 getBlockCenterTopPosition( const ci::vec3& target_position );
    ci::ivec3 getBlockMaxCell();


private: // Method

    bool chunkMeshReLoaded();
    bool calcChunks();

    ci::ivec3 getChunkCellFromPosition( const ci::vec3& position );
    ci::ivec3 getBlockCellFromPosition( const ci::vec3& position );

private: // Member

    cChunkHolder mChunkHolder;
    std::vector<std::thread> mChunkLoadThreads;
    std::mutex mMainMutex;
    std::atomic<bool> mIsRunning{ true };

};
}
}
