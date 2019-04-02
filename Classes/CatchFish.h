#pragma once
#include "xx_pos.h"
#include "PKG_class.h"
#include "chipmunk.h"

struct Physics : PKG::CatchFish::Configs::Physics {
	cpSpace* space = nullptr;
	inline void InitCascade() noexcept override {
		// todo: space = ......
	}
	~Physics() {
		if (space) {
			// todo: release
		}
	}
};
using Physics_s = std::shared_ptr<Physics>;


struct Scene : PKG::CatchFish::Scene {

	// 自减id ( 从 -1 开始, 用于服务器下发鱼生成 )
	int autoDecId = 0;

	// server 下发鱼生成专用
	xx::Random rnd2;
};
using Scene_s = std::shared_ptr<Scene>;
using Scene_w = std::weak_ptr<Scene>;

// 放在全局方便到处使用. 在 CatchFish.Init 里赋值
inline Scene* scene = nullptr;
inline PKG::CatchFish::Configs::Config* cfg = nullptr;

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


struct Fish : PKG::CatchFish::Fish {
	// todo: cocos sprite, node...
	PKG::CatchFish::Configs::Fish* cfg = nullptr;

	inline void InitCascade() noexcept override {
		cfg = &*::cfg->fishs->At(0);
		if (cfg->minCoin == cfg->maxCoin) {
			this->coin = cfg->minCoin;
		}
		else {
			this->coin = scene->rnd->Next(cfg->minCoin, cfg->maxCoin + 1);
		}

		//cfgId = 0;
		speedScale = 1;
		scale = 1;
		wayIndex = 0;
		wayPointIndex = 0;
		wayPointDistance = 0;
		spriteFrameIndex = 0;
		frameRatio = 1;
		reverse = false;


		// todo: sprite create ....
	}

	~Fish() {
		// todo: sprite release ....
	}
};
using Fish_s = std::shared_ptr<Fish>;
using Fish_w = std::weak_ptr<Fish>;

// todo: more 

struct CatchFish {
	CatchFish() {
		// 注册替代类型
		xx::BBuffer::Register<Physics>(xx::TypeId_v<PKG::CatchFish::Configs::Physics>);
		xx::BBuffer::Register<Scene>(xx::TypeId_v<PKG::CatchFish::Scene>);
		xx::BBuffer::Register<Player>(xx::TypeId_v<PKG::CatchFish::Player>);
		// todo: more
	}
	CatchFish(CatchFish&& o) = default;
	CatchFish(CatchFish const& o) = delete;
	CatchFish& operator=(CatchFish&& o) = default;
	CatchFish& operator=(CatchFish const& o) = delete;

	PKG::CatchFish::Configs::Config_s cfg;
	xx::List<Player_s> players;
	Scene_s scene;
	Player_w selfPlayer;	// 指向玩家自己的 player


	inline int Init(std::string cfgName) {
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
		cfg->InitCascade();
		::cfg = &*cfg;									// 存到全局方便访问( 单线程适用 )

		// 模拟收到 sync( 含 players & scene )
		xx::MakeTo(scene);
		::scene = &*scene;								// 存到全局方便访问( 单线程适用 )
		xx::MakeTo(scene->borns);
		xx::MakeTo(scene->fishs);
		xx::MakeTo(scene->freeSits);
		xx::MakeTo(scene->items);
		xx::MakeTo(scene->players);
		xx::MakeTo(scene->rnd, 123);
		xx::MakeTo(scene->stage);

		auto&& plr = xx::Make<Player>();
		selfPlayer = plr;
		plr->scene = ::scene;
		players.Add(std::move(plr));

		scene->players->Add(plr);

		auto&& fish = xx::Make<Fish>();
		fish->cfgId = 0;
		fish->InitCascade();	// fill cfg
		fish->way = MakeBeeline(fish->cfg->maxDetectRadius);

		scene->fishs->Add(fish);


		return 0;
	}

	inline int Update() {
		return 0;
	}

	bool disposed = false;
	void Dispose(int const& flag) noexcept {
		if (disposed) return;
		// todo
	}


	static const int ScreenWidth = 1280;
	static const int ScreenHeight = 720;

	// 填充随机生成的有限角度的能立即出现在屏幕上的线段路径
	// -45 ~ 45, 135 ~ 225 在这两段角度之间随机一个角度值,  + 180 之后的 45 度范围内再次随机一个角度, 用旋转函数转为两个坐标点. 连为1根直线, 最后找出安全出生框与直线的交点
	// 由于最终计算出两个交点之后, 可以通过交换顺序的方式反向, 故只需要一段角度作为起始角度即可. 简化起见, 直接 135 ~ 225 ( 不考虑开区间误差 )
	inline PKG::CatchFish::Way_s MakeBeeline(int const& itemRadius) {
		auto&& w = (ScreenWidth + itemRadius) / 2.0f;
		auto&& h = (ScreenHeight + itemRadius) / 2.0f;
		auto&& a = scene->rnd->Next(135, 225);
		auto&& p1 = xx::Rotate(xx::Pos{ 1, 0 }, a / 180.0f * float(M_PI));
		xx::Pos abs{ std::fabs(p1.x), std::fabs(p1.y) };
		if (abs.x / w > abs.y / h) {
			p1 = p1 * (w / abs.x);
		}
		else {
			p1 = p1 * (h / abs.y);
		}
		a = a + 180 - 23 + scene->rnd->Next(46);
		auto&& p2 = xx::Rotate(xx::Pos{ 1, 0 }, a / 180.0f * float(M_PI));
		abs.x = std::fabs(p2.x);
		abs.y = std::fabs(p2.y);
		if (abs.x / w> abs.y / h) {
			p2 = p2 * (w / abs.x);
		}
		else {
			p2 = p2 * (h / abs.y);
		}

		if (scene->rnd->Next(2) == 1) {
			std::swap(p1, p2);
		}
		auto&& way = xx::Make<PKG::CatchFish::Way>();
		xx::MakeTo(way->points);
		way->points->Add(PKG::CatchFish::WayPoint{ p1, xx::GetAngle(p1, p2) / float(M_PI) * 180.0f, p1.distance(p2) });
		way->points->Add(PKG::CatchFish::WayPoint{ p2, 0, 0 });	// 非循环轨迹最后个点距离和角度不用计算, 也不做统计
		way->distance = way->points->At(0).distance;
		way->loop = false;
		return way;
	}

};
using CatchFish_s = std::shared_ptr<CatchFish>;
