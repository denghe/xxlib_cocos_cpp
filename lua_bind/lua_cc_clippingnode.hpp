#pragma once

inline void Lua_Register_ClippingNode(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ClippingNode*>::value, TypeNames<cocos2d::Node*>::value);

	// new + init
	Lua_NewFunc(L, "new", [](lua_State* L)
	{

		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			auto&& o = new (std::nothrow) cocos2d::ClippingNode();
			if (!o) return 0;
			if (!o->init()) { delete o; return 0; }
			return Lua_Pushs(L, o);
		}
		case 1:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*>(L);
			auto&& o = new (std::nothrow) cocos2d::ClippingNode();
			if (!o) return 0;
			if (!o->init(std::get<0>(t))) { delete o; return 0; }
			return Lua_Pushs(L, o);
		}
		default:
			return luaL_error(L, "new ClippingNode error! need 0 ~ 1 args: Node *stencil");
		}
	});

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ClippingNode* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 0:
		{
			o = cocos2d::ClippingNode::create();
			break;
		}
		case 1:
		{
			auto&& t = Lua_ToTuple<cocos2d::Node*>(L);
			o = cocos2d::ClippingNode::create(std::get<0>(t));
			break;
		}
		default:
			return luaL_error(L, "create ClippingNode error! need 0 ~ 1 args: Node *stencil");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "getStencil", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*>(L, "getSpriteFrame error! need 1 args: self");
		auto&& r = std::get<0>(t)->getStencil();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setStencil", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*, cocos2d::Node*>(L, "setStencil error! need 2 args: self, Node stencil");
		std::get<0>(t)->setStencil(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "hasContent", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*>(L, "hasContent error! need 1 args: self");
		auto&& r = std::get<0>(t)->hasContent();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getAlphaThreshold", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*>(L, "getAlphaThreshold error! need 1 args: self");
		auto&& r = std::get<0>(t)->getAlphaThreshold();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setAlphaThreshold", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*, GLfloat>(L, "setAlphaThreshold error! need 2 args: self, GLfloat alphaThreshold");
		std::get<0>(t)->setAlphaThreshold(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isInverted", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*>(L, "isInverted error! need 1 args: self");
		auto&& r = std::get<0>(t)->isInverted();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setInverted", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*, bool>(L, "setInverted error! need 2 args: self, bool inverted");
		std::get<0>(t)->setInverted(std::get<1>(t));
		return 0;
	});




	// 因为还没有实现 DrawNode 的 binding, 故先实现一个常用的 stencil 在这

	// 画一个中心在 0,0 的实心矩形作为 stencil
	Lua_NewFunc(L, "setStencilRect", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::ClippingNode*, float, float>(L, "setStencilRect error! need 3 args: self, float width, height");
		auto stencil = cocos2d::DrawNode::create();
		auto w2 = std::get<1>(t) / 2;
		auto h2 = std::get<2>(t) / 2;
		cocos2d::Vec2 rectangle[4];
		rectangle[0] = cocos2d::Vec2(-w2, -h2);
		rectangle[1] = cocos2d::Vec2(-w2, h2);
		rectangle[2] = cocos2d::Vec2(w2, h2);
		rectangle[3] = cocos2d::Vec2(w2, -h2);
		stencil->drawPolygon(rectangle, 4, cocos2d::Color4F::WHITE, 1, cocos2d::Color4F::WHITE);
		std::get<0>(t)->setStencil(stencil);
		return 0;
	});

	lua_pop(L, 1);
}
