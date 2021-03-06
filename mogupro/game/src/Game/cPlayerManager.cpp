#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cStrategyManager.h>
#include <Game/Strategy/cDrill.h>
#include <Game/cFieldManager.h>
#include <Game/cClientAdapter.h>


void Game::cPlayerManager::playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	//生成
	for (int i = 0; i < player_number; i++) {
		//通信で代入
		if (i == active_player_id) {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, true, 0, 0, static_cast<Game::Player::Team>(teams[i])));
			//アクティブユーザに代入
			active_player = players[i];
			this->active_player_id = active_player_id;
			this->active_player_team_id = teams[i];
		}
		else {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, false, 0, 0, static_cast<Game::Player::Team>(teams[i])));
		}
	}
}

void Game::cPlayerManager::playerDrillMove(const float & delta_time)
{
	//CAMERA->shakeCamera(0.1f, 0.1f);
	//カメラの方向に移動
	active_player->move(CAMERA->getCameraLook() * active_player->getDrillSpeed());
}

void Game::cPlayerManager::playerAttack(const float & delta_time)
{
	if (active_player->isDead())return;
	active_player->getMainWeapon()->pushCall(ENV->pushKey(ci::app::KeyEvent::KEY_t));
	active_player->getMainWeapon()->pressCall(ENV->pushKey(ci::app::KeyEvent::KEY_t));
	active_player->getMainWeapon()->pullCall(ENV->pushKey(ci::app::KeyEvent::KEY_t));
}

