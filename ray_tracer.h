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
using std::shared_ptr;
using std::make_shared;

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

struct Material;
using DirectedMaterial = std::pair<Vector, Material>;
class ScatterBehaviour;

struct HitRecord {
	Vector position = {};
	Vector normal = {};
	ValueType t = ValueType{};
	std::shared_ptr<ScatterBehaviour> scattered_ = nullptr;
};

using PathType = std::vector<Point>;

inline std::ostream & operator << (std::ostream &out, const PathType &path)
{
	for (const auto &p: path) { out << "{" << p << "}" << "----"; }
	return out;
}

class Route {
public:
	Route() = default;
	explicit Route(std::vector<HitRecord> records) : records_(std::move(records)) {}
	void Push(const HitRecord &rec) { records_.push_back(rec); }

	PathType GetPath();

private:
	std::vector<HitRecord> records_;
};


struct Material {
	ValueType shear_speed = ValueType{};
	ValueType compr_speed = ValueType{};
	ValueType rho = ValueType{};
};

class ScatterBehaviour {
public:
	virtual bool Scatter(const Ray &ray, const HitRecord &rec, Ray &scattered) const = 0;
};

class Reflector : public ScatterBehaviour {
public:
	explicit Reflector(DirectedMaterial  material): material1_(std::move(material)) {
		material2_.first  = -material1_.first;
		material2_.second = material1_.second;
	}
	bool Scatter(const Ray &ray, const HitRecord &rec, Ray &scattered) const override;

private:
	DirectedMaterial material1_;
	DirectedMaterial material2_;
};

class Refractor : public ScatterBehaviour {
public:
	Refractor(DirectedMaterial  material1, DirectedMaterial  material2)
	: material1_(std::move(material1)), material2_(std::move(material2)) {}

	bool Scatter(const Ray &ray, const HitRecord &rec, Ray &scattered) const override;

private:
	DirectedMaterial material1_;
	DirectedMaterial material2_;
};


class Hitable {
public:
	virtual bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &ret) const = 0;
	virtual bool Deflect(const Ray& ray, const HitRecord& rec, Ray& scattered) const = 0;
};

// entity list
class HitableSet : public Hitable {
public:
	HitableSet() = default;
	explicit HitableSet(const shared_ptr<Hitable> &obj) { Push(obj); }
	void Push(const shared_ptr<Hitable> &obj) { objects_.push_back(obj); }

	void Clear() { objects_.clear(); }

	bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &rec) const override;

	bool Deflect(const Ray& ray, const HitRecord& rec, Ray& scattered) const override;

private:
	std::vector<shared_ptr<Hitable>> objects_;
};

// Flat surface
class Surface : public Hitable {
public:
	Surface(Polygon poly, Vector normal, std::shared_ptr<ScatterBehaviour> scattered)
		: poly_(std::move(poly)), normal_(normal), scattered_(std::move(scattered)) {}

	bool Hit(const Ray &ray, ValueType t_min, ValueType t_max, HitRecord &ret) const override;

	bool Deflect(const Ray& ray, const HitRecord& rec, Ray& scattered) const override {
		return scattered_->Scatter(ray, rec, scattered);
	};

private:
	Polygon poly_;
	Vector normal_;
	std::shared_ptr<ScatterBehaviour> scattered_;
};

// Interface
Route RayTrace(const Ray &ray, const HitableSet &hit_set, int n = 1);

}

#endif //CTCI_RAYTRACER_RAY_TRACER_H_
















