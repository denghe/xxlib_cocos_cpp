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

		// cleanup context data
		Reset();

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

		// store players
		for (auto&& p : *es->players) {
			::catchFish->players.Add(xx::As<Player>(p));
		}

		// store scene
		::catchFish->scene = xx::As<Scene>(es->scene);

		// store current player
		player = xx::As<Player>(es->self.lock());

		// set current player's flag
		player->isSelf = true;

		// restore scene
		if (int r = ::catchFish->scene->InitCascade()) return r;

		// restore player
		for (auto&& p : ::catchFish->players) {
			if (int r = p->InitCascade(&*::catchFish->scene)) return r;
		}

		// handle finish, pop / drop.
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
	int r = 0;
	while (!recvs.empty()) {
		switch (recvs.front()->GetTypeId()) {
		case xx::TypeId_v<PKG::CatchFish_Client::FrameEvents>: {
			auto&& fe = xx::As<PKG::CatchFish_Client::FrameEvents>(recvs.front());
			// 如果本地帧编号慢于 server 则追帧
			while (fe->frameNumber > ::catchFish->scene->frameNumber) {
				::catchFish->scene->Update();
			}
			// 依次处理事件集合
			for (auto&& e : *fe->events) {
				switch (e->GetTypeId()) {
				case xx::TypeId_v<PKG::CatchFish::Events::Enter>:
					r = Handle(xx::As<PKG::CatchFish::Events::Enter>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::Leave>:
					r = Handle(xx::As<PKG::CatchFish::Events::Leave>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::NoMoney>:
					r = Handle(xx::As<PKG::CatchFish::Events::NoMoney>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::Refund>:
					r = Handle(xx::As<PKG::CatchFish::Events::Refund>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::FishDead>:
					r = Handle(xx::As<PKG::CatchFish::Events::FishDead>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::PushWeapon>:
					r = Handle(xx::As<PKG::CatchFish::Events::PushWeapon>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::PushFish>:
					r = Handle(xx::As<PKG::CatchFish::Events::PushFish>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::OpenAutoLock>:
					r = Handle(xx::As<PKG::CatchFish::Events::OpenAutoLock>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::Aim>:
					r = Handle(xx::As<PKG::CatchFish::Events::Aim>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::CloseAutoLock>:
					r = Handle(xx::As<PKG::CatchFish::Events::CloseAutoLock>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::OpenAutoFire>:
					r = Handle(xx::As<PKG::CatchFish::Events::OpenAutoFire>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::CloseAutoFire>:
					r = Handle(xx::As<PKG::CatchFish::Events::CloseAutoFire>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::Fire>:
					r = Handle(xx::As<PKG::CatchFish::Events::Fire>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::CannonSwitch>:
					r = Handle(xx::As<PKG::CatchFish::Events::CannonSwitch>(e)); break;
				case xx::TypeId_v<PKG::CatchFish::Events::CannonCoinChange>:
					r = Handle(xx::As<PKG::CatchFish::Events::CannonCoinChange>(e)); break;
				default:
					// todo: log?
					assert(false);	// 不该执行到这里
				}
				if (r) return r;
			}
		}
		default: {
			// todo: log?
			return -1;
		}
		}
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


inline int Dialer::Handle(PKG::CatchFish::Events::Enter_s o) noexcept {
	auto&& p = xx::Make<Player>();

	return 0;
}
inline int Dialer::Handle(PKG::CatchFish::Events::Leave_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::NoMoney_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::Refund_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::FishDead_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::PushWeapon_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::PushFish_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::OpenAutoLock_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::Aim_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::CloseAutoLock_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::OpenAutoFire_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::CloseAutoFire_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::Fire_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::CannonSwitch_s o) noexcept { return 0; }
inline int Dialer::Handle(PKG::CatchFish::Events::CannonCoinChange_s o) noexcept { return 0; }
