#pragma once

#ifndef var
#define var decltype(auto)
#endif

// todo: 优化函数名和使用, 考虑参考 cocos lua 框架代码提供 return self 以便连写
// todo: 根 L 上留个 funcs 不可取. 还是运行时取表安全
// todo: L 都是一个值. 局部能省传参
// todo: 限定 self, x, y 只有一种参数格式. 不可变长以简化设计
// todo: 延迟到回调外执行的队列

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
