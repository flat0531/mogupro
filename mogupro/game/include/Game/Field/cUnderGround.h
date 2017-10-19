#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/cChunk.h>
using uint = uint32_t;
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

public: // Player,Strategy で使う関数

    bool isOutOfRange( const ci::ivec3& cell_num );
    bool blockBreak( const ci::vec3& position, const float& radius );
    bool blockBreak( const ci::ivec3& cell_num, const float& radius );
    ci::vec3 getBlockCenterTopPosition( const ci::vec3& target_position );
    ci::ivec3 getBlockMaxCell();

public: // Thread で使う関数


private: // Method

    bool createUnderGround();
    bool createMesh();
    void blockMeshErase( std::shared_ptr<cBlock> b );
    bool blockDigged( const  ci::ivec3& cell_num );
    void blockAllClear();

    ci::ivec3 getCellNumFromPosition( const ci::vec3& position );

private: // Member

    std::vector<std::vector<std::vector<std::shared_ptr<cBlock>>>> mBlocks;
    ci::ivec3 mBlockMaxCell;
    float mIntervalOffset;
    float mScale;
    ci::vec3 mOffset;
    ci::ivec3 mChunkNum;

    std::vector<ci::vec3> mVertices;
    std::vector<uint> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVboMesh;
};
}
}
