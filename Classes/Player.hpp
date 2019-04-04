inline int Player::InitCascade(void* const& o) noexcept {
	scene = (Scene*)o;

	// 前置填充
	for (auto&& cannon : *cannons) {
		xx::As<Cannon>(cannon)->player = this;
	}

	// 开始瀑布初始化
	return this->PKG::CatchFish::Player::InitCascade(o);
}

inline int Player::Update(int const& frameNumber) noexcept {
	// foreach cannon
	return 0;
};
