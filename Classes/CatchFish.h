#pragma once
#include "xx_pos.h"
#include "PKG_class.h"
#include "chipmunk.h"

#define DRAW_PHYSICS_POLYGON 0

static constexpr int ScreenWidth = 1280;
static constexpr int ScreenHeight = 720;
static constexpr float ScreenWidthRatio = float(ScreenWidth) / float(ScreenWidth + ScreenHeight);

struct Scene;
inline Scene* scene = nullptr;
inline PKG::CatchFish::Configs::Config* cfg = nullptr;
#ifdef CC_TARGET_PLATFORM
inline cocos2d::Scene* cc_scene = nullptr;
inline xx::List<cocos2d::Touch*> touchs;
#endif

#include "SpriteFrame.h"
#include "Physics.h"
#include "Scene.h"
#include "Player.h"
#include "Fish.h"

// todo: more 
struct Cannon : PKG::CatchFish::Cannon {
	PKG::CatchFish::Configs::Cannon* cfg = nullptr;
	xx::Pos pos;
	Player* owner = nullptr;

	inline int InitCascade() noexcept override {
		assert(!cfg);
		cfg = &*::cfg->cannons->At(cfgId);
		pos = ::cfg->sitPositons->At((int)owner->sit);
		DrawInit();
		return 0;
	}

	inline virtual int UpdateVoid() noexcept override {
		// todo: scan input, calc angle, shoot logic here

		DrawUpdate();
		return 0;
	};

	~Cannon() {
		DrawClear();
	}

#pragma region

#ifdef CC_TARGET_PLATFORM
	cocos2d::Sprite* body = nullptr;
#endif

	inline virtual void DrawInit() {
#ifdef CC_TARGET_PLATFORM
		assert(!body);
		body = cocos2d::Sprite::create();
		body->retain();
		body->setGlobalZOrder(cfg->zOrder);
		auto&& sf = xx::As<SpriteFrame>(cfg->frames->At(0))->spriteFrame;
		body->setSpriteFrame(sf);
		body->setPosition(pos);
		body->setScale(cfg->scale);
		body->setRotation(-angle);
		cc_scene->addChild(body);
#endif
	}

	inline virtual void DrawUpdate() {
#ifdef CC_TARGET_PLATFORM
		assert(body);
		//body->setRotation(-angle);
#endif
	}

	inline virtual void DrawClear() {
#ifdef CC_TARGET_PLATFORM
		if (!body) return;

		if (body->getParent()) {
			body->removeFromParent();
		}
		body->release();
		body = nullptr;
#endif
	}

#pragma endregion

};



struct CatchFish {
	CatchFish() {
		// 注册替代类型
		xx::BBuffer::Register<Physics>(xx::TypeId_v<PKG::CatchFish::Configs::Physics>);
		xx::BBuffer::Register<Scene>(xx::TypeId_v<PKG::CatchFish::Scene>);
		xx::BBuffer::Register<Player>(xx::TypeId_v<PKG::CatchFish::Player>);
		xx::BBuffer::Register<Fish>(xx::TypeId_v<PKG::CatchFish::Fish>);
		xx::BBuffer::Register<SpriteFrame>(xx::TypeId_v<PKG::CatchFish::Configs::SpriteFrame>);
		// todo: more
	}
	CatchFish(CatchFish&& o) = default;
	CatchFish(CatchFish const& o) = delete;
	CatchFish& operator=(CatchFish&& o) = default;
	CatchFish& operator=(CatchFish const& o) = delete;

	PKG::CatchFish::Configs::Config_s cfg;
	xx::List<Player_s> players;
	Scene_s scene;
	Player_w selfPlayer;				// 指向玩家自己的 player

	int Init(std::string cfgName);
	int Update();
	PKG::CatchFish::Way_s MakeBeeline(int const& itemRadius);
	Fish_s MakeRandomFish();

	bool disposed = false;
	void Dispose(int const& flag) noexcept {
		if (disposed) return;
		// todo: release all
	}

};
using CatchFish_s = std::shared_ptr<CatchFish>;

#include "CatchFish_MakeBeeline.h"
#include "CatchFish_Update.h"
#include "CatchFish_Init.h"
#include "CatchFish_MakeRandomFish.h"
