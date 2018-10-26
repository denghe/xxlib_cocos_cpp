#pragma once

// 一组全局公用容器. 用于解决读取 lua 参数遇到c异常时 cpp 对象不方便回滚的问题( 除非 lua*.c 用 cpp 串编 ). 还能提升点性能.
inline static std::vector<float> gFloats;
inline static std::vector<double> gDoubles;
inline static std::vector<std::string> gStrings;
inline static std::vector<int> gInts;
inline static std::vector<int64_t> gLongs;

inline const char * const LuaKey_null = "null";
inline const char * const LuaKey_FrameUpdateFunc = "FrameUpdateFunc";
inline const char * const LuaKey_UvLoop = "UvLoop";
inline const char * const LuaKey_Ref = "Ref";
inline const char * const LuaKey_Node = "Node";
inline const char * const LuaKey_Scene = "Scene";
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
	decltype(auto) p = (T**)lua_touserdata(L, idx);
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
inline std::pair<char const*, size_t> Lua_ToString(lua_State* const& L)
{
	std::pair<char const*, size_t> rtv;
	if (!lua_isstring(L, idx))
	{
		luaL_error(L, "args[%d] is not string.", idx);
	}
	rtv.first = lua_tolstring(L, idx, &rtv.second);
	return rtv;
}

// 试着将 idx 所在转为 T. 有问题直接报错
template<typename T, int idx>
inline T Lua_ToNumber(lua_State* const& L)
{
	static_assert(std::is_arithmetic<T>::value);

	int isNum = 0;
	T rtv;
	if constexpr (std::is_integral<T>::value)
	{
		rtv = (T)lua_tointegerx(L, idx, &isNum);
		if (!isNum)
		{
			luaL_error(L, "args[%d] is not integral.", idx);
		}
	}
	else
	{
		rtv = (T)lua_tonumberx(L, idx, &isNum);
		if (!isNum)
		{
			luaL_error(L, "args[%d] is not float/double.", idx);
		}
	}
	return rtv;
}

// 试着将 idx 所在 table 转为 vector<T>. 有问题直接报错. 外部已判断过类型是 table.
template<typename T, int idx>
inline std::vector<T> const& Lua_ToValues(lua_State* const& L)
{
	// clear
	if constexpr (std::is_same<T, int>::value) { gInts.clear(); }
	else if constexpr (std::is_same<T, int64_t>::value) { gLongs.clear(); }
	else if constexpr (std::is_same<T, float>::value) { gFloats.clear(); }
	else if constexpr (std::is_same<T, double>::value) { gDoubles.clear(); }
	else { gStrings.clear(); }

	// foreach fill
	lua_pushnil(L);								// ... t, nil
	int n = 1;
	while (lua_next(L, idx) != 0)				// ... t, k, v
	{
		if constexpr (std::is_same<T, int>::value)
		{
			int isNum = 0;
			decltype(auto) v = (T)lua_tointegerx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not int.", idx, n);
			}
			gInts.push_back(v);
		}
		else if constexpr (std::is_same<T, int64_t>::value)
		{
			int isNum = 0;
			decltype(auto) v = (T)lua_tointegerx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not long.", idx, n);
			}
			gLongs.push_back(v);
		}
		else if constexpr (std::is_same<T, float>::value)
		{
			int isNum = 0;
			decltype(auto) v = (T)lua_tonumberx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not float.", idx, n);
			}
			gFloats.push_back(v);
		}
		else if constexpr (std::is_same<T, double>::value)
		{
			int isNum = 0;
			decltype(auto) v = (T)lua_tonumberx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not double.", idx, n);
			}
			gDoubles.push_back(v);
		}
		else	// string
		{
			if (!lua_isstring(L, -1))
			{
				luaL_error(L, "args[%d][%d] is not string.", idx, n);
			}
			size_t len;
			decltype(auto) buf = lua_tolstring(L, -1, &len);
			gStrings.emplace(buf, len);
		}
		lua_pop(L, 1);							// ... t, k
		++n;
	}											// ... t

	// return
	if constexpr (std::is_same<T, int>::value) { return gInts; }
	else if constexpr (std::is_same<T, int64_t>::value) { return gLongs; }
	else if constexpr (std::is_same<T, float>::value) { return gFloats; }
	else if constexpr (std::is_same<T, double>::value) { return gDoubles; }
	else { return gStrings; }
}

