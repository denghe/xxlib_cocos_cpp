#pragma once

inline void Lua_Register_cc(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, LuaKey_cc);									// cc

#include "lua_cc_application.hpp"
#include "lua_cc_director.hpp"
#include "lua_cc_eventdispacher.hpp"
#include "lua_cc_fileutils.hpp"
#include "lua_cc_userdefault.hpp"
#include "lua_cc_texturecache.hpp"
#include "lua_cc_spriteframecache.hpp"

#include "lua_cc_data.hpp"
#include "lua_cc_ref.hpp"
#include "lua_cc_assetsmanagerex.hpp"
#include "lua_cc_actions.hpp"
#include "lua_cc_node.hpp"
#include "lua_cc_label.hpp"
#include "lua_cc_scene.hpp"
#include "lua_cc_touch.hpp"
#include "lua_cc_event.hpp"
#include "lua_cc_eventlisteners.hpp"
#include "lua_cc_layer.hpp"
#include "lua_cc_spriteframe.hpp"
#include "lua_cc_sprite.hpp"
#include "lua_cc_scrollview.hpp"
#include "lua_cc_texture.hpp"

#include "lua_cc_uiwidget.hpp"
#include "lua_cc_uibutton.hpp"
#include "lua_cc_uieditbox.hpp"
#include "lua_cc_uiimageview.hpp"
#include "lua_cc_uilayout.hpp"
#include "lua_cc_uiloadingbar.hpp"

	// todo: AnimationCache

	lua_pop(L, 1);													//
	assert(lua_gettop(L) == 0);
}
