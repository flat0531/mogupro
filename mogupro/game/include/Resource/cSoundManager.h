#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <Sound/cSE.h>
#include <Sound/cBGM.h>
namespace Resource
{
class cSoundManager : public Utility::cSingletonAble<cSoundManager>
{
public:
    cSoundManager( );
    // 例: Assets/SE/player/attack.wav なら
    // ＞: player/attack.wav と入力する。
    Sound::cSE& findSe( std::string const& underAssetsUnderSEUnderPath );
    // 例: Assets/BGM/main/buttle1.wav なら
    // ＞: main/buttle1.wav と入力する。
    Sound::cBGM& findBgm( std::string const& underAssetsUnderBGMUnderPath );
private:
    void loadSe( );
    void loadBgm( );
private:
    std::map<std::string, Sound::cSE> mSes;
    std::map<std::string, Sound::cBGM> mBgms;
};
}
