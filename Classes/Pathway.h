#pragma once
#include "FileExts_class_lite.h"
#include "xx_point.h"
#include "ajson.hpp"

// 移动路线管理器：负责加载 .pathway 文件，并转为 线段 数据格式 返回
struct PathwayLoader {

	// 已加载的 pathway 容器
	std::unordered_map<std::string, std::shared_ptr<xx::Pathway>> pathways;

	// 临时容器
	std::vector<FileExts::PathwayPoint> bs, cs;

	// 加载并返回一条 移动路线
	// 如果 pathways 中没找到 就加载文件 并放入
	std::shared_ptr<xx::Pathway> Load(std::string const& fn);

	// 清除 pathways 值部分引用计数为 1 的 移动路线
	void RemoveUnused();

	PathwayLoader() = default;
	// 禁用复制，只能在智能指针环境下使用
	PathwayLoader(PathwayLoader const&) = delete;
	PathwayLoader& operator=(PathwayLoader const&) = delete;
};
