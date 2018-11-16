#pragma once

#include "lua.hpp"

#ifndef var
#define var decltype(auto)
#endif

// todo: 优化函数名和使用, 考虑参考 cocos lua 框架代码提供 return self 以便连写

// 用于临时装载从 lua 获取的参数
inline cocos2d::Vector<cocos2d::FiniteTimeAction*> gActions;
inline std::vector<std::string> gStrings;

#include "lua_keys.hpp"
#include "lua_func.hpp"
#include "lua_pushcall.hpp"
#include "lua_new_xxx.hpp"
#include "lua_to_xxx.hpp"

#include "lua_xx_mempool.hpp"
#include "lua_xx_object.hpp"
#include "lua_xx_bbuffer.hpp"
#include "lua_xx_uvloop.hpp"
#include "lua_xx_uvtcpclient.hpp"
#include "lua_xx.hpp"

#include "lua_cc.hpp"

#include "lua_cca.hpp"

#include "lua_spine.hpp"

// 读取 print 的参数
inline int get_string_for_print(lua_State* L, std::string* out)
{
	int n = lua_gettop(L);  /* number of arguments */
	int i;

	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		size_t sz;
		s = lua_tolstring(L, -1, &sz);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) out->append("\t");
		out->append(s, sz);
		lua_pop(L, 1);  /* pop result */
	}
	return 0;
}

inline int Lua_Main(lua_State* L)
{
	// 加载常用库
	luaL_openlibs(L);

	// 替换 print 的实现 for android 输出
	lua_pushcclosure(L, [](lua_State* L)							// package, searchers, func
	{
		std::string t;
		get_string_for_print(L, &t);
		cocos2d::log("%s", t.c_str());	// [LUA-print] 
		return 0;
	}, 0);
	lua_setglobal(L, "print");

	// 令 require 支持读取 search path 里的文件( 包里 & 可写目录 )
	lua_getglobal(L, "package");									// package
	lua_getfield(L, -1, "searchers");								// package, searchers
	lua_pushcclosure(L, [](lua_State* L)							// package, searchers, func
	{
		size_t len;
		var fn = lua_tolstring(L, 1, &len);
		var fu = cocos2d::FileUtils::getInstance();
		if (!fu->isFileExist(std::string(fn, len)))
		{
			return luaL_error(L, "require file '%s' failed. can't find file.", fn);
		}
		var data = cocos2d::FileUtils::getInstance()->getDataFromFile(std::string(fn, len));
		var buf = (char*)data.getBytes();
		len = data.getSize();
		if (len >= 3 && (uint8_t)buf[0] == 0xEF && (uint8_t)buf[1] == 0xBB && (uint8_t)buf[2] == 0xBF)
		{
			buf += 3;
			len -= 3;
		}
		var r = luaL_loadbuffer(L, buf, len, fn);
		if (r == LUA_OK) return 1;
		return luaL_error(L, "require file '%s' failed. luaL_loadbuffer r = %d", fn, r);
	}, 0);
	lua_rawseti(L, -2, 1);											// package, searchers
	lua_pop(L, 2);													//


	// 注册 null 到全局
	lua_pushlightuserdata(L, nullptr);								// null
	lua_setglobal(L, LuaKey_null);									//

	// 创建函数表
	lua_createtable(L, 0, 100);										// funcs
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)LuaKey_Callbacks);		//

	// 加载 xx.* 对象 & 函数映射
	Lua_Register_xx(L);

	// 加载 cc.* 对象 & 函数映射
	Lua_Register_cc(L);

	// 加载 cca.* 对象 & 函数映射( audio )
	Lua_Register_cca(L);

	// 加载 spine.* 对象 & 函数映射
	Lua_Register_spine(L);

	// 执行 main.lua
	if (int r = luaL_dostring(L, "require \"main.lua\""))			//
	{
		auto s = lua_tostring(L, -1);
		cocos2d::log("%d %s", r, (s ? s : ""));
		lua_pop(L, 1);
		return 0;
	}

	assert(lua_gettop(L) == 0);
	return 0;
}

inline int Lua_Init()
{
	// 使用内存池创建 lua state ( 部分操作性能提升 40% )
	var L = gLua = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((xx::MemPool*)ud)->Realloc(ptr, nsize, osize);
	}
	, mp);
	assert(L);

	// 将 Lua_Main 压入 L 安全执行
	lua_pushcclosure(L, &Lua_Main, 0);								// cfunc
	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))					//
	{
		auto s = lua_tostring(L, -1);
		cocos2d::log("%d %s", r, (s ? s : ""));
		lua_pop(L, 1);
		return r;
	}

	assert(lua_gettop(L) == 0);										//
	return 0;
}
