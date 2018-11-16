#pragma once

inline void Lua_Register_FileUtils(lua_State* const& L)
{
	Lua_NewFunc(L, "purgeCachedEntries", [](lua_State* L)
	{
		cocos2d::FileUtils::getInstance()->purgeCachedEntries();
		return 0;
	});

	Lua_NewFunc(L, "getStringFromFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->getStringFromFile(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->getStringFromFile(std::get<0>(t), [f = std::move(std::get<1>(t))](std::string str)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, str);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "getStringFromFile error! need 1 ~ 2 args: string filename, function<void(string)> callback");
		}
	});

	Lua_NewFunc(L, "getDataFromFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->getDataFromFile(std::get<0>(t));
			var data = new cocos2d::Data(std::move(r));
			return Lua_Pushs(L, data);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->getDataFromFile(std::get<0>(t), [f = std::move(std::get<1>(t))](cocos2d::Data data)
			{
				var d = new cocos2d::Data(std::move(data));
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, d);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "getDataFromFile error! need 1 ~ 2 args: string filename, function<void(Data)> callback");
		}
	});

	Lua_NewFunc(L, "getFileDataFromZip", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, std::string>(L, "getFileDataFromZip error! need 2 args: string zipFilePath, filename");
		ssize_t size = 0;
		var r = cocos2d::FileUtils::getInstance()->getFileDataFromZip(std::get<0>(t), std::get<1>(t), &size);
		var d = new cocos2d::Data();
		d->fastSet(r, size);
		return Lua_Pushs(L, d);
	});

	Lua_NewFunc(L, "fullPathForFilename", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "fullPathForFilename error! need 1 args: string filename");
		var r = cocos2d::FileUtils::getInstance()->fullPathForFilename(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	// 不实现 loadFilenameLookupDictionaryFromFile setFilenameLookupDictionary

	Lua_NewFunc(L, "fullPathFromRelativeFile", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, std::string>(L, "fullPathFromRelativeFile error! need 2 args: string filename, relativeFile");
		var r = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(std::get<0>(t), std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	// 不实现 setSearchResolutionsOrder, addSearchResolutionsOrder getSearchResolutionsOrder

	Lua_NewFunc(L, "setSearchPaths", [](lua_State* L)
	{
		gStrings.clear();
		var numArgs = lua_gettop(L);
		if (numArgs)
		{
			for (int i = 1; i <= numArgs; ++i)
			{
				gStrings.emplace_back();
				Lua_Get(gStrings.back(), L, i);
			}
		}
		cocos2d::FileUtils::getInstance()->setSearchPaths(gStrings);
		return 0;
	});

	Lua_NewFunc(L, "getDefaultResourceRootPath", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getDefaultResourceRootPath();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDefaultResourceRootPath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "setDefaultResourceRootPath error! need 1 args: string path");
		cocos2d::FileUtils::getInstance()->setDefaultResourceRootPath(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "addSearchPath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, bool>(L, "addSearchPath error! need 2 args: string path, bool front");
		cocos2d::FileUtils::getInstance()->addSearchPath(std::get<0>(t), std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSearchPaths", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getSearchPaths();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getOriginalSearchPaths", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getOriginalSearchPaths();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getWritablePath", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getWritablePath();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setWritablePath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string, bool>(L, "setWritablePath error! need 2 args: string writablePath");
		cocos2d::FileUtils::getInstance()->setWritablePath(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "setPopupNotify", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "setPopupNotify error! need 1 args: bool notify");
		cocos2d::FileUtils::getInstance()->setPopupNotify(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "isPopupNotify", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->isPopupNotify();
		return Lua_Pushs(L, r);
	});

	// 不实现 getValueMapFromFile getValueMapFromData writeToFile

	Lua_NewFunc(L, "writeStringToFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string, std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->writeStringToFile(std::get<0>(t), std::get<1>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->writeStringToFile(std::get<0>(t), std::get<1>(t), [f = std::move(std::get<2>(t))](bool successful)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successful);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "writeStringToFile error! need 2 ~ 3 args: string dataStr, string fullPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "writeDataToFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::Data*, std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->writeDataToFile(*std::get<0>(t), std::get<1>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<cocos2d::Data*, std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->writeDataToFile(std::move(*std::get<0>(t)), std::get<1>(t), [f = std::move(std::get<2>(t))](bool successful)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successful);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "writeDataToFile error! need 2 ~ 3 args: Data data, string fullPath, function<void(bool)> callback");
		}
	});

	// 不实现 writeValueMapToFile writeValueVectorToFile writeValueVectorToFile getSuitableFOpen getValueVectorFromFile

	Lua_NewFunc(L, "isFileExist", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->isFileExist(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->isFileExist(std::get<0>(t), [f = std::move(std::get<1>(t))](bool exists)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, exists);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "isFileExist error! need 1 ~ 2 args: string filename, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "getFileExtension", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getFileExtension error! need 1 args: string filePath");
		var r = cocos2d::FileUtils::getInstance()->getFileExtension(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isAbsolutePath", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "isAbsolutePath error! need 1 args: string path");
		var r = cocos2d::FileUtils::getInstance()->isAbsolutePath(std::get<0>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isDirectoryExist", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->isDirectoryExist(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->isDirectoryExist(std::get<0>(t), [f = std::move(std::get<1>(t))](bool exists)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, exists);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "isDirectoryExist error! need 1 ~ 2 args: string dirPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "createDirectory", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->createDirectory(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->createDirectory(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "createDirectory error! need 1 ~ 2 args: string dirPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "removeDirectory", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->removeDirectory(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->removeDirectory(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "removeDirectory error! need 1 ~ 2 args: string dirPath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "removeFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->removeFile(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->removeFile(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "removeFile error! need 1 ~ 2 args: string filepath, function<void(bool)> callback");
		}
	});

	Lua_NewFunc(L, "renameFile", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string, std::string, std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->renameFile(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, std::string, std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->renameFile(std::get<0>(t), std::get<1>(t), std::get<2>(t), [f = std::move(std::get<3>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "renameFile error! need 3 ~ 4 args: string path, oldname, name, function<void(bool)> callback");
		}
	});

	// 不实现 renameFile( string string func )

	Lua_NewFunc(L, "getFileSize", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->getFileSize(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->getFileSize(std::get<0>(t), [f = std::move(std::get<1>(t))](bool successfully)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, successfully);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "getFileSize error! need 1 ~ 2 args: string filepath, function<void(long)> callback");
		}
	});

	Lua_NewFunc(L, "listFiles", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			var r = cocos2d::FileUtils::getInstance()->listFiles(std::get<0>(t));
			return Lua_Pushs(L, r);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->listFilesAsync(std::get<0>(t), [f = std::move(std::get<1>(t))](std::vector<std::string> fs)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, fs);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "listFiles error! need 1 ~ 2 args: string dirPath, function<void(std::vector<std::string>)> callback");
		}
	});

	Lua_NewFunc(L, "listFilesRecursively", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<std::string>(L);
			gStrings.clear();
			cocos2d::FileUtils::getInstance()->listFilesRecursively(std::get<0>(t), &gStrings);
			return Lua_Pushs(L, gStrings);
		}
		case 2:
		{
			var t = Lua_ToTuple<std::string, Lua_Func>(L);
			cocos2d::FileUtils::getInstance()->listFilesRecursivelyAsync(std::get<0>(t), [f = std::move(std::get<1>(t))](std::vector<std::string> fs)
			{
				assert(!lua_gettop(gLua));
				Lua_PCall(gLua, f, fs);
				lua_settop(gLua, 0);
			});
			return 0;
		}
		default:
			return luaL_error(L, "%s", "listFilesRecursively error! need 1 ~ 2 args: string dirPath, function<void(std::vector<std::string>)> callback");
		}
	});

	Lua_NewFunc(L, "getFullPathCache", [](lua_State* L)
	{
		var r = cocos2d::FileUtils::getInstance()->getFullPathCache();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getNewFilename", [](lua_State* L)
	{
		var t = Lua_ToTuple<std::string>(L, "getNewFilename error! need 1 args: string filename");
		var r = cocos2d::FileUtils::getInstance()->getNewFilename(std::get<0>(t));
		return Lua_Pushs(L, r);
	});
}
