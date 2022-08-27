#include <rasterizer.hpp>
#include <catch2/catch.hpp>
#include <vector>
#include <color.hpp>
#include <frame_buffer.hpp>

struct dummy_frame_buffer {
	dummy_frame_buffer(int width,int height) {}
	void set_pixel(int x,int y,color::rgb_24_bit) {}
};

TEST_CASE("rasterizer draw triangle with horizontal base at bottom") {
	rastorizer::Zbuffer_rasterizor<frame_buffer::Frame_Buffer<
		color::rgb_24_bit>> rastorizer
		(13,13,0,13,0,13,13);
	rastorizer.rasterize_triangle_helper_bottom({7,7,2},
			{4,4,2}, {8,4,2});

	std::vector<std::vector<double>> expected(13,
			std::vector<double>(13,13.0));

	expected[6][6] = 2;
	expected[5][5] = 2;
	expected[5][6] = 2;
	expected[4][4] = 2;
	expected[4][5] = 2;
	expected[4][6] = 2;
	expected[4][7] = 2;
	


	REQUIRE(rastorizer.get_zbuffer()==expected);

}

TEST_CASE("rasterizer draw triangle with horizontal base at top") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(13,13,0,13,0,13,13);
	rastorizer.rasterize_triangle_helper_top({4,7,2},
			{8,7,2}, {7,4,2});

	std::vector<std::vector<double>> expected(13,
			std::vector<double>(13,13.0));

	expected[4][6] = 2;
	expected[5][5] = 2;
	expected[5][6] = 2;
	expected[6][4] = 2;
	expected[6][5] = 2;
	expected[6][6] = 2;
	expected[6][7] = 2;

	REQUIRE(rastorizer.get_zbuffer()==expected);

}

TEST_CASE("rasterizer draw not draw bottom row 1") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(13,13,0,13,0,13,13);
	rastorizer.rasterize_triangle(shape::Polygon({{7,7,2},
			{4,4.5,2}, {8,4.5,2}}));

	std::vector<std::vector<double>> expected(13,
			std::vector<double>(13,13.0));

	expected[6][6] = 2;
	expected[5][5] = 2;
	expected[5][6] = 2;
	expected[5][7] = 2;
	


	REQUIRE(rastorizer.get_zbuffer()==expected);

}

TEST_CASE("rasterizer draw not draw bottom row 2") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(13,13,0,13,0,13,13);
	rastorizer.rasterize_triangle(shape::Polygon({{7.5,4.5,2},
			{4,7,2}, {8,7,2}}));

	std::vector<std::vector<double>> expected(13,
			std::vector<double>(13,13.0));

	expected[5][6] = 2;
	expected[5][7] = 2;
	expected[6][5] = 2;
	expected[6][6] = 2;
	expected[6][7] = 2;




	REQUIRE(rastorizer.get_zbuffer()==expected);

}


TEST_CASE("rasterizer triangle horizontal base") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(13,13,0,13,0,13,13);
	rastorizer.rasterize_triangle(shape::Polygon({{4,7,2},
			{8,7,2}, {7,4,2}}));

	std::vector<std::vector<double>> expected(13,
			std::vector<double>(13,13.0));

	expected[4][6] = 2;
	expected[5][5] = 2;
	expected[5][6] = 2;
	expected[6][4] = 2;
	expected[6][5] = 2;
	expected[6][6] = 2;
	expected[6][7] = 2;

	REQUIRE(rastorizer.get_zbuffer()==expected);

	rastorizer.rasterize_triangle(shape::Polygon({{7,7,1},
			{4,4,1}, {8,4,1}}));


	expected[6][6] = 1;
	expected[5][5] = 1;
	expected[5][6] = 1;
	expected[4][4] = 1;
	expected[4][5] = 1;
	expected[4][6] = 1;
	expected[4][7] = 1;


	REQUIRE(rastorizer.get_zbuffer()==expected);
}



