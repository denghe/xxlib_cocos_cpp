﻿// 所在场景
PKG::CatchFish::Scene* scene = nullptr;

// 指向具体配置
PKG::CatchFish::Configs::Fish* cfg = nullptr;

// 执行移动逻辑( frameRatio 控制了 Move 次数 )
virtual int Update(int const& frameNumber) noexcept override;

// 做一次移动
virtual int Move() noexcept;

// 碰撞检测。如果与传入子弹相撞则返回 1
virtual int HitCheck(Bullet* const& bullet) noexcept;

// 令鱼死。返回 1: 成功. 0: 失败. 通常非炸弹等状态改变鱼会返回 1
virtual int Die() noexcept;

#ifdef CC_TARGET_PLATFORM
virtual int InitCascade(void* const& o) noexcept override;

RefHolder<cocos2d::Sprite> body;
RefHolder<cocos2d::Sprite> shadow;
#if DRAW_PHYSICS_POLYGON
RefHolder<cocos2d::DrawNode> debugNode;
#endif
virtual void DrawInit() noexcept;
virtual void DrawUpdate() noexcept;
#else
// 分类统计计数器集合( 利用引用计数来统计个数 )
xx::List<std::shared_ptr<int>> counters;
#endif
