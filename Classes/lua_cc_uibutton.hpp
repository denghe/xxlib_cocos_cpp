#pragma once

inline void Lua_Register_uiButton(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ui::Button*>::value, TypeNames<cocos2d::ui::Widget*>::value);

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::ui::Button();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "init", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Button*, std::string, std::string, std::string, int>(L, "init error! need 5 args: self, normalImg(string), selectedImg(string), disableImg(string), texType(UITextureResType)");
		std::get<0>(t)->init(std::get<1>(t), std::get<2>(t), std::get<3>(t), (cocos2d::ui::Widget::TextureResType)std::get<4>(t));
		return 0;
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::ui::Button::create();
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "createWithImages", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, std::string, std::string, int>(L, "createWithImages error! need 4 args: normalImg(string), selectedImg(string), disableImg(string), texType(UITextureResType)");
		var o = cocos2d::ui::Button::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), (cocos2d::ui::Widget::TextureResType)std::get<3>(t));
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);

}
