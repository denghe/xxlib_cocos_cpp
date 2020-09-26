#pragma once
#include "Pathway.h"
#include "xx_object.h"
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"
#include <spine/spine-cocos2dx.h>

#include "AppMacros.h"

//#include "xx_lua.h"
//#include <iostream>
//#include <chrono>
//
//namespace XL = xx::Lua;
//
//#include "FileExts_class_lite.h"
//// todo: ajson macro
//
//

// todo: 最后来补序列化支持, 以及加客户端判断宏以跳过一些服务器做不了或不需要做的事情

// todo: size_t 换 int 以便 lua 更好的支持？


// 跨 frames / atlas / c3b 的动画基类
struct Anim {
	// 在容器中的显示坐标
	xx::Point pos;

	// 角度
	float angle = 0;

	// 尺寸缩放比. 默认为 1
	float scaleX = 1;
	float scaleY = 1;

	// z order?

	// shadow
    float shadowX = 0.f;
    float shadowY = 0.f;
    float shadowScale = 0.f;

	// 当前动作已经历时长
	float totalElapsedSeconds = 0;

	// 显示 / 隐藏
	bool visible = true;

	// 是否循环播放
	bool loop = true;

	// 所在容器
	cocos2d::Node* container = nullptr;

	// 文件名
	std::string fileName;

	// 根据 fileName 加载文件. 成功返回 0. 该函数被设计为只执行一次（依赖成员变量初始值的正确性）
	virtual int Load() = 0;

	// 设置当前要播的动画。会跳到 0 秒处。如果 index < 0 则只是重置动画
	virtual void SetAction(const std::string& name) = 0;

	// 驱动动画往前播放指定时长. 不调用就是暂停. 传更长的时间入内就是快进. 返回非 0 表示停止播放( 到头了？）
	virtual int Update(float const& elapsedSeconds) = 0;

	// 绘制到屏幕
	virtual void Draw() = 0;

	// 析构
	virtual ~Anim() = default;
};

struct Anim_Frames : Anim {
	// 文件内容容器
	FileExts::File_Frames file;

	// 指向当前动作( 位于 file.actions )
	FileExts::Action_Frames* action;

	// 当前帧下标
	int frameIndex = 0;

	// 当前用于显示的类实例
	cocos2d::Sprite* sprite = nullptr;
    cocos2d::Sprite* spriteShadow = nullptr;

	int Load() override;
	void SetAction(const std::string& name) override;
	int Update(float const& elapsedSeconds) override;
	void Draw() override;
	~Anim_Frames() override;
};

struct Anim_Atlas : Anim {
    spine::SkeletonAnimation* skeletonAnimation = nullptr;
    spine::SkeletonAnimation* skeletonAnimationShadow = nullptr;
	int Load() override;
	void SetAction(const std::string& name) override;
	int Update(float const& elapsedSeconds) override;
	void Draw() override;
	~Anim_Atlas() override;
};

struct Anim_C3b : Anim {
	// todo: 文件内容容器
	// todo: sprite3d node?
	int Load() override;
	void SetAction(const std::string& name) override;
	int Update(float const& elapsedSeconds) override;
	void Draw() override;
	~Anim_C3b() override;
};

// 根据文件扩展名路由加载相应类型动画 并 设置参数为 循环播放第一个动作
std::shared_ptr<Anim> CreateAnim(std::string const& fn, cocos2d::Node* const& container);

// 动画扩展版基类。能沿着 pathway 移动。带碰撞检测啥的逻辑功能
struct AnimExt {
	// 当前动画
	std::shared_ptr<Anim> anim;

	// 文件名
	std::string fileName;

	// 所在容器
	cocos2d::Node* container = nullptr;

	// 预设移动路线集合
	std::vector<std::shared_ptr<xx::Pathway>> pathways;

	// 当前移动路线( 指向 pathways 的成员 )
	xx::Pathway* pathway = nullptr;

	// pathway 之 当前点下标
	size_t pathwayI = 0;

	// pathway 之 当前点已移动距离
	float pathwayD = 0;

	// 移动速度系数. 默认为 1, 0 为停止, 负数为倒退
	float speedScale = 1;

	// 时间系数. 默认为 1, 0 为停止. 不能为负
	float timeScale = 1;

	// 子集合，组合啥的会用到。母体函数分发判断 lock attack 啥的。pathway 通常依附于母体，走相对坐标
	std::vector<std::shared_ptr<AnimExt>> childs;

	// 根据 fileName 加载文件. 成功返回 0. 该函数被设计为只执行一次（依赖成员变量初始值的正确性）
	virtual int Load() = 0;

	// 设置使用哪条移动路线。pathway = &*pathways[idx], 初始化状态变量并同步坐标角度啥的
	virtual void SetPathway(size_t const& idx) = 0;

	// 调用动画 Update 并沿 pathway 移动. 返回 非 0 表示 移动到头了
	virtual int Update(float const& elapsedSeconds) = 0;

	// Update 时遇到 speed 变为负数时将转为 eventId 触发该函数调用. elapsedSeconds 为事件触发后又过了多长时间
	virtual void Event(int const& eventId, float const& elapsedSeconds) = 0;

	// 判断 传入点(r == 0) 或 圆 是否和某 cdCircle 相交( touch, bullet hit 判断需要 )
	virtual bool IsIntersect(float const& x, float const& y, float const& r) const = 0;

	// 判断是否能被 lock( 有某锁定点在屏幕范围内 )
	virtual bool Lockable() const = 0;

