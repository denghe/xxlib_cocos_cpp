inline PathwayPoint(float const& x, float const& y, float const& tension, int const& numSegments)
	: x(x)
	, y(y)
	, tension(tension)
	, numSegments(numSegments) {}

inline bool operator==(const PathwayPoint& v) const noexcept {
	return x == v.x && y == v.y;
}
inline PathwayPoint& operator+=(PathwayPoint const& v) noexcept {
	x += v.x;
	y += v.y;
	return *this;
}
inline PathwayPoint operator+(PathwayPoint const& v) const noexcept {
	return PathwayPoint{ x + v.x, y + v.y, 0, 0 };
}
inline PathwayPoint operator-(PathwayPoint const& v) const noexcept {
	return PathwayPoint{ x - v.x, y - v.y, 0, 0 };
}
inline PathwayPoint operator*(float const& s) const noexcept {
	return PathwayPoint{ x * s, y * s, 0, 0 };
}
inline PathwayPoint operator/(float const& s) const noexcept {
	return PathwayPoint{ x / s, y / s, 0, 0 };
}
