#pragma once

#include "xx_data_rw.h"
#include "xx_string.h"
#include <cmath>        // PI.....

namespace xx {

	// 坐标
	struct Point {
		float x = 0, y = 0;

		Point() = default;

		Point(float const& x, float const& y) : x(x), y(y) {}

		Point(Point const&) = default;

		Point& operator=(Point const&) = default;

		template<typename T>
		Point& operator=(T const& o) {
			x = o.x;
			y = o.y;
			return *this;
		}

		inline bool operator==(const Point& v) const noexcept {
			return x == v.x && y == v.y;
		}

		inline Point& operator+=(Point const& v) noexcept {
			x += v.x;
			y += v.y;
			return *this;
		}

		inline Point operator+(Point const& v) const noexcept {
			return Point{ x + v.x, y + v.y };
		}

		inline Point operator-(Point const& v) const noexcept {
			return Point{ x - v.x, y - v.y };
		}

		inline Point operator*(float const& s) const noexcept {
			return Point{ x * s, y * s };
		}

		inline Point operator/(float const& s) const noexcept {
			return Point{ x / s, y / s };
		}
	};


	// 移动路线 -- 点
	struct PathwayPoint {
		// 2d坐标
		xx::Point pos;
		// 角度, 距离
		float a = 0, d = 0;

		PathwayPoint() = default;

		PathwayPoint(PathwayPoint const&) = default;

		PathwayPoint& operator=(PathwayPoint const&) = default;

		PathwayPoint(float const& x, float const& y, float const& a, float const& d) : pos(x, y), a(a), d(d) {}

		template<typename T>
		explicit PathwayPoint(T const& p) : pos(p.x, p.y) {}

		template<typename T>
		PathwayPoint& operator=(T const& p) {
			pos.x = p.x;
			pos.y = p.y;
			return *this;
		}
	};


	// 曲线途经点
	struct CurvePoint {
		float x, y, tension;
		int numSegments;
		//CurvePoint() : x(0), y(0), tension(0), numSegments(0) {}
		//CurvePoint(float const& x, float const& y, float const& tension, int const& numSegments) : x(x), y(y), tension(tension), numSegments(numSegments) {}

		inline bool operator==(const CurvePoint& v) const noexcept {
			return x == v.x && y == v.y;
		}
		inline CurvePoint& operator+=(CurvePoint const& v) noexcept {
			x += v.x;
			y += v.y;
			return *this;
		}
		inline CurvePoint operator+(CurvePoint const& v) const noexcept {
			return CurvePoint{ x + v.x, y + v.y, 0, 0 };
		}
		inline CurvePoint operator-(CurvePoint const& v) const noexcept {
			return CurvePoint{ x - v.x, y - v.y, 0, 0 };
		}
		inline CurvePoint operator*(float const& s) const noexcept {
			return CurvePoint{ x * s, y * s, 0, 0 };
		}
		inline CurvePoint operator/(float const& s) const noexcept {
			return CurvePoint{ x / s, y / s, 0, 0 };
		}
	};

	// 移动路线
	struct Pathway {
		// 是否闭合( 是 则 最后一个点 的下一个指向 第一个点 )
		bool isLoop = false;

		// 点集合
		std::vector<PathwayPoint> points;

		Pathway() = default;

		// 禁用复制，只能在智能指针环境下使用
		Pathway(Pathway const&) = delete;

		Pathway& operator=(Pathway const&) = delete;

		// 前进: 传入 移动距离( 正数 )，当前点下标，当前点已移动距离，回填坐标 & 角度
		// 返回是否已移动到终点( isLoop == false )
		bool Forward(float total, size_t& i, float& d, xx::Point& pos, float& a) const;

		// 后退: 传入 移动距离( 正数 )，当前点下标，当前点剩余距离，回填坐标 & 角度
		// 返回是否已移动到起点( isLoop == false )
		bool Backward(float total, size_t& i, float& d, xx::Point& pos, float& a) const;

		// 获取起点的数据
		void Begin(size_t& i, float& d, xx::Point& pos, float& a) const;

		// 获取终点的数据
		void End(size_t& i, float& d, xx::Point& pos, float& a) const;

		// 针对手工填充了坐标的数据，填充 角度 和 距离
		void FillDA();

		// 创建一个曲线连接途经点的 pathway
		static std::shared_ptr<Pathway> Make(bool isLoop, std::vector<CurvePoint> const& ps);
	};


	// pathway 线段构造器
	struct PathwayMaker {
		// 指向被 make 的 移动路线
		std::shared_ptr<Pathway> pathway;

		// make pathway 并 push 起始坐标 作为第一个点
		explicit PathwayMaker(xx::Point const& pos);

