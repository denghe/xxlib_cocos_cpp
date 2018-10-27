#pragma once
#ifndef var
#define var decltype(auto)
#endif

// todo: 优化函数名和使用, 考虑参考 cocos lua 框架代码提供 return self 以便连写

inline std::vector<float> gFloats;
inline std::vector<double> gDoubles;
inline std::vector<std::string> gStrings;
inline std::vector<int> gInts;
inline std::vector<int64_t> gLongs;


inline const char* const LuaKey_null = "null";
inline const char* const LuaKey_Callbacks = "Callbacks";
inline const char* const LuaKey_FrameUpdateFunc = "FrameUpdateFunc";
inline const char* const LuaKey_UvLoop = "UvLoop";
inline const char* const LuaKey_Ref = "Ref";
inline const char* const LuaKey_Touch = "Touch";
inline const char* const LuaKey_Event = "Event";
inline const char* const LuaKey_EventListener = "EventListener";
inline const char* const LuaKey_EventListenerTouchOneByOne = "EventListenerTouchOneByOne";
inline const char* const LuaKey_EventListenerTouchAllAtOnce = "EventListenerTouchAllAtOnce";
inline const char* const LuaKey_Node = "Node";
inline const char* const LuaKey_Scene = "Scene";
inline const char* const LuaKey_Layer = "Layer";
inline const char* const LuaKey_Sprite = "Sprite";
inline const char* const LuaKey_SpriteFrame = "SpriteFrame";
inline const char* const LuaKey_Texture = "Texture";
inline const char* const LuaKey_TextureCache = "TextureCache";
// ...


// 被 std::function 携带, 当捕获列表析构发生时, 自动从 L 中反注册函数
struct Lua_FuncHolder : xx::Object
{
	// 全局自增函数 id
	inline static int autoIncFuncId = 1;

	int funcId;
	Lua_FuncHolder(xx::MemPool* const& mp, lua_State* const& L, int const& idx)
		: xx::Object(mp)
		, funcId(autoIncFuncId)
	{
		lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// ..., t
		lua_pushvalue(L, idx);										// ..., t, func
		lua_rawseti(L, -2, funcId);									// ..., t
		lua_pop(L, 1);												// ...
		++autoIncFuncId;
	}

	Lua_FuncHolder(Lua_FuncHolder const&) = delete;

	// 将函数压栈( 之后调用方接着压入 参数 )
	inline void PushFunc() const
	{
		lua_rawgeti(gLua, 1, funcId);						// funcs, func
	}

