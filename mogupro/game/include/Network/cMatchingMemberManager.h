#pragma once
#include <Network.hpp>
#include <Utility/cSingletonAble.h>

//! @file MatchingMemberManager
//! @brief MatchingMemberを取得できます
//! @note 
//!      今後これを複数にする場合があるかもしれない
//! @date 2017-10-20
//! @author Taka Nomoto

namespace Network
{
class cMatchingMemberManager : public Utility::cSingletonAble<cMatchingMemberManager>
{
public:
	cMatchingMemberManager();
	~cMatchingMemberManager();
	bool addRoomMembers(Network::cNetworkHandle addMember);
	cNetworkHandle mMasterHandle;
	std::vector<cNetworkHandle> mRoomMembers;
	//これ分ける必要があるのか
	std::vector<cNetworkHandle> m1PMembers;
	std::vector<cNetworkHandle> m2PMembers;

};
}
