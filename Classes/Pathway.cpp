#include "Pathway.h"
#include "FileExts_class_lite.ajson.h"
#include "Tools.h"

std::shared_ptr<xx::Pathway> Loader<xx::Pathway, void>::Load(std::string const& fn) {
	auto&& iter = cache.find(fn);
	// 已存在：短路返回
	if (iter != cache.end()) return iter->second;

	FileExts::File_pathway f;

	// 从 json 加载
	if (LoadJson(f, fn)) return nullptr;

	// 准备返回值容器
	auto rtv = xx::Make<xx::Pathway>();

	auto&& ps = f.points;
	// 缓存长度
	auto siz = ps.size();

	// 点数量需要 >= 2
	if (siz < 2) return nullptr;

	auto&& rps = rtv->points;

	// 如果只有 2 个点：直线
	if (siz == 2) {
		// 填充第一个点: 计算到第二个点的角度和距离
		rps.emplace_back(ps[0].x, ps[0].y
			, xx::GetAngle(ps[0], ps[1])
			, xx::GetDistance(ps[0], ps[1]));
		rtv->isLoop = f.isLoop;
		// 填充第二个点
		if (f.isLoop) {
			// 不断来回弹的直线? 角度相反，距离相同
			rps.emplace_back(ps[1].x, ps[1].y
				, rps[0].a + M_PI
				, rps[0].d);
		}
		else {
			// 角度相同，距离为 0
			rps.emplace_back(ps[1].x, ps[1].y, rps[0].a, 0);
		}
	}
	// 曲线
	else {
		// 曲线 转为 带控制点的贝塞尔
		if (f.isLoop) {
			// 循环曲线则前后各追加几个点算控制点
			bs.push_back(ps[siz - 2]);
			bs.push_back(ps[siz - 1]);
			bs.insert(bs.end(), ps.begin(), ps.end());
			bs.push_back(ps[0]);
			bs.push_back(ps[1]);
			bs.push_back(ps[2]);
			xx::CurveToBezier(cs, bs);
			bs.clear();
			// 移除追加
			cs.resize(cs.size() - 6);
			cs.erase(cs.begin(), cs.begin() + 6);
		}
		else {
			xx::CurveToBezier(cs, ps);
		}

		// 带控制点的贝塞尔 转为 点集合
		xx::BezierToPoints(rps, cs);
		cs.clear();

		// 填充距离和角度
		rtv->FillDA();
	}

	// 放入字典并返回
	cache[fn] = rtv;
	return rtv;
}