	// 调用函数, 返回产生了多少个返回值( 之后调用方读出返回值, 并 settop(1) 清除它们 )
	inline int CallFunc(int const& numArgs) const
	{
		var L = gLua;
		if (int r = lua_pcall(L, numArgs, LUA_MULTRET, 0))			// funcs
		{
			std::cout << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		return lua_gettop(L) - 1;
	}

	// 随 lambda 析构时删掉函数
	~Lua_FuncHolder()
	{
		var L = gLua;
		lua_pushnil(L);												// funcs, nil
		lua_rawseti(L, 1, funcId);									// funcs
	}
};

// 试着将 idx 所在转为 T** 取出 T*. 检查 metatable. 有问题直接报错
template<typename T, int idx>
inline T* Lua_ToPointer(lua_State* const& L, char const* const& mtKey)
{
	if (!lua_isuserdata(L, idx))
	{
		luaL_error(L, "args[%d] is not userdata.", idx);
	}
	var p = (T**)lua_touserdata(L, idx);
	if (!p)
	{
		luaL_error(L, "args[%d] is nullptr.", idx, mtKey);
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

// 试着将 idx 所在转为 boolean. 有问题直接报错
template<int idx>
inline bool Lua_ToBoolean(lua_State* const& L)
{
	if (!lua_isboolean(L, idx))
	{
		luaL_error(L, "args[%d] is not string.", idx);
	}
	return lua_toboolean(L, idx);
}

// 试着将 idx 所在转为 funcId. 有问题直接报错
template<int idx>
inline xx::Ptr<Lua_FuncHolder> Lua_ToFuncHolder(lua_State* const& L)
{
	if (!lua_isfunction(L, idx))
	{
		luaL_error(L, "args[%d] is not function.", idx);
	}
	return mp->MPCreatePtr<Lua_FuncHolder>(L, idx);
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
			var v = (T)lua_tointegerx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not int.", idx, n);
			}
			gInts.push_back(v);
		}
		else if constexpr (std::is_same<T, int64_t>::value)
		{
			int isNum = 0;
			var v = (T)lua_tointegerx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not long.", idx, n);
			}
			gLongs.push_back(v);
		}
		else if constexpr (std::is_same<T, float>::value)
		{
			int isNum = 0;
			var v = (T)lua_tonumberx(L, -1, &isNum);
			if (!isNum)
			{
				luaL_error(L, "args[%d][%d] is not float.", idx, n);
			}
			gFloats.push_back(v);
		}
		else if constexpr (std::is_same<T, double>::value)
		{
			int isNum = 0;
			var v = (T)lua_tonumberx(L, -1, &isNum);
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
			var buf = lua_tolstring(L, -1, &len);
			gStrings.emplace_back(buf, len);
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

// 满足 self, x, y 或 self, {x, y} 的传参的调用，试着获取 x,y 
inline cocos2d::Vec2 Lua_ToVec2(lua_State* const& L, char const* const& funcName)
{
	cocos2d::Vec2 rtv;
	var numArgs = lua_gettop(L);
	if (numArgs < 2)
	{
		luaL_error(L, "%s error! need 1 args: {x,y} or 2 args: x,y", funcName);
	}
	if (numArgs == 2)	// {x,y}
	{
		var vals = Lua_ToValues<float, 2>(L);
		if (vals.size() != 2)
		{
			luaL_error(L, "%s error! need 1 args: {x,y} or 2 args: x,y", funcName);
		}
		rtv.x = vals[0];
		rtv.y = vals[1];
	}
	else if (numArgs >= 3)	// x, y
	{
		rtv.x = Lua_ToNumber<float, 2>(L);
		rtv.y = Lua_ToNumber<float, 3>(L);
	}
	return rtv;
}

// 创建一个 userdata 存对象指针, 并设置其元表
inline int Lua_NewUserdataMT(lua_State* const& L, void* const& o, char const* const& mtKey)
{
	var ph = (void**)lua_newuserdata(L, sizeof(void**));	// ..., &o
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
		var o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->retain();
		return 0;
	}, 0);
	lua_rawset(L, -3);												// cc, Ref

	lua_pushstring(L, "release");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "release error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->release();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "autorelease");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "autorelease error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
		o->autorelease();
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "getReferenceCount");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getReferenceCount error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Ref, 1>(L, LuaKey_Ref);
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
		var o = new (std::nothrow) cocos2d::Node();		// 从 create() 函数抄的. 可能需要和具体的 init 相配合
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Node);
	}, 0);
	lua_rawset(L, -3);												// cc, Sprite


	lua_pushstring(L, "addChild");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "addChild error! need 2 args: self");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var c = Lua_ToPointer<cocos2d::Node, 2>(L, LuaKey_Node);
		o->addChild(c);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "removeFromParent");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "removeFromParent error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		o->removeFromParent();
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "setPosition");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var xy = Lua_ToVec2(L, "setPosition");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		o->setPosition(xy.x, xy.y);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "setAnchorPoint");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var xy = Lua_ToVec2(L, "setAnchorPoint");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		o->setAnchorPoint(xy);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "setLocalZOrder");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setLocalZOrder error! need 2 args: self, int z-order");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var z = Lua_ToNumber<int, 2>(L);
		o->setLocalZOrder(z);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "setRotation");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setRotation error! need 2 args: self, float angle( 360 )");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var r = Lua_ToNumber<float, 2>(L);
		o->setRotation(r);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "setScale");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var v = Lua_ToVec2(L, "setScale");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		o->setScale(v.x, v.y);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "convertToNodeSpace");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var v = Lua_ToVec2(L, "convertToNodeSpace");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var r = o->convertToNodeSpace(v);
		lua_pushnumber(L, r.x);
		lua_pushnumber(L, r.y);
		return 2;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "getContentSize");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var r = o->getContentSize();
		lua_pushnumber(L, r.width);
		lua_pushnumber(L, r.height);
		return 2;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "containsPoint");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var v = Lua_ToVec2(L, "containsPoint");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);

		var s = o->getContentSize();
		cocos2d::Rect r(0, 0, s.width, s.height);
		var b = r.containsPoint(v);

		lua_pushboolean(L, b);
		return 1;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "containsTouch");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var t = Lua_ToPointer<cocos2d::Touch, 2>(L, LuaKey_Touch);

		var tL = t->getLocation();
		var p = o->convertToNodeSpace(tL);
		var s = o->getContentSize();
		cocos2d::Rect r{ 0,0, s.width, s.height };
		var b = r.containsPoint(p);

		lua_pushboolean(L, b);
		return 1;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "addEventListener");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "addEventListener( WithSceneGraphPriority ) error! need 2 args: self, listener");
		var o = Lua_ToPointer<cocos2d::Node, 1>(L, LuaKey_Node);
		var l = Lua_ToPointer<cocos2d::EventListener, 2>(L, LuaKey_EventListener);
		o->getEventDispatcher()->addEventListenerWithSceneGraphPriority(l, o);
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

