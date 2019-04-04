inline int Scene::InitCascade(void* const& o) noexcept {
	// 将 scene 指针刷下去保存在类里面以便于使用
	return this->BaseType::InitCascade(this);
}

inline int Scene::Update(int const&) noexcept {
	// 遍历更新. 倒序扫描, 交换删除. 如果存在内部乱序删除的情况, 则需要 名单机制 或 标记机制 在更新结束之后挨个删掉
	auto&& fishs = *this->fishs;
	if (fishs.len) {
		for (size_t i = fishs.len - 1; i != -1; --i) {
			if (fishs[i]->Update(frameNumber)) {
				fishs[fishs.len - 1]->indexAtContainer = i;
				fishs.SwapRemoveAt(i);
			}
		}
	}
	// todo: foreach fishs, players, ..... call Update

	++frameNumber;
	return 0;
};
