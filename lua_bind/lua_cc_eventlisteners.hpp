#pragma once

inline void Lua_Register_EventListener(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::EventListener*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*, bool>(L, "setEnabled error! need 2 args: self, bool");
		std::get<0>(t)->setEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*>(L, "isEnabled error! need 1 args: self");
		auto&& r = std::get<0>(t)->isEnabled();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "checkAvailable", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*>(L, "checkAvailable error! need 1 args: self");
		auto&& r = std::get<0>(t)->checkAvailable();
		return Lua_Push(L, r);
	});


	lua_pop(L, 1);
}






inline void Lua_Register_EventListenerTouchAllAtOnce(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::EventListenerTouchAllAtOnce*>::value, TypeNames<cocos2d::EventListener*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		auto&& o = new (std::nothrow) cocos2d::EventListenerTouchAllAtOnce();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& o = cocos2d::EventListenerTouchAllAtOnce::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "onTouchesBegan", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesBegan error! need 2 args: self, func<e, touchs...>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesBegan = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, ts);
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesMoved error! need 2 args: self, func<e, touchs...>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesMoved = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, ts);
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesEnded error! need 2 args: self, func<e, touchs...>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesEnded = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, ts);
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchAllAtOnce*, Lua_Func>(L, "onTouchesCancelled error! need 2 args: self, func<e, touchs...>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchesCancelled = [f = std::move(std::get<1>(t))](const std::vector<cocos2d::Touch*>& ts, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, e, ts);
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
		auto&& o = new (std::nothrow) cocos2d::EventListenerTouchOneByOne();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& o = cocos2d::EventListenerTouchOneByOne::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "setSwallowTouches", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, bool>(L, "setSwallowTouches error! need 2 args: self, bool");
		std::get<0>(t)->setSwallowTouches(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isSwallowTouches", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*>(L, "isSwallowTouches error! need 1 args: self");
		auto&& r = std::get<0>(t)->isSwallowTouches();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "onTouchBegan", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchBegan error! need 2 args: self, func<touch, e>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchBegan = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, touch, e);
				auto&& t = Lua_ToTuple<bool>(gLua, "onTouchBegan return error! need return 1 result: bool");
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchMoved error! need 2 args: self, func<touch, e>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchMoved = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, touch, e);
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchEnded error! need 2 args: self, func<touch, e>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchEnded = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, touch, e);
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerTouchOneByOne*, Lua_Func>(L, "onTouchCancelled error! need 2 args: self, func<touch, e>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onTouchCancelled = [f = std::move(std::get<1>(t))](cocos2d::Touch* touch, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, touch, e);
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
		auto&& o = new (std::nothrow) cocos2d::EventListenerKeyboard();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& o = cocos2d::EventListenerKeyboard::create();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "onKeyPressed", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListenerKeyboard*, Lua_Func>(L, "onKeyPressed error! need 2 args: self, func<KeyCode, Event>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onKeyPressed = [f = std::move(std::get<1>(t))](cocos2d::EventKeyboard::KeyCode kc, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, kc, e);
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
		auto&& t = Lua_ToTuple<cocos2d::EventListenerKeyboard*, Lua_Func>(L, "onKeyReleased error! need 2 args: self, func<KeyCode, Event>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->onKeyReleased = [f = std::move(std::get<1>(t))](cocos2d::EventKeyboard::KeyCode kc, cocos2d::Event* e)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, kc, e);
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






inline void Lua_Register_EventListenerAssetsManagerEx(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::extension::EventAssetsManagerEx*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "getEventCode", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getEventCode error! need 1 args: self");
		auto&& r = std::get<0>(t)->getEventCode();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getCURLECode", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getCURLECode error! need 1 args: self");
		auto&& r = std::get<0>(t)->getCURLECode();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getCURLMCode", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getCURLMCode error! need 1 args: self");
		auto&& r = std::get<0>(t)->getCURLMCode();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getMessage", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getMessage error! need 1 args: self");
		auto&& r = std::get<0>(t)->getMessage();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getAssetId", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getAssetId error! need 1 args: self");
		auto&& r = std::get<0>(t)->getAssetId();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getAssetsManagerEx", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getAssetsManagerEx error! need 1 args: self");
		auto&& r = std::get<0>(t)->getAssetsManagerEx();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getPercent", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getPercent error! need 1 args: self");
		auto&& r = std::get<0>(t)->getPercent();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getPercentByFile", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventAssetsManagerEx*>(L, "getPercentByFile error! need 1 args: self");
		auto&& r = std::get<0>(t)->getPercentByFile();
		return Lua_Pushs(L, r);
	});

	lua_pop(L, 1);





	Lua_NewMT(L, TypeNames<cocos2d::extension::EventListenerAssetsManagerEx*>::value, TypeNames<cocos2d::EventListener*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		auto&& o = new (std::nothrow) cocos2d::extension::EventListenerAssetsManagerEx();
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "init", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::EventListenerAssetsManagerEx*, cocos2d::extension::AssetsManagerEx*, Lua_Func>(L, "init error! need 3 args: self, AssetsManagerEx am, function<void(EventAssetsManagerEx*)>& callback");
		std::get<0>(t)->init(std::get<1>(t), [f = std::move(std::get<2>(t))](cocos2d::extension::EventAssetsManagerEx* e)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, e);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*, Lua_Func>(L, "create EventListenerAssetsManagerEx error! need 3 args: self, AssetsManagerEx am, function<void(EventAssetsManagerEx*)>& callback");
		auto&& o = cocos2d::extension::EventListenerAssetsManagerEx::create(std::get<0>(t), [f = std::move(std::get<1>(t))](cocos2d::extension::EventAssetsManagerEx* e)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, e);
			lua_settop(gLua, 0);
		});
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);


	lua_pushstring(L, TypeNames<cocos2d::extension::EventAssetsManagerEx::EventCode>::value);
	lua_createtable(L, 1, 0);
	lua_pushstring(L, "ERROR_NO_LOCAL_MANIFEST");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST);	lua_rawset(L, -3);
	lua_pushstring(L, "ERROR_DOWNLOAD_MANIFEST");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST);	lua_rawset(L, -3);
	lua_pushstring(L, "ERROR_PARSE_MANIFEST");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST);	lua_rawset(L, -3);
	lua_pushstring(L, "NEW_VERSION_FOUND");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND);	lua_rawset(L, -3);
	lua_pushstring(L, "ALREADY_UP_TO_DATE");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE);	lua_rawset(L, -3);
	lua_pushstring(L, "UPDATE_PROGRESSION");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION);	lua_rawset(L, -3);
	lua_pushstring(L, "ASSET_UPDATED");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ASSET_UPDATED);	lua_rawset(L, -3);
	lua_pushstring(L, "ERROR_UPDATING");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_UPDATING);	lua_rawset(L, -3);
	lua_pushstring(L, "UPDATE_FINISHED");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::UPDATE_FINISHED);	lua_rawset(L, -3);
	lua_pushstring(L, "UPDATE_FAILED");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::UPDATE_FAILED);	lua_rawset(L, -3);
	lua_pushstring(L, "ERROR_DECOMPRESS");	lua_pushinteger(L, (int)cocos2d::extension::EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS);	lua_rawset(L, -3);
	lua_rawset(L, -3);

}
