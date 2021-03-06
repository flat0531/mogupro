#pragma once

#include <Utility/cSingletonAble.h>
#include <memory>
#include "cSceneBase.h"

//! @file cSceneManager
//! @brief Scene切り替えを含め現在のSceneのすべてを管理するクラスです
//! @note 
//!      Changeで引数にSceneBaseを継承したClassを渡してシーンを変えます
//!      そこで判断しています
//! @date 2017-10-12
//! @author Taka Nomoto

class cSceneManager : public Utility::cSingletonAble<cSceneManager>
{
public:
	cSceneManager() = default;

	template<class scene,class... T>
	void change(T...args)
	{
		mSceneBase = std::make_shared<scene>(args...);
	};

	cSceneBase& now()
	{
		return *mSceneBase;
	};

	template<class scene, class... T>
	void shift(T...args)
	{
		if ( mSceneBase )
		{
			mSceneBase->shutDown( );
		}
		change<scene>(args...);
		mSceneBase->setup();
	}
private:

	std::shared_ptr<cSceneBase> mSceneBase;

};