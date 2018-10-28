#pragma once

inline void Lua_Register_Node(lua_State* const& L)
{
	Lua_NewMT(L, LuaKey_Node, LuaKey_Ref);						// cc, Node : Ref

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Node();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_NewUserdataMT(L, o, LuaKey_Node);
	});

	Lua_NewFunc(L, "addChild", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Node, cocos2d::Node>(L, "addChild error! need 2 args: self, child", LuaKey_Node, LuaKey_Node);
		std::get<0>(t)->addChild(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeFromParent", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Node>(L, "removeFromParent error! need 1 args: self", LuaKey_Node);
		std::get<0>(t)->removeFromParent();
		return 0;
	});

	Lua_NewFunc(L, "setPosition", [](lua_State* L)
	{
		var t = Lua_ToTFF<cocos2d::Node>(L, "setPosition error! need 3 args: self, x, y", LuaKey_Node);
		std::get<0>(t)->setPosition(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "setAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTFF<cocos2d::Node>(L, "setAnchorPoint error! need 3 args: self, x, y", LuaKey_Node);
		std::get<0>(t)->setAnchorPoint({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setLocalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTI<cocos2d::Node>(L, "setLocalZOrder error! need 2 args: self, z(int)", LuaKey_Node);
		std::get<0>(t)->setLocalZOrder(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setRotation", [](lua_State* L)
	{
		var t = Lua_ToTF<cocos2d::Node>(L, "setRotation error! need 2 args: self, angle(360 float)", LuaKey_Node);
		std::get<0>(t)->setRotation(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setScale", [](lua_State* L)
	{
		var t = Lua_ToTF<cocos2d::Node>(L, "setScale error! need 2 args: self, scale(assign to x & y)", LuaKey_Node);
		std::get<0>(t)->setScale(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setScaleXY", [](lua_State* L)
	{
		var t = Lua_ToTFF<cocos2d::Node>(L, "setScaleXY error! need 3 args: self, scaleX, scaleY", LuaKey_Node);
		std::get<0>(t)->setScale(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "convertToNodeSpace", [](lua_State* L)
	{
		var t = Lua_ToTFF<cocos2d::Node>(L, "convertToNodeSpace error! need 3 args: self, x, y", LuaKey_Node);
		var r = std::get<0>(t)->convertToNodeSpace({ std::get<1>(t), std::get<2>(t) });
		lua_pushnumber(L, r.x);
		lua_pushnumber(L, r.y);
		return 2;
	});

	Lua_NewFunc(L, "getContentSize", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Node>(L, "getContentSize error! need 1 args: self", LuaKey_Node);
		var r = std::get<0>(t)->getContentSize();
		lua_pushnumber(L, r.width);
		lua_pushnumber(L, r.height);
		return 2;
	});

	Lua_NewFunc(L, "containsPoint", [](lua_State* L)
	{
		var t = Lua_ToTFF<cocos2d::Node>(L, "containsPoint error! need 3 args: self, x, y", LuaKey_Node);
		var s = std::get<0>(t)->getContentSize();
		cocos2d::Rect r(0, 0, s.width, s.height);
		var b = r.containsPoint({ std::get<1>(t), std::get<2>(t) });
		lua_pushboolean(L, b);
		return 1;
	});

	Lua_NewFunc(L, "containsTouch", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Node, cocos2d::Touch>(L, "containsTouch error! need 2 args: self, touch", LuaKey_Node, LuaKey_Touch);
		var tL = std::get<1>(t)->getLocation();
		var p = std::get<0>(t)->convertToNodeSpace(tL);
		var s = std::get<0>(t)->getContentSize();
		cocos2d::Rect r{ 0,0, s.width, s.height };
		var b = r.containsPoint(p);
		lua_pushboolean(L, b);
		return 1;
	});

	Lua_NewFunc(L, "addEventListener", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Node, cocos2d::EventListener>(L, "addEventListener error! need 2 args: self, eventListener", LuaKey_Node, LuaKey_EventListener);
		std::get<0>(t)->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<1>(t), std::get<0>(t));
		return 0;
	});

	lua_pop(L, 1);													// cc
}
