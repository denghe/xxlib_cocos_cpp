#pragma once
#include "xx_pos.h"
#include "PKG_class.h"
#include "chipmunk.h"

// todo: 切割下面的类到独立文件

#define DRAW_PHYSICS_POLYGON 0

// 放在全局方便到处使用. 在 CatchFish.Init 里赋值
struct Scene;
inline Scene* scene = nullptr;
inline PKG::CatchFish::Configs::Config* cfg = nullptr;
inline cocos2d::Scene* cc_scene = nullptr;


cocos2d::Vec2& Vec2(xx::Pos& pos) {
	return *(cocos2d::Vec2*)&pos;
}
cocos2d::Vec2 Vec2(xx::Pos const& pos) {
	return *(cocos2d::Vec2*)&pos;
}

struct SpriteFrame : PKG::CatchFish::Configs::SpriteFrame {
	cocos2d::SpriteFrame* spriteFrame = nullptr;
	inline void InitCascade() noexcept override {
		auto&& sfc = cocos2d::SpriteFrameCache::getInstance();
		sfc->addSpriteFramesWithFile(*this->plistName);
		spriteFrame = sfc->getSpriteFrameByName(*this->frameName);
		spriteFrame->retain();
		// todo: check spriteFrame null
		// todo: InitCascade shoud be return int value
	}
	~SpriteFrame() {
		if (spriteFrame) {
			spriteFrame->release();
			spriteFrame = nullptr;
		}
	}
};


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
	PKG::CatchFish::Configs::Fish* cfg = nullptr;

	inline void InitCascade() noexcept override {
		assert(!cfg);
		cfg = &*::cfg->fishs->At(cfgId);

		DrawInit();
	}

#ifdef CC_TARGET_PLATFORM
	cocos2d::Sprite* body = nullptr;
	cocos2d::Sprite* shadow = nullptr;
#if DRAW_PHYSICS_POLYGON
	cocos2d::DrawNode* debugNode = nullptr;