void Game::cPlayerManager::playerNormalMove(const float& delta_time)
{
	//プレイヤーの速度
	float player_speed = active_player->getSpeed() * delta_time;

	ci::vec3 keybord_velocity = ci::vec3(0);

	float x_axis = 0;
	float z_axis = 0;
	//斜め用スタック
	int diagonal = 0;

	//リセット
	if (ENV->pressKey(ci::app::KeyEvent::KEY_r)) {
		active_player->resetPos();
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_w)) {
		z_axis = active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_s)) {
		z_axis = -active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_d)) {
		x_axis = -active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_a)) {
		x_axis = active_player->getSpeed();
		diagonal++;
	}

	keybord_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	keybord_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));

	if (diagonal >= 2) {
		std::sqrtf(keybord_velocity.x);
		std::sqrtf(keybord_velocity.z);
	}


	active_player->move(keybord_velocity);

	//ジャンプはMoveの後に呼ぶ
	if (ENV->pushKey(ci::app::KeyEvent::KEY_SPACE)) {
		active_player->jump(true);
	}

	//掘削機設置
	if (ENV->pushKey(ci::app::KeyEvent::KEY_o)) {
		auto drill_pos = Game::cFieldManager::getInstance()->getBlockTopPosition(active_player->getPos() + active_player->getInstallationPosition());
		cClientAdapter::getInstance()->sendSetQuarry(drill_pos, Game::Strategy::cDrill::DrillType::Level1);
	}

}
void Game::cPlayerManager::playerMove(const float & delta_time)
{
	
	//カメラのマウス操作ON　OFF
	if (ENV->pushKey(ci::app::KeyEvent::KEY_ESCAPE)) {
		ENV->setMouseControl(mouse_on);
		if (mouse_on == true) {
			mouse_on = false;
		}
		else {
			mouse_on = true;
		}
	}
	CAMERA->addCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.05f), ENV->getPadAxis(3)*(-0.05f)));
	
	//プレイヤーが死んでいたらカメラ以外操作不能
	if (active_player->isDead())return;

	//掘削中はtrue 
	if (ENV->pressKey(ci::app::MouseEvent::LEFT_DOWN) &&
		Game::cFieldManager::getInstance()->isBreakBlock(active_player->getPos() + (glm::normalize(CAMERA->getCamera().getViewDirection()) * ci::vec3(active_player->getStatus().drill_speed / 4)), 0.5f)) {
		active_player->Drilling(true);
	}
	else {
		active_player->Drilling(false);
	}
	if (ENV->pullKey(ci::app::MouseEvent::LEFT_DOWN)) {
		active_player->Drilling(false);
	}
	if (ENV->isPadPress(ENV->BUTTON_2)) {
		active_player->Drilling(true);
	}

	//ダッシュ
	//304 = シフト
	if (ENV->pullKey(304)) {
		active_player->setDefaultSpeed();
	}
	if (ENV->pushKey(304)) {
		active_player->setSpeed(10.0f);
	}

	//パッドの判定仮置き
	if (ENV->isPadPush(ENV->BUTTON_1)) {
		CAMERA->shakeCamera(0.1f, 0.1f);
	}
	

	padMove(delta_time);

	//掘削中は動き方が変わる
	if (active_player->isDrilling()) {
		playerDrillMove(delta_time);
	}
	else {
		playerNormalMove(delta_time);
	}
	//G-BACK
	if (ENV->pressKey(ci::app::KeyEvent::KEY_l)) {
		active_player->receiveDamage(10.0f, 5);
	}
	//大砲にジェムを入れる
	if (ENV->pressKey(ci::app::KeyEvent::KEY_f)) {
		auto cannon = cStrategyManager::getInstance()->getCannons()[static_cast<Player::Team>(active_player->getWhichTeam())];
		if (cannon->getAABB().intersects(active_player->getAABB())) {
			//cannon->receivePlayerGem(active_player->getgems);
			active_player->sendCannonGems(cannon->getGemStorePos());
			ci::app::console() << active_player->getgems.size() << std::endl;
		}
		else {
			ci::app::console() << active_player->getgems.size() << std::endl;
		}
	}
	if (ENV->pressKey(ci::app::KeyEvent::KEY_UP)) {
		CAMERA->addCameraAngle(ci::vec2(0, 0.05f));
	}
	if (ENV->pressKey(ci::app::KeyEvent::KEY_DOWN))
		CAMERA->addCameraAngle(ci::vec2(0, -0.05f));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_RIGHT))
		CAMERA->addCameraAngle(ci::vec2(-0.05f, 0));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_LEFT))
		CAMERA->addCameraAngle(ci::vec2(0.05f, 0));

	///////////////////デバックでライトボムを増やす
	if (ENV->pushKey(ci::app::KeyEvent::KEY_h)) {
		active_player->useSubWeapon.addSubWeapon(Game::Weapons::SubWeapon::LIGHT_BOMB);
	}

	/////////////////アイテムを使う
	if (ENV->pushKey(ci::app::KeyEvent::KEY_g)) {
		active_player->useSubWeapon.useWeapon(active_player_id);
	}
	///////////////////

}
void Game::cPlayerManager::padMove(const float & delta_time)
{
	ci::vec3 pad_velocity = ci::vec3(0);

	float x_axis = -1 * ENV->getPadAxis(0) * delta_time * active_player->getSpeed();
	float z_axis = -1 * ENV->getPadAxis(1) * delta_time * active_player->getSpeed();

	pad_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	pad_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));

	active_player->move(pad_velocity*ci::vec3(3));
}
void Game::cPlayerManager::killCamera(const float & delta_time)
{
	if (!active_player->isDead())return;
	if (active_player->getRespawnCount() > 1.5f) {
		for (auto& it : players) {
			if (active_player->getDamagedId() == it->getPlayerId()) {
				
				CAMERA->refPosition = it->getPos() + ci::vec3(0, 0, 0);
			}
		}
	}
}
void Game::cPlayerManager::setPlayersPosition(std::vector<ci::vec3> positions)
{
	for (int i = 0; i < players.size(); i++) {
		ci::vec3 vec = positions[i] - players[i]->getPos();
		players[i]->move(vec);
	}
}
//プレイヤーのコリジョンスピードはコリジョンマネージャの
//後に呼ぶ
void Game::cPlayerManager::playerCollisionAfterUpdate(const float& delta_time)
{
	for (auto& it : players) {
		it->setColliderSpeed();
		it->gemsUpdate(delta_time);
		it->weaponUpdae(delta_time);
	}
	playerAttack(delta_time);
}
void Game::cPlayerManager::setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	playerInstance(positions, player_number, active_player_id, teams);
	//ポジションの参照とカメラのズームを設定
	for (auto& it : players) {
		it->setup();
	}
}
void Game::cPlayerManager::update(const float& delta_time)
{
	if (!active_player->isDead()) {
		CAMERA->refPosition = active_player->getPos() + ci::vec3(0, 0, 0);
	}
	playerMove(delta_time);
	for (auto& it : players) {
		it->update(delta_time);
	}
	killCamera(delta_time);
	cClientAdapter::getInstance()->sendPlayer(active_player->getPos(), ci::quat());
}

void Game::cPlayerManager::draw()
{
	for (auto& it : players) {
		if (it->getActiveUser()) {

			if (CAMERA->getCameraMode() == CameraManager::CAMERA_MODE::TPS) {
				it->draw();
			}
		}
		else {
			it->draw();
		}
	}
}
