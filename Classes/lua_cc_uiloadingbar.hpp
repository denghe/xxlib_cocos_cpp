#pragma once

inline void Lua_Register_uiLoadingBar(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ui::LoadingBar*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		auto&& o = new (std::nothrow) cocos2d::ui::LoadingBar();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ui::LoadingBar* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			o = cocos2d::ui::LoadingBar::create();
			break;
		}
		case 1:
		{
			auto&& t = Lua_ToTuple<std::string>(L);
			o = cocos2d::ui::LoadingBar::create(std::get<0>(t));
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<std::string, cocos2d::ui::Widget::TextureResType>(L);
			o = cocos2d::ui::LoadingBar::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 3:
		{
			auto&& t = Lua_ToTuple<std::string, cocos2d::ui::Widget::TextureResType, float>(L);
			o = cocos2d::ui::LoadingBar::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "create LoadingBar error! need 0 ~ 3 args: string textureName, TextureResType texType, float percentage = 0");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "setDirection", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*, cocos2d::ui::LoadingBar::Direction>(L, "setDirection error! need 2 args: self, Direction");
		std::get<0>(t)->setDirection(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getDirection", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*>(L, "setDirection error! need 1 args: self");
		auto&& r = std::get<0>(t)->getDirection();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "loadTexture", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*, std::string>(L);
			std::get<0>(t)->loadTexture(std::get<1>(t));
			break;
		}
		case 3:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTexture(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTexture error! need 2 ~ 3 args: self, string textureName, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "setPercent", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*, float>(L, "setPercent error! need 2 args: self, float percent");
		std::get<0>(t)->setPercent(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getPercent", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*>(L, "getPercent error! need 1 args: self");
		auto&& r = std::get<0>(t)->getPercent();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setScale9Enabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*, bool>(L, "setScale9Enabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setScale9Enabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isScale9Enabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*>(L, "isScale9Enabled error! need 1 args: self");
		auto&& r = std::get<0>(t)->isScale9Enabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setCapInsets", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*, float, float, float, float>(L, "setCapInsets error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsets({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsets", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::LoadingBar*>(L, "getCapInsets error! need 1 args: self");
		auto&& r = std::get<0>(t)->getCapInsets();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	lua_pop(L, 1);

	lua_pushstring(L, TypeNames<cocos2d::ui::LoadingBar::Direction>::value);
	lua_createtable(L, 2, 0);
	lua_pushstring(L, "LEFT");	lua_pushinteger(L, (int)cocos2d::ui::LoadingBar::Direction::LEFT);	lua_rawset(L, -3);
	lua_pushstring(L, "RIGHT");	lua_pushinteger(L, (int)cocos2d::ui::LoadingBar::Direction::RIGHT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

}
