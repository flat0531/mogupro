#pragma once
#include<Game\Weapons\SubWeapon\SubWeaponType.h>
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
namespace Game
{
namespace Capsule
{
class cCapsuleBase
{
public:
    cCapsuleBase( );
	virtual  ~cCapsuleBase( );
	virtual void setup(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type) = 0;
	virtual void update(const float& delta_time) = 0;
	virtual void draw() = 0;
	virtual bool deleteThis();
	Game::Weapons::SubWeapon::SubWeaponType getType();
	void setType(const Game::Weapons::SubWeapon::SubWeaponType type);
	ci::vec3 getPos();
	ci::vec3 getScale();                                                                                       
	void setPos(const ci::vec3 pos);
	void setScale(const ci::vec3 scale);
	ci::AxisAlignedBox getAABB();
	bool getIsget();
	void setIsget(const bool isget);
protected:
	ci::vec3 mPos;
	ci::vec3 mScale;
	ci::vec3 mRotate;
	ci::AxisAlignedBox mAABB;
	Game::Weapons::SubWeapon::SubWeaponType mType;
	bool mIsget = false;
private:

};
}
}