	// 获取锁定坐标
	virtual xx::Point GetLockPoint() const = 0;

	// 判断是否能被 attack( 可能存在 无敌？遁入虚空？等无法攻击的状态 )
	virtual bool Attackable() const = 0;

	// 被锁定通知（显示准星？)
	virtual void Lock() = 0;	// todo: 参数？准星样式？

	// 清除被锁定状态, 客户端在显示上可能有所体现（清除准星？）
	virtual void Unlock() = 0;

	// 被击中通知（变红一会儿？）
	virtual void Attack() = 0;	// todo: 参数？

	// 死亡通知
	virtual void Death() = 0;	// todo: 参数？死法？

	// 绘制
	virtual void Draw() = 0;

	virtual ~AnimExt() = default;
};

struct AnimExt_Anim : AnimExt {
	// 文件内容容器
	FileExts::File_AnimExt file;

	// 指向当前 action( 位于 file.actions )
	FileExts::Action_AnimExt* action = nullptr;

	// 动画已经历秒数
	float totalElapsedSeconds = 0;

	// 记录相应时间线的游标/下标
	size_t lpsCursor = 0;
	size_t cdsCursor = 0;
	size_t ssCursor = 0;
	// 缓存当前速度
	float speed = 0;

#ifdef DEBUG_DRAW_PATHWAY
    cocos2d::DrawNode* debug_path = nullptr;
#endif
#ifdef DEBUG_DRAW_ANIM_INFO
    cocos2d::DrawNode* debug_info = nullptr;
#endif
	int Load() override;
	void SetPathway(size_t const& idx) override;
	int Update(float const& elapsedSeconds) override;
	void Event(int const& eventId, float const& elapsedSeconds) override;
	bool IsIntersect(float const& x, float const& y, float const& r) const override;
	bool Lockable() const override;
	xx::Point GetLockPoint() const override;
	bool Attackable() const override;
	void Lock() override;
	void Unlock() override;
	void Attack() override;
	void Death() override;
	void Draw() override;
	~AnimExt_Anim() override;
};

struct AnimExt_Lua : AnimExt {
	// todo: override
};

// 根据文件扩展名路由加载相应类型扩展动画. 后续还要自己继续初始化，设置 pathway 啥的
std::shared_ptr<AnimExt> CreateAnimExt(std::string const& fn, cocos2d::Node* const& container);



//// 带移动功能的动画 基类
//struct AnimBase : xx::Object {
//

//	// 别的扩展事件
//	virtual void Event(std::string const& event) = 0;
//
//	// 根据扩展名( .frames,  .atlas.ext, .c3b.ext, .lua )创建相应的 Anim 类，并 Load
//	static std::shared_ptr<AnimBase> Create(std::string const& fileName);
//
//	//T() = default;
//	//T(T const&) = default;
//	//T& operator=(T const&) = default;
//	//T(T&& o) noexcept;
//	//T& operator=(T&& o) noexcept;
//	//uint16_t GetTypeId() const override;
//	//void Serialize(xx::DataWriterEx& dw) const override;
//	//int Deserialize(xx::DataReaderEx& dr) override;
//	//void ToString(xx::ObjectHelper& oh) const override;
//	//void ToStringCore(xx::ObjectHelper& oh) const override;
//	//void Clone1(xx::ObjectHelper& oh, std::shared_ptr<Object> const& tar) const override;
//	//void Clone2(xx::ObjectHelper& oh, std::shared_ptr<Object> const& tar) const override;
//};
//
//enum class AnimTypes {
//	Unknown, Frames, Atlas, C3b
//};
//
//// 文件类动画基类( spine, 3d, frames )
//struct Anim : AnimBase {
//	// 动画类型。 由 Load 决定
//	AnimTypes animType;
//
//	// 指向当前 anim
//	std::shared_ptr<FileExts::File_Anim> anim;
//
//	// 指向当前 action( 位于 anim 中 )
//	FileExts::Action* action = nullptr;
//
//	// 动画已经历秒数
//	float totalElapsedSeconds = 0;
//
//	// 记录相应时间线的游标/下标
//	size_t lpsCursor = 0;
//	size_t cdsCursor = 0;
//	size_t ssCursor = 0;
//	size_t fsCursor = 0;
//
//	// .frames,  .atlas.ext, .c3b.ext
//	int Load(std::string const& fileName) override;
//
//	int ActionEnd() override;
//
//	int Move(float const& distance) override;
//
//	int Update(float elapsedSeconds) override;
//
//	static bool IsIntersect(FileExts::CDCircle const& c, float const& x, float const& y, float const& r);
//
//	bool IsIntersect(float const& x, float const& y, float const& r) const override;
//
//	bool Lockable() const override;
//
//	std::tuple<float, float> GetLockPoint() const override;
//
//	void Lock() override;
//
//	void Unlock() override;
//
//	bool Attackable() const override;
//
//	void Draw() override;
//};




//// Lua 类动画基类, 虚函数调用到和 lua 函数绑定的 std::function
//struct AnimLua : AnimBase {
//    lua_State *L;
//    std::string scriptName;
//    // AnimXxxx 容器? lua 可能创建 n 个 并控制它们. 通常创建 1 个
//
//    // 加载脚本并映射函数
//    AnimLua(lua_State *const &L, std::string scriptName) : L(L), scriptName(std::move(scriptName)) {
//        // todo: load script, bind func
//    };
//
//    std::function<void(std::string const &actionName)> onSetAction;
//
//    void SetAction(std::string const &actionName) override {
//        onSetAction(actionName);
//    }
//
//    // more
//};