TEST_CASE("rasterizer triangle non horizontal base") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(8,7,-3,5,-2,5,20);
	rastorizer.rasterize_triangle(shape::Polygon({{-2,4,6},
			{-1,-1,6}, {4,1,6}}));

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	expected[1][2] = 6;

	expected[2][2] = 6;
	expected[2][3] = 6;
	expected[2][4] = 6;
	expected[2][5] = 6;

	expected[3][1] = 6;
	expected[3][2] = 6;
	expected[3][3] = 6;
	expected[3][4] = 6;
	expected[3][5] = 6;

	expected[4][1] = 6;
	expected[4][2] = 6;
	expected[4][3] = 6;

	expected[5][1] = 6;
	
	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}


TEST_CASE("rasterizer triangle with varying z values") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(8,7,-3,5,-2,5,20);
	rastorizer.rasterize_triangle(shape::Polygon({{-2,4,4},
			{-1,-1,-1}, {4,1,1}}));

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	expected[1][2] = -0.5;

	expected[2][2] = 0.5;
	expected[2][3] = 0.5;
	expected[2][4] = 0.5;
	expected[2][5] = 0.5;

	expected[3][1] = 1.5;
	expected[3][2] = 1.5;
	expected[3][3] = 1.5;
	expected[3][4] = 1.5;
	expected[3][5] = 1.5;

	expected[4][1] = 2.5;
	expected[4][2] = 2.5;
	expected[4][3] = 2.5;

	expected[5][1] = 3.5;
	
	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}




TEST_CASE("rasterizer triangle with varying z values 2") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(8,7,-3,5,-2,5,20);
	rastorizer.rasterize_triangle(shape::Polygon({{2,0,2},
			{-2,2,-2}, {2,4,2}}));

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	expected[2][4] = 1.5;

	expected[3][2] = -0.5;
	expected[3][3] = 0.5;
	expected[3][4] = 1.5;

	expected[4][2] = -0.5;
	expected[4][3] = 0.5;
	expected[4][4] = 1.5;

	expected[5][4] = 1.5;
	
	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}

TEST_CASE("rasterizer degenerate triangle 1") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(8,7,-3,5,-2,5,20);
	rastorizer.rasterize_triangle(shape::Polygon({{2,2,2},
			{2,2,2}, {2,2,2}}));

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}

TEST_CASE("rasterizer degenerate triangle 2") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(8,7,-3,5,-2,5,20);
	rastorizer.rasterize_triangle(shape::Polygon({{4,2,2},
			{6,2,2}, {-2,2,2}}));

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}

TEST_CASE("rasterizer degenerate triangle 3") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(20,20,0,20,0,20,20);
	rastorizer.rasterize_triangle(shape::Polygon({{2,2,2},
			{4,2,2}, {8,2,2}}));

	std::vector<std::vector<double>> expected(20,
			std::vector<double>(20,20.0));

	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}


TEST_CASE("rasterizer triangle with clipping (cut top)") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(20,20,0,20,0,20,20);
	rastorizer.rasterize_triangle(shape::Polygon({{4,21.5,2},
			{0.5,17.5,2}, {5.5,17,2}}));

	std::vector<std::vector<double>> expected(20,
			std::vector<double>(20,20.0));

	expected[17][1] = 2;
	expected[17][2] = 2;
	expected[17][3] = 2;
	expected[17][4] = 2;
	
	expected[18][1] = 2;
	expected[18][2] = 2;
	expected[18][3] = 2;
	expected[18][4] = 2;

	expected[19][2] = 2;
	expected[19][3] = 2;
	expected[19][4] = 2;

	rastorizer.rasterize_triangle(shape::Polygon({{4,21.5,1},
			{1,22,1}, {5.5,17,1}}));

	expected[18][4] = 1;
	expected[19][3] = 1;
	expected[19][4] = 1;


	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}



