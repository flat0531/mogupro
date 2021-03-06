#pragma once
#include <Utility/cSingletonAble.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <map>
namespace Game
{
class cGameManager : public Utility::cSingletonAble<cGameManager>
{
public:
	cGameManager( );
	~cGameManager( ) = default;
public:
	enum class State
	{
		// ロードが終わって全員を待機している時
		STAND_BY,
		// プレイヤーにカメラが移動し、本当にバトルが開始されるまでの間
		READY,
		// ゲーム中
		BATTLE,
		// リザルト発表中
		RESULT,
	};
public:
	// ゲーム開始パケットが届いたらステートごとの絶対移行時間を代入してください。
	void setup( boost::posix_time::ptime ready, boost::posix_time::ptime battle, boost::posix_time::ptime result );
	inline bool isStandBy( ) { return state == State::STAND_BY; }
	inline bool isStandByShift( ) { return flash && ( state == State::BATTLE ); }
	inline bool isReady( ) { return state == State::READY; }
	inline bool isReadyShift( ) { return flash && ( state == State::READY ); }
	inline bool isBattle( ) { return state == State::BATTLE; }
	inline bool isBattleShift( ) { return flash && ( state == State::BATTLE ); }
	inline bool isResult( ) { return state == State::RESULT; }
	inline bool isResultShift( ) { return flash && ( state == State::RESULT ); }
	std::string getLeftBattleTime( );
	void preUpdate( float delta );
	void update( float delta );
private:
	void shift( State state );
	// 現在のステート
	State state = State::BATTLE;
	// 一フレーム前のステート
	State prevState = state;
	// ステートが移った瞬間
	bool flash;

	// ステートごとの絶対移行時間。
	// ゲームメインに移動した時点で明確に決めておきます。
	std::map<State, boost::posix_time::ptime> shiftSeconds;

	// 
	std::map<State, std::function<void( float )>> mUpdates;
	// 
	std::map<State, std::function<void( float )>> mPreUpdates;
};
}
