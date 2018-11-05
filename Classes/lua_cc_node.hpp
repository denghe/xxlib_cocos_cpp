#pragma once

inline void Lua_Register_Node(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Node*>::value, TypeNames<cocos2d::Ref*>::value);						// cc, Node : Ref

	Lua_NewFunc(L, "new", [](lua_State* L)
	{
		var o = new (std::nothrow) cocos2d::Node();
		if (!o) return 0;
		if (!o->init()) { delete o; return 0; }
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var o = cocos2d::Node::create();
		return Lua_Push(L, o);
	});

	Lua_NewFunc(L, "addChild", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*>(L, "addChild error! need 2 args: self, child");
		std::get<0>(t)->addChild(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeFromParent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "removeFromParent error! need 1 args: self");
		std::get<0>(t)->removeFromParent();
		return 0;
	});

	Lua_NewFunc(L, "setPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setPosition error! need 3 args: self, x, y");
		std::get<0>(t)->setPosition(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "getPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPosition error! need 1 args: self");
		var r = std::get<0>(t)->getPosition();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setPosition3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float, float>(L, "setPosition3D error! need 4 args: self, x, y, z");
		std::get<0>(t)->setPosition3D({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getPosition3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPosition3D error! need 1 args: self");
		var r = std::get<0>(t)->getPosition3D();
		return Lua_Pushs(L, r.x, r.y, r.z);
	});

	Lua_NewFunc(L, "setAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setAnchorPoint error! need 3 args: self, x, y");
		std::get<0>(t)->setAnchorPoint({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getAnchorPoint error! need 1 args: self");
		var r = std::get<0>(t)->getAnchorPoint();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setLocalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "setLocalZOrder error! need 2 args: self, z(int)");
		std::get<0>(t)->setLocalZOrder(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setRotation", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float>(L, "setRotation error! need 2 args: self, angle");
		std::get<0>(t)->setRotation(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getRotation", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getRotation error! need 1 args: self");
		var r = std::get<0>(t)->getRotation();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setRotation3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float, float>(L, "setRotation3D error! need 4 args: self, xa, ya, za");
		std::get<0>(t)->setRotation3D({ std::get<1>(t) ,std::get<2>(t) ,std::get<3>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getRotation3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getRotation3D error! need 1 args: self");
		var r = std::get<0>(t)->getRotation3D();
		return Lua_Pushs(L, r.x, r.y, r.z);
	});

	Lua_NewFunc(L, "setScale", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float>(L, "setScale error! need 2 args: self, scale(assign to x & y)");
		std::get<0>(t)->setScale(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getScale", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScale error! need 1 args: self");
		var r = std::get<0>(t)->getScale();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setScaleXY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setScaleXY error! need 3 args: self, scaleX, scaleY");
		std::get<0>(t)->setScale(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "getScaleXY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScaleXY error! need 1 args: self");
		var r1 = std::get<0>(t)->getScaleX();
		var r2 = std::get<0>(t)->getScaleY();
		return Lua_Pushs(L, r1, r2);
	});

	Lua_NewFunc(L, "setVisible", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setVisible error! need 2 args: self, visible(bool)");
		std::get<0>(t)->setVisible(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isVisible", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "isVisible error! need 1 args: self");
		var r = std::get<0>(t)->isVisible();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "convertToNodeSpace", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "convertToNodeSpace error! need 3 args: self, x, y");
		var r = std::get<0>(t)->convertToNodeSpace({ std::get<1>(t), std::get<2>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setContentSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setContentSize error! need 3 args: self, w, h");
		std::get<0>(t)->setContentSize({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getContentSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getContentSize error! need 1 args: self");
		var r = std::get<0>(t)->getContentSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "containsPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "containsPoint error! need 3 args: self, x, y");
		var s = std::get<0>(t)->getContentSize();
		cocos2d::Rect r(0, 0, s.width, s.height);
		var b = r.containsPoint({ std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, b);
	});

	Lua_NewFunc(L, "containsTouch", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Touch*>(L, "containsTouch error! need 2 args: self, touch");
		var tL = std::get<1>(t)->getLocation();
		var p = std::get<0>(t)->convertToNodeSpace(tL);
		var s = std::get<0>(t)->getContentSize();
		cocos2d::Rect r{ 0,0, s.width, s.height };
		var b = r.containsPoint(p);
		return Lua_Push(L, b);
	});

	Lua_NewFunc(L, "removeAllChildrenWithCleanup", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*, bool>(L, "removeAllChildrenWithCleanup error! need 2 args: self, bool");
		std::get<0>(t)->removeAllChildrenWithCleanup(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*, bool>(L, "setTag error! need 2 args: self, int");
		std::get<0>(t)->setTag(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getTag error! need 1 args: self");
		var r = std::get<0>(t)->getTag();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Scene*, std::string>(L, "setName error! need 2 args: self, int");
		std::get<0>(t)->setName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getName error! need 1 args: self");
		var r = std::get<0>(t)->getName();
		return Lua_Pushs(L, r);
	});





	// 增加一组易于使用的函数. 大写开头. 和 cocos 本体函数以示区别
	// 本组函数因为返回了 self, 可连写

	Lua_NewFunc(L, "addEventListenerWithSceneGraphPriority", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::EventListener*>(L, "addEventListenerWithSceneGraphPriority error! need 2 args: self, eventListener");
		std::get<0>(t)->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<1>(t), std::get<0>(t));
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
	// todo: more

	lua_pop(L, 1);													// cc
}
