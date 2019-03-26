#pragma once

inline void Lua_Register_uiImageView(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ui::ImageView*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		auto&& o = new (std::nothrow) cocos2d::ui::ImageView();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "init", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*>(L);
			std::get<0>(t)->init();
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string>(L);
			std::get<0>(t)->init(std::get<1>(t));
			break;
		}
		case 3:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->init(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "init error! need 1 ~ 3 args: self, string imageFileName, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ui::ImageView* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			o = cocos2d::ui::ImageView::create();
			break;
		}
		case 1:
		{
			auto&& t = Lua_ToTuple<std::string>(L);
			o = cocos2d::ui::ImageView::create(std::get<0>(t));
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<std::string, cocos2d::ui::Widget::TextureResType>(L);
			o = cocos2d::ui::ImageView::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
			return luaL_error(L, "init error! need 0 ~ 2 args: string imageFileName, TextureResType texType = LOCAL");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "loadTexture", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string>(L);
			std::get<0>(t)->loadTexture(std::get<1>(t));
			break;
		}
		case 3:
		{
			auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->loadTexture(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "loadTexture error! need 2 ~ 3 args: self, string fileName, TextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "setTextureRect", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, float, float, float, float>(L, "setTextureRect error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setTextureRect({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setScale9Enabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, bool>(L, "setScale9Enabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setScale9Enabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isScale9Enabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*>(L, "isScale9Enabled error! need 1 args: self");
		auto&& r = std::get<0>(t)->isScale9Enabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setCapInsets", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*, float, float, float, float>(L, "setCapInsets error! need 5 args: self, float rectX, rectY, rectW, rectH");
		std::get<0>(t)->setCapInsets({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getCapInsets", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*>(L, "getCapInsets error! need 1 args: self");
		auto&& r = std::get<0>(t)->getCapInsets();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "getRenderFile", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ui::ImageView*>(L, "getRenderFile error! need 1 args: self");
		auto&& r = std::get<0>(t)->getRenderFile();
		return Lua_Pushs(L, r.type, r.file, r.plist);
	});


	lua_pop(L, 1);
}