TEST_CASE("rasterizer triangle with clipping (cut bottom)") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(20,20,0,20,0,20,20);
	rastorizer.rasterize_triangle(shape::Polygon({{1.5,-1.5,2},
			{0.5,2,2}, {3.5,3.5,2}}));

	std::vector<std::vector<double>> expected(20,
			std::vector<double>(20,20.0));

	expected[0][1] = 2;
	expected[1][1] = 2;
	expected[1][2] = 2;
	expected[2][1] = 2;
	expected[2][2] = 2;

	rastorizer.rasterize_triangle(shape::Polygon({{2.5,-1,1},
			{0.5,-0.5,1}, {1.5,3,1}}));

	expected[0][1] = 1;
	expected[1][1] = 1;
	expected[2][1] = 1;


	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}

TEST_CASE("rasterizer triangle with clipping (cut left and right)") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(3,3,0,3,0,3,20);
	rastorizer.rasterize_triangle(shape::Polygon({{0.5,7,8},
			{-3,2.5,-3.5}, {5,-1,9}}));

	std::vector<std::vector<double>> expected(3,
			std::vector<double>(3,20.0));

	expected[0][2] = 5.5;
	expected[1][0] = 2.5;
	expected[1][1] = 4.5;
	expected[1][2] = 6.5;
	expected[2][0] = 3.5; 	
	expected[2][1] = 5.5; 	
	expected[2][2] = 7.5; 	

	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}





TEST_CASE("rasterizer intersecting triangles") {
	rastorizer::Zbuffer_rasterizor<dummy_frame_buffer> rastorizer
		(8,7,-3,5,-2,5,20);
	rastorizer.rasterize_triangle(shape::Polygon({{-2,4,4},
			{-1,-1,-1}, {4,1,1}}));

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	expected[1][2] = -0.5;

	expected[2][2] = 0.5;
	expected[2][3] = 0.5;
	expected[2][4] = 0.5;
	expected[2][5] = 0.5;

	expected[3][1] = 1.5;
	expected[3][2] = 1.5;
	expected[3][3] = 1.5;
	expected[3][4] = 1.5;
	expected[3][5] = 1.5;

	expected[4][1] = 2.5;
	expected[4][2] = 2.5;
	expected[4][3] = 2.5;

	expected[5][1] = 3.5;

	rastorizer.rasterize_triangle(shape::Polygon({{2,0,2},
			{-2,2,-2}, {2,4,2}}));

	expected[3][2] = -0.5;
	expected[3][3] = 0.5;

	expected[4][2] = -0.5;
	expected[4][3] = 0.5;
	expected[4][4] = 1.5;

	expected[5][4] = 1.5;

	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
}

TEST_CASE("rasterizer intersecting triangles get_view") {
	rastorizer::Zbuffer_rasterizor<frame_buffer::Frame_Buffer<
		color::rgb_24_bit>> rastorizer(8,7,-3,5,-2,5,20);

	auto poly1 = shape::Polygon({{-2,4,4},{-1,-1,-1}, {4,1,1}});
	poly1.set_color({255,0,0});
	rastorizer.rasterize_triangle(poly1);

	std::vector<std::vector<double>> expected(7,
			std::vector<double>(8,20.0));

	expected[1][2] = -0.5;

	expected[2][2] = 0.5;
	expected[2][3] = 0.5;
	expected[2][4] = 0.5;
	expected[2][5] = 0.5;

	expected[3][1] = 1.5;
	expected[3][2] = 1.5;
	expected[3][3] = 1.5;
	expected[3][4] = 1.5;
	expected[3][5] = 1.5;

	expected[4][1] = 2.5;
	expected[4][2] = 2.5;
	expected[4][3] = 2.5;

	expected[5][1] = 3.5;

	auto poly2 = shape::Polygon({{2,0,2},{-2,2,-2}, {2,4,2}});
	poly2.set_color({0,255,0});
	rastorizer.rasterize_triangle(poly2);

	expected[3][2] = -0.5;
	expected[3][3] = 0.5;

	expected[4][2] = -0.5;
	expected[4][3] = 0.5;
	expected[4][4] = 1.5;

	expected[5][4] = 1.5;

	auto ans = rastorizer.get_zbuffer();
	REQUIRE(ans==expected);
	
	std::cout << "********" << std::endl;
	auto fb = rastorizer.get_frame_buffer();
	frame_buffer::save_as_bmp(fb,"intersecting_triangles.bmp");

}

