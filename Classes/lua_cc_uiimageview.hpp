#pragma once

inline void Lua_Register_uiImageView(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ui::ImageView*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::ui::ImageView();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "init", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::ui::ImageView*>(L);
			std::get<0>(t)->init();
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string>(L);
			std::get<0>(t)->init(std::get<1>(t));
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string, cocos2d::ui::Widget::TextureResType>(L);
			std::get<0>(t)->init(std::get<1>(t), std::get<2>(t));
		}
		default:
			return luaL_error(L, "init error! need 1 ~ 3 args: self, string imageFileName, cc.UITextureResType texType = LOCAL");
		}
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ui::ImageView* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			o = cocos2d::ui::ImageView::create();
		}
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			o = cocos2d::ui::ImageView::create(std::get<0>(t));
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, cocos2d::ui::Widget::TextureResType>(L);
			o = cocos2d::ui::ImageView::create(std::get<0>(t), std::get<1>(t));
		}
		default:
			return luaL_error(L, "init error! need 0 ~ 2 args: string imageFileName, cc.UITextureResType texType = LOCAL");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);
}
