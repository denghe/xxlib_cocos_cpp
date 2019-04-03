inline int CatchFish::Update() {
	// 遍历更新. 倒序扫描, 交换删除. 如果存在内部乱序删除的情况, 则需要 名单机制 或 标记机制 在更新结束之后挨个删掉
	auto&& fishs = *scene->fishs;
	if (fishs.len) {
		for (size_t i = fishs.len - 1; i != -1; --i) {
			if (fishs[i]->UpdateVoid()) {
				fishs[fishs.len - 1]->indexAtContainer = i;
				fishs.SwapRemoveAt(i);
			}
		}
	}

	// test: 自动补鱼
	while (fishs.len < 10000) {
		fishs.Add(MakeRandomFish());
	}
	return 0;
}

