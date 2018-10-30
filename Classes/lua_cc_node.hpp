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

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::Node::create();
		if (!o) return 0;
		return Lua_NewUserdataMT(L, o, LuaKey_Node);
	});

	Lua_NewFunc(L, "addChild", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Node*, cocos2d::Node*>(L, "addChild error! need 2 args: self, child", LuaKey_Node, LuaKey_Node);
		std::get<0>(t)->addChild(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeFromParent", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Node*>(L, "removeFromParent error! need 1 args: self", LuaKey_Node);
		std::get<0>(t)->removeFromParent();
		return 0;
	});

	Lua_NewFunc(L, "setPosition", [](lua_State* L)
	{
		var t = Lua_ToTff<cocos2d::Node*>(L, "setPosition error! need 3 args: self, x, y", LuaKey_Node);
		std::get<0>(t)->setPosition(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "setPosition3D", [](lua_State* L)
	{
		var t = Lua_ToTfff<cocos2d::Node*>(L, "setPosition3D error! need 4 args: self, x, y, z", LuaKey_Node);
		std::get<0>(t)->setPosition3D({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTff<cocos2d::Node*>(L, "setAnchorPoint error! need 3 args: self, x, y", LuaKey_Node);
		std::get<0>(t)->setAnchorPoint({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setLocalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTi<cocos2d::Node*>(L, "setLocalZOrder error! need 2 args: self, z(int)", LuaKey_Node);
		std::get<0>(t)->setLocalZOrder(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setRotation", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::Node*>(L, "setRotation error! need 2 args: self, angle", LuaKey_Node);
		std::get<0>(t)->setRotation(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setRotation3D", [](lua_State* L)
	{
		var t = Lua_ToTfff<cocos2d::Node*>(L, "setRotation3D error! need 4 args: self, xa, ya, za", LuaKey_Node);
		std::get<0>(t)->setRotation3D({ std::get<1>(t) ,std::get<2>(t) ,std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setScale", [](lua_State* L)
	{
		var t = Lua_ToTf<cocos2d::Node*>(L, "setScale error! need 2 args: self, scale(assign to x & y)", LuaKey_Node);
		std::get<0>(t)->setScale(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setScaleXY", [](lua_State* L)
	{
		var t = Lua_ToTff<cocos2d::Node*>(L, "setScaleXY error! need 3 args: self, scaleX, scaleY", LuaKey_Node);
		std::get<0>(t)->setScale(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "setVisible", [](lua_State* L)
	{
		var t = Lua_ToTb<cocos2d::Node*>(L, "setVisible error! need 2 args: self, bool", LuaKey_Node);
		std::get<0>(t)->setVisible(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "convertToNodeSpace", [](lua_State* L)
	{
		var t = Lua_ToTff<cocos2d::Node*>(L, "convertToNodeSpace error! need 3 args: self, x, y", LuaKey_Node);
		var r = std::get<0>(t)->convertToNodeSpace({ std::get<1>(t), std::get<2>(t) });
		lua_pushnumber(L, r.x);
		lua_pushnumber(L, r.y);
		return 2;
	});

	Lua_NewFunc(L, "getContentSize", [](lua_State* L)
	{
		var t = Lua_ToT<cocos2d::Node*>(L, "getContentSize error! need 1 args: self", LuaKey_Node);
		var r = std::get<0>(t)->getContentSize();
		lua_pushnumber(L, r.width);
		lua_pushnumber(L, r.height);
		return 2;
	});

	Lua_NewFunc(L, "setContentSize", [](lua_State* L)
	{
		var t = Lua_ToTff<cocos2d::Node*>(L, "setContentSize error! need 3 args: self, w, h", LuaKey_Node);
		std::get<0>(t)->setContentSize({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "containsPoint", [](lua_State* L)
	{
		var t = Lua_ToTff<cocos2d::Node*>(L, "containsPoint error! need 3 args: self, x, y", LuaKey_Node);
		var s = std::get<0>(t)->getContentSize();
		cocos2d::Rect r(0, 0, s.width, s.height);
		var b = r.containsPoint({ std::get<1>(t), std::get<2>(t) });
		lua_pushboolean(L, b);
		return 1;
	});

	Lua_NewFunc(L, "containsTouch", [](lua_State* L)
	{
		var t = Lua_ToTT<cocos2d::Node*, cocos2d::Touch*>(L, "containsTouch error! need 2 args: self, touch", LuaKey_Node, LuaKey_Touch);
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
		var t = Lua_ToTT<cocos2d::Node*, cocos2d::EventListener*>(L, "addEventListener error! need 2 args: self, eventListener", LuaKey_Node, LuaKey_EventListener);
		std::get<0>(t)->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<1>(t), std::get<0>(t));
		return 0;
	});






	// 增加一组易于使用的函数. 大写开头. 和 cocos 本体函数以示区别

	Lua_NewFunc(L, "SetOP", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, float, float>
			(L, "SetOP(Owner Positon) error! need 4 args: self, owner, px, py");
		std::get<1>(t)->addChild(std::get<0>(t));
		std::get<0>(t)->setPosition(std::get<2>(t),std::get<3>(t));
		lua_pushvalue(L, 1);
		return 1;
	});

	Lua_NewFunc(L, "SetOPA", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, float, float, float, float>
			(L, "SetOPA(Owner Positon Anchor) error! need 6 args: self, owner, px, py, ax, ay");
		std::get<1>(t)->addChild(std::get<0>(t));
		std::get<0>(t)->setPosition(std::get<2>(t),std::get<3>(t));
		std::get<0>(t)->setAnchorPoint({ std::get<4>(t), std::get<5>(t) });
		lua_pushvalue(L, 1);
		return 1;
	});

	Lua_NewFunc(L, "SetOPAS", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, float, float, float, float, float, float>
			(L, "SetOPAS(Owner Positon Anchor ScaleXY) error! need 8 args: self, owner, px, py, ax, ay, sx, sy");
		std::get<1>(t)->addChild(std::get<0>(t));
		std::get<0>(t)->setPosition(std::get<2>(t), std::get<3>(t));
		std::get<0>(t)->setAnchorPoint({ std::get<4>(t), std::get<5>(t) });
		std::get<0>(t)->setScaleX(std::get<6>(t));
		std::get<0>(t)->setScaleY(std::get<7>(t));
		lua_pushvalue(L, 1);
		return 1;
	});

	Lua_NewFunc(L, "SetVZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool, int>(L, "SetVZ(Visible LocalZOrder) error! need 3 args: self, visible(bool), z(int)");
		std::get<0>(t)->setVisible(std::get<1>(t));
		std::get<0>(t)->setLocalZOrder(std::get<2>(t));
		lua_pushvalue(L, 1);
		return 1;
	});

	lua_pop(L, 1);													// cc
}
