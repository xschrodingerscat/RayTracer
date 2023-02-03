
#include "ray_tracer.h"
//
// Created by Win10 on 2023/1/31.
//

namespace xray {

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

bool Surface::Hit(const Ray &ray, const ValueType t_min, const ValueType t_max, HitRecord &ret) const
{
	// TODO: implement it
	return false;
}

bool Polyhedron::Hit(const Ray &ray, const ValueType t_min, const ValueType t_max, HitRecord &ret) const
{
	// TODO: implement it
	return false;
}

bool Relector::Scatter(const Ray &in, const HitRecord &rec, Ray &scattered) const
{
	Vector reflected = reflect(unit_vector(in.direction()), rec.normal);
	scattered = Ray(rec.position, reflected);
	return dot(scattered.direction(), rec.normal) > 0;
}

bool Refractor::Scatter(const Ray &in, const HitRecord &rec, Ray &scattered) const
{
	// TODO: implement it
	return false;
}

std::vector<Point> Route::GetPath()
{
	std::vector<Point> path;
	for (const auto rec: records_)
	{
		auto p = rec.position;
		path.push_back(p);
	}
	return path;
}

Route RayTrace(const Ray& ray, const HitableSet& hit_set) {
	return Route();
}
}














