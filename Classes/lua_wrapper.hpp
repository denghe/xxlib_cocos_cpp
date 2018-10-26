#pragma once

inline const char * const LuaKey_null = "null";
inline const char * const LuaKey_FrameUpdateFunc = "FrameUpdateFunc";
inline const char * const LuaKey_UvLoop = "UvLoop";
inline const char * const LuaKey_Ref = "Ref";
inline const char * const LuaKey_Node = "Node";
inline const char * const LuaKey_Sprite = "Sprite";
inline const char * const LuaKey_SpriteFrame = "SpriteFrame";
inline const char * const LuaKey_Texture = "Texture";
inline const char * const LuaKey_TextureCache = "TextureCache";
// ...

// 试着将 idx 所在转为 T** 取出 T*. 检查 metatable. 有问题直接报错
template<typename T, int idx>
inline T* Lua_ToPointer(lua_State* const& L, char const* const& mtKey)
{
	if (!lua_isuserdata(L, idx))
	{
		luaL_error(L, "args[%d] is not userdata.", idx);
	}
	auto p = (T**)lua_touserdata(L, idx);
	if (!p)
	{
		luaL_error(L, "can't convert args[%d] to %s**.", idx, mtKey);
	}
#if 0
	if (!lua_getmetatable(L, idx))				// ..., mt?
	{
		luaL_error(L, "args[%d]'s mt is not found.", idx);
	}
	lua_rawgetp(L, idx, mtKey);					// ..., mt, mt
	if (!lua_rawequal(L, -1, -2))
	{
		luaL_error(L, "args[%d]'s mt is not %s.", mtKey);
	}
	lua_pop(L, 2);
#endif
	return *p;
}

// 试着将 idx 所在转为 string. 有问题直接报错
template<int idx>
inline std::pair<char const*, size_t> const& Lua_ToString(lua_State* const& L)
{
	std::pair<char const*, size_t> rtv;
	if (!lua_isstring(L, idx))
	{
		luaL_error(L, "args[%d] is not string.", idx);
	}
	rtv.first = lua_tolstring(L, idx, &rtv.second);
	return rtv;
}

// 创建一个 userdata 存对象指针, 并设置其元表
inline int Lua_NewUserdataMT(lua_State* const& L, void* const& o, char const* const& mtKey)
{
	auto ph = (void**)lua_newuserdata(L, sizeof(void**));			// ..., &o
	*ph = o;
	lua_rawgetp(L, LUA_REGISTRYINDEX, mtKey);						// ..., &o, mt
	lua_setmetatable(L, -2);										// ..., &o
	return 1;
}

// 创建 cc.Xxxxx 元表. 同时在注册表里面也存一份
inline void Lua_NewCcMT(lua_State* const& L, char const* const& mtKey, char const* const& parentMtKey = nullptr)
{
	lua_createtable(L, 0, 20);										// cc, MT
	lua_pushvalue(L, -1);											// cc, MT, MT
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)mtKey);				// cc, MT

	lua_pushstring(L, LuaKey_Sprite);								// cc, MT, "mtKey"
	lua_pushvalue(L, -2);											// cc, MT, "mtKey", MT
	lua_rawset(L, -4);												// cc, MT

	lua_pushstring(L, "__index");									// cc, MT, "__index"
	lua_pushvalue(L, -2);											// cc, MT, "__index", MT
	lua_rawset(L, -3);												// cc, MT

	if (parentMtKey)
	{
		lua_rawgetp(L, LUA_REGISTRYINDEX, mtKey);					// cc, MT, PMT
		lua_setmetatable(L, -2);									// cc, MT
	}
}

inline void Lua_Register_Ref(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Ref);										// cc, Ref

	lua_pushstring(L, "retain");									// cc, Ref, "retain"
	lua_pushcclosure(L, [](lua_State* L)							// cc, Ref, "retain", func
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "retain error! need 1 args: self");
		auto o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->retain();
		return 0;
	}, 0);
	lua_rawset(L, -3);												// cc, Ref

	lua_pushstring(L, "release");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "release error! need 1 args: self");
		auto o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->release();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "autorelease");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "autorelease error! need 1 args: self");
		auto o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->autorelease();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "getReferenceCount");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getReferenceCount error! need 1 args: self");
		auto o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		lua_pushinteger(L, o->getReferenceCount());
		return 1;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);													// cc
}

