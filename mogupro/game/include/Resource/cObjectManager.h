#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <cinder/gl/VboMesh.h>
namespace Resource
{
class cObjectManager : public Utility::cSingletonAble<cObjectManager>
{
public:
    cObjectManager( );
    // 例: Assets/OBJ/player.obj なら
    // ＞: player.obj と入力する。
    cinder::gl::VboMeshRef& findObject( std::string const& underAssetsUnderSEUnderPath );
private:
    std::map<std::string, cinder::gl::VboMeshRef> mObjects;
};
}
