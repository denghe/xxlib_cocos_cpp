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
