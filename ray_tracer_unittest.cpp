//
// Created by Win10 on 2023/1/31.
//

#include <ray_tracer.h>

#include <gmock/gmock.h>

using namespace xray;

TEST(RayTracer, NonePath)
{
	Ray ray;
	HitableSet hit_set;
	auto route = RayTrace(ray, hit_set, 1);
	auto path = route.GetPath();

	EXPECT_THAT(path.size(), 1);
}

TEST(RayTracer, OneSurfaceAndNonePath)
{
	// construct Surface
	DirectedMaterial material{{0, -1, 0}, {1, 0, 0}};
	auto piece = std::make_shared<Reflector>(material);

	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}};
	Vector normal{0, 1, 0};
	auto surface = std::make_shared<Surface>(poly, normal, piece);

	HitableSet hit_set;
	hit_set.Push(surface);

	// construct Ray
	Point origin{0, 0, 0};
	Vector dir{0, 1, 0};
	Ray ray{origin, dir};

	auto route = RayTrace(ray, hit_set, 1);
	auto path = route.GetPath();
	EXPECT_THAT(path.size(), 1);
}

TEST(RayTracer, OneSurfaceAndOnePath)
{
	// construct Surface
	DirectedMaterial material{{0, -1, 0}, {1, 0, 0}};
	auto piece = std::make_shared<Reflector>(material);

	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}};
	Vector normal{0, 1, 0};
	auto surface = std::make_shared<Surface>(poly, normal, piece);

	HitableSet hit_set;
	hit_set.Push(surface);

	Point origin{0, -1, 0};
	Vector dir{0, 1, 0};
	Ray ray{origin, dir};

	auto route = RayTrace(ray, hit_set, 1);
	auto path = route.GetPath();
	EXPECT_THAT(path.size(), 2);
}

TEST(RayTracer, TwoSurfaceAndTwoPath)
{
	// construct Surface
	Vector normal{0, -1, 0};
	Material material{1, 0, 0};
	DirectedMaterial dmaterial{normal, material};
	auto piece = std::make_shared<Reflector>(dmaterial);

	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}};
	auto surface1 = std::make_shared<Surface>(poly, normal, piece);

	Vector normal2{0, 1, 0};
	Material material2{1.2, 0, 0};
	DirectedMaterial dmaterial2{normal2, material2};
	auto piece2 = std::make_shared<Reflector>(dmaterial2);

	Polygon poly2{{{0, -2, 0}, {1, -2, 0}, {1, -2, 1}, {0, -2, 1}}};
	auto surface2 = std::make_shared<Surface>(poly2, normal2, piece2);

	HitableSet hit_set;
	hit_set.Push(surface1);
	hit_set.Push(surface2);

	Point origin{0, 0, 0};
	Vector dir{1, -1, 1};
	Ray ray{origin, dir};

	auto route = RayTrace(ray, hit_set, 2);
	auto path = route.GetPath();
	EXPECT_THAT(path.size(), 3);
}

TEST(RayTracer, OneRefractiveAndOneReflectedSurfaceAndThreePath)
{
	// construct Surface
	Vector normal1{0, 1, 0};
	Material material1{0.8, 0, 0};
	DirectedMaterial dmaterial1{normal1, material1};

	Vector normal2 = -normal1;
	Material material2{1.2, 0, 0};
	DirectedMaterial dmaterial2{normal2, material2};
	auto piece = std::make_shared<Refractor>(dmaterial1, dmaterial2);

	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}};
	Vector normal = normal1;
	auto surface1 = std::make_shared<Surface>(poly, normal, piece);

	Vector normal3{0, 1, 0};
	Material material3{1, 0, 0};
	DirectedMaterial dmaterial3{normal3, material3};

	auto piece2 = std::make_shared<Reflector>(dmaterial3);

	Polygon poly2{{{0, 2, 0}, {1, 2, 0}, {1, 2, 1}, {0, 2, 1}}};
	auto surface2 = std::make_shared<Surface>(poly2, normal2, piece2);

	HitableSet hit_set;
	hit_set.Push(surface1);
	hit_set.Push(surface2);

	Point origin{0, -1, 0};
	Vector dir{1, 1, 0};
	Ray ray{origin, dir};

	auto route = RayTrace(ray, hit_set, 3);
	auto path = route.GetPath();

	auto IsPointNear = [](const Point &a, const Point &b) {
		ValueType threshold = xmath::epsilon03;
		return xmath::near(a.x(), b.x(), threshold)
			&& xmath::near(a.y(), b.y(), threshold)
			&& xmath::near(a.z(), b.z(), threshold);
	};

	EXPECT_TRUE(IsPointNear(path.at(0), {0, -1, 0}));
	EXPECT_TRUE(IsPointNear(path.at(1), {1, 0, 0}));
	EXPECT_TRUE(IsPointNear(path.at(2), {2.06904, 2, 0}));
	EXPECT_TRUE(IsPointNear(path.at(3), {3.13809, 0, 0}));
}
















