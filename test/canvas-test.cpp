#include <canvas.hpp>
#include <catch2/catch.hpp>
#include <iostream>
#include <vector>

TEST_CASE("canvas test") {
	canvas::Canvas canvas;
	shape::Polygon triangle({{0,0,0},{1,0,0},{0,1,0}});

	canvas.add_shape(triangle);
	canvas.push(geometry::rotate(3.14159/2,{0,0,1,0}));
	canvas.add_shape(triangle);
	
	canvas.update_transform(geometry::rotate(3.14159/2,{0,0,1,0}));
	canvas.add_shape(triangle);
	
	canvas.pop();
	canvas.add_shape(triangle);
	
	std::vector<shape::Polygon> expected;
	expected.push_back(triangle);
	expected.push_back(shape::apply_transform(triangle,
				geometry::rotate(3.14159/2,{0,0,1,0})));
	expected.push_back(shape::apply_transform(triangle,
				geometry::rotate(3.14159,{0,0,1,0})));
	expected.push_back(triangle);
	
	REQUIRE(canvas.get_display_list() == expected);
	REQUIRE_FALSE(canvas.get_display_list() != expected);
}
