//
// Created by Win10 on 2023/2/2.
//

#ifndef CTCI_RAYTRACER_GEOMETRY_HELPER_H_
#define CTCI_RAYTRACER_GEOMETRY_HELPER_H_

#include "geometry.h"

namespace xgeo_helper {

using namespace xgeo;

// whether points are in the same plane
bool IsCoplanar(const std::vector<Point> &points);

// whether is a point in a plane
bool IsInside(const Point& p, const Plane& plane);

// convert a polygon (3d, simple closed and coplanar) to a 2d-polygon
Polygon2D Convert(const Polygon &poly, const Point& origin, const Vector& base_vx, const Vector& base_vy);

// convert a 3d point to a 2d point
Point2D Covert(const Point &poly, const Point& origin, const Vector& base_vx, const Vector& base_vy);

enum : int { kOnBoundary = -1, kOutside, kInside, };
int IsInside(const Point&p, const Polygon& poly);

}

#endif //CTCI_RAYTRACER_GEOMETRY_HELPER_H_













