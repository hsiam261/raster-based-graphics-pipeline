#include "geometry.hpp"
#include <vector>
#include <catch2/catch.hpp>

std::vector<double> containerize(double* d,int size) {
	std::vector<double> v(size);
	for(int i=0;i<size;i++) v[i]=d[i];
	return v;
}

TEST_CASE("bare matrix add") {
	double mat1[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
	double mat2[12] = {0,-1,2,-3,4,-5,6,-7,8,-9,10,-11};

	double ans[12];
	double expected[12] = {0,0,4,0,8,0,12,0,16,0,20,0};

	geometry::add(mat1,mat2,ans,4,3);
	REQUIRE_THAT(containerize(ans,12),
			Catch::Matchers::Approx(containerize(ans,12)));
}

TEST_CASE("bare matrix sub") {
	double mat1[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
	double mat2[12] = {0,-1,2,-3,4,-5,6,-7,8,-9,10,-11};

	double ans[12];
	double expected[12] = {0,2,0,6,0,10,0,14,0,18,0,22};

	geometry::sub(mat1,mat2,ans,4,3);
	REQUIRE_THAT(containerize(ans,12),
			Catch::Matchers::Approx(containerize(ans,12)));
}

TEST_CASE("bare matrix mul") {
	double mat1[12] = {17,21,0.71,
		0.3,7,6};
	double mat2[12] = {6,8,12,21,
		0.71,0.6,0.8,
		3.71,1,2,3,4};

	double ans[12];
	double expected[12] = {117.62,150.02,222.93,437.75,
		12.77,18.6,27.2,56.27}; 

	geometry::mul(mat1,mat2,ans,2,3,4);
	REQUIRE_THAT(containerize(ans,8),
			Catch::Matchers::Approx(containerize(expected,8)));
	
	//this should fail
	//CHECK_THAT(containerize(ans,8),
	//		Catch::Matchers::Equals(containerize(expected,8)));


} 

TEST_CASE("bare matrix scaling") {
	double mat1[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

	double ans[12];
	double expected[12] = {0,-2,-4,-6,-8,-10,-12,-14,-16,-18,-20,-22};

	geometry::scale(-2.0,mat1,ans,4,3);
	REQUIRE_THAT(containerize(ans,12),
			Catch::Matchers::Approx(containerize(expected,12)));
}

TEST_CASE("bare matrix transpose") {
	double mat[12] = {0,1,2,3,
		4,5,6,7,
		8,9,10,11};

	double ans[12];
	double expected[12] = {0,4,8,
		1,5,9,
		2,6,10,
		3,7,11
	};

	geometry::transpose(mat,ans,3,4);
	REQUIRE_THAT(containerize(ans,12),
			Catch::Matchers::Approx(containerize(expected,12)));



}

TEST_CASE("vector construction") {
	geometry::Vector4d v(5,4,3,2);
	CHECK(v[0]==Approx(5));
	CHECK(v[1]==Approx(4));
	CHECK(v[2]==Approx(3)); 
	CHECK(v[3]==Approx(2));

	geometry::Vector4d v2(v);
	REQUIRE(v==v2);

	v2[0]=4.99;
	REQUIRE_FALSE(v==v2);
	//REQUIRE(v==v2);
}

TEST_CASE("vector addition") {
	geometry::Vector4d v1(1,2,3,4);
	geometry::Vector4d v2(1,12,-3,4);

	auto out  = v1+v2;
	geometry::Vector4d expected(2,14,0,8);

	REQUIRE(out==expected);
}


TEST_CASE("vector subtraction") {
	geometry::Vector4d v1(1,2,3,4);
	geometry::Vector4d v2(1,12,-3,4);

	auto out  = v1-v2;
	geometry::Vector4d expected(0,-10,6,0);

	REQUIRE(out==expected);
}


TEST_CASE("vector scaling") {
	geometry::Vector4d v(1,2,3,4);
	auto out = v*(-3.0/2);

	geometry::Vector4d expected(-3.0/2,-3,-9.0/2,-6);

	REQUIRE(out==expected);

	out = -3.0/2*v;
	REQUIRE(out==expected);
}


TEST_CASE("vector equality and inequality") {
	geometry::Vector4d v1(1,2,3,4);
	geometry::Vector4d v2(0.9999999,2.000000001,3,4);
	geometry::Vector4d v3(0.9999,2.000000001,3,4);
	geometry::Vector4d v4 (NAN,2,3,4);
	REQUIRE(v1==v2);
	REQUIRE(v1!=v3);
	REQUIRE_FALSE(v4==v1);
	REQUIRE(v1!=v4);
}

TEST_CASE("other assignments") {
	geometry::Vector4d v(1,2,3,4);
	v+=v;

	REQUIRE(v == geometry::Vector4d(2,4,6,8));

	v*=3;
	REQUIRE(v==geometry::Vector4d(6,12,18,24));

	v-=v;
	REQUIRE(v==geometry::Vector4d(0,0,0,0));
}


TEST_CASE("vector dot product") {
	geometry::Vector4d v1(3,7,9,4);
	geometry::Vector4d v2(8,7,2,-3);
	auto out = geometry::dot(v1,v2);
	double expected = 24+49+18-12;
	REQUIRE(out == Approx(expected));
	double out2 = geometry::dot3d(v1,v2);
	double expected2 = (24+49+18);
	REQUIRE(out2 == Approx(expected2));
}

TEST_CASE("vector cross product") {
	geometry::Vector4d v1(3,7,9,4);
	geometry::Vector4d v2(8,7,2,-3);

	auto out = geometry::cross(v1,v2);

	geometry::Vector4d expected1(-49,66,-35,-12);
	geometry::Vector4d expected2(-49.0/-12,66.0/-12,-35.0/-12,1);

	REQUIRE(out == expected1);
	REQUIRE(homogenize(out) == expected2);
}

TEST_CASE("vector homogenize") {
	geometry::Vector4d v(-49,66,-35,-12);
	auto out = geometry::homogenize(v);
	geometry::Vector4d expected(-49.0/-12,66.0/-12,-35.0/-12,1);
	REQUIRE(out == expected);

}


TEST_CASE("vector normalize3d") {
	geometry::Vector4d v(-21,3,-6,0);
	auto out = geometry::normalize3d(v);
	geometry::Vector4d expected(-0.952579, 0.136083, -0.272166,0);
	REQUIRE(out == expected);

}

TEST_CASE("matrix construction") {
	geometry::Matrix4d mat({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16});
	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++) CHECK(mat[i][j]==i*4+j+1);
	}

	geometry::Matrix4d mat2(mat);
	CHECK(mat == mat2);
	
}


TEST_CASE("matrix assignment") {
	geometry::Matrix4d mat1({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16});
	geometry::Matrix4d mat2({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0});
	auto mat3 = mat2;

	mat2 = mat1;
	REQUIRE(mat2 == mat1);
	REQUIRE_FALSE(mat1 == mat3);
	REQUIRE_FALSE(mat3 == mat1);
	REQUIRE(mat1 == geometry::Matrix4d({1,2,3,4,
				5,6,7,8,
				9,10,11,12,
				13,14,15,16}));
}

TEST_CASE("matrix addition") {
	geometry::Matrix4d mat1({1,2,3,4,
			5,6,7,8,
			9,10,11,12,
			13,14,15,16});
	geometry::Matrix4d mat2({1,-2,3,-4,
			5,-6,7,-8,
			9,-10,11,-12,
			13,-14,15,-16});

	geometry::Matrix4d expected({2,0,6,0,
			10,0,14,0,
			18,0,22,0,
			26,0,30,0});

	auto out = mat1+mat2;

	REQUIRE(out == expected);
}


TEST_CASE("matrix subtraction") {
	geometry::Matrix4d mat1({1,2,3,4,
			5,6,7,8,
			9,10,11,12,
			13,14,15,16});
	geometry::Matrix4d mat2({1,-2,3,-4,
			5,-6,7,-8,
			9,-10,11,-12,
			13,-14,15,-16});

	geometry::Matrix4d expected({0,4,0,8,
			0,12,0,16,
			0,20,0,24,
			0,28,0,32});

	auto out = mat1-mat2;

	REQUIRE(out == expected);
}

TEST_CASE("matrix scaling") {
	geometry::Matrix4d mat1({1,2,3,4,
			5,6,7,8,
			9,10,11,12,
			13,14,15,16});
	geometry::Matrix4d expected({-0.5,-1,-1.5,-2,
			-2.5,-3,-3.5,-4,
			-4.5,-5,-5.5,-6,
			-6.5,-7,-7.5,-8});

	auto out = mat1 * -0.5;
	REQUIRE(out == expected);
	
	out = -0.5*mat1;
	REQUIRE(out == expected);
}


TEST_CASE("matrix multiplication") {
	geometry::Matrix4d mat1({0.873037, 0.200931, 0.842818, 0.716217,
			0.192971, 0.403154, 0.951343, 0.394696, 
			0.953117, 0.00553063, 0.39256, 0.106504, 
			0.942903, 0.912815, 0.775577, 0.436921});
	geometry::Matrix4d mat2({0.78307, 0.701203, 0.187429, 0.453141, 
			0.881114, 0.994986, 0.988278, 0.890502, 
			0.0219716, 0.262237, 0.439863, 0.289656,
			0.932956, 0.137329, 0.652143, 0.978218});
	
	auto out1 = mat1*mat2;
	auto out2 = mat2*mat1;	
	
	geometry::Matrix4d expected1({1.54741, 1.13148, 1.20001, 1.51928, 
			0.895471, 0.840125, 1.11046, 1.10811, 
			0.859219, 0.791401, 0.426236, 0.654713, 
			1.96732, 1.83279, 1.70492, 1.89219});

	geometry::Matrix4d expected2({1.42487, 0.854706, 1.75209, 1.05556, 
			2.74285, 1.39651, 2.7678, 1.51812, 
			0.762145, 0.376972, 0.665319, 0.292644, 
			2.38494, 1.13936, 1.93165, 1.21926});

	REQUIRE(out1==expected1);
	REQUIRE(out2==expected2);
}


TEST_CASE("matrix vector multiplication") {
	geometry::Matrix4d mat({0.78307, 0.701203, 0.187429, 0.453141, 
			0.881114, 0.994986, 0.988278, 0.890502, 
			0.0219716, 0.262237, 0.439863, 0.289656,
			0.932956, 0.137329, 0.652143, 0.978218});
	
	
	geometry::Vector4d v(0.282505,0.972827,0.217877,0.0803125);

	auto out = mat*v;

	geometry::Vector4d expected(0.9806,1.50371,0.380417,0.617812);

	REQUIRE(out==expected);
}

TEST_CASE("matrix equality and inequality") {
	geometry::Matrix4d mat1({0.873037, 0.200931, 0.842818, 0.716217,
			0.192971, 0.403154, 0.951343, 0.394696, 
			0.953117, 0.00553063, 0.39256, 0.106504, 
			0.942903, 0.912815, 0.775577, 0.436921});
	geometry::Matrix4d mat2({0.873007, 0.200931, 0.842818, 0.716217,
			0.192971, 0.403154, 0.951343, 0.394696, 
			0.953117, 0.00553063, 0.39256, 0.106504, 
			0.942903, 0.912815, 0.775577, 0.436921});
	geometry::Matrix4d mat3({0.873037, 0.200931, 0.842818, 0.716217,
			0.192971, 0.403154, 0.951343, 0.394696, 
			0.953117, 0.00553063, 0.39256, 0.106504, 
			0.942903, 0.912815, 0.775577, 0.436921});
	geometry::Matrix4d mat4({NAN, 0.200931, 0.842818, 0.716217,
			0.192971, 0.403154, 0.951343, 0.394696, 
			0.953117, 0.00553063, 0.39256, 0.106504, 
			0.942903, 0.912815, 0.775577, 0.436921});



	REQUIRE_FALSE(mat1==mat2);
	REQUIRE(mat1!=mat2);
	REQUIRE(mat1==mat3);
	REQUIRE(mat3!=mat4);
}

TEST_CASE("matrix other assignments") {
	auto mat1 = geometry::identity();
	auto mat2 = mat1;
	mat1+=mat1;
	REQUIRE(mat1 == 2*geometry::identity());
	
	mat2-=mat1;
	mat2*=mat2;
	REQUIRE(mat2==geometry::identity());

	auto mat3 = geometry::identity();
	mat3*=4;
	REQUIRE(mat3==geometry::Matrix4d({4,0,0,0,
				0,4,0,0,
				0,0,4,0,
				0,0,0,4}));
}

TEST_CASE("matrix transpose") {
	geometry::Matrix4d m({1,2,3,4,
			5,6,7,8,
			9,10,11,12,
			13,14,15,16});
	auto out = geometry::transpose(m);
	geometry::Matrix4d expected({1,5,9,13,
			2,6,10,14,
			3,7,11,15,
			4,8,12,16});
	REQUIRE(out==expected);
}

TEST_CASE("vector rotation") {
	geometry::Vector4d axis(7,1,2);
	double angle_in_radian = 15;
	auto out = geometry::rotate(angle_in_radian,axis);
	auto expected = geometry::Matrix4d({0.8370659,0.0511218,0.5447084,0,
			0.4050936, -0.7271011, -0.5542771, 0,
			0.3677224,  0.6846244, -0.6293406, 0,
			0,0,0,1});
	REQUIRE(out == expected);
}


