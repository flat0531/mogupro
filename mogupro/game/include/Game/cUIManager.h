#pragma once
#include <Node/node.h>
#include <Utility/cSingletonAble.h>
namespace Game
{
class cUIManager : public Utility::cSingletonAble<cUIManager>
{
public:
    cUIManager( ) = default;
    ~cUIManager( ) = default;
public:
	void setup( );
	void update( float delta );
	void draw( );
private:
	hardptr<Node::node> mRoot;
	softptr<Node::node> mTime;
	softptr<Node::node> mLive;
	softptr<Node::node> mCapsule;
	softptr<Node::node> mMyTeamCannonPower;
	softptr<Node::node> mEnemyTeamCannonPower;
	softptr<Node::node> mPlayerScreenEffect;
};
}
