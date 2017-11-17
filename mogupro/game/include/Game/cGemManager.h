#pragma once
#include "Game/Gem/cGem.h"
#include "Utility/cSingletonAble.h"
#include "cinder\/Surface.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Context.h"
#include "cinder/gl/Texture.h"
#include "CameraManager\cCameraManager.h"
#include "Utility\cTimeMeasurement.h"
#include "cPlayerManager.h"
#include <Utility/cSingletonAble.h>
#include <memory>
#include <cinder/Rand.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/gl/Texture.h>
#include <vector>
#include <map>
#define GemManager Game::cGemManager::getInstance()



namespace Game
{
	class cGemManager : public Utility::cSingletonAble<cGemManager>
	{
	public:

		cGemManager() {}
		~cGemManager() {}
		// position           中心座標(基準値)   x,y,z全て+方向にマップチップで生成します。
		// randomRange        ランダムの生成範囲(片方陣地のマップサイズ)
		// mapChipSize        マップチップ一マスの大きさ
		// gemScale           ジェム一個の大きさ
		// gemMaxNum          gemの生成数
		// seed               シード値(現在は入力しても変動なし)
		void setUp(ci::vec3 position, ci::vec3 randomRange, float mapChipSize, float gemScale, int gemMaxNum, unsigned long seed);
		void draw();
		void drawFbo();
		void update( float delta );

		void create();
		// team     0がfirst,1がsecond
		void gemCountUp(int team, Gem::GemType type);
		void gemDelete(int it);
		void buildMesh();
		//oldcode-----------
		std::vector<std::shared_ptr<Gem::cGem>> getGems() { return mStaticGem; }
		//-------------------
		std::shared_ptr<Gem::cGem> FindGem(int id);
		std::shared_ptr<Gem::cGem> AcquisitionGem(int id);

	private:

		void sort();

		const int mDrawNum = 100;

		std::vector<std::shared_ptr<Gem::cGem>> mStaticGem;
		std::vector<std::shared_ptr<Gem::cGem>> mActiveGem;
		ci::TriMeshRef mesh;

		std::vector<std::shared_ptr<Gem::cGem>> mGemsptr;
		std::map<Gem::GemType, int> mTeamGems[2];
		ci::vec3 mPosition;
		ci::vec3 mRandomRange;
		float mMapChipSize;
		float mGemScale;
		int mGemMaxNum;
		ci::gl::FboRef mGemBuffer;
		ci::gl::GlslProgRef mShader;
		ci::gl::GlslProgRef mHShader;
		ci::gl::GlslProgRef mVShader;
		ci::gl::GlslProgRef mVboShader;
		ci::gl::VboMeshRef mGemsVbo;
		float blurSize;
		float mBloom;
		unsigned long mSeed;
		float mTime;
	};
}