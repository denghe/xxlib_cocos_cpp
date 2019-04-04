inline int Cannon::InitCascade(void* const& o) noexcept {
	scene = (Scene*)o;
	assert(!cfg);
	cfg = &*scene->cfg->cannons->At(cfgId);
	pos = scene->cfg->sitPositons->At((int)player->sit);
	DrawInit();
	return 0;
}

inline int Cannon::Update(int const& frameNumber) noexcept {
	// todo: scan input, calc angle, shoot logic here
	// foreach bullet
	DrawUpdate();
	return 0;
};

Cannon::~Cannon() {
	DrawDispose();
}

#pragma region

#ifdef CC_TARGET_PLATFORM
cocos2d::Sprite* body = nullptr;
#endif

inline void Cannon::DrawInit() noexcept {
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

inline void Cannon::DrawUpdate() noexcept {
#ifdef CC_TARGET_PLATFORM
	assert(body);
	//body->setRotation(-angle);
#endif
}

inline void Cannon::DrawDispose() noexcept {
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