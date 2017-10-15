#include <Game/Field/cBlock.h>
#include <cinder/gl/gl.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
static vec3 vertex0[] = {
    vec3( -0.5f,  0.5f, 0.5f ),
    vec3( 0.5f, -0.5f, 0.5f ),
    vec3( 0.5f,  0.5f, 0.5f ),
    vec3( -0.5f,  0.5f, 0.5f ),
    vec3( -0.5f, -0.5f, 0.5f ),
    vec3( 0.5f, -0.5f, 0.5f ), };
static vec3 vertex1[] = {
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( 0.5f,  0.5f, -0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ),
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( -0.5f,  0.5f, -0.5f ),
    vec3( 0.5f,  0.5f, -0.5f ), };
static vec3 vertex2[] = {
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( -0.5f,  0.5f, -0.5f ),
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( -0.5f,  0.5f,  0.5f ),
    vec3( -0.5f,  0.5f, -0.5f ), };
static vec3 vertex3[] = {
    vec3( 0.5f,  0.5f,  0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ),
    vec3( 0.5f,  0.5f, -0.5f ),
    vec3( 0.5f,  0.5f,  0.5f ),
    vec3( 0.5f, -0.5f,  0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ), };
static vec3 vertex4[] = {
    vec3( -0.5f, 0.5f, -0.5f ),
    vec3( 0.5f, 0.5f,  0.5f ),
    vec3( 0.5f, 0.5f, -0.5f ),
    vec3( -0.5f, 0.5f, -0.5f ),
    vec3( -0.5f, 0.5f,  0.5f ),
    vec3( 0.5f, 0.5f,  0.5f ), };
static vec3 vertex5[] = {
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ),
    vec3( 0.5f, -0.5f,  0.5f ),
    vec3( -0.5f, -0.5f,  0.5f ),
    vec3( -0.5f, -0.5f, -0.5f ),
    vec3( 0.5f, -0.5f, -0.5f ), };

static vec3 normal0[] = {
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ),
    vec3( 0.0f, 0.0f, 1.0f ), };
static vec3 normal1[] = {
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ),
    vec3( 0.0f, 0.0f, -1.0f ), };
static vec3 normal2[] = {
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ),
    vec3( -1.0f, 0.0f, 0.0f ), };
static vec3 normal3[] = {
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ), };
static vec3 normal4[] = {
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ), };
static vec3 normal5[] = {
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ),
    vec3( 0.0f, -1.0f, 0.0f ), };


GLfloat cube_vtx[] = {
    // �O
    //  x,     y,    z,
    -0.5f,  0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    // ��
    -0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,

    // �E
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,

    // ��
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,

    // ��
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f,  0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f,  0.5f,
    0.5f, 0.5f,  0.5f,

    // ��
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
};

GLfloat cube_normal[] = {
    // �O
    // x,    y,    z,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // ��
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    // �E
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    // ��
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    // ��
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    // ��
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
};

std::vector<uint32_t> getIndices( int side_size, int offset = 0 )
{
    std::vector<GLuint> indices = std::vector<GLuint>( side_size * 6 );
    int c = 0;
    for (int i = 0; i < side_size * 4; i += 4)
    {
        indices[c++] = i + offset;
        indices[c++] = i + 2 + offset;
        indices[c++] = i + 1 + offset;
        indices[c++] = i + 2 + offset;
        indices[c++] = i + offset;
        indices[c++] = i + 3 + offset;
    }
    return indices;
}

std::vector<vec2> getUv( int side_num )
{
    std::vector<vec2> uv = std::vector<vec2>( side_num * 4 );
    int c = 0;
    for (int i = 0; i < side_num; i++)
    {
        uv[c++] = vec2( 0.00000, 1.00000 );
        uv[c++] = vec2( 0.00000, 0.00000 );
        uv[c++] = vec2( 1.00000, 0.00000 );
        uv[c++] = vec2( 1.00000, 1.00000 );
    }
    return uv;
}

vec3* getVertices( int side_num )
{
    switch (side_num)
    {
        case 0: return vertex0;
        case 1: return vertex1;
        case 2: return vertex3;
        case 3: return vertex2;
        case 4: return vertex4;
        case 5: return vertex5;
    }
    return new vec3();
}

