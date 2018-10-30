#pragma once

inline const char* const LuaKey_null = "null";

inline const char* const LuaKey_xx = "xx";
inline const char* const LuaKey_Object = "Object";
inline const char* const LuaKey_MemPool = "MemPool";
inline const char* const LuaKey_UvLoop = "UvLoop";

inline const char* const LuaKey_Callbacks = "Callbacks";
inline const char* const LuaKey_FrameUpdateFunc = "FrameUpdateFunc";

inline const char* const LuaKey_cc = "cc";


struct Lua_BBuffer;
struct Lua_Func;

template<typename T>
struct TypeNames;
//{
//	inline static const char* value = "unknown";
//};
template<>
struct TypeNames<int>
{
	inline static const char* value = "int";
};
template<>
struct TypeNames<int64_t>
{
	inline static const char* value = "int64";
};
template<>
struct TypeNames<float>
{
	inline static const char* value = "float";
};
template<>
struct TypeNames<double>
{
	inline static const char* value = "double";
};
template<>
struct TypeNames<bool>
{
	inline static const char* value = "bool";
};
template<>
struct TypeNames<const char*>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<char*>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<std::string>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<Lua_Func>
{
	inline static const char* value = "func";
};



template<>
struct TypeNames<cocos2d::Ref*>
{
	inline static const char* value = "Ref";
};
template<>
struct TypeNames<cocos2d::Touch*>
{
	inline static const char* value = "Touch";
};
template<>
struct TypeNames<cocos2d::Event*>
{
	inline static const char* value = "Event";
};
template<>
struct TypeNames<cocos2d::EventListener*>
{
	inline static const char* value = "EventListener";
};
template<>
struct TypeNames<cocos2d::EventListenerTouchOneByOne*>
{
	inline static const char* value = "EventListenerTouchOneByOne";
};
template<>
struct TypeNames<cocos2d::EventListenerTouchAllAtOnce*>
{
	inline static const char* value = "EventListenerTouchAllAtOnce";
};
template<>
struct TypeNames<cocos2d::Node*>
{
	inline static const char* value = "Node";
};
template<>
struct TypeNames<cocos2d::Scene*>
{
	inline static const char* value = "Scene";
};
template<>
struct TypeNames<cocos2d::Layer*>
{
	inline static const char* value = "Layer";
};
template<>
struct TypeNames<cocos2d::Sprite*>
{
	inline static const char* value = "Sprite";
};
template<>
struct TypeNames<cocos2d::SpriteFrame*>
{
	inline static const char* value = "SpriteFrame";
};
template<>
struct TypeNames<cocos2d::Texture2D*>
{
	inline static const char* value = "Texture2D";
};
template<>
struct TypeNames<cocos2d::TextureCache*>
{
	inline static const char* value = "TextureCache";
};

// todo: more


template<>
struct TypeNames<xx::BBuffer*>
{
	inline static const char* value = "BBuffer";
};
template<>
struct TypeNames<Lua_BBuffer*>
{
	inline static const char* value = "BBuffer";
};
template<>
struct TypeNames<xx::UvTcpClient_w>
{
	inline static const char* value = "UvTcpClient";
};


// todo: more

