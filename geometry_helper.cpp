//
// Created by Win10 on 2023/2/2.
//

#include "geometry_helper.h"

#include <cassert>

namespace xgeo_helper {

using xmath::cross;

bool IsCoplanar(const std::vector<Point> &points)
{
	// colinear (single or two points)
	if (points.size() <= 2) return true;

	Point p0 = points.at(0);
	Point p1 = points.at(1);
	Point p2 {};

	size_t i = 2;
	for (; i < points.size(); ++ i)
	{
		p2 = points.at(i);

		Vector v1 = p0 - p1;
		Vector v2 = p0 - p2;

		if (cross(v1, v2).length() > xmath::epsilon16) break;
	}

	// colinear multipoints
	if (i == points.size()) return true;

	Vector v1 = p0 - p1;
	Vector v2 = p0 - p2;

	Vector normal = unit_vector(cross(v1, v2));

	return std::all_of(std::begin(points), std::end(points), [&](auto &p) {
		Vector vp = p0 - p;
		ValueType d = std::fabs(dot(normal, vp));
		return d <= xmath::epsilon16;
	});
}

bool IsInside(const Point& p, const Plane& plane)
{
	auto p0 = plane.support_p0();
	return fabs(xmath::dot(p0 - p, plane.normal())) <= xmath::epsilon16;
}

#if 0
// point and segment are collinear
inline bool OnSegment(const Point &e, const Segment &s)
{
	Vector v1 = s.endpoint1() - e;
	Vector v2 = s.endpoint2() - e;

	Vector zero = Vector{0, 0, 0};
	Vector ret = cross(v1, v2);

	return ret == zero && sgn(dot(v1, v2)) <= 0;
}


std::optional<int> IsInside(const Point &p, const Polygon &poly)
{
	if (!poly.IsValid()) return std::nullopt;

	int count = 0;
	auto points = poly.GetPoints();

	const int n = poly.NumberOfPoints();
	for (int i = 0; i < n; ++ i)
	{
		if (OnSegment(p, {points.at(i), points.at((i + 1) % n)}))
			return kOnBoundary;

		auto v1 = points.at((i + 1) % n) - points.at(i);
		auto v2 = points.at(i) - p;
		int k = sgn(c)
		int k = sgn()
	}

	// when count is odd
	auto isOdd = [](int i) { return i & 1;};

	if (isOdd(count)) return kInside;

	return kOutside;
}
#endif

int IsInside(const Point&p, const Polygon& poly)
{
	// TODO: implement it
	return kInside;
}

Polygon2D Convert(const Polygon &poly, const Point& origin, const Vector& unit_x, const Vector& unit_y)
{
	// TODO: implement it
	return Polygon2D{};
}

Point2D Covert(const Point &poly, const Point& origin, const Vector& base_vx, const Vector& base_vy)
{
	// TODO :implement it
	return Point2D{};
}

}