vec3* getNormals( int side_num )
{
    switch (side_num)
    {
        case 0: return normal0;
        case 1: return normal1;
        case 2: return normal3;
        case 3: return normal2;
        case 4: return normal4;
        case 5: return normal5;
    }
    return new vec3();
}

int getIndex( const vec3 & vtx, const vec3 & normal,
              const std::vector<vec3>& vertices,
              const std::vector<vec3>& normals )
{
    for (int i = 0; i < (vertices.size()); ++i)
    {
        if (vertices[i] == vtx
             && normals[i] == normal)
        {
            // �S����v
            return i;
        }
    }
    return -1;
}

cBlock::cBlock( const ci::vec3 position, const float scale )
{
    mPosition = position;
    mScale = scale;
    mIsActive = true;
}
cBlock::~cBlock()
{
}
void cBlock::update()
{
}
void cBlock::draw()
{
    if (mIsActive == false) return;
    gl::pushModelView();
    gl::translate( mPosition );
    gl::scale( vec3( mScale ) );
    gl::drawCube( vec3( 0 ), vec3( 1 ) );
    gl::popModelView();
}
void cBlock::drawMesh()
{
    if (mIsActive == false) return;
    if (mVertices.size() == 0)return;
    gl::pushModelView();
    gl::translate( mPosition );
    gl::scale( vec3( mScale ) );
    //gl::draw( mVboMesh );
    gl::draw( *mMesh );
    gl::popModelView();
}
void cBlock::createSide( const  int& offset_index )
{
    if (mDrawSide.size() <= 0)
        return;

    //for (size_t i = 0; i < draw_side.size(); i++)
    //{
    //    // ���_
    //    vec3* temp = getVertices( draw_side[i] );
    //    for (int k = 0; k < 6; k++)
    //        mVertices.emplace_back( mPosition + temp[k] );
    //    // �@��
    //    temp = getNormals( draw_side[i] );
    //    for (int k = 0; k < 6; k++)
    //        mNormals.emplace_back( temp[k] );
    //}
    //mIndices = getIndices( draw_side.size(), offset_index );

    mUv = getUv( mDrawSide.size() );

    // DrawElements�p�̃f�[�^���쐬
    for (int i = 0; i < 36; ++i)
    {
        int side_num = i / 6;
        if (!std::any_of( mDrawSide.begin(), mDrawSide.end(),
                          [&]( int n ) { return side_num == n; } ))
            continue;

        vec3 vertex = vec3( cube_vtx[i * 3 + 0],
                            cube_vtx[i * 3 + 1],
                            cube_vtx[i * 3 + 2] ) + mPosition;
        vec3 normal = vec3( cube_normal[i * 3 + 0],
                            cube_normal[i * 3 + 1],
                            cube_normal[i * 3 + 2] );

        int id = getIndex( vertex, normal,
                           mVertices, mNormals );

        if (id < 0)
        {
            // �������W�Ɩ@���������_�͌����炸
            mIndices.emplace_back( mVertices.size() + offset_index );

            mVertices.emplace_back( vertex );
            mNormals.emplace_back( normal );
        }
        else
        {
            // ��������
            mIndices.emplace_back( id + offset_index );
        }
    }

    // ���b�V�������
    //mMesh = ci::TriMesh::create();
    //if (mVertices.size() > 0)
    //    mMesh->appendPositions( &mVertices[0], mVertices.size() );
    //if (mIndices.size() > 0)
    //    mMesh->appendIndices( &mIndices[0], mIndices.size() );
    //if (mUv.size() > 0)
    //    mMesh->appendTexCoords0( &mUv[0], mUv.size() );
    //if (mNormals.size() > 0)
    //    mMesh->appendNormals( &mNormals[0], mNormals.size() );
}
void cBlock::clear()
{
    mVertices.clear();
    mIndices.clear();
    mUv.clear();
    mDrawSide.clear();
    //mMesh->clear();
}
void cBlock::toBreak()
{
    mIsActive = false;
    clear();
}
}
}