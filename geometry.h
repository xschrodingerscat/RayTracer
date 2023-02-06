//
// Created by Win10 on 2023/2/2.
//

#ifndef CTCI_RAYTRACER_GEOMETRY_H_
#define CTCI_RAYTRACER_GEOMETRY_H_

#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

#include "xmath.h"
#include "matrix.h"

namespace xgeo {

using Vector = xmath::Vector3D<ValueType>;
using Point = xmath::Point3D<ValueType>;
using Vector2D = xmath::Vector2D<ValueType>;
using Point2D = xmath::Point2D<ValueType>;

// Line segment
class Segment {
public:
	Segment() = default;
	Segment(const Point &e1, const Point &e2) : e1_(e1), e2_(e2) {}

	Point endpoint1() const { return e1_; }
	Point endpoint2() const { return e2_; }
	ValueType length() const { return (e1_ - e2_).length(); }

private:
	// endpoint1 and endpoint2
	Point e1_ = {};
	Point e2_ = {};
};

class Plane {
public:
	Plane(const std::vector<Point> &supp, const Vector &normal) : normal_(normal)
	{
		support_[0] = supp.at(0);
		support_[1] = supp.at(1);
		support_[2] = supp.at(2);
	}

	Plane(const Point &p0, const Point &p1, const Point p2, const Vector &normal) : normal_(normal)
	{
		support_[0] = p0;
		support_[1] = p1;
		support_[2] = p2;
	}

	bool IsValid() const;

	Point support_p0() const { return support_[0]; }
	Point support_p1() const { return support_[1]; }
	Point support_p2() const { return support_[2]; }

	Vector normal() const { return normal_; }

private:
	Point support_[3];
	Vector normal_ = {};
};

class Polygon2D { public: };

// Simple Closed and Coplanar
class Polygon {
public:
	Polygon() = default;
	explicit Polygon(std::vector<Point> points) : points_(std::move(points)) {}

	static std::optional<Polygon> Create(const std::vector<Point> &points);

	int NumberOfPoints() const { return static_cast<int>(points_.size()); }
	const std::vector<Point> &GetPoints() const { return points_; }

	bool IsValid() const;

	std::vector<Point> GetSupports() const;

private:
	bool isSimple() const;
	bool isCoplanar() const;

	std::vector<Point> points_ = {};
};

}
#endif //CTCI_RAYTRACER_GEOMETRY_H_
