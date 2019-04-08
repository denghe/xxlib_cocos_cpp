inline int CatchFish::Update() noexcept {
#ifdef CC_TARGET_PLATFORM
	dialer->lineNumber = dialer->UpdateCore(dialer->lineNumber);
	if (!dialer->lineNumber) return -1;
#endif

	return scene->Update();
}
