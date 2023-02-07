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
	Point p2{};

	size_t i = 2;
	for (; i < points.size(); ++i) {
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

bool IsInside(const Point &p, const Plane &plane)
{
	auto p0 = plane.support_p0();
	return fabs(xmath::dot(p0 - p, plane.normal())) <= xmath::epsilon16;
}

bool isIntersect(const Segment2D &s1, const Segment2D &s2)
{
	int dir1 = Direction(s1.endpoint1(), s1.endpoint2(), s2.endpoint1());
	int dir2 = Direction(s1.endpoint1(), s1.endpoint2(), s2.endpoint2());
	int dir3 = Direction(s2.endpoint1(), s2.endpoint2(), s1.endpoint1());
	int dir4 = Direction(s2.endpoint1(), s2.endpoint2(), s1.endpoint2());

	// when intersecting
	if (dir1 != dir2 && dir3 != dir4) return true;

	// when p2 f line2 are on the line1
	if (dir1 == kColinear && OnSegment(s2.endpoint1(), s1))
		return true;

	if (dir2 == kColinear && OnSegment(s2.endpoint2(), s1))
		return true;

	if (dir3 == kColinear && OnSegment(s1.endpoint1(), s2))
		return true;

	if (dir4 == kColinear && OnSegment(s1.endpoint2(), s2))
		return true;

	return false;
}

int IsInside(const Point &p, const Polygon &poly)
{
	auto supp = poly.GetSupports();
	Vector normal = cross(supp.at(1) - supp.at(0), supp.at(2) - supp.at(0));
	Vector unit_n = xmath::unit_vector(normal);

	Plane plane{supp, unit_n};
	// not be coplanar
	if (!IsInside(p, plane)) return kOutside;

	auto origin = supp.at(0);
	Vector unit_x = xmath::unit_vector(supp.at(1) - supp.at(0));
	Vector unit_y = cross(unit_x, unit_n);

	auto poly_2d = Convert(poly, origin, unit_x, unit_y);
	auto p_2d = Convert(p, origin, unit_x, unit_y);
	return IsInside(p_2d, poly_2d);
}

int IsInside(const Point2D &p, const Polygon2D &poly)
{
	if (poly.NumberOfPoints() < 3) return kUnknown;

	auto points = poly.GetPoints();
	auto index = [&](int i) { return i % static_cast<int>(points.size()); };

	Segment2D ex_segment{p, {10e29, p.y()}};
	int count = 0;
	int i = 0;
	do {
		Segment2D segment{points.at(index(i)), points.at(index(i + 1))};
		if (isIntersect(ex_segment, segment)) {
			if (Direction(segment.endpoint1(), p, segment.endpoint2()) == kColinear)
				return OnSegment(p, segment);
			count++;
		}
		i = index(i + 1);
	} while (i != 0);

	if (count & 1) return kInside;
	else return kOutside;
}

Polygon2D Convert(const Polygon &poly, const Point &origin, const Vector &unit_x, const Vector &unit_y)
{
	auto points = poly.GetPoints();
	std::vector<Point2D> v;
	for (const auto &p : points) {
		ValueType x = dot(p - origin, unit_x);
		ValueType y = dot(p - origin, unit_y);
		v.emplace_back(x, y);
	}
	return Polygon2D{v};
}

Point2D Convert(const Point &p, const Point &origin, const Vector &unit_x, const Vector &unit_y)
{
	ValueType x = dot(p - origin, unit_x);
	ValueType y = dot(p - origin, unit_y);
	return Point2D{x, y};
}

bool OnSegment(const Point2D &p, const Segment2D &s)
{
	if (std::min(s.endpoint1().x(), s.endpoint2().x()) <= p.x()
		&& p.x() <= std::max(s.endpoint1().x(), s.endpoint2().x())
		&& std::min(s.endpoint1().y(), s.endpoint2().y()) <= p.y()
		&& p.y() <= std::max(s.endpoint1().y(), s.endpoint2().y())) {
		return true;
	}

	return false;
};

int Direction(const Point2D &a, const Point2D &b, const Point2D &c)
{
	ValueType val = (b.y() - a.y()) * (c.x() - b.x()) - (b.x() - a.x()) * (c.y() - b.y());

	if (val == 0)
		return kColinear;
	else if (val <= 0)
		return kAntiClockwise;
	return kClockwise;
}

}