
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

bool HitableSet::Deflect(const Ray &ray, const HitRecord &rec, Ray &scattered) const
{
	if (rec.scattered_ == nullptr) return false;
	return rec.scattered_->Scatter(ray, rec, scattered);
}

bool Surface::Hit(const Ray &ray, const ValueType t_min, const ValueType t_max, HitRecord &rec) const
{
	auto supports = poly_.GetSupports();
	auto p0 = supports.at(0);
	auto r0 = ray.origin();

	// unit normal
	auto n1 = unit_vector(normal_);
	auto n2 = unit_vector(ray.direction());

	auto t = dot((p0 - r0), n1) / dot(n1, n2);

	// must be in the direction of the ray
	if (t < 0) return false;

	// intersection point
	auto intersection = r0 + t * n2;

	auto segment_len = (r0 - intersection).length();
	// limit the length of the ray
	if (!(t_min < segment_len && segment_len < t_max))
		return false;

	// the intersection must be in the poly
	if (!IsInside(intersection, poly_)) return false;

	rec.position = intersection;
	rec.t = t;
	rec.normal = dot(n1, n2) <=0 ? normal_ : -normal_;
	rec.scattered_ = scattered_;

	return true;
}

std::vector<Point> Route::GetPath()
{
	std::vector<Point> path;
	for (const auto& rec : records_) {
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
		// get record
		if (!hit_set.Hit(r_in, epsilon12, infinity, rec)) break;

		route.Push(rec);

		Ray scattered;
		// fetch the scattered ray
		if (!hit_set.Deflect(r_in, rec, scattered)) break;

		// update incident ray
		r_in = scattered;
	}

	return route;
}

bool Reflector::Scatter(const Ray &ray, const HitRecord &rec, Ray &scattered) const
{
	auto unit_normal = unit_vector(rec.normal);
	Vector reflected = reflect(ray.direction(), unit_normal);
	scattered = Ray(rec.position, reflected);
	return true;
}

bool Refractor::Scatter(const Ray &ray, const HitRecord &rec, Ray &scattered) const
{
	auto getEtaiOverEtat = [&](const Ray& ray) {
		auto speed1 = material1_.second.shear_speed;
		auto speed2 = material2_.second.shear_speed;

		if (dot(ray.direction(), material1_.first) >= 0)
			std::swap(speed1, speed2);

		// sin(theta_i) / sin(theta_t) = vi / vt = eta_t / eta_i
		return speed2 / speed1;
	};

	auto verify = [](const Vector &uv, const Vector &n, ValueType etai_over_etat) {
		auto cos_theta = fmin(dot(-uv, n), 1.0);
		Vector3D r_out_perp = etai_over_etat * (uv + cos_theta * n);
		ValueType ret = (1.0 - r_out_perp.length_squared());
		return  0. <= ret && ret <= 1.;
	};

	const auto etai_over_etat = getEtaiOverEtat(ray);

	Vector uv = unit_vector(ray.direction());
	Vector normal = unit_vector(rec.normal);

	// refracted ray vanished
	if (!verify(uv, normal, etai_over_etat)) return false;

	auto refracted = refract(uv, normal, etai_over_etat);
	scattered = Ray(rec.position, refracted);
	return true;
}

}














