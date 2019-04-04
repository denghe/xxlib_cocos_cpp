inline int CatchFish::Update() {
	scene->Update();

	// 自动补鱼
	if (scene->fishs->len < 1000) {
		scene->fishs->Add(MakeRandomFish());
	}

	return 0;
}