// 创建一个 userdata 存对象指针, 并设置其元表
inline int Lua_NewUserdataMT(lua_State* const& L, void* const& o, char const* const& mtKey)
{
	decltype(auto) ph = (void**)lua_newuserdata(L, sizeof(void**));			// ..., &o
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

	lua_pushstring(L, mtKey);										// cc, MT, "mtKey"
	lua_pushvalue(L, -2);											// cc, MT, "mtKey", MT
	lua_rawset(L, -4);												// cc, MT

	lua_pushstring(L, "__index");									// cc, MT, "__index"
	lua_pushvalue(L, -2);											// cc, MT, "__index", MT
	lua_rawset(L, -3);												// cc, MT

	if (parentMtKey)
	{
		lua_rawgetp(L, LUA_REGISTRYINDEX, parentMtKey);				// cc, MT, PMT
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
		decltype(auto) o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->retain();
		return 0;
	}, 0);
	lua_rawset(L, -3);												// cc, Ref

	lua_pushstring(L, "release");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "release error! need 1 args: self");
		decltype(auto) o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->release();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "autorelease");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "autorelease error! need 1 args: self");
		decltype(auto) o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->autorelease();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "getReferenceCount");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getReferenceCount error! need 1 args: self");
		decltype(auto) o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
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
		decltype(auto) o = new (std::nothrow) cocos2d::Node();				// 从 create() 函数抄的. 可能需要和具体的 init 相配合
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Node);
	}, 0);
	lua_rawset(L, -3);												// cc, Sprite


	lua_pushstring(L, "addChild");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "addChild error! need 2 args: self");
		decltype(auto) o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		decltype(auto) c = Lua_ToPointer<cocos2d::Node, 2>(L, LuaKey_Node);
		o->addChild(c);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "removeFromParent");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "removeFromParent error! need 1 args: self");
		decltype(auto) o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		o->removeFromParent();
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "setPosition");
	lua_pushcclosure(L, [](lua_State* L)
	{
		decltype(auto) numArgs = lua_gettop(L);
		if (numArgs < 2) return luaL_error(L, "setPosition error! need 1 args: {x,y} or 2 args: x,y");
		decltype(auto) o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		if (numArgs == 2)	// {x,y}
		{
			decltype(auto) vals = Lua_ToValues<float, 2>(L);
			if (vals.size() != 2)
			{
				return luaL_error(L, "setPosition error! need 1 args: {x,y} or 2 args: x,y");
			}
			o->setPosition(vals[0], vals[1]);
		}
		else if (numArgs == 3)	// x, y
		{
			decltype(auto) x = Lua_ToNumber<float, 2>(L);
			decltype(auto) y = Lua_ToNumber<float, 3>(L);
			o->setPosition(x, y);
		}
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);													// cc
}

inline void Lua_Register_Scene(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Scene, LuaKey_Node);
	lua_pop(L, 1);
}

inline void Lua_Register_Sprite(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Sprite, LuaKey_Node);

	lua_pushstring(L, "new");
	lua_pushcclosure(L, [](lua_State* L)
	{
		decltype(auto) o = new (std::nothrow) cocos2d::Sprite();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithTexture");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithTexture error! need 2 args: self, texture");
		decltype(auto) o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		decltype(auto) t = Lua_ToPointer<cocos2d::Texture2D, 2>(L, LuaKey_Texture);
		o->initWithTexture(t);
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithFileName");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithFileName error! need 2 args: self, texture");
		decltype(auto) o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		decltype(auto) fn = Lua_ToString<2>(L);
		o->initWithFile(std::string(fn.first, fn.second));
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithSpriteFrameName");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithSpriteFrameName error! need 2 args: self, texture");
		decltype(auto) o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		decltype(auto) sfn = Lua_ToString<2>(L);
		o->initWithSpriteFrameName(std::string(sfn.first, sfn.second));
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithSpriteFrame");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithSpriteFrame error! need 2 args: self, texture");
		decltype(auto) o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		decltype(auto) sf = Lua_ToPointer<cocos2d::SpriteFrame, 2>(L, LuaKey_SpriteFrame);
		o->initWithSpriteFrame(sf);
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "setSpriteFrame");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setSpriteFrame error! need 2 args: self, spriteFrame");
		decltype(auto) o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		decltype(auto) sf = Lua_ToPointer<cocos2d::SpriteFrame, 2>(L, LuaKey_SpriteFrame);
		o->setSpriteFrame(sf);
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
		decltype(auto) fn = Lua_ToString<1>(L);
		decltype(auto) o = cocos2d::TextureCache::getInstance()->addImage(std::string(fn.first, fn.second));
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Texture);
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

inline void Lua_Register_Texture(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Texture, LuaKey_Ref);
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

	lua_pushstring(L, "scene");
	lua_pushcclosure(L, [](lua_State* L)
	{
		return Lua_NewUserdataMT(L, AppDelegate::scene, LuaKey_Scene);
	}, 0);
	lua_rawset(L, -3);

	// todo: more like addSearchPath...

	Lua_Register_Ref(L);											// cc
	Lua_Register_Node(L);											// cc
	Lua_Register_Scene(L);											// cc
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
	decltype(auto) L = AppDelegate::L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
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
