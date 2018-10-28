#pragma once

inline void Lua_Register_cc(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, "cc");											// cc

	// 创建 重启游戏 函数
	lua_pushstring(L, "restart");									// cc, "restart"
	lua_pushcclosure(L, [](lua_State* L)							// cc, "restart", func
	{
		var d = cocos2d::Director::getInstance();
		d->mainLoopCallback = [d]
		{
			d->restart();
			d->mainLoopCallback = nullptr;
		};
		return 0;
	}, 0);
	lua_rawset(L, -3);												// cc

	// 创建 拿 scene 函数
	lua_pushstring(L, "scene");
	lua_pushcclosure(L, [](lua_State* L)
	{
		return Lua_NewUserdataMT(L, gScene, LuaKey_Scene);
	}, 0);
	lua_rawset(L, -3);

	// todo: more like addSearchPath...

	// 创建 cc.Xxxxxx 元表及函数
	Lua_Register_Ref(L);
	Lua_Register_Node(L);
	Lua_Register_Scene(L);
	Lua_Register_Touch(L);
	Lua_Register_Event(L);
	Lua_Register_EventListener(L);
	Lua_Register_EventListenerTouchAllAtOnce(L);
	Lua_Register_EventListenerTouchOneByOne(L);
	Lua_Register_Sprite(L);
	//Lua_Register_SpriteFrame(L);							
	Lua_Register_Texture(L);
	Lua_Register_TextureCache(L);
	// .....
	// .....
	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
