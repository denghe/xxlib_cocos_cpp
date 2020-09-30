#pragma once
#include "FileExts_class_lite.h"
#include "xx_point.h"
#include "ajson.hpp"

// 加载器：负责加载指定文件，缓存 并 返回
// 这是基类
template<typename T>
struct LoaderBase {
	// cache
	std::unordered_map<std::string, std::shared_ptr<T>> cache;

	// 清除 cache 中 use_count == 1 的
	void RemoveUnused() {
		for (auto iter = cache.begin(); iter != cache.end(); ) {
			if (iter->second.use_count() == 1) {
				iter = cache.erase(iter);
			}
			else {
				++iter;
			}
		}
	}

	LoaderBase() = default;
	// 禁用复制
	LoaderBase(LoaderBase const&) = delete;
	LoaderBase& operator=(LoaderBase const&) = delete;
};

// 加载器模板适配
template<typename T, class ENABLED>
struct Loader;

// Pathway 加载器( 在读入 json 后还有个转换过程 )
template<>
struct Loader<xx::Pathway, void> : LoaderBase<xx::Pathway> {
	// tmp
	std::vector<FileExts::PathwayPoint> bs, cs;

	// 加载并返回
	std::shared_ptr<xx::Pathway> Load(std::string const& fn);
};

// Frames, Ext 等 json 对象直读类型 加载器
// todo: 适配更多类型
template<typename T>
struct Loader<T, std::enable_if_t<std::is_same_v<T, FileExts::File_Frames> || std::is_same_v<T, FileExts::File_AnimExt>>> : LoaderBase<T> {
	// 加载并返回
	std::shared_ptr<T> Load(std::string const& fn) {
		// 在 cache 中查找. 已存在：短路返回
		auto&& iter = this->cache.find(fn);
		if (iter != this->cache.end()) return iter->second;

		// 文件容器
		auto r = xx::Make<T>();

		// 从 json 加载
		if (LoadJson(*r, fn)) return nullptr;

		// 放入字典并返回
        this->cache[fn] = r;
		return r;
	}
};

// todo: 全局静态的各式 LoadXXXXXX 函数. 上面的类 单例 或 静态使用

// todo: Anim & AnimExt 里面的文件容器改为 shared_ptr 版, 从加载器返回. 各种加载器可全局单例线程安全。
// 所有 loader 可映射 lambda 到 lua
// Anim, AnimExt, Pathway 映射 shared_ptr 版到 lua, 他们的容器也可以映射下标赋值函数到 lua
// 最终, 可用 lua 实现上述所有类的常规操作
