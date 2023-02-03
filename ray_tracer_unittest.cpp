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
	auto route = RayTrace(ray, hit_set);
	auto path = route.GetPath();

	EXPECT_THAT(path.size(), 0);
}

TEST(RayTracer, OneSurfaceAndZeroPointsPath)
{
	// construct Ray
	Point origin {0, 0, 0};
	Vector dir {0, 1, 0};
	Ray ray {origin, dir};

	// construct Surface
	shared_ptr<Relector> reflector;
	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}};
	Vector normal{0, 1, 0};
	auto surface = std::make_shared<Surface>(poly, normal, reflector.get());

	HitableSet hit_set;
	hit_set.Push(surface);

	auto route = RayTrace(ray, hit_set);
	auto path = route.GetPath();
	EXPECT_THAT(path.size(), 0);
}

TEST(RayTracer, OneSurfaceAndOnePointPath)
{
	Point origin {0, 0, 0};
	Vector dir {0, 1, 0};
	Ray ray {origin, dir};

	// construct Surface
	shared_ptr<Relector> reflector;
	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}}};
	Vector normal{0, 1, 0};
	auto surface = std::make_shared<Surface>(poly, normal, reflector.get());

	HitableSet hit_set;
	hit_set.Push(surface);

	auto route = RayTrace(ray, hit_set);
	auto path = route.GetPath();
	EXPECT_THAT(path.size(), 0);
}
















