#pragma once
#include <Scene/cSceneBase.h>
namespace Scene
{
namespace Member
{
class cBegin final : public cSceneBase
{
public:
    cBegin( ) = default;
    ~cBegin( ) = default;
public:
	void setup( ) override;
	void shutDown( ) override;
	void update( float t ) override;
	void draw( ) override;
	void draw2D( ) override;
	void resize( ) override;
};
}
}
