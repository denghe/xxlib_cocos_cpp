#pragma once

inline void Lua_Register_cca(lua_State* const& L)
{
	lua_createtable(L, 0, 100);										// cca
	lua_pushvalue(L, -1);											// cca, cca
	lua_setglobal(L, LuaKey_cca);									// cca


	Lua_NewFunc(L, "play2d", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::experimental::AudioEngine::play2d(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, bool>(L);
			var r = cocos2d::experimental::AudioEngine::play2d(std::get<0>(t), std::get<1>(t));
			return Lua_Pushs(L, r);
		}
		case 3:
		{
			var t = Lua_ToTuple<std::string, bool, float>(L);
			var r = cocos2d::experimental::AudioEngine::play2d(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			return Lua_Pushs(L, r);
		}
		default:
		{
			return luaL_error(L, "%s", "play2d error! need 1 ~ 3 args: string filePath, bool loop = false, float volume = 1.0f");
		}
		}
	});

	Lua_NewFunc(L, "setLoop", [](lua_State* L)
	{
		var t = Lua_ToTuple<int, bool>(L, "setLoop error! need 2 args: int audioID, bool loop");
		cocos2d::experimental::AudioEngine::setLoop(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isLoop", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "isLoop error! need 1 args: int audioID");
		var r = cocos2d::experimental::AudioEngine::isLoop(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setVolume", [](lua_State* L)
	{
		var t = Lua_ToTuple<int, float>(L, "setVolume error! need 2 args: int audioID, float volume");
		cocos2d::experimental::AudioEngine::setVolume(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getVolume", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "getVolume error! need 1 args: int audioID");
		var r = cocos2d::experimental::AudioEngine::getVolume(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "pause", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "pause error! need 1 args: int audioID");
		cocos2d::experimental::AudioEngine::pause(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "pauseAll", [](lua_State* L)
	{
		cocos2d::experimental::AudioEngine::pauseAll();
		return 0;
	});

	Lua_NewFunc(L, "resume", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "resume error! need 1 args: int audioID");
		cocos2d::experimental::AudioEngine::resume(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "resumeAll", [](lua_State* L)
	{
		cocos2d::experimental::AudioEngine::resumeAll();
		return 0;
	});

	Lua_NewFunc(L, "stop", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "stop error! need 1 args: int audioID");
		cocos2d::experimental::AudioEngine::stop(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "stopAll", [](lua_State* L)
	{
		cocos2d::experimental::AudioEngine::stopAll();
		return 0;
	});

	Lua_NewFunc(L, "setCurrentTime", [](lua_State* L)
	{
		var t = Lua_ToTuple<int, float>(L, "setCurrentTime error! need 2 args: int audioID, float sec");
		cocos2d::experimental::AudioEngine::setCurrentTime(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getCurrentTime", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "getCurrentTime error! need 1 args: int audioID");
		var r = cocos2d::experimental::AudioEngine::getCurrentTime(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getDuration", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "getDuration error! need 1 args: int audioID");
		var r = cocos2d::experimental::AudioEngine::getDuration(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getState", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "getState error! need 1 args: int audioID");
		var r = cocos2d::experimental::AudioEngine::getState(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	lua_pushstring(L, TypeNames<cocos2d::experimental::AudioEngine::AudioState>::value);
	lua_createtable(L, 3, 1);
	lua_pushstring(L, "ERROR");	lua_pushinteger(L, (int)cocos2d::experimental::AudioEngine::AudioState::ERROR);	lua_rawset(L, -3);
	lua_pushstring(L, "INITIALIZING");	lua_pushinteger(L, (int)cocos2d::experimental::AudioEngine::AudioState::INITIALIZING);	lua_rawset(L, -3);
	lua_pushstring(L, "PLAYING");	lua_pushinteger(L, (int)cocos2d::experimental::AudioEngine::AudioState::PLAYING);	lua_rawset(L, -3);
	lua_pushstring(L, "PAUSED");	lua_pushinteger(L, (int)cocos2d::experimental::AudioEngine::AudioState::PAUSED);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	Lua_NewFunc(L, "setFinishCallback", [](lua_State* L)
	{
		var t = Lua_ToTuple<int, Lua_Func>(L, "setFinishCallback error! need 2 args: int audioID, function<void(int,const std::string&)>& callback");
		cocos2d::experimental::AudioEngine::setFinishCallback(std::get<0>(t), [f = std::move(std::get<1>(t))](int audioID, const std::string& fileName)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, audioID, fileName);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "getMaxAudioInstance", [](lua_State* L)
	{
		var r = cocos2d::experimental::AudioEngine::getMaxAudioInstance();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setMaxAudioInstance", [](lua_State* L)
	{
		var t = Lua_ToTuple<int>(L, "setMaxAudioInstance error! need 1 args: int maxInstances");
		var r = cocos2d::experimental::AudioEngine::setMaxAudioInstance(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "uncache", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "uncache error! need 1 args: string filePath");
		cocos2d::experimental::AudioEngine::uncache(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "uncacheAll", [](lua_State* L)
	{
		cocos2d::experimental::AudioEngine::uncacheAll();
		return 0;
	});

	Lua_NewFunc(L, "preload", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			cocos2d::experimental::AudioEngine::preload(std::get<0>(t));
			break;
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::experimental::AudioEngine::preload(std::get<0>(t), [f = std::move(std::get<1>(t))](bool isSuccess)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, isSuccess);
				lua_settop(gLua, 0);
			});
			break;
		}
		default:
		{
			return luaL_error(L, "%s", "preload error! need 1 ~ 2 args: string filePath, std::function<void(bool isSuccess)> callback");
		}
		}
		return 0;
	});

	Lua_NewFunc(L, "getPlayingAudioCount", [](lua_State* L)
	{
		var r = cocos2d::experimental::AudioEngine::getPlayingAudioCount();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		var r = cocos2d::experimental::AudioEngine::isEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setEnabled error! need 1 args: bool isEnabled");
		cocos2d::experimental::AudioEngine::setEnabled(std::get<0>(t));
		return 0;
	});


	lua_pop(L, 1);
	assert(lua_gettop(L) == 0);
}
