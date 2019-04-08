inline void Dialer::Accept(std::shared_ptr<xx::UvKcpBasePeer> peer_) noexcept {
	auto&& peer = xx::As<ClientPeer>(peer_);
	peer->dialer = this;
}

inline int Dialer::Update() noexcept {
	lineNumber = UpdateCore(lineNumber);
	return lineNumber ? 0 : -1;
}

inline int Dialer::UpdateCore(int const& lineNumber) noexcept {
	// todo: client network logic here
	COR_BEGIN
		while (true) {
			COR_YIELD
		}
	COR_END
}
