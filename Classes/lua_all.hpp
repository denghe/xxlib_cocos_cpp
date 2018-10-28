#pragma once

#ifndef var
#define var decltype(auto)
#endif

// todo: 补 create 系列函数
// todo: 优化函数名和使用, 考虑参考 cocos lua 框架代码提供 return self 以便连写
// todo: 延迟到回调外执行的队列

// 用于将参数转为全局以便立即在无法传参的函数中使用
inline int gFuncId = 0;
inline cocos2d::Touch* gTouch = nullptr;
inline cocos2d::Event* gEvent = nullptr;
inline std::vector<cocos2d::Touch*> const* gTouchs = nullptr;

#include "lua_keys.hpp"
#include "lua_funcholder.hpp"
#include "lua_pushxxx.hpp"
#include "lua_newxxx.hpp"
#include "lua_toxxx.hpp"

#include "lua_cc_ref.hpp"
#include "lua_cc_node.hpp"
#include "lua_cc_scene.hpp"
#include "lua_cc_touch.hpp"
#include "lua_cc_event.hpp"
#include "lua_cc_eventlisteners.hpp"
//#include "lua_cc_spriteframe.hpp"
#include "lua_cc_sprite.hpp"
#include "lua_cc_texturecache.hpp"
#include "lua_cc_texture.hpp"
#include "lua_cc.hpp"

#include "lua_init.hpp"
//...