#endif
#endif

	inline virtual void DrawInit() {
#ifdef CC_TARGET_PLATFORM
		assert(!body);
		body = cocos2d::Sprite::create();
		body->setGlobalZOrder(cfg->zOrder);
		body->retain();

		shadow = cocos2d::Sprite::create();
		shadow->setGlobalZOrder(cfg->zOrder);
		shadow->setColor(cocos2d::Color3B::BLACK);
		shadow->setOpacity(125);
		shadow->retain();
#if DRAW_PHYSICS_POLYGON
		debugNode = cocos2d::DrawNode::create();
		debugNode->setGlobalZOrder(cfg->zOrder);
		debugNode->retain();
#endif
		DrawUpdate();
		cc_scene->addChild(shadow);
		cc_scene->addChild(body);
#if DRAW_PHYSICS_POLYGON
		cc_scene->addChild(debugNode);
#endif
#endif
	}

	inline virtual void DrawUpdate() {
#ifdef CC_TARGET_PLATFORM
		assert(body);
		auto&& sf = xx::As<SpriteFrame>(cfg->moveFrames->At(spriteFrameIndex)->frame)->spriteFrame;

		// 设鱼的帧图, 坐标, 方向, 缩放
		body->setSpriteFrame(sf);
		body->setRotation(-angle);
		body->setPosition(Vec2(pos));
		body->setScale(scale * cfg->scale);

		shadow->setSpriteFrame(sf);
		shadow->setRotation(-angle);
		shadow->setPosition(Vec2(pos + cfg->shadowOffset));
		shadow->setScale(scale * cfg->scale * cfg->shadowScale);
#if DRAW_PHYSICS_POLYGON
		// 碰撞多边形显示
		debugNode->setPosition(Vec2(pos));
		debugNode->setRotation(-angle);
		debugNode->setScale(scale * cfg->scale);
		auto&& sfs = cfg->moveFrames->At(spriteFrameIndex);
		if (sfs->physics) {
			debugNode->clear();
			debugNode->drawCircle({ 0,0 }, cfg->maxDetectRadius, 0, 50, true, cocos2d::Color4F::RED);
			for (auto&& polygon : *sfs->physics->polygons) {
				for (size_t i = 0; i < polygon->len - 1; ++i) {
					debugNode->drawLine(Vec2(polygon->At(i)), Vec2(polygon->At(i + 1)), cocos2d::Color4F::GREEN);
				}
				debugNode->drawLine(Vec2(polygon->At(0)), Vec2(polygon->At(polygon->len - 1)), cocos2d::Color4F::GREEN);
			}
		}
#endif
#endif
	}

	inline virtual void DrawClear() {
#ifdef CC_TARGET_PLATFORM
		if (!body) return;

		if (body->getParent()) {
			body->removeFromParent();
		}
		body->release();
		body = nullptr;

		if (shadow->getParent()) {
			shadow->removeFromParent();
		}
		shadow->release();
		shadow = nullptr;

#if DRAW_PHYSICS_POLYGON
		if (debugNode->getParent()) {
			debugNode->removeFromParent();
		}
		debugNode->release();
		debugNode = nullptr;
#endif
#endif
	}

	inline virtual int UpdateVoid() noexcept override {
		// 取到当前帧结束时应该前进的距离
		auto&& dist = cfg->moveFrames->At(spriteFrameIndex)->moveDistance * speedScale;

		// 帧下标循环前进
		if (++spriteFrameIndex == cfg->moveFrames->len) {
			spriteFrameIndex = 0;
		}

		// 定位到轨迹
		auto&& way = this->way ? this->way : cfg->ways->At(wayIndex);

	LabKeepMoving:
		// 试累加鱼当前鱼点移动距离. 如果跨点, 则用多个点的距离来凑够
		// 如果累加距离之后跨点了, 则将 dist 视作可能跨更多点的距离容器, 继续用下一个点的距离从中减去

		auto&& p = &way->points->At(wayPointIndex);
		auto&& left = p->distance - wayPointDistance;
		if (dist > left) {
			// 循环鱼线: 最后一个点指向第一个点
			if (way->loop) {
				// 从 dist 中减去当前鱼线点剩余距离
				dist -= left;
				wayPointDistance = 0;

				// 指向下一个鱼线点, 如果到终点, 就指向起点
				if (++wayPointIndex == way->points->len) {
					wayPointIndex = 0;
				}

				// 继续 while 从 dist 减 p->distance
				goto LabKeepMoving;
			}
			// 如果还有下一个鱼线点存在( 最后一个点不算 )
			else if (wayPointIndex + 2 < (int)way->points->len) {
				// 从 dist 中减去当前鱼线点剩余距离
				dist -= left;
				wayPointDistance = 0;

				// 指向下一个鱼线点
				++wayPointIndex;

				// 继续 while 从 dist 减 p->distance
				goto LabKeepMoving;
			}
			else
				return -1;			// 通知删鱼
		}
		else {
			wayPointDistance += dist;
		}

		// 按当前鱼线点上已经前进的距离, 结合下一个点的坐标, 按比例修正 p 坐标 & 角度
		if (wayPointIndex == way->points->len - 1) {
			auto&& np = &way->points->At(0);
			pos = p->pos + ((np->pos - p->pos) * (wayPointDistance / p->distance));
		}
		else {
			pos = p->pos + (((p + 1)->pos - p->pos) * (wayPointDistance / p->distance));
		}

		angle = p->angle;
		DrawUpdate();
		return 0;
	};

	~Fish() {
		DrawClear();
	}
};
using Fish_s = std::shared_ptr<Fish>;
using Fish_w = std::weak_ptr<Fish>;




// todo: more 

// todo: 声明实现分离, 将实现切割到独立文件

