//
// Created by tangs on 2020/9/18.
//

#ifndef COCOS2D_X_TOOLS_H
#define COCOS2D_X_TOOLS_H

#include <string>
#include <cstdlib>

#include "xx_data.h"
#ifndef COCOS2D_VERSION
#include "xx_file.h"
#endif
#include "ajson.hpp"

// 加载 json 文件到 o
template<typename T>
static int LoadJson(T& o, std::string const& fn) {
    char const* buf = nullptr;
    size_t len = 0;
#ifdef COCOS2D_VERSION
    auto d = cocos2d::FileUtils::getInstance()->getDataFromFile(fn);
    // 错误检查
    if (d.getSize() == 0) return -1;
    buf = (char*)d.getBytes();
    len = d.getSize();
#else
    xx::Data d;
    // 错误检查
    if (xx::ReadAllBytes(fn, d) || !d.len) return -1;
    buf = d.buf;
    len = d.len;
#endif
    ajson::load_from_buff(o, buf, len); // try catch?
    return 0;
}

#endif //COCOS2D_X_TOOLS_H
