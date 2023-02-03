//
// Created by Win10 on 2023/1/31.
//

#ifndef CTCI_RAYTRACER_RAY_TRACER_H_
#define CTCI_RAYTRACER_RAY_TRACER_H_

#include <utility>
#include <vector>
#include <numeric>
#include <memory>

#include "common.h"
#include "geometry_helper.h"
#include "matrix.h"

namespace xray {

using Vector = xmath::Vector3D<ValueType>;
using Point = xmath::Point3D<ValueType>;

using namespace xgeo;
using xmath::dot;

class Ray {
public:
	Ray() = default;
	Ray(const Point &origin, Vector dir) : orig_(origin), dir_(dir) {}

	Point origin() const { return orig_; }
	Vector direction() const { return dir_; }
	Vector At(const ValueType &t) const { return orig_ + t * dir_; }

private:
	Point orig_;
	Vector dir_;
};

struct HitRecord {
	Vector position = {};
	Vector normal = {};
	ValueType t = ValueType{};
	bool front_face = false;

	void SetFaceNormal(const Ray &r, const Vector &outward_normal)
	{
		front_face = xmath::dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Route {
public:
	Route() = default;
	explicit Route(std::vector<HitRecord> records) : records_(std::move(records)) {}
	void Push(const HitRecord &rec) { records_.push_back(rec); }

	std::vector<Point> GetPath();

private:
	std::vector<HitRecord> records_;
};

class Hitable {
public:
	virtual bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &ret) const = 0;
};

class Material {
public:
	virtual bool Scatter(const Ray &in, const HitRecord &rec, Ray &scattered) const = 0;
};

using std::shared_ptr;
using std::make_shared;

// entity list
class HitableSet : public Hitable {
public:
	HitableSet() = default;
	explicit HitableSet(const shared_ptr<Hitable> &obj) { Push(obj); }
	void Push(const shared_ptr<Hitable> &obj) { objects_.push_back(obj); }

	void Clear() { objects_.clear(); }

	bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &rec) const override;

private:
	std::vector<shared_ptr<Hitable>> objects_;
};

// Flat surface
class Surface : public Hitable {
public:
	Surface(Polygon poly, Vector normal, const Material *material)
		: poly_(std::move(poly)), normal_(normal), material_(material) {}
	bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &ret) const override;

private:
	Polygon poly_;
	Vector normal_;
	const Material *material_ = nullptr;
};

// closed surface and among them
class Polyhedron : public Hitable {
public:
	explicit Polyhedron(std::vector<Surface> polys) : polys_(std::move(polys)) {}
	void AddSurface(const Surface &surface) { polys_.push_back(surface); }
	bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &ret) const override;

private:
	std::vector<Surface> polys_;
};

class Relector : public Material {
public:
	Relector() = default;
	bool Scatter(const Ray &in, const HitRecord &rec, Ray &scattered) const override;
};

class Refractor : public Material {
public:
	bool Scatter(const Ray &in, const HitRecord &rec, Ray &scattered) const override;

};

Route RayTrace(const Ray &ray, const HitableSet &hit_set, int n = 1);

}

#endif //CTCI_RAYTRACER_RAY_TRACER_H_
















