//
// Created by Win10 on 2023/2/2.
//
#include "geometry.h"
#include "geometry_helper.h"

namespace xgeo {

bool Polygon::isCoplanar() const
{
	return xgeo_helper::IsCoplanar(points_);
}

bool Polygon::isSimpleClosed() const
{

	// TODO: implement it
	return true;
}

bool Polygon::IsValid() const
{
	return isCoplanar() && isSimpleClosed();
}

std::optional<Polygon> Polygon::Create(const std::vector<Point> &points)
{
		Polygon poly;
		poly.points_ = points;

		if (poly.IsValid())
			return poly;

		return std::nullopt;
}

}

