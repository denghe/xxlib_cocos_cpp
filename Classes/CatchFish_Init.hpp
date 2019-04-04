inline int CatchFish::Init(std::string cfgName) {
#ifdef CC_TARGET_PLATFORM
	assert(!cc_scene);
	// 初始化 cocos 相关
	cc_scene = cocos2d::Director::getInstance()->getRunningScene();
	cc_listener = cocos2d::EventListenerTouchAllAtOnce::create();
	cc_listener->retain();
	cc_listener->onTouchesBegan = [](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e) {
		cc_touchs.AddRange(ts.data(), ts.size());
	};
	cc_listener->onTouchesMoved = [](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e) {
	};
	cc_listener->onTouchesEnded = [](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e) {
		for (auto&& t : ts) {
			cc_touchs.Remove(t);
		}
	};
	cc_listener->onTouchesCancelled = cc_listener->onTouchesEnded;
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(cc_listener, cc_scene);
#endif

	// 从文件加载 cfg. 出问题返回非 0
	{
		auto&& data = cocos2d::FileUtils::getInstance()->getDataFromFile(cfgName);
		xx::BBuffer bb;
		bb.Reset(data.getBytes(), data.getSize());
		auto&& r = bb.ReadRoot(cfg);
		bb.Reset();
		if (r) return r;
	}

	// 初始化派生类的东西
	if (int r = cfg->InitCascade()) return r;

	// 模拟收到 sync( 含 players & scene )
	xx::MakeTo(scene);
	xx::MakeTo(scene->borns);
	xx::MakeTo(scene->fishs);
	xx::MakeTo(scene->freeSits);
	xx::MakeTo(scene->items);
	xx::MakeTo(scene->players);
	xx::MakeTo(scene->rnd, 123);
	xx::MakeTo(scene->stage);

	auto&& plr = xx::Make<Player>();
	players.Add(std::move(plr));
	scene->players->Add(plr);

	//auto&& cannon = xx::Make<Cannon>();
	// todo

	// todo
	// selfPlayer = plr;

	scene->cfg = &*cfg;
	return scene->InitCascade();
}
