#pragma once

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
#include "lua_cc_tmxtiledmap.hpp"

#include "lua_cc_uiwidget.hpp"
#include "lua_cc_uibutton.hpp"
#include "lua_cc_uieditbox.hpp"
#include "lua_cc_uiimageview.hpp"
#include "lua_cc_uilayout.hpp"
#include "lua_cc_uiloadingbar.hpp"

#include "lua_cc_application.hpp"
#include "lua_cc_director.hpp"
#include "lua_cc_eventdispacher.hpp"
#include "lua_cc_fileutils.hpp"
#include "lua_cc_userdefault.hpp"
#include "lua_cc_texturecache.hpp"
#include "lua_cc_spriteframecache.hpp"
#include "lua_cc_animationcache.hpp"

inline void Lua_Register_cc(lua_State* const& L)
{
	// 创建全局 cc 基表
	lua_createtable(L, 0, 100);										// cc
	lua_pushvalue(L, -1);											// cc, cc
	lua_setglobal(L, LuaKey_cc);									// cc

	Lua_Register_Data(L);
	Lua_Register_Ref(L);
	Lua_Register_AssetsManagerEx(L);
	Lua_Register_Node(L);
	Lua_Register_Layer(L);
	Lua_Register_Scene(L);
	Lua_Register_Touch(L);
	Lua_Register_Event(L);
	Lua_Register_EventListener(L);
	Lua_Register_EventListenerTouchAllAtOnce(L);
	Lua_Register_EventListenerTouchOneByOne(L);
	Lua_Register_EventListenerKeyboard(L);
	Lua_Register_EventListenerAssetsManagerEx(L);
	Lua_Register_Sprite(L);
	Lua_Register_Label(L);
	Lua_Register_ScrollViews(L);
	Lua_Register_SpriteFrame(L);
	Lua_Register_Texture(L);
	Lua_Register_Actions(L);
	Lua_Register_TMXTiledMap(L);
	// .....

	Lua_Register_uiWidget(L);
	Lua_Register_uiButton(L);
	Lua_Register_uiImageView(L);
	Lua_Register_uiLayout(L);
	Lua_Register_uiLoadingBar(L);
	Lua_Register_uiEditBox(L);
	// .....

	Lua_Register_Application(L);
	Lua_Register_Director(L);
	Lua_Register_EventDispacher(L);
	Lua_Register_FileUtils(L);
	Lua_Register_UserDefault(L);
	Lua_Register_TextureCache(L);
	Lua_Register_SpriteFrameCache(L);
	Lua_Register_AnimationCache(L);
	// ...

	lua_pop(L, 1);
	assert(lua_gettop(L) == 0);
}