inline void Lua_Register_Touch(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Touch, LuaKey_Ref);

	lua_pushstring(L, "getLocation");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getLocation error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Touch, 1>(L, LuaKey_Touch);
		var v = o->getLocation();
		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		return 2;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "getLocationInView");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "getLocationInView error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::Touch, 1>(L, LuaKey_Touch);
		var v = o->getLocationInView();
		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		return 2;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

inline void Lua_Register_Event(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Event, LuaKey_Ref);

	// event->getCurrentTarget()

	lua_pop(L, 1);
}

inline void Lua_Register_EventListener(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_EventListener, LuaKey_Ref);

	lua_pushstring(L, "setEnabled");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setEnabled error! need 2 args: self, bool");
		var o = Lua_ToPointer<cocos2d::EventListener, 1>(L, LuaKey_EventListener);
		var b = Lua_ToBoolean<2>(L);
		o->setEnabled(b);
		return 0;
	}, 0);
	lua_rawset(L, -3);


	lua_pushstring(L, "isEnabled");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 1) return luaL_error(L, "setEnabled error! need 1 args: self");
		var o = Lua_ToPointer<cocos2d::EventListener, 1>(L, LuaKey_EventListener);
		bool rtv = o->isEnabled();
		lua_pushboolean(L, rtv);
		return 1;
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

inline void Lua_Register_EventListenerTouchAllAtOnce(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_EventListenerTouchAllAtOnce, LuaKey_EventListener);

	lua_pushstring(L, "new");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerTouchAllAtOnce();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_EventListenerTouchAllAtOnce);
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "onTouchesBegan");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "onTouchesBegan error! need 2 args: self, func");
		var o = Lua_ToPointer<cocos2d::EventListenerTouchAllAtOnce, 1>(L, LuaKey_EventListenerTouchAllAtOnce);
		var f = Lua_ToFuncHolder<2>(L);

		o->onTouchesBegan = [f = std::move(f)](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
		{
			var L = gLua;
			f->PushFunc();
			Lua_NewUserdataMT(L, e, LuaKey_Event);
			for (var t : ts)
			{
				Lua_NewUserdataMT(L, t, LuaKey_Touch);
			}
			int numResults = f->CallFunc(ts.size() + 1);
			if (numResults)
			{
				std::cout << "warning: callback func does not need return values!" << std::endl;
				lua_settop(L, 1);	// 清除返回值. 如果有的话. 这里不需要返回值.
			}
		};
		return 0;
	}, 0);
	lua_rawset(L, -3);

	//ccTouchesCallback onTouchesMoved;
//ccTouchesCallback onTouchesEnded;
//ccTouchesCallback onTouchesCancelled;

	lua_pop(L, 1);
}

