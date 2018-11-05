#pragma once

inline void Lua_Register_uiWidget(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ui::Widget*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "addTouchEventListener", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, Lua_Func>(L, "addTouchEventListener error! need 2 args: self, func<Ref, UITouchEventType>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->addTouchEventListener([f = std::move(std::get<1>(t))](cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType tet)
			{
				Lua_PCall(gLua, f, ref, tet);
			});
		}
		else
		{
			std::get<0>(t)->addTouchEventListener(nullptr);
		}
		return 0;
	});

	lua_pop(L, 1);

	lua_pushstring(L, "UITouchEventType");
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "BEGAN");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::BEGAN);	lua_rawset(L, -3);
	lua_pushstring(L, "MOVED");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::MOVED);	lua_rawset(L, -3);
	lua_pushstring(L, "ENDED");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::ENDED);	lua_rawset(L, -3);
	lua_pushstring(L, "CANCELED");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::CANCELED);	lua_rawset(L, -3);
	lua_rawset(L, -3);
}