		// 从最后个点前进一段距离，形成新的点，新点.a = 最后个点.a，新点.d = 0
		PathwayMaker& Forward(float const& d);

		// 改变最后个点角度( = )
		PathwayMaker& RotateTo(float const& a);

		// 改变最后个点角度指向目标坐标
		PathwayMaker& RotateTo(xx::Point const& tarPos);

		// 改变最后个点角度( + )
		PathwayMaker& RotateBy(float const& a);

		// 令最后个点针对 tarPos 计算 a, d, 追加形成新的点，新点.a = 最后个点.a，新点.d = 0
		PathwayMaker& To(xx::Point const& tarPos);

		// RotateTo + Forward
		PathwayMaker& ForwardTo(xx::Point const& tarPos, float const& d);

		// 从最后个点弹跳前进一段距离，遇到边界会反弹( 类似台球 ). 会在改变角度时形成新的节点
		PathwayMaker& BounceForward(float d, float const& rectX, float const& rectY, float const& rectW, float const& rectH);

		// 令最后个点针对第一个点计算 a, d，标记循环 并返回 pathway 容器
		std::shared_ptr<Pathway> Loop();

		// 返回 pathway 容器
		std::shared_ptr<Pathway> End() const;

		PathwayMaker() = delete;

		PathwayMaker(PathwayMaker const&) = delete;

		PathwayMaker& operator=(PathwayMaker const&) = delete;
	};






	// 标识内存可移动
	template<>
	struct IsPod<Point, void> : std::true_type {
	};

	// 适配 Point 之 序列化 & 反序列化
	template<>
	struct DataFuncs<Point, void> {
		static inline void Write(DataWriter& dw, Point const& in) {
			dw.WriteFixed(in);
		}

		static inline int Read(DataReader& dr, Point& out) {
			return dr.ReadFixed(out);
		}
	};

	// 适配 Point 之 ToString
	template<>
	struct StringFuncs<Point, void> {
		static inline void Append(std::string& s, Point const& in) {
			xx::Append(s, '[', in.x, ',', in.y, ']');
		}
	};


	// 标识内存可移动
	template<>
	struct IsPod<PathwayPoint, void> : std::true_type {
	};

	// 适配 PathwayPoint 之 序列化 & 反序列化
	template<>
	struct DataFuncs<PathwayPoint, void> {
		static inline void Write(DataWriter& dw, PathwayPoint const& in) {
			dw.WriteFixed(in);
		}

		static inline int Read(DataReader& dr, PathwayPoint& out) {
			return dr.ReadFixed(out);
		}
	};

	// 适配 PathwayPoint 之 ToString
	template<>
	struct StringFuncs<PathwayPoint, void> {
		static inline void Append(std::string& s, PathwayPoint const& in) {
			xx::Append(s, '[', in.pos.x, ',', in.pos.y, ',', in.a, ',', in.d, ']');
		}
	};


	// 适配 Pathway 之 序列化 & 反序列化
	template<>
	struct DataFuncs<Pathway, void> {
		static inline void Write(DataWriter& dw, Pathway const& in) {
			dw.Write(in.isLoop, in.points);
		}

		static inline int Read(DataReader& dr, Pathway& out) {
			return dr.Read(out.isLoop, out.points);
		}
	};

	// 适配 Pathway 之 ToString
	template<>
	struct StringFuncs<Pathway, void> {
		static inline void Append(std::string& s, Pathway const& in) {
			xx::Append(s, "{\"isLoop\":", in.isLoop, ",\"points\":", in.points, '}');
		}
	};






	// 弧度 转为角度要 * (180.0f / float(M_PI))

	// 计算直线的弧度
	template<typename Point1, typename Point2>
	float GetAngle(Point1 const& from, Point2 const& to) noexcept {
		if (from.x == to.x && from.y == to.y) return 0.0f;
		auto&& len_y = to.y - from.y;
		auto&& len_x = to.x - from.x;
		return atan2f(len_y, len_x);
	}

	// 计算距离
	template<typename Point1, typename Point2>
	float GetDistance(Point1 const& a, Point2 const& b) noexcept {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return sqrtf(dx * dx + dy * dy);
	}

	// 点围绕 0,0 为中心旋转 a 弧度   ( 角度 * (float(M_PI) / 180.0f) )
	template<typename Point>
	inline Point Rotate(Point const& pos, float const& a) noexcept {
		auto&& sinA = sinf(a);
		auto&& cosA = cosf(a);
		return Point{ pos.x * cosA - pos.y * sinA, pos.x * sinA + pos.y * cosA };
	}

