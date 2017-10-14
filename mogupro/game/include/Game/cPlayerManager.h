#pragma once
#include <Utility/cSingletonAble.h>
namespace Game
{
class cPlayer;
class cPlayerManager : public ::Utility::cSingletonAble<cPlayerManager>
{
private:
    //全プレイヤーリスト
    std::vector<std::shared_ptr<Player::cPlayer>> player;
    //自分が操作するプレイヤー
    std::shared_ptr<Player::cPlayer> active_player;

    bool mouse_on = false;
    void playerInstance();
    void playerMove();
public:
    void setup();
    void update( const float& delta_time );
    void draw();
};
}
