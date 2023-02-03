
//
// Created by Win10 on 2023/1/31.
//

#include <cassert>

#include "ray_tracer.h"

namespace xray {

using namespace xmath;
using namespace xgeo_helper;

bool HitableSet::Hit(const Ray &ray, const ValueType t_min, const ValueType t_max, HitRecord &rec) const
{
	HitRecord tmp_rec;
	bool hit_any = false;
	ValueType closet_so_far = t_max;

	for (const auto &object : objects_) {
		if (object->Hit(ray, t_min, closet_so_far, tmp_rec)) {
			hit_any = true;
			closet_so_far = tmp_rec.t;
			rec = tmp_rec;
		}
	}
	return hit_any;
}

bool Surface::Hit(const Ray &ray, const ValueType t_min, const ValueType t_max, HitRecord &rec) const
{
	auto supports = poly_.GetSupports();
	auto p0 = supports.at(0);
	auto r0 = ray.origin();

	Plane plane{supports, normal_};

	if (IsInside(r0, plane)) return false;

	// outward normal
	auto n1 = normal_;
	auto n2 = ray.direction();

	auto t = dot((p0 - r0), n1) / dot(n1, n2);

	auto target = r0 + t * n2;

	auto segment_len = (r0 - target).length();
	if (segment_len < t_min || segment_len > t_max)
		return false;

	rec.normal = normal_;
	rec.position = target;
	rec.t = t;

	assert(IsInside(target, plane));
	return true;
}

std::vector<Point> Route::GetPath()
{
	std::vector<Point> path;
	for (const auto rec : records_) {
		auto p = rec.position;
		path.push_back(p);
	}
	return path;
}

Route RayTrace(const Ray &ray, const HitableSet &hit_set, int n)
{
	HitRecord start_rec;
	start_rec.position = ray.origin();
	start_rec.normal = ray.direction();

	Route route;
	route.Push(start_rec);

	Ray r_in = ray;
	for (int i = 0; i < n; ++i) {
		HitRecord rec;
		if (!hit_set.Hit(r_in, epsilon12, infinity, rec))
			break;

		route.Push(rec);
		r_in = Ray(rec.position, rec.normal);
	}

	return route;
}

bool Reflector::Scatter(const Ray &ray, const HitRecord &rec, const Ray &scattered)
{
	return false;
}

bool Refractor::Scatter(const Ray &ray, const HitRecord &rec, const Ray &scattered)
{
	return false;
}

}














