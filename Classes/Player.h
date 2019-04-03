struct Player : PKG::CatchFish::Player {
	// 所在场景
	Scene* scene = nullptr;

	// 当 Client 通过 Lobby 服务到 Game 发 Enter 时, Game 需要生成一个 token 以便 Client Enter 时传入以校验身份
	std::string token;

	// 开炮等行为花掉的金币数汇总 ( 统计 )
	int64_t consumeCoin = 0;

	// todo: peer, recv, ....
};
using Player_s = std::shared_ptr<Player>;
using Player_w = std::weak_ptr<Player>;
