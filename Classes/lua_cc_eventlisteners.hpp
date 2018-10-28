#pragma once

inline void Lua_Register_EventListener(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_EventListener, LuaKey_Ref);

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		var t = Lua_ToTB<cocos2d::EventListener>(L, "setEnabled error! need 2 args: self, bool", LuaKey_EventListener);
		std::get<0>(t)->setEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		var t = Lua_ToTB<cocos2d::EventListener>(L, "isEnabled error! need 1 args: self", LuaKey_EventListener);
		var r = std::get<0>(t)->isEnabled();
		lua_pushboolean(L, r);
		return 1;
	});

	lua_pop(L, 1);
}

inline void Lua_Register_EventListenerTouchAllAtOnce(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_EventListenerTouchAllAtOnce, LuaKey_EventListener);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerTouchAllAtOnce();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_EventListenerTouchAllAtOnce);
	});

	Lua_NewFunc(L, "onTouchesBegan", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchAllAtOnce>(L, "onTouchesBegan error! need 2 args: self, func/null", LuaKey_EventListenerTouchAllAtOnce);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchesBegan = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouchs = &ts;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					for (var t : *gTouchs)
					{
						Lua_NewUserdataMT(L, t, LuaKey_Touch);					// funcs, func, event, t1, t2, ....
					}
					lua_call(L, gTouchs->size() + 1, 0);						// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouchs = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchesBegan = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onTouchesMoved", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchAllAtOnce>(L, "onTouchesMoved error! need 2 args: self, func/null", LuaKey_EventListenerTouchAllAtOnce);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchesMoved = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouchs = &ts;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					for (var t : *gTouchs)
					{
						Lua_NewUserdataMT(L, t, LuaKey_Touch);					// funcs, func, event, t1, t2, ....
					}
					lua_call(L, gTouchs->size() + 1, 0);						// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouchs = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchesMoved = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onTouchesEnded", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchAllAtOnce>(L, "onTouchesEnded error! need 2 args: self, func/null", LuaKey_EventListenerTouchAllAtOnce);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchesEnded = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouchs = &ts;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					for (var t : *gTouchs)
					{
						Lua_NewUserdataMT(L, t, LuaKey_Touch);					// funcs, func, event, t1, t2, ....
					}
					lua_call(L, gTouchs->size() + 1, 0);						// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouchs = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchesEnded = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onTouchesCancelled", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchAllAtOnce>(L, "onTouchesCancelled error! need 2 args: self, func/null", LuaKey_EventListenerTouchAllAtOnce);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchesCancelled = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouchs = &ts;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					for (var t : *gTouchs)
					{
						Lua_NewUserdataMT(L, t, LuaKey_Touch);					// funcs, func, event, t1, t2, ....
					}
					lua_call(L, gTouchs->size() + 1, 0);						// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouchs = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchesCancelled = nullptr;
		}
		return 0;
	});

	lua_pop(L, 1);
}






inline void Lua_Register_EventListenerTouchOneByOne(lua_State* const& L)
{
	Lua_NewCcMT(L, LuaKey_EventListenerTouchOneByOne, LuaKey_EventListener);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerTouchOneByOne();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_EventListenerTouchOneByOne);
	});

	Lua_NewFunc(L, "setSwallowTouches", [](lua_State* L)
	{
		var t = Lua_ToTB<cocos2d::EventListenerTouchOneByOne>(L, "setSwallowTouches error! need 2 args: self, bool", LuaKey_EventListenerTouchOneByOne);
		std::get<0>(t)->setSwallowTouches(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isSwallowTouches", [](lua_State* L)
	{
		var t = Lua_ToTB<cocos2d::EventListenerTouchOneByOne>(L, "isSwallowTouches error! need 1 args: self", LuaKey_EventListenerTouchOneByOne);
		var r = std::get<0>(t)->isSwallowTouches();
		lua_pushboolean(L, r);
		return 1;
	});

	Lua_NewFunc(L, "onTouchBegan", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchOneByOne>(L, "onTouchBegan error! need 2 args: self, func/null", LuaKey_EventListenerTouchOneByOne);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchBegan = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouch = touch;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					var L = gLua;
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					Lua_NewUserdataMT(L, gTouch, LuaKey_Touch);					// funcs, func, event, touch
					lua_call(L, 2, 1);											// funcs, bool?
					if (!lua_isboolean(L, 2))
					{
						luaL_error(L, "error! onTouchBegan need return bool.");
					}
					return 1;
				}, 0);
				if (int r = lua_pcall(L, 0, 1, 0))								// bool?
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
					return false;
				}
				return (bool)lua_toboolean(L, 1);
			};
		}
		else
		{
			std::get<0>(t)->onTouchBegan = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onTouchMoved", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchOneByOne>(L, "onTouchMoved error! need 2 args: self, func/null", LuaKey_EventListenerTouchOneByOne);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchMoved = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouch = touch;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					Lua_NewUserdataMT(L, gTouch, LuaKey_Touch);					// funcs, func, event, touch
					lua_call(L, 2, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouch = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchMoved = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onTouchEnded", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchOneByOne>(L, "onTouchEnded error! need 2 args: self, func/null", LuaKey_EventListenerTouchOneByOne);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchEnded = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouch = touch;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					Lua_NewUserdataMT(L, gTouch, LuaKey_Touch);					// funcs, func, event, touch
					lua_call(L, 2, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouch = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchEnded = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onTouchCancelled", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::EventListenerTouchOneByOne>(L, "onTouchCancelled error! need 2 args: self, func/null", LuaKey_EventListenerTouchOneByOne);
		if (std::get<1>(t).funcId)
		{
			std::get<0>(t)->onTouchCancelled = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				var L = gLua;
				gFuncId = f.funcId;
				gTouch = touch;
				gEvent = e;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					Lua_NewUserdataMT(L, gEvent, LuaKey_Event);					// funcs, func, event
					Lua_NewUserdataMT(L, gTouch, LuaKey_Touch);					// funcs, func, event, touch
					lua_call(L, 2, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				gFuncId = 0;
				gTouch = nullptr;
				gEvent = nullptr;
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			std::get<0>(t)->onTouchCancelled = nullptr;
		}
		return 0;
	});

	lua_pop(L, 1);
}
