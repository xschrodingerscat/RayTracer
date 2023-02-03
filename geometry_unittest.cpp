//
// Created by Win10 on 2023/2/2.
//

#include <gmock/gmock.h>

#include "geometry_helper.h"

using namespace xgeo_helper;

TEST(Xgeo_Helper, IsCoplanar_Colinear2Points)
{
	std::vector<Point> v {{0, 0, 0}, {1, 1, 1}};
	EXPECT_THAT(IsCoplanar(v), true);
}

TEST(Xgeo_Helper, IsCoplanar_Colinear3Points)
{
	std::vector<Point> v {{0, 0, 0}, {1, 1, 1}, {2, 2, 2}};
	EXPECT_THAT(IsCoplanar(v), true);
}

TEST(Xgeo_Helper, IsCoplanar_Colinear4Points)
{
	std::vector<Point> v {{0, 0, 0}, {1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
	EXPECT_THAT(IsCoplanar(v), true);
}

TEST(Xgeo_Helper, IsCoplanar_Coplanar4Points)
{
	std::vector<Point> v {{0, 0, 0}, {1, 0, 0}, {0, 1, 1}, {1, 1, 1}};
	EXPECT_THAT(IsCoplanar(v), true);
}

TEST(Xgeo_Helper, IsCoplanar_NotCoplanar4Points)
{
	std::vector<Point> v {{0, 0, 0}, {1, 0, 0}, {0, 1, 1}, {1, 1, 2}};
	EXPECT_THAT(IsCoplanar(v), false);
}