struct CatchFish {
	CatchFish() {
		// 注册替代类型
		xx::BBuffer::Register<Physics>(xx::TypeId_v<PKG::CatchFish::Configs::Physics>);
		xx::BBuffer::Register<Scene>(xx::TypeId_v<PKG::CatchFish::Scene>);
		xx::BBuffer::Register<Player>(xx::TypeId_v<PKG::CatchFish::Player>);
		xx::BBuffer::Register<Fish>(xx::TypeId_v<PKG::CatchFish::Fish>);
		xx::BBuffer::Register<SpriteFrame>(xx::TypeId_v<PKG::CatchFish::Configs::SpriteFrame>);
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


	Fish_s MakeRandomFish() {
		auto&& fish = xx::Make<Fish>();
		fish->cfgId = 0;
		fish->coin = 3;			// if minCoin < maxCoin    scene->rnd->Next(cfg->minCoin, cfg->maxCoin + 1);
		fish->speedScale = 1;
		fish->scale = 1;
		fish->wayIndex = 0;
		fish->wayPointIndex = 0;
		fish->wayPointDistance = 0;
		fish->spriteFrameIndex = 0;
		fish->frameRatio = 1;
		fish->reverse = false;
		fish->way = MakeBeeline(cfg->fishs->At(0)->maxDetectRadius);		// 先随便生成一条轨迹

		auto&& p = fish->way->points->At(fish->wayPointIndex);
		fish->pos = p.pos;
		fish->angle = p.angle;

		fish->InitCascade();
		return fish;
	}

	inline int Init(std::string cfgName) {
		::cc_scene = cocos2d::Director::getInstance()->getRunningScene();

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

		auto&& fish = MakeRandomFish();
		scene->fishs->Add(fish);

		return 0;
	}

	inline int Update() {
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
		while (fishs.len < 1000) {
			fishs.Add(MakeRandomFish());
		}
		return 0;
	}

	bool disposed = false;
	void Dispose(int const& flag) noexcept {
		if (disposed) return;
		// todo
	}

	// todo: 工具函数独立文件放置以便共享

	static constexpr int ScreenWidth = 1280;
	static constexpr int ScreenHeight = 720;
	static constexpr float ScreenWidthRatio = float(ScreenWidth) / float(ScreenWidth + ScreenHeight);

	// 填充随机生成的有限角度的能立即出现在屏幕上的线段路径
	// -45 ~ 45, 135 ~ 225 在这两段角度之间随机一个角度值,  + 180 之后的 45 度范围内再次随机一个角度, 用旋转函数转为两个坐标点. 连为1根直线, 最后找出安全出生框与直线的交点
	// 由于最终计算出两个交点之后, 可以通过交换顺序的方式反向, 故只需要一段角度作为起始角度即可. 简化起见, 直接 135 ~ 225 ( 不考虑开区间误差 )
	inline PKG::CatchFish::Way_s MakeBeeline(int const& itemRadius) {
		auto&& w = (ScreenWidth + itemRadius) / 2.0f;
		auto&& h = (ScreenHeight + itemRadius) / 2.0f;
		auto&& a = scene->rnd->Next(135, 225);
		auto&& p1 = xx::Rotate(xx::Pos{ 1, 0 }, a / 180.0f * float(M_PI));
		xx::Pos abs{ std::fabs(p1.x), std::fabs(p1.y) };
		if (abs.x / (abs.x + abs.y) > ScreenWidthRatio) {
			p1 = p1 * (w / abs.x);
		}
		else {
			p1 = p1 * (h / abs.y);
		}
		a = a + 180 - 23 + scene->rnd->Next(46);
		auto&& p2 = xx::Rotate(xx::Pos{ 1, 0 }, a / 180.0f * float(M_PI));
		abs.x = std::fabs(p2.x);
		abs.y = std::fabs(p2.y);
		if (abs.x / (abs.x + abs.y) > ScreenWidthRatio) {
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
