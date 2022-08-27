#include "camera.hpp"
#include <catch2/catch.hpp>

TEST_CASE("camera initialization") {
	auto camera1 = camera::Camera();
	CHECK(camera1.get_camera_transform() == geometry::Matrix4d({1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			}));

	auto camera2 = camera::Camera(geometry::Vector4d(7,9,8,-3));
	CHECK(camera2.get_camera_transform() == 
			geometry::Matrix4d({1,0,0,7.0/3,
				0,1,0,9.0/3,
				0,0,1,8.0/3,
				0,0,0,1
			}));

	auto camera3 = camera::Camera(geometry::Vector4d(0,0,50,1),
			geometry::Vector4d(0,0,0,1),
			geometry::Vector4d(0,1,0,0));

	CHECK(camera3.get_camera_transform() == 
			geometry::Matrix4d({1,0,0,0,
				0,1,0,0,
				0,0,1,-50,
				0,0,0,1
			}));
	
	auto camera4 = camera::Camera(geometry::Vector4d(-7,12,-4),
			geometry::Vector4d(31,12,21),
			geometry::Vector4d(6,1,2,0));

	auto expected = geometry::Matrix4d({-0.287813,0.851926,0.437476,-10.4879,
			0.468233,0.523661,-0.711714,-5.85316,
			-0.835417,0,-0.549617,-8.04639,
			0,0,0,1});
	CHECK(camera4.get_camera_transform()==expected);

	auto camera5 = camera::Camera(geometry::Vector4d(-13,2,21),
			geometry::Vector4d(0.314,36,77),
			geometry::Vector4d(0.617,2.1415,3.76104,0));

	expected = geometry::Matrix4d({0.418532,-0.81705,0.39656,-1.2527445,
			-0.886097,-0.271613,0.375577,-18.8631616,
			-0.199155,-0.508582,-0.837664,16.0191062,
			0,0,0,1});
	CHECK(camera5.get_camera_transform()==expected);
}

TEST_CASE("set camera") {
	auto c = camera::Camera();
	c.set_position(geometry::Vector4d(0,0,50));

	auto expected = geometry::Matrix4d({1,0,0,0,
			0,1,0,0,
			0,0,1,-50,
			0,0,0,1});

	REQUIRE(c.get_camera_transform() == expected);
}
TEST_CASE("look_at") {
	auto camera = camera::Camera({-7,12,-4});
	camera.look_at({31,12,21},{6,1,2,0});

	auto expected = geometry::Matrix4d({
			-0.287813,0.851926,0.437476,-10.4879,
			0.468233,0.523661,-0.711714,-5.85316,
			-0.835417,0,-0.549617,-8.04639,
			0,0,0,1});
	CHECK(camera.get_camera_transform()==expected);

}
