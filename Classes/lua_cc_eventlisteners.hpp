#pragma once

inline void Lua_Register_EventListener(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::EventListener*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*, bool>(L, "setEnabled error! need 2 args: self, bool");
		std::get<0>(t)->setEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*>(L, "isEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isEnabled();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "checkAvailable", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListener*>(L, "checkAvailable error! need 1 args: self");
		var r = std::get<0>(t)->checkAvailable();
		return Lua_Push(L, r);
	});


	lua_pop(L, 1);
}






inline void Lua_Register_EventListenerTouchAllAtOnce(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::EventListenerTouchAllAtOnce*>::value, TypeNames<cocos2d::EventListener*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerTouchAllAtOnce();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::EventListenerTouchAllAtOnce::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "onTouchesBegan", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesBegan error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesBegan = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				lua_checkstack(gLua, ts.size() + 3);
				Lua_Push(gLua, f);
				Lua_Push(gLua, e);
				for (var t : ts)
				{
					Lua_Push(gLua, t);
				}
				Lua_PCall(gLua, ts.size() + 1);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesMoved error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesMoved = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				lua_checkstack(gLua, ts.size() + 3);
				Lua_Push(gLua, f);
				Lua_Push(gLua, e);
				for (var t : ts)
				{
					Lua_Push(gLua, t);
				}
				Lua_PCall(gLua, ts.size() + 1);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesEnded error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesEnded = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				lua_checkstack(gLua, ts.size() + 3);
				Lua_Push(gLua, f);
				Lua_Push(gLua, e);
				for (var t : ts)
				{
					Lua_Push(gLua, t);
				}
				Lua_PCall(gLua, ts.size() + 1);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesCancelled error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesCancelled = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				lua_checkstack(gLua, ts.size() + 3);
				Lua_Push(gLua, f);
				Lua_Push(gLua, e);
				for (var t : ts)
				{
					Lua_Push(gLua, t);
				}
				Lua_PCall(gLua, ts.size() + 1);
				lua_settop(gLua, 0);
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
	Lua_NewMT(L, TypeNames<cocos2d::EventListenerTouchOneByOne*>::value, TypeNames<cocos2d::EventListener*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerTouchOneByOne();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::EventListenerTouchOneByOne::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "setSwallowTouches", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, bool>(L, "setSwallowTouches error! need 2 args: self, bool");
		std::get<0>(t)->setSwallowTouches(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isSwallowTouches", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*>(L, "isSwallowTouches error! need 1 args: self");
		var r = std::get<0>(t)->isSwallowTouches();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "onTouchBegan", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchBegan error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchBegan = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, touch);
				var t = Lua_ToTuple<bool>(gLua, "onTouchBegan return error! need return 1 result: bool");
				lua_settop(gLua, 0);
				return std::get<0>(t);
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
		var t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchMoved error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchMoved = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, touch);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchEnded error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchEnded = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, touch);
				lua_settop(gLua, 0);
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
		var t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchCancelled error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchCancelled = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, touch);
				lua_settop(gLua, 0);
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






inline void Lua_Register_EventListenerKeyboard(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::EventListenerKeyboard*>::value, TypeNames<cocos2d::EventListener*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::EventListenerKeyboard();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::EventListenerKeyboard::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "onKeyPressed", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListenerKeyboard*, Lua_Func>(L, "onKeyPressed error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onKeyPressed = [f = std::move(std::get<1>(t))](cocos2d::EventKeyboard::KeyCode kc, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, kc);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			std::get<0>(t)->onKeyPressed = nullptr;
		}
		return 0;
	});

	Lua_NewFunc(L, "onKeyReleased", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::EventListenerKeyboard*, Lua_Func>(L, "onKeyReleased error! need 2 args: self, func/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onKeyReleased = [f = std::move(std::get<1>(t))](cocos2d::EventKeyboard::KeyCode kc, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, kc);
				lua_settop(gLua, 0);
			};
		}
		else
		{
			std::get<0>(t)->onKeyReleased = nullptr;
		}
		return 0;
	});

	lua_pop(L, 1);
}
