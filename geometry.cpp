//
// Created by Win10 on 2023/2/2.
//
#include "geometry.h"

#include <cassert>

#include "geometry_helper.h"



namespace xgeo {
using xmath::cross;

bool Polygon::isCoplanar() const
{
	return xgeo_helper::IsCoplanar(points_);
}

bool Polygon::isSimple() const
{

	// TODO: implement it
	return true;
}

bool Polygon::IsValid() const
{
	return isCoplanar() && isSimple();
}

std::optional<Polygon> Polygon::Create(const std::vector<Point> &points)
{
		Polygon poly;
		poly.points_ = points;

		if (poly.IsValid())
			return poly;

		return std::nullopt;
}

std::vector<Point> Polygon::GetSupports() const
{
	assert(IsValid());

	auto p0 = points_.at(0);
	auto p1 = points_.at(1);
	Vector v0 =  p0 - p1;

	Point p2 {};
	for (const auto &p : points_)
	{
		p2 = p;
		Vector v1 = p - p1;
		if (cross(v0, v1).length() >= xmath::epsilon16)
			break;
	}

	return {p0, p1, p2};
}

bool Plane::IsValid() const
{
	Vector v0 = support_[0] - support_[1];
	Vector v1 = support_[1] - support_[2];

	return cross(v0, v1).length() >= xmath::epsilon16;
}

}