inline void Lua_Register_EventListenerTouchOneByOne(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_EventListenerTouchOneByOne, LuaKey_EventListener);

	lua_pushstring(L, "new");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerTouchOneByOne();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_EventListenerTouchOneByOne);
	}, 0);
	lua_rawset(L, -3);

	// void setSwallowTouches(bool needSwallow);
	// bool isSwallowTouches();

	lua_pop(L, 1);
}

inline void Lua_Register_Layer(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Layer, LuaKey_Node);

	lua_pushstring(L, "new");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Layer();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Layer);
	}, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

inline void Lua_Register_Sprite(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_Sprite, LuaKey_Node);

	lua_pushstring(L, "new");
	lua_pushcclosure(L, [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Sprite();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Sprite);
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithTexture");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithTexture error! need 2 args: self, texture");
		var o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		var t = Lua_ToPointer<cocos2d::Texture2D, 2>(L, LuaKey_Texture);
		o->initWithTexture(t);
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithFileName");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithFileName error! need 2 args: self, texture");
		var o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		var fn = Lua_ToString<2>(L);
		o->initWithFile(std::string(fn.first, fn.second));
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithSpriteFrameName");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithSpriteFrameName error! need 2 args: self, texture");
		var o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		var sfn = Lua_ToString<2>(L);
		o->initWithSpriteFrameName(std::string(sfn.first, sfn.second));
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "initWithSpriteFrame");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "initWithSpriteFrame error! need 2 args: self, texture");
		var o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		var sf = Lua_ToPointer<cocos2d::SpriteFrame, 2>(L, LuaKey_SpriteFrame);
		o->initWithSpriteFrame(sf);
		return 0;
	}, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "setSpriteFrame");
	lua_pushcclosure(L, [](lua_State* L)
	{
		if (lua_gettop(L) < 2) return luaL_error(L, "setSpriteFrame error! need 2 args: self, spriteFrame");
		var o = Lua_ToPointer<cocos2d::Sprite, 1>(L, LuaKey_Sprite);
		var sf = Lua_ToPointer<cocos2d::SpriteFrame, 2>(L, LuaKey_SpriteFrame);
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
		var fn = Lua_ToString<1>(L);
		var o = cocos2d::TextureCache::getInstance()->addImage(std::string(fn.first, fn.second));
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
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, "cc");											// cc

	// 创建 重启游戏 函数
	lua_pushstring(L, "restart");									// cc, "restart"
	lua_pushcclosure(L, [](lua_State* L)							// cc, "restart", func
	{
		var d = cocos2d::Director::getInstance();
		d->mainLoopCallback = [d]
		{
			d->restart();
			d->mainLoopCallback = nullptr;
		};
		return 0;
	}, 0);
	lua_rawset(L, -3);												// cc

	// 创建 拿 scene 函数
	lua_pushstring(L, "scene");
	lua_pushcclosure(L, [](lua_State* L)
	{
		return Lua_NewUserdataMT(L, gScene, LuaKey_Scene);
	}, 0);
	lua_rawset(L, -3);

	// todo: more like addSearchPath...

	// 创建 cc.Xxxxxx 元表及函数
	Lua_Register_Ref(L);
	Lua_Register_Node(L);
	Lua_Register_Scene(L);
	Lua_Register_Touch(L);
	Lua_Register_Event(L);
	Lua_Register_EventListener(L);
	Lua_Register_EventListenerTouchAllAtOnce(L);
	Lua_Register_EventListenerTouchOneByOne(L);
	Lua_Register_Sprite(L);
	//Lua_Register_SpriteFrame(L);							
	Lua_Register_Texture(L);
	Lua_Register_TextureCache(L);
	// .....
	// .....
	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}

// 读取 print 的参数
inline int get_string_for_print(lua_State* L, std::string* out)
{
	int n = lua_gettop(L);  /* number of arguments */
	int i;

	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		size_t sz;
		s = lua_tolstring(L, -1, &sz);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) out->append("\t");
		out->append(s, sz);
		lua_pop(L, 1);  /* pop result */
	}
	return 0;
}

