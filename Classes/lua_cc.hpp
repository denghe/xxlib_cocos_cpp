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
			cocos2d::Director::getInstance()->restart();	// 下帧重启
		};
		return 0;
	});

	// 创建 拿 scene 函数
	Lua_NewFunc(L, "scene", [](lua_State* L)
	{
		return Lua_NewUserdataMT(L, gScene, LuaKey_Scene);
	});

	// 创建 注册帧回调 函数
	Lua_NewFunc(L, "mainLoopCallback", [](lua_State* L)
	{
		if (lua_gettop(L) < 1)
		{
			return luaL_error(L, "mainLoopCallback error! need 1 args: func/null");
		}
		Lua_FuncHolder f;
		Lua_Get<Lua_FuncHolder>(f, L, 1);
		if (f.funcId)
		{
			cocos2d::Director::getInstance()->mainLoopCallback = [f = std::move(f)]
			{
				uv->Run(xx::UvRunMode::NoWait);

				var L = gLua;
				gFuncId = f.funcId;
				lua_pushcclosure(L, [](lua_State* L)							// cfunc
				{
					lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);	// funcs
					lua_rawgeti(L, 1, gFuncId);									// funcs, func
					lua_call(L, 0, 0);											// funcs, ...?
					lua_settop(L, 0);											// 
					return 0;
				}, 0);
				if (int r = lua_pcall(L, 0, 0, 0))								//
				{
					cocos2d::log("%s", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
			};
		}
		else
		{
			cocos2d::Director::getInstance()->mainLoopCallback = []
			{
				uv->Run(xx::UvRunMode::NoWait);
			};
		}

		return 0;
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