	// 判断两线段( p0-p1, p2-p3 )是否相交, 并且往 p 填充交点
	template<typename Point>
	inline bool GetSegmentIntersection(Point const& p0, Point const& p1, Point const& p2, Point const& p3, Point* const& p = nullptr) noexcept {
		Point s02, s10, s32;
		float s_numer, t_numer, denom, t;
		s10.x = p1.x - p0.x;
		s10.y = p1.y - p0.y;
		s32.x = p3.x - p2.x;
		s32.y = p3.y - p2.y;

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0) return false; // Collinear
		bool denomPositive = denom > 0;

		s02.x = p0.x - p2.x;
		s02.y = p0.y - p2.y;
		s_numer = s10.x * s02.y - s10.y * s02.x;
		if ((s_numer < 0) == denomPositive) return false; // No collision

		t_numer = s32.x * s02.y - s32.y * s02.x;
		if ((t_numer < 0) == denomPositive) return false; // No collision

		if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive)) return false; // No collision

		t = t_numer / denom;        // Collision detected
		if (p) {
			p->x = p0.x + (t * s10.x);
			p->y = p0.y + (t * s10.y);
		}
		return true;
	}

	// 途径点曲线( p p p ... ) 转 2 控制点的贝塞尔( p c c p c c p ... )
	template<typename List1, typename List2>
	void CurveToBezier(List1& bs, List2 const& cs) {
		auto n = cs.size();
		auto len = n * 3 - 2;
		bs.resize(len);

		bs[0] = cs[0];
		bs[1] = (cs[1] - cs[0]) * cs[0].tension + cs[0];

		for (size_t i = 0; i < n - 2; i++) {
			auto diff = (cs[i + 2] - cs[i]) * cs[i].tension;
			bs[3 * i + 2] = cs[i + 1] - diff;
			bs[3 * i + 3] = cs[i + 1];
			bs[3 * i + 4] = cs[i + 1] + diff;
		}
		bs[len - 2] = (cs[n - 2] - cs[n - 1]) * cs[n - 2].tension + cs[n - 1];
		bs[len - 1] = cs[n - 1];
	}

	// 2 控制点的贝塞尔( p c c p c c p ... ) 切片转点
	template<typename Point1, typename Point2>
	void BezierToPoints(std::vector<Point1>& ps, std::vector<Point2> const& bs) {
		auto len = (bs.size() - 1) / 3;
		size_t totalSegments = 0;
		for (size_t j = 0; j < len; ++j) {
			totalSegments += bs[j * 3].numSegments;
		}
		ps.resize(totalSegments);
		totalSegments = 0;
		for (size_t j = 0; j < len; ++j) {
			auto idx = j * 3;
			auto numSegments = bs[idx].numSegments;
			auto step = 1.0f / numSegments;
			for (int i = 0; i < numSegments; ++i) {
				auto t = step * i;
				auto t1 = 1 - t;
				ps[totalSegments + i] = bs[idx] * t1 * t1 * t1
					+ bs[idx + 1] * 3 * t * t1 * t1
					+ bs[idx + 2] * 3 * t * t * (1 - t)
					+ bs[idx + 3] * t * t * t;
			}
			totalSegments += numSegments;
		}
	}


	inline bool Pathway::Forward(float total, size_t& i, float& d, xx::Point& pos, float& a) const {
		auto siz = points.size();
	LabBegin:
		auto left = points[i].d - d;
		// 总距离大于当前点剩余距离：从 total 中减去, 剩余距离清0, i 指向下一个点
		if (total >= left) {
			// 从 dist 中减去当前路点剩余距离
			total -= left;
			d = 0;
			if (isLoop) {
				// 指向下一个路点, 如果到终点, 就指向起点
				if (++i == siz) {
					i = 0;
				}
				// 继续 while 从 dist 减 p->distance
				goto LabBegin;
			}
			// 如果还有下一个路点存在( 最后一个点不算 )
			else if (i + 2 < siz) {
				++i;
				goto LabBegin;
			}
			else {
				pos = points[i].pos;
				a = points[i].a;
				return true;
			}
		}
		// 记入当前点已移动距离
		else {
			d += total;
		}
		// 根据当前点上已经前进的距离, 结合下一个点的位置算坐标
		pos = points[i].pos + ((points[i == siz - 1 ? 0 : i + 1].pos - points[i].pos) * (d / points[i].d));
		a = points[i].a;
		return false;
	}

	inline bool Pathway::Backward(float total, size_t& i, float& d, xx::Point& pos, float& a) const {
		auto siz = points.size();
	LabBegin:
		if (total >= d) {
			total -= d;
			if (isLoop) {
				i = i ? (i - 1) : (siz - 1);
			}
			else {
				if (i == 0) {
					d = 0;
					pos = points[0].pos;
					a = points[0].a;
					return true;
				}
				--i;
			}
			d = points[i].d;
			goto LabBegin;
		}
		else {
			d -= total;
		}
		pos = points[i].pos + ((points[i == siz - 1 ? 0 : i + 1].pos - points[i].pos) * (d / points[i].d));
		a = points[i].a;
		return false;
	}


	inline void Pathway::Begin(size_t& i, float& d, xx::Point& pos, float& a) const {
		i = 0;
		d = 0;
		pos = points[0].pos;
		a = points[0].a;
	}

	inline void Pathway::End(size_t& i, float& d, xx::Point& pos, float& a) const {
		i = points.size() - 1;
		d = points[i].d;
		pos = points[i].pos;
		a = points[i].a;
	}

	inline void Pathway::FillDA() {
		auto n = points.size() - 1;
		size_t i = 0;
		for (; i < n; ++i) {
			points[i].a = GetAngle(points[i].pos, points[i + 1].pos);
			points[i].d = GetDistance(points[i].pos, points[i + 1].pos);
		}
		if (isLoop) {
			points[i].a = GetAngle(points[i].pos, points[0].pos);
			points[i].d = GetDistance(points[i].pos, points[0].pos);
		}
		else {
			points[i].a = points[i - 1].a;
			points[i].d = 0;
		}
	}


	inline PathwayMaker::PathwayMaker(xx::Point const& pos) {
		xx::MakeTo(pathway);
		pathway->points.emplace_back(pos);
	}

	inline PathwayMaker& PathwayMaker::Forward(float const& d) {
		auto&& p = pathway->points.back();
		p.d = d;
		auto a = p.a;
		auto pos = p.pos;
		pathway->points.emplace_back(xx::Rotate(xx::Point{ d, 0 }, a) + pos).a = a;
		return *this;
	}

	inline PathwayMaker& PathwayMaker::RotateTo(float const& a) {
		pathway->points.back().a = a;
		return *this;
	}

	inline PathwayMaker& PathwayMaker::RotateTo(xx::Point const& tarPos) {
		pathway->points.back().a = GetAngle(pathway->points.back().pos, tarPos);
		return *this;
	}

	inline PathwayMaker& PathwayMaker::RotateBy(float const& a) {
		pathway->points.back().a += a;
		return *this;
	}

	inline PathwayMaker& PathwayMaker::To(xx::Point const& tarPos) {
		auto&& p = pathway->points.back();
		auto a = p.a = GetAngle(p.pos, tarPos);
		p.d = GetDistance(p.pos, tarPos);
		pathway->points.emplace_back(tarPos).a = a;
		return *this;
	}

	inline  PathwayMaker& PathwayMaker::ForwardTo(xx::Point const& tarPos, float const& d) {
		pathway->points.back().a = GetAngle(pathway->points.back().pos, tarPos);
		Forward(d);
		return *this;
	}

	inline PathwayMaker& PathwayMaker::BounceForward(float d, float const& rectX, float const& rectY, float const& rectW, float const& rectH) {
		// todo：根据 rect 求出 4 条边界的坐标，依次和 当前点 前进 d 产生的 线段 判断 相交？
		// 如果有交点，根据边界方位，计算反弹角度? 令 d 减去 出发点到交点的距离？
		// 如果 d 还有剩，使用新的角度前进并找交点？
		return *this;
	}


	inline std::shared_ptr<Pathway> PathwayMaker::Loop() {
		auto&& p1 = pathway->points.back();
		auto&& p2 = pathway->points[0];
		p1.a = GetAngle(p1.pos, p2.pos);
		p1.d = GetDistance(p1.pos, p2.pos);
		pathway->isLoop = true;
		return pathway;
	}

	inline std::shared_ptr<Pathway> PathwayMaker::End() const {
		return pathway;
	}


	inline std::shared_ptr<Pathway> Pathway::Make(bool isLoop, std::vector<CurvePoint> const& ps) {
		auto rtv = xx::Make<xx::Pathway>();
		rtv->isLoop = isLoop;

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
			rtv->isLoop = isLoop;
			// 填充第二个点
			if (isLoop) {
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
			std::vector<CurvePoint> bs, cs;

			// 曲线 转为 带控制点的贝塞尔
			if (isLoop) {
				// 循环曲线则前后各追加几个点算控制点
				bs.push_back(ps[siz - 3]);
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
				cs.erase(cs.begin(), cs.begin() + 9);
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

		return rtv;
	}
}
