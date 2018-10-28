#pragma once

inline void Lua_Register_cc(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, LuaKey_cc);									// cc

	// 创建 重启游戏 函数
	Lua_NewFunc(L, "restart", [](lua_State* L)
	{
		cocos2d::Director::getInstance()->mainLoopCallback = []
		{
			gScene->removeAllChildrenWithCleanup(true);
			cocos2d::Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

			// 这个函数只是打个重启标记. 下帧才开始重启
			cocos2d::Director::getInstance()->restart();		// 已知问题: listener 绑的 function 似乎并没被正确回收
		};
		return 0;
	});

	// 创建 拿 scene 函数
	Lua_NewFunc(L, "scene", [](lua_State* L)
	{
		return Lua_NewUserdataMT(L, gScene, LuaKey_Scene);
	});

	// todo: more like addSearchPath...

	// 创建 cc.Xxxxxx 元表及函数									// cc
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