inline int Lua_Main(lua_State* L)
{
	assert(L == gLua);

	// 加载常用库
	luaL_openlibs(L);

	// 替换 print 的实现 for android 输出
	lua_pushcclosure(L, [](lua_State* L)							// package, searchers, func
	{
		std::string t;
		get_string_for_print(L, &t);
		cocos2d::log("[LUA-print] %s", t.c_str());
		return 0;
	}, 0);
	lua_setglobal(L, "print");

	// 令 require 支持读取 search path 里的文件( 包里 & 可写目录 )
	lua_getglobal(L, "package");									// package
	lua_getfield(L, -1, "searchers");								// package, searchers
	lua_pushcclosure(L, [](lua_State* L)							// package, searchers, func
	{
		size_t len;
		var fn = lua_tolstring(L, 1, &len);
		var fu = cocos2d::FileUtils::getInstance();
		if (!fu->isFileExist(std::string(fn, len)))
		{
			return luaL_error(L, "require file '%s' failed. can't find file.", fn);
		}
		var data = cocos2d::FileUtils::getInstance()->getDataFromFile(std::string(fn, len));
		var buf = (char*)data.getBytes();
		len = data.getSize();
		if (len >= 3 && (uint8_t)buf[0] == 0xEF && (uint8_t)buf[1] == 0xBB && (uint8_t)buf[2] == 0xBF)
		{
			buf += 3;
			len -= 3;
		}
		var r = luaL_loadbuffer(L, buf, len, fn);
		if (r == LUA_OK) return 1;
		return luaL_error(L, "require file '%s' failed. luaL_loadbuffer r = %d", fn, r);
	}, 0);
	lua_rawseti(L, -2, 1);											// package, searchers
	lua_pop(L, 2);													//


	// 创建函数表
	lua_createtable(L, 0, 100);										// funcs
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);		//


	// 加载 cc.* 对象 & 函数映射
	Lua_Register_cc(L);

	// 执行 main.lua
	if (int r = luaL_dostring(L, "require \"main.lua\""))			//
	{
		auto s = lua_tostring(L, -1);
		CCLOG("%d %s", r, (s ? s : ""));
		lua_pop(L, 1);
		return 0;
	}

	// 拿出函数表
	lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);		// funcs

	// 加载 loop.lua
	if (int r = luaL_dostring(L, "return require \"loop.lua\""))	// funcs, loop
	{
		auto s = lua_tostring(L, -1);
		CCLOG("%d %s", r, (s ? s : ""));
		lua_pop(L, 1);
		return 0;
	}

	// 将 loop 放入函数表
	lua_rawsetp(L, 1, (void*)LuaKey_FrameUpdateFunc);				// funcs
	lua_pop(L, 1);													//

	// 注册每帧回调 cpp 函数
	cocos2d::Director::getInstance()->mainLoopCallback = []
	{
		var L = gLua;

		// root L[1] 已被放置了 函数表
		assert(lua_gettop(L) == 1);									// funcs

		// 拿出 loop, 执行之
		lua_rawgetp(L, 1, (void*)LuaKey_FrameUpdateFunc);			// funcs, loop
		if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))				// funcs
		{
			auto s = lua_tostring(L, -1);
			CCLOG("%d %s", r, (s ? s : ""));
			lua_pop(L, 1);
		}
	};

	assert(lua_gettop(L) == 0);
	return 0;
}


inline int Lua_Init()
{
	// 使用内存池创建 lua state ( 部分操作性能提升 40% )
	var L = gLua = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((xx::MemPool*)ud)->Realloc(ptr, nsize, osize);
	}
	, mp);
	assert(L);

	// 将 Lua_Main 压入 L 安全执行
	lua_pushcclosure(L, &Lua_Main, 0);								// cfunc
	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))					//
	{
		auto s = lua_tostring(L, -1);
		CCLOG("%d %s", r, (s ? s : ""));
		lua_pop(L, 1);
		return r;
	}
	lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);		// funcs

	return 0;
}
