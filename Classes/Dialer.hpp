inline int Dialer::Update() noexcept {
	lineNumber = UpdateCore(lineNumber);
	return lineNumber ? 0 : -1;
}

inline int Dialer::UpdateCore(int const& lineNumber) noexcept {
	// todo: client network logic here
	COR_BEGIN
	LabDial:
		// clear flag
		finished = false;

		// event: set flag
		OnAccept = [this](auto peer) {
			finished = true;
		};

		// try connect to server
		Dial("127.0.0.1", 12345, 2000);

		// clear recv datas( 这个时机清的比较彻底. 因为 Dial 函数会 Cleanup, 清除原有 Peer, 如果存在的话 )
		recvs.clear();

		// wait connected or timeout
		while (!finished) {
			COR_YIELD
		}

		// timeout ?
		if (!peer) {
			// todo: sleep?
			goto LabDial;
		}

		// send enter package
		xx::MakeTo(pkgEnter);
		if (r = peer->SendPush(pkgEnter)) {
			// todo: log?
			goto LabDial;
		}

		// wait recv data
		waitMS = xx::NowSteadyEpochMS() + 5000;	// 5 sec timeout
		while (!recvs.size()) {
			COR_YIELD
			if (xx::NowSteadyEpochMS() > waitMS) goto LabDial;
		}

		// first package handle
		if (r = HandleFirstPackage()) {
			// todo: show error?
			goto LabDial;
		}

		while (!peer->Disposed()) {				// disconnect checker
			if (r = HandlePackages()) {
				// todo: show error?
				goto LabDial;
			}
			COR_YIELD
		}
	COR_END
}

inline int Dialer::HandleFirstPackage() noexcept {
	switch (recvs.front()->GetTypeId()) {
	case xx::TypeId_v<PKG::CatchFish_Client::EnterSuccess>: {
		auto&& es = xx::As<PKG::CatchFish_Client::EnterSuccess>(recvs.front());
		for (auto&& p : *es->players) {
			::catchFish->players.Add(xx::As<Player>(p));
		}
		::catchFish->scene = xx::As<Scene>(es->scene);
		player = xx::As<Player>(es->self.lock());
		recvs.pop_front();
		return 0;
	}
	case xx::TypeId_v<PKG::Generic::Error>: {
		// todo: show error msg?
		return -1;
	}
	default: {
		// todo: log?
		return -1;
	}
	}
	assert(false);
}

inline int Dialer::HandlePackages() noexcept {
	// todo: handle frame events
	while (!recvs.empty()) {
		switch (recvs.front()->GetTypeId()) {
		case xx::TypeId_v<PKG::CatchFish_Client::FrameEvents>: {
			auto&& fe = xx::As<PKG::CatchFish_Client::FrameEvents>(recvs.front());

		}
		case xx::TypeId_v<PKG::Generic::Error>: {
			// todo: show error msg?
			return -1;
		}
		default: {
			// todo: log?
			return -1;
		}
		}
		// recvs.front()
		recvs.pop_front();
	}
	return 0;
}

inline void Dialer::Reset() noexcept {
	recvs.clear();
	player.reset();
	::catchFish->players.Clear();
	::catchFish->scene.reset();
}
