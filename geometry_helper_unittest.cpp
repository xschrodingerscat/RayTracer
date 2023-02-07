//
// Created by Win10 on 2023/2/6.
//

#include <gmock/gmock.h>

#include <geometry_helper.h>

using namespace xmath;
using namespace xgeo_helper;

TEST(IsInside3D, TwoPointPoly)
{
	Point p{0, 0, 0};
	Polygon poly{{{0, 0, 0}, {1, 0, 0}}};

	EXPECT_FALSE(IsInside(p, poly));
}

TEST(IsInside3D, FourPointPoly)
{
	Polygon poly{{{0, 0, 0}, {1, 0, 0}, {0, 0, 1}, {1, 0, 1}}};

	Point p{0, 0, 0};
	EXPECT_THAT(IsInside(p, poly), true);

	Point p1{0, -1, 0};
	EXPECT_THAT(IsInside(p1, poly), false);

	Point p2{0.5, 0.5, 0};
	EXPECT_THAT(IsInside(p, poly), true);
}

TEST(IsInside3D, MultiPointPoly)
{
	Polygon poly{{{1, 0, 0}, {0, 0, 1}, {1, 0, 2}, {2, 0, 1.5}, {2, 0, 1}, {3, 0, 0.5}}};

	Point p{1, 0, 1};
	EXPECT_THAT(IsInside(p, poly), true);
}

TEST(IsInside3D, MultiPointPolySlope)
{
	std::vector<Point> points{{0, 0, 0}, {1, 0, 0}, {1, 1, 1}, {0, 1, 1}};
	Polygon poly(points);

	Point p{0.5, 0.5, 0.5};
	EXPECT_THAT(IsInside(p, poly), true);
}



