#pragma once

inline void Lua_Register_EventDispacher(lua_State* const& L)
{
	Lua_NewFunc(L, "addEventListenerWithSceneGraphPriority", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*, cocos2d::Node*>(L, "addEventListenerWithSceneGraphPriority error! need 2 args: EventListener listener, Node target");
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addEventListenerWithFixedPriority", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*, int>(L, "addEventListenerWithFixedPriority error! need 2 args: EventListener listener, int fixedPriority");
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	// 不实现 addCustomEventListener

	Lua_NewFunc(L, "removeEventListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*>(L, "removeEventListener error! need 1 args: EventListener listener");
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeEventListenersForType", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener::Type>(L, "removeEventListenersForType error! need 1 args: EventListenerType type");
		cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForType(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeEventListenersForTarget", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(std::get<0>(t));
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*, bool>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "removeEventListenersForTarget error! need 1 ~ 2 args: Node target, bool recursive = false");
		}
		}
		return 0;
	});

	// 不实现 removeCustomEventListeners

	Lua_NewFunc(L, "removeAllEventListeners", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
		return 0;
	});

	Lua_NewFunc(L, "pauseEventListenersForTarget", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(std::get<0>(t));
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*, bool>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "pauseEventListenersForTarget error! need 1 ~ 2 args: Node target, bool recursive = false");
		}
		}
		return 0;
	});

	Lua_NewFunc(L, "resumeEventListenersForTarget", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(std::get<0>(t));
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*, bool>(L);
			cocos2d::Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "resumeEventListenersForTarget error! need 1 ~ 2 args: Node target, bool recursive = false");
		}
		}
		return 0;
	});

	Lua_NewFunc(L, "setPriority", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::EventListener*, int>(L, "setPriority error! need 2 args: EventListener* listener, int fixedPriority");
		cocos2d::Director::getInstance()->getEventDispatcher()->setPriority(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<bool>(L, "setEnabled error! need 1 args: bool isEnabled");
		cocos2d::Director::getInstance()->getEventDispatcher()->setEnabled(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		auto&& r = cocos2d::Director::getInstance()->getEventDispatcher()->isEnabled();
		return Lua_Pushs(L, r);
	});

	// 不实现 dispatchEvent dispatchCustomEvent

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string>(L);
		auto&& r = cocos2d::Director::getInstance()->getEventDispatcher()->hasEventListener(std::get<0>(t));
		return Lua_Pushs(L, r);
	});


	lua_pushstring(L, TypeNames<cocos2d::EventListener::Type>::value);
	lua_createtable(L, 9, 0);
	lua_pushstring(L, "UNKNOWN");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::UNKNOWN);	lua_rawset(L, -3);
	lua_pushstring(L, "TOUCH_ONE_BY_ONE");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::TOUCH_ONE_BY_ONE);	lua_rawset(L, -3);
	lua_pushstring(L, "TOUCH_ALL_AT_ONCE");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::TOUCH_ALL_AT_ONCE);	lua_rawset(L, -3);
	lua_pushstring(L, "KEYBOARD");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::KEYBOARD);	lua_rawset(L, -3);
	lua_pushstring(L, "MOUSE");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::MOUSE);	lua_rawset(L, -3);
	lua_pushstring(L, "ACCELERATION");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::ACCELERATION);	lua_rawset(L, -3);
	lua_pushstring(L, "FOCUS");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::FOCUS);	lua_rawset(L, -3);
	lua_pushstring(L, "GAME_CONTROLLER");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::GAME_CONTROLLER);	lua_rawset(L, -3);
	lua_pushstring(L, "CUSTOM");	lua_pushinteger(L, (int)cocos2d::EventListener::Type::CUSTOM);	lua_rawset(L, -3);
	lua_rawset(L, -3);
}
