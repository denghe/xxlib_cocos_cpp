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
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*>(L);
			std::get<0>(t)->addChild(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, int>(L);
			std::get<0>(t)->addChild(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			if (lua_isnumber(L, 4))
			{
				var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, int, int>(L);
				std::get<0>(t)->addChild(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			}
			else
			{
				var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, int, std::string>(L);
				std::get<0>(t)->addChild(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "addChild error! need 2 ~ 4 args: self, Node child, int localZOrder, int tag/string name");
		}
		return 0;
	});

	Lua_NewFunc(L, "getChildByTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "getChildByTag error! need 2 args: self, int tag");
		var r = std::get<0>(t)->getChildByTag(std::get<1>(t));
		if (!r) return 0;
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "getChildByName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, std::string>(L, "getChildByName error! need 2 args: self, string name");
		var r = std::get<0>(t)->getChildByName(std::get<1>(t));
		if (!r) return 0;
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "enumerateChildren", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, std::string, Lua_Func>(L, "enumerateChildren error! need 3 args: self, string name, function<bool(Node* node)>");
		std::get<0>(t)->enumerateChildren(std::get<1>(t), [f = std::move(std::get<2>(t))](cocos2d::Node* node)->bool
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, node);
			var t = Lua_ToTuple<bool>(gLua, "enumerateChildren return error! need return 1 result: bool");
			lua_settop(gLua, 0);
			return std::get<0>(t);
		});
		return 0;
	});

	Lua_NewFunc(L, "getChildren", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getChildren error! need 1 args: self");
		var r = std::get<0>(t)->getChildren();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "getChildrenCount", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getChildrenCount error! need 1 args: self");
		var r = std::get<0>(t)->getChildrenCount();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "setParent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*>(L, "setParent error! need 2 args: self, Node parent");
		std::get<0>(t)->setParent(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getParent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getParent error! need 1 args: self");
		var r = std::get<0>(t)->getParent();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "removeFromParent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "removeFromParent error! need 1 args: self");
		std::get<0>(t)->removeFromParent();
		return 0;
	});

	Lua_NewFunc(L, "removeFromParentAndCleanup", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "removeFromParentAndCleanup error! need 2 args: self, bool cleanup");
		std::get<0>(t)->removeFromParentAndCleanup(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "removeChild", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*>(L);
			std::get<0>(t)->removeChild(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, bool>(L);
			std::get<0>(t)->removeChild(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "removeChild error! need 2 ~ 3 args: self, Node child, bool cleanup = true");
		}
		return 0;
	});

	Lua_NewFunc(L, "removeChildByTag", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, int>(L);
			std::get<0>(t)->removeChildByTag(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, int, bool>(L);
			std::get<0>(t)->removeChildByTag(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "removeChildByTag error! need 2 ~ 3 args: self, int tag, bool cleanup = true");
		}
		return 0;
	});

	Lua_NewFunc(L, "removeChildByName", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, std::string>(L);
			std::get<0>(t)->removeChildByName(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, std::string, bool>(L);
			std::get<0>(t)->removeChildByName(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "removeChildByName error! need 2 ~ 3 args: self, string name, bool cleanup = true");
		}
		return 0;
	});

	Lua_NewFunc(L, "removeAllChildren", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "removeAllChildren error! need 1 args: self");
		std::get<0>(t)->removeAllChildren();
		return 0;
	});

	Lua_NewFunc(L, "removeAllChildrenWithCleanup", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "removeAllChildrenWithCleanup error! need 2 args: self, bool cleanup");
		std::get<0>(t)->removeAllChildrenWithCleanup(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "reorderChild", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Node*, int>(L, "reorderChild error! need 3 args: self, Node child, int localZOrder");
		std::get<0>(t)->reorderChild(std::get<1>(t), std::get<2>(t));
		return 0;
	});

	Lua_NewFunc(L, "sortAllChildren", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "sortAllChildren error! need 1 args: self");
		std::get<0>(t)->sortAllChildren();
		return 0;
	});

	Lua_NewFunc(L, "setPositionNormalized", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setPositionNormalized error! need 3 args: self, float x, float y");
		std::get<0>(t)->setPositionNormalized({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getPositionNormalized", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPositionNormalized error! need 1 args: self");
		var r = std::get<0>(t)->getPositionNormalized();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setPosition error! need 3 args: self, float x, float y");
		std::get<0>(t)->setPosition(std::get<1>(t), std::get<2>(t));
		return 0;
	});
	Lua_NewFunc(L, "getPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPosition error! need 1 args: self");
		var r = std::get<0>(t)->getPosition();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setPositionX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setPositionX error! need 3 args: self, float x");
		std::get<0>(t)->setPositionX(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getPositionX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPositionX error! need 1 args: self");
		var r = std::get<0>(t)->getPositionX();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPositionY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setPositionY error! need 3 args: self, float y");
		std::get<0>(t)->setPositionY(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getPositionY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPositionY error! need 1 args: self");
		var r = std::get<0>(t)->getPositionY();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPositionZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setPositionZ error! need 3 args: self, float z");
		std::get<0>(t)->setPositionZ(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getPositionZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPositionZ error! need 1 args: self");
		var r = std::get<0>(t)->getPositionZ();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPosition3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float, float>(L, "setPosition3D error! need 4 args: self, float x, float y, float z");
		std::get<0>(t)->setPosition3D({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getPosition3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getPosition3D error! need 1 args: self");
		var r = std::get<0>(t)->getPosition3D();
		return Lua_Pushs(L, r.x, r.y, r.z);
	});

	Lua_NewFunc(L, "setSkewX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setSkewX error! need 3 args: self, float x");
		std::get<0>(t)->setSkewX(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getSkewX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getSkewX error! need 1 args: self");
		var r = std::get<0>(t)->getSkewX();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setSkewY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setSkewY error! need 3 args: self, float y");
		std::get<0>(t)->setSkewY(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getSkewY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getSkewY error! need 1 args: self");
		var r = std::get<0>(t)->getSkewY();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setAnchorPoint error! need 3 args: self, float x, float y");
		std::get<0>(t)->setAnchorPoint({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getAnchorPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getAnchorPoint error! need 1 args: self");
		var r = std::get<0>(t)->getAnchorPoint();
		return Lua_Pushs(L, r.x, r.y);
	});
	Lua_NewFunc(L, "getAnchorPointInPoints", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getAnchorPointInPoints error! need 1 args: self");
		var r = std::get<0>(t)->getAnchorPointInPoints();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setLocalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "setLocalZOrder error! need 2 args: self, int z");
		std::get<0>(t)->setLocalZOrder(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getLocalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getLocalZOrder error! need 1 args: self");
		std::get<0>(t)->getLocalZOrder();
		return 0;
	});

	Lua_NewFunc(L, "setGlobalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "setGlobalZOrder error! need 2 args: self, int z");
		std::get<0>(t)->setGlobalZOrder(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getGlobalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getGlobalZOrder error! need 1 args: self");
		std::get<0>(t)->getGlobalZOrder();
		return 0;
	});

	Lua_NewFunc(L, "setRotationSkewX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "setRotationSkewX error! need 2 args: self, int sx");
		std::get<0>(t)->setRotationSkewX(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getRotationSkewX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getRotationSkewX error! need 1 args: self");
		std::get<0>(t)->getRotationSkewX();
		return 0;
	});

	Lua_NewFunc(L, "setRotationSkewY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "setRotationSkewY error! need 2 args: self, int sy");
		std::get<0>(t)->setRotationSkewY(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getRotationSkewY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getRotationSkewY error! need 1 args: self");
		std::get<0>(t)->getRotationSkewY();
		return 0;
	});

	Lua_NewFunc(L, "setRotation", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float>(L);
			std::get<0>(t)->setRotation(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float, float>(L);
			std::get<0>(t)->setRotation3D({ std::get<1>(t), std::get<2>(t), 0 });
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float, float, float>(L);
			std::get<0>(t)->setRotation3D({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		case 5:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float, float, float, float>(L);
			std::get<0>(t)->setRotationQuat({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
			break;
		}
		default:
			return luaL_error(L, "setScale error! need 2 ~ 5 args: self, float angle / angleX, float angleY, float angleZ, float W");
		}
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
		var t = Lua_ToTuple<cocos2d::Node*, float, float, float>(L, "setRotation3D error! need 4 args: self, float angleX, float angleY, float angleZ");
		std::get<0>(t)->setRotation3D({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getRotation3D", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getRotation3D error! need 1 args: self");
		var r = std::get<0>(t)->getRotation3D();
		return Lua_Pushs(L, r.x, r.y, r.z);
	});

	Lua_NewFunc(L, "setRotationQuat", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float, float, float>(L, "setRotationQuat error! need 5 args: self, float x, float y, float z, float w");
		std::get<0>(t)->setRotationQuat({ std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getRotationQuat", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getRotationQuat error! need 1 args: self");
		var r = std::get<0>(t)->getRotationQuat();
		return Lua_Pushs(L, r.x, r.y, r.z, r.w);
	});

	Lua_NewFunc(L, "setScale", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float>(L);
			std::get<0>(t)->setScale(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float, float>(L);
			std::get<0>(t)->setScale(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<cocos2d::Node*, float, float, float>(L);
			std::get<0>(t)->setScale(std::get<1>(t), std::get<2>(t));
			std::get<0>(t)->setScaleZ(std::get<3>(t));
			break;
		}
		default:
			return luaL_error(L, "setScale error! need 2 ~ 4 args: self, float scaleX&Y / scaleX, float scaleY, float scaleZ");
		}
		return 0;
	});
	Lua_NewFunc(L, "getScale", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScale error! need 1 args: self");
		var r = std::get<0>(t)->getScale();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setScaleX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float>(L, "setScaleX error! need 2 args: self, float x");
		std::get<0>(t)->setScaleX(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getScaleX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScale error! need 1 args: self");
		var r = std::get<0>(t)->getScaleX();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setScaleY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float>(L, "setScaleY error! need 2 args: self, float y");
		std::get<0>(t)->setScaleY(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getScaleY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScale error! need 1 args: self");
		var r = std::get<0>(t)->getScaleY();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setScaleZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float>(L, "setScaleZ error! need 2 args: self, float z");
		std::get<0>(t)->setScaleZ(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getScaleZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScale error! need 1 args: self");
		var r = std::get<0>(t)->getScaleZ();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setVisible", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setVisible error! need 2 args: self, bool visible");
		std::get<0>(t)->setVisible(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "isVisible", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "isVisible error! need 1 args: self");
		var r = std::get<0>(t)->isVisible();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setIgnoreAnchorPointForPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setIgnoreAnchorPointForPosition error! need 2 args: self, bool ignore");
		std::get<0>(t)->setIgnoreAnchorPointForPosition(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "isIgnoreAnchorPointForPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "isIgnoreAnchorPointForPosition error! need 1 args: self");
		var r = std::get<0>(t)->isIgnoreAnchorPointForPosition();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setContentSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "setContentSize error! need 3 args: self, float w, float h");
		std::get<0>(t)->setContentSize({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getContentSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getContentSize error! need 1 args: self");
		var r = std::get<0>(t)->getContentSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "convertToNodeSpace", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "convertToNodeSpace error! need 3 args: self, float worldPointX, float worldPointY");
		var r = std::get<0>(t)->convertToNodeSpace({ std::get<1>(t), std::get<2>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "convertToNodeSpaceAR", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "convertToNodeSpaceAR error! need 3 args: self, float worldPointX, float worldPointY");
		var r = std::get<0>(t)->convertToNodeSpaceAR({ std::get<1>(t), std::get<2>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "convertToWorldSpace", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "convertToWorldSpace error! need 3 args: self, float nodePointX, float nodePointY");
		var r = std::get<0>(t)->convertToWorldSpace({ std::get<1>(t), std::get<2>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "convertToWorldSpaceAR", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "convertToWorldSpaceAR error! need 3 args: self, float nodePointX, float nodePointY");
		var r = std::get<0>(t)->convertToWorldSpaceAR({ std::get<1>(t), std::get<2>(t) });
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "convertTouchToNodeSpace", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Touch*>(L, "convertTouchToNodeSpace error! need 2 args: self, touch");
		var r = std::get<0>(t)->convertTouchToNodeSpace(std::get<1>(t));
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "convertTouchToNodeSpaceAR", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Touch*>(L, "convertTouchToNodeSpaceAR error! need 2 args: self, touch");
		var r = std::get<0>(t)->convertTouchToNodeSpaceAR(std::get<1>(t));
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "setOpacity error! need 2 args: self, byte opacity");
		std::get<0>(t)->setOpacity(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getOpacity error! need 1 args: self");
		var r = std::get<0>(t)->getOpacity();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "updateDisplayedOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "updateDisplayedOpacity error! need 2 args: self, byte parentOpacity");
		std::get<0>(t)->updateDisplayedOpacity(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getDisplayedOpacity", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getDisplayedOpacity error! need 1 args: self");
		var r = std::get<0>(t)->getDisplayedOpacity();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setCascadeOpacityEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setCascadeOpacityEnabled error! need 2 args: self, bool cascadeOpacityEnabled");
		std::get<0>(t)->setCascadeOpacityEnabled(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "isCascadeOpacityEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "isCascadeOpacityEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isCascadeOpacityEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, GLubyte, GLubyte, GLubyte>(L, "setColor error! need 4 args: self, GLubyte r, GLubyte g, GLubyte b");
		std::get<0>(t)->setColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getColor error! need 1 args: self");
		var r = std::get<0>(t)->getColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "updateDisplayedColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, GLubyte, GLubyte, GLubyte>(L, "updateDisplayedColor error! need 4 args: self, GLubyte parentR, GLubyte parentG, GLubyte parentB");
		std::get<0>(t)->updateDisplayedColor({ std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return 0;
	});
	Lua_NewFunc(L, "getDisplayedColor", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getDisplayedColor error! need 1 args: self");
		var r = std::get<0>(t)->getDisplayedColor();
		return Lua_Pushs(L, r.r, r.g, r.b);
	});

	Lua_NewFunc(L, "setCascadeColorEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setCascadeColorEnabled error! need 2 args: self, bool cascadeColorEnabled");
		std::get<0>(t)->setCascadeColorEnabled(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "isCascadeColorEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "isCascadeColorEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isCascadeColorEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setOpacityModifyRGB", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setOpacityModifyRGB error! need 4 args: self, bool value");
		std::get<0>(t)->setOpacityModifyRGB(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "isOpacityModifyRGB", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "isOpacityModifyRGB error! need 1 args: self");
		var r = std::get<0>(t)->isOpacityModifyRGB();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setOnEnterCallback", [](lua_State* L)
	{
		var msg = "setOnEnterCallback error! need 2 args: self, function<void()> callback";
		var t = Lua_ToTuple<cocos2d::Node*, Lua_Func>(L, msg);
		if (!std::get<1>(t))
		{
			return luaL_error(L, "%s", msg);
		}
		std::get<0>(t)->setOnEnterCallback([f = std::get<1>(t)]
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f);
			lua_settop(gLua, 0);
		});
		return 0;
	});
	Lua_NewFunc(L, "setOnExitCallback", [](lua_State* L)
	{
		var msg = "setOnExitCallback error! need 2 args: self, function<void()> callback";
		var t = Lua_ToTuple<cocos2d::Node*, Lua_Func>(L, msg);
		if (!std::get<1>(t))
		{
			return luaL_error(L, "%s", msg);
		}
		std::get<0>(t)->setOnExitCallback([f = std::get<1>(t)]
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setCameraMask", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Node*, int>(L);
			std::get<0>(t)->setCameraMask(std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::Node*, int, bool>(L);
			std::get<0>(t)->setCameraMask(std::get<1>(t), std::get<2>(t));
			break;
		}
		default:
			return luaL_error(L, "setCameraMask error! need 2 ~ 3 args: self, unsigned short mask, bool applyChildren = true");
		}
		return 0;
	});
	Lua_NewFunc(L, "getCameraMask", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getCameraMask error! need 1 args: self");
		var r = std::get<0>(t)->getCameraMask();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool>(L, "setTag error! need 2 args: self, int tag");
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
		var t = Lua_ToTuple<cocos2d::Node*, std::string>(L, "setName error! need 2 args: self, string name");
		std::get<0>(t)->setName(std::get<1>(t));
		return 0;
	});
	Lua_NewFunc(L, "getName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getName error! need 1 args: self");
		var r = std::get<0>(t)->getName();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "cleanup", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "cleanup error! need 1 args: self");
		std::get<0>(t)->cleanup();
		return 0;
	});

	Lua_NewFunc(L, "getScene", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScene error! need 1 args: self");
		var r = std::get<0>(t)->getScene();
		return Lua_Push(L, r);
	});

	Lua_NewFunc(L, "getBoundingBox", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getBoundingBox error! need 1 args: self");
		var r = std::get<0>(t)->getBoundingBox();
		return Lua_Pushs(L, r.origin.x, r.origin.y, r.size.width, r.size.height);
	});

	Lua_NewFunc(L, "schedule", [](lua_State* L)
	{
		var msg = "schedule error! need 6 args: self, function<void(float)> callback, float interval, unsigned int repeat, float delay, string key";
		var t = Lua_ToTuple<cocos2d::Node*, Lua_Func, float, int, float, std::string>(L, msg);
		if (!std::get<1>(t))
		{
			return luaL_error(L, "%s", msg);
		}
		std::get<0>(t)->schedule([f = std::get<1>(t)](float delta) 
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, delta);
			lua_settop(gLua, 0);
		}
		, std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t));
		return 0;
	});

	Lua_NewFunc(L, "unschedule", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, std::string>(L, "unschedule error! need 2 args: self, key");
		std::get<0>(t)->unschedule(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "unscheduleAllCallbacks", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "unscheduleAllCallbacks error! need 1 args: self");
		std::get<0>(t)->unscheduleAllCallbacks();
		return 0;
	});

	Lua_NewFunc(L, "runAction", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Action*>(L, "runAction error! need 2 args: self, action");
		var r = std::get<0>(t)->runAction(std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "stopAllActions", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "stopAllActions error! need 1 args: self");
		std::get<0>(t)->stopAllActions();
		return 0;
	});

	Lua_NewFunc(L, "stopAction", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::Action*>(L, "stopAction error! need 2 args: self, action");
		std::get<0>(t)->stopAction(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "stopActionByTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "stopActionByTag error! need 2 args: self, int tag");
		std::get<0>(t)->stopActionByTag(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "stopAllActionsByTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "stopAllActionsByTag error! need 2 args: self, int tag");
		std::get<0>(t)->stopAllActionsByTag(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "stopActionsByFlags", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "stopActionsByFlags error! need 2 args: self, int flag");
		std::get<0>(t)->stopActionsByFlags(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getActionByTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "getActionByTag error! need 2 args: self, action");
		var r = std::get<0>(t)->getActionByTag(std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getNumberOfRunningActions", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getNumberOfRunningActions error! need 1 args: self");
		var r = std::get<0>(t)->getNumberOfRunningActions();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getNumberOfRunningActionsByTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, int>(L, "getNumberOfRunningActionsByTag error! need 2 args: self, int tag");
		var r = std::get<0>(t)->getNumberOfRunningActionsByTag(std::get<1>(t));
		return Lua_Pushs(L, r);
	});


	// todo: getGLProgram setGLProgram getGLProgramState setGLProgramState ......


	// 增加一组易于使用的函数. 大写开头. 和 cocos 本体函数以示区别

	Lua_NewFunc(L, "containsPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, float, float>(L, "containsPoint error! need 3 args: self, float x, float y");
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

	Lua_NewFunc(L, "getScaleXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*>(L, "getScale error! need 1 args: self");
		var x = std::get<0>(t)->getScaleX();
		var y = std::get<0>(t)->getScaleY();
		var z = std::get<0>(t)->getScaleZ();
		return Lua_Pushs(L, x, y, z);
	});

	Lua_NewFunc(L, "addEventListenerWithSceneGraphPriority", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, cocos2d::EventListener*>(L, "addEventListenerWithSceneGraphPriority error! need 2 args: self, listener");
		std::get<0>(t)->getEventDispatcher()->addEventListenerWithSceneGraphPriority(std::get<1>(t), std::get<0>(t));
		lua_pushvalue(L, 1);
		return 1;
	});

	Lua_NewFunc(L, "Set_Visible_LocalZOrder", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::Node*, bool, int>(L, "Set_Visible_LocalZOrder error! need 3 args: self, bool visible, int z");
		std::get<0>(t)->setVisible(std::get<1>(t));
		std::get<0>(t)->setLocalZOrder(std::get<2>(t));
		lua_pushvalue(L, 1);
		return 1;
	});
	// todo: more

	lua_pop(L, 1);													// cc
}
