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
		var t = Lua_ToTuple<cocos2d::ui::ImageView*, std::string, int>(L, "init error! need 3 args: self, img(string), texType(UITextureResType)");
		std::get<0>(t)->init(std::get<1>(t), (cocos2d::ui::Widget::TextureResType)std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::ui::ImageView::create();	// todo
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);
}