inline void Lua_Register_Node(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Node, LuaKey_Ref);						// cc, Node

	lua_pushstring(L, "new");										// cc, Node, "new"
	lua_pushcclosure(L, [](lua_State* L)							// cc, Node, "new", func
	{
		auto o = new (std::nothrow) cocos2d::Node();				// 从 create() 函数抄的. 可能需要和具体的 init 相配合
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Node);
	}, 0);
	lua_rawset(L, -3);												// cc, Sprite

	lua_pushstring(L, "removeFromParent");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "removeFromParent error! need 1 args: self");
		auto o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		o->removeFromParent();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);													// cc
}

inline void Lua_Register_Sprite(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Sprite, LuaKey_Node);

	lua_pushstring(L, "new");
	lua_pushcclosure(L, [](lua_State* L)
	{
		auto o = new (std::nothrow) cocos2d::Sprite();
		if (!o) return 0; if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "setSpriteFrame");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setSpriteFrame error! need 2 args: self, spriteFrame");
		auto o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		auto sf = Lua_ToPointer<cocos2d::SpriteFrame, 2>(L, LuaKey_SpriteFrame);
		o->setSpriteFrame(sf);
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "setTexture");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setTexture error! need 2 args: self, texture");
		auto o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		auto t = Lua_ToPointer<cocos2d::Texture2D, 2>(L, LuaKey_Texture);
		o->setTexture(t);
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

inline void Lua_Register_TextureCache(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_TextureCache);

	lua_pushstring(L, "addImage");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "addImage error! need 1 args: texture's file name.");
		auto fn = Lua_ToString<1>(L);
		auto o = cocos2d::TextureCache::getInstance()->addImage(std::string(fn.first, fn.second));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Texture);
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

inline void Lua_Register_Texture(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Texture, LuaKey_Ref);

	// todo: release ?

	lua_pop(L, 1);
}

inline void Lua_Register_cc(lua_State* const& L)
{
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, "cc");											// cc

	lua_pushstring(L, "restart");									// cc, "restart"
	lua_pushcclosure(L, [](lua_State* L)							// cc, "restart", func
	{
		std::cout << "restart" << std::endl;
		cocos2d::Director::getInstance()->mainLoopCallback = []
		{
			AppDelegate::instance->Restart();
		};
		return 0;
	}, 0);
	lua_rawset(L, -3);												// cc

	// todo: more like addSearchPath...

	Lua_Register_Ref(L);											// cc
	Lua_Register_Node(L);											// cc
	Lua_Register_Sprite(L);											// cc
	//Lua_Register_SpriteFrame(L);									// cc
	Lua_Register_Texture(L);										// cc
	Lua_Register_TextureCache(L);									// cc
	// .....
	// .....
	lua_pop(L, 1);													//
}

inline int Lua_Main(lua_State* L)
{
	luaL_openlibs(L);

	Lua_Register_cc(L);

	if (int r = luaL_loadfile(L, "main.lua"))						// main
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return 0;
	}

	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))					//
	{
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return r;
	}

	if (int r = luaL_loadfile(L, "loop.lua"))						// loop
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return 0;
	}

	lua_pushlightuserdata(L, (void*)LuaKey_FrameUpdateFunc);		// loop, key
	lua_pushvalue(L, 1);											// loop, key, loop
	lua_rawset(L, LUA_REGISTRYINDEX);								// loop
	assert(lua_gettop(L) == 1);										// loop
	lua_pop(L, 1);													//

	cocos2d::Director::getInstance()->mainLoopCallback = [L]
	{
		lua_pushlightuserdata(L, (void*)LuaKey_FrameUpdateFunc);	// key
		lua_rawget(L, LUA_REGISTRYINDEX);							// loop
		if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))				//
		{
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	};

	return 0;
}

inline int Lua_Init()
{
	auto L = AppDelegate::L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((xx::MemPool*)ud)->Realloc(ptr, nsize, osize);
	}
	, &AppDelegate::instance->mp);

	assert(L);

	lua_pushcclosure(L, &Lua_Main, 0);
	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return r;
	}
	return 0;
}
