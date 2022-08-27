#include <shapes.hpp>
#include <catch2/catch.hpp>
#include <vector>

TEST_CASE("polygon initialization") {
	shape::Polygon p({{0,0,0},{0,0,1},{0,1,1},{0,1,0}});
	CHECK(p.size()==4);
	REQUIRE(p[0]==geometry::Vector4d(0,0,0));
	REQUIRE(p[1]==geometry::Vector4d(0,0,1));
	REQUIRE(p[2]==geometry::Vector4d(0,1,1));
	REQUIRE(p[3]==geometry::Vector4d(0,1,0));
}

TEST_CASE("polygon apply_transform") {
	shape::Polygon p({{0,0,0},{0,0,1},{0,1,1},{0,1,0}});
	auto out = shape::apply_transform(p,
			geometry::rotate(3.141592653589793,
				{1,0,0,0}));

	shape::Polygon expected({{0,0,0},{0,0,-1},{0,-1,-1},{0,-1,0}});
	REQUIRE(out == expected);
}


TEST_CASE("polygon clip_against_plane") {
	shape::Polygon p({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},{2,-2,0}});
	geometry::Vector4d normal(-1.5,1,0);
	geometry::Vector4d point(-1,-3,0);

	shape::Polygon expected({{-2,-1,0},{-2,1,0},{1,3,0},
			{2.2,1.8,0},{0,-1.5,0}});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}

TEST_CASE("polygon clip_against_plane no intersection") {
	shape::Polygon p({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},{2,-2,0}});
	geometry::Vector4d normal(-1.5,1,0);
	geometry::Vector4d point(5,0,0);


	shape::Polygon expected({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},
			{2,-2,0}});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}



TEST_CASE("polygon clip_against_plane no vertices") {
	shape::Polygon p({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},{2,-2,0}});
	geometry::Vector4d normal(-1.5,1,0);
	geometry::Vector4d point(-3,2,0);


	shape::Polygon expected({});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}


TEST_CASE("polygon clip_against_plane touching a point") {
	shape::Polygon p({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},{2,-2,0}});
	geometry::Vector4d normal(-1.5,1,0);
	geometry::Vector4d point(3,1,0);


	shape::Polygon expected({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0}
			,{1.1428571428571,-1.7857142857142857,0}});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}


TEST_CASE("polygon clip_against_plane touching a side") {
	shape::Polygon p({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},{2,-2,0}});
	geometry::Vector4d normal(1,4,0);
	geometry::Vector4d point(-2,-1,0);


	shape::Polygon expected({{-2,-1,0},{-2,1,0},{1,3,0},
			{3,1,0},{2,-2,0}});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}



TEST_CASE("polygon clip_against_plane touching a side 2") {
	shape::Polygon p({{-2,-1,0},{-2,1,0},{1,3,0},{3,1,0},{2,-2,0}});
	geometry::Vector4d normal(-1,-4,0);
	geometry::Vector4d point(-2,-1,0);


	shape::Polygon expected({{-2,-1,0},{2,-2,0}});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}



TEST_CASE("polygon clip_against_plane 3d case 1") {
	shape::Polygon p({{-3,0,0},{0,0,2},{0,-3,0}});
	geometry::Vector4d normal(-1,-2.15,-2.15);
	geometry::Vector4d point(0,0,-1);


	shape::Polygon expected({{-3,0,0},{-387.0/146,0,17.0/73},{0,-1.8,0.8},
			{0,-3,0}});

	auto ans = shape::clip_against_plane(p,normal,point);

	REQUIRE(ans == expected);
}

TEST_CASE("get_baricentric_coordinates") {
	shape::Polygon triangle({{3,2,0},{6,5,0},{2,4,0}});
	geometry::Vector4d p(3,3,0);

	auto ans = shape::get_baricentric_coordinates(p,triangle);
	geometry::Vector4d expected(5.0/9,1.0/9,1.0/3);

	REQUIRE(ans==expected);
}

TEST_CASE("triangulate_polygon empty") {
	shape::Polygon p({});
	auto ans = shape::triangulate_polygon(p);
	std::vector<shape::Polygon> v;

	REQUIRE(ans==v);
}

TEST_CASE("get_baricentric_coordinates points outside") {
	shape::Polygon triangle({{3,2,1.142857},{6,5,1.142857},{2,4,1.142857}});
	geometry::Vector4d p(6,-2,1.142857);

	auto ans = shape::get_baricentric_coordinates(p,triangle);
	geometry::Vector4d expected(28.0/9,2.0/9,-7.0/3);

	REQUIRE(ans==expected);
}
