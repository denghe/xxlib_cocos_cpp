#pragma once

inline void Lua_Register_AssetsManagerEx(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::extension::Manifest*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "isVersionLoaded", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::Manifest*>(L, "isVersionLoaded error! need 1 args: self");
		auto&& r = std::get<0>(t)->isVersionLoaded();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isLoaded", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::Manifest*>(L, "isLoaded error! need 1 args: self");
		auto&& r = std::get<0>(t)->isLoaded();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getPackageUrl", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::Manifest*>(L, "getPackageUrl error! need 1 args: self");
		auto&& r = std::get<0>(t)->getPackageUrl();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getManifestFileUrl", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::Manifest*>(L, "getManifestFileUrl error! need 1 args: self");
		auto&& r = std::get<0>(t)->getManifestFileUrl();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getVersion", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::Manifest*>(L, "getVersion error! need 1 args: self");
		auto&& r = std::get<0>(t)->getVersion();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getSearchPaths", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::Manifest*>(L, "getSearchPaths error! need 1 args: self");
		auto&& r = std::get<0>(t)->getSearchPaths();
		return Lua_Pushs(L, r);
	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::extension::AssetsManagerEx*>::value, TypeNames<cocos2d::Ref*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<std::string, std::string>(L, "create AssetsManagerEx error! need 2 args: string manifestUrl, storagePath");
		auto&& o = cocos2d::extension::AssetsManagerEx::create(std::get<0>(t), std::get<1>(t));
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "checkUpdate", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "checkUpdate error! need 1 args: self");
		std::get<0>(t)->checkUpdate();
		return 0;
	});

	Lua_NewFunc(L, "update", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "update error! need 1 args: self");
		std::get<0>(t)->update();
		return 0;
	});

	Lua_NewFunc(L, "downloadFailedAssets", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "downloadFailedAssets error! need 1 args: self");
		std::get<0>(t)->downloadFailedAssets();
		return 0;
	});

	Lua_NewFunc(L, "getState", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "getState error! need 1 args: self");
		auto&& r = std::get<0>(t)->getState();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getStoragePath", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "getStoragePath error! need 1 args: self");
		auto&& r = std::get<0>(t)->getStoragePath();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getLocalManifest", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "getLocalManifest error! need 1 args: self");
		auto&& r = std::get<0>(t)->getLocalManifest();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getRemoteManifest", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "getRemoteManifest error! need 1 args: self");
		auto&& r = std::get<0>(t)->getRemoteManifest();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getMaxConcurrentTask", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*>(L, "getMaxConcurrentTask error! need 1 args: self");
		auto&& r = std::get<0>(t)->getMaxConcurrentTask();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setMaxConcurrentTask", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*, int>(L, "setMaxConcurrentTask error! need 2 args: self, int max");
		std::get<0>(t)->setMaxConcurrentTask(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setVersionCompareHandle", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*, Lua_Func>(L, "setVersionCompareHandle error! need 2 args: self, function<int(string versionA, string versionB)>& handle");
		std::get<0>(t)->setVersionCompareHandle([f = std::move(std::get<1>(t))](const std::string& versionA, const std::string& versionB)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, versionA, versionB);
			auto&& rt = Lua_ToTuple<int>(gLua, "setVersionCompareHandle's function error! need 1 results: int");
			lua_settop(gLua, 0);
			return std::get<0>(rt);
		});
		return 0;
	});

	Lua_NewFunc(L, "setVerifyCallback", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<cocos2d::extension::AssetsManagerEx*, Lua_Func>(L, "setVerifyCallback error! need 2 args: self, function<bool(string path, Manifest::Asset[ string md5, path, bool compressed, float size, int downloadState ] asset)>& callback");
		std::get<0>(t)->setVerifyCallback([f = std::move(std::get<1>(t))](const std::string& path, cocos2d::extension::Manifest::Asset asset)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, path, asset.md5, asset.path, asset.compressed, asset.size, asset.downloadState);
			auto&& rt = Lua_ToTuple<int>(gLua, "setVerifyCallback's function error! need 1 results: bool");
			lua_settop(gLua, 0);
			return std::get<0>(rt);
		});
		return 0;
	});

	lua_pop(L, 1);

	lua_pushstring(L, TypeNames<cocos2d::extension::AssetsManagerEx::State>::value);
	lua_createtable(L, 12, 0);
	lua_pushstring(L, "UNCHECKED");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::UNCHECKED);	lua_rawset(L, -3);
	lua_pushstring(L, "PREDOWNLOAD_VERSION");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::PREDOWNLOAD_VERSION);	lua_rawset(L, -3);
	lua_pushstring(L, "DOWNLOADING_VERSION");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::DOWNLOADING_VERSION);	lua_rawset(L, -3);
	lua_pushstring(L, "VERSION_LOADED");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::VERSION_LOADED);	lua_rawset(L, -3);
	lua_pushstring(L, "PREDOWNLOAD_MANIFEST");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::PREDOWNLOAD_MANIFEST);	lua_rawset(L, -3);
	lua_pushstring(L, "DOWNLOADING_MANIFEST");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::DOWNLOADING_MANIFEST);	lua_rawset(L, -3);
	lua_pushstring(L, "MANIFEST_LOADED");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::MANIFEST_LOADED);	lua_rawset(L, -3);
	lua_pushstring(L, "NEED_UPDATE");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::NEED_UPDATE);	lua_rawset(L, -3);
	lua_pushstring(L, "UPDATING");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::UPDATING);	lua_rawset(L, -3);
	lua_pushstring(L, "UNZIPPING");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::UNZIPPING);	lua_rawset(L, -3);
	lua_pushstring(L, "UP_TO_DATE");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::UP_TO_DATE);	lua_rawset(L, -3);
	lua_pushstring(L, "FAIL_TO_UPDATE");	lua_pushinteger(L, (int)cocos2d::extension::AssetsManagerEx::State::FAIL_TO_UPDATE);	lua_rawset(L, -3);
	lua_rawset(L, -3);
}
