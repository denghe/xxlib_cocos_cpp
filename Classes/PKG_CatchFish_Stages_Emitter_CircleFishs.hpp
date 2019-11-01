﻿inline int PKG::CatchFish::Stages::Emitter_CircleFishs::InitCascade(void* const& o) noexcept {
	scene = (PKG::CatchFish::Scene*)o;
	return InitCascadeCore(o);
}

inline int PKG::CatchFish::Stages::Emitter_CircleFishs::Update(int const& ticks) noexcept {
	if (bornAvaliableTicks <= ticks) {
		auto&& fishCfg = scene->cfg->fishs->At(0);	// todo: 指定鱼的类型, 根据类型路由创建函数?
		auto&& fish = xx::Make<PKG::CatchFish::Fish>();
		fish->scene = scene;
		fish->id = ++scene->autoIncId;
		fish->cfgId = fishCfg->id;
		fish->cfg = &*fishCfg;
		fish->coin = cfg_coin;
		fish->scale = cfg_scale;
		fish->speedScale = 1;
		fish->spriteFrameIndex = 0;
		fish->pos = xx::Pos{ 0,0 };
		fish->angle = angle;
		fish->moveInc = xx::Rotate({ cfg_speed ,0 }, angle);
		fish->frameRatio = 1;
		angle += cfg_angleIncrease;

		fish->indexAtContainer = (int)scene->fishs->len;
		scene->fishs->Add(fish);
#ifdef CC_TARGET_PLATFORM
		fish->DrawInit();
#endif

		// 计算下次可生成鱼的时间点
		bornAvaliableTicks = ticks + cfg_bornTicksInterval;
	}
	return 0;
}
