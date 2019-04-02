#pragma once
#include "xx_pos.h"
#include "PKG_class.h"

using namespace PKG::CatchFish;
struct CatchFish {
	CatchFish() = default;
	CatchFish(CatchFish&& o) = default;
	CatchFish(CatchFish const& o) = delete;
	CatchFish& operator=(CatchFish&& o) = default;
	CatchFish& operator=(CatchFish const& o) = delete;

	Configs::Config_s cfg;
	xx::List<Player_s> players;
	Scene_s scene;

	inline int Init(std::string cfgName) {
		auto&& data = cocos2d::FileUtils::getInstance()->getDataFromFile(cfgName);
		xx::BBuffer bb;
		bb.Reset(data.getBytes(), data.getSize());
		auto&& r = bb.ReadRoot(cfg);
		bb.Reset();
		return r;
	}

	inline int Update() {
		return 0;
	}

	bool disposed = false;
	void Dispose(int const& flag) noexcept {
		if (disposed) return;
		// todo
	}
};
using CatchFish_s = std::shared_ptr<CatchFish>;
