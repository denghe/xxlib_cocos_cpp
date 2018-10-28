#pragma once

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
		var numArgs = lua_gettop(L);
		if (numArgs < 1) return luaL_error(L, "onTouchesBegan error! need 2/1 args: self, func/nil");
		var o = Lua_ToPointer<cocos2d::EventListenerTouchAllAtOnce, 1>(L, LuaKey_EventListenerTouchAllAtOnce);
		if (numArgs > 1)
		{
			o->onTouchesBegan = [f = Lua_ToFuncHolder<2>(L)](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				f.PushFunc();									// func
				Lua_NewUserdataMT(gLua, e, LuaKey_Event);		// func, e
				for (var t : ts)
				{
					Lua_NewUserdataMT(gLua, t, LuaKey_Touch);	// func, e, t1, t2, t3, ...
				}
				int numResults = f.CallFunc(ts.size() + 1);		//
				if (numResults)
				{
					std::cout << "warning: callback func does not need return values!" << std::endl;
					lua_settop(gLua, 1);	// 清除返回值. 如果有的话. 这里不需要返回值.
				}
			};
		}
		else
		{
			o->onTouchesBegan = nullptr;
		}
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
