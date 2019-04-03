struct SpriteFrame : PKG::CatchFish::Configs::SpriteFrame {
#ifdef CC_TARGET_PLATFORM
	cocos2d::SpriteFrame* spriteFrame = nullptr;
#endif
	inline int InitCascade() noexcept override {
#ifdef CC_TARGET_PLATFORM
		auto&& sfc = cocos2d::SpriteFrameCache::getInstance();
		sfc->addSpriteFramesWithFile(*this->plistName);
		spriteFrame = sfc->getSpriteFrameByName(*this->frameName);
		if (!spriteFrame) return -1;
		spriteFrame->retain();
		return 0;
#endif
	}
	~SpriteFrame() {
#ifdef CC_TARGET_PLATFORM
		if (spriteFrame) {
			spriteFrame->release();
			spriteFrame = nullptr;
		}
#endif
	}
};
using SpriteFrame_s = std::shared_ptr<SpriteFrame>;
