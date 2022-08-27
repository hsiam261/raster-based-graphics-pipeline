#include "camera.hpp"

camera::Camera::Camera() : r(1,0,0,0), u(0,1,0,0), l(0,0,-1,0), 
	position(0,0,0), point_to_look_at(0,0,-1) {}
camera::Camera::Camera(const geometry::Vector4d& pos) : Camera() {
	set_position(pos);
}

camera::Camera::Camera(const geometry::Vector4d& pos, 
		const geometry::Vector4d& point_to_look_at,
		const geometry::Vector4d& up_direction) {
	set_position(pos);
	look_at(point_to_look_at, up_direction);
}

void camera::Camera::set_position(const geometry::Vector4d &pos) {
	auto homogenized_pos = geometry::homogenize(pos);
	auto d = homogenized_pos-this->position; 
	this->position = homogenized_pos;
	this->point_to_look_at+=d;
}

void camera::Camera::look_at(const geometry::Vector4d &point_to_look_at, 
		const geometry::Vector4d &up_direction) {
	this->point_to_look_at = geometry::homogenize(point_to_look_at);

	l = this->point_to_look_at-this->position;
	u = up_direction;	
	l = geometry::normalize3d(l);
	r = geometry::normalize3d(geometry::cross(l,u));
	u = geometry::cross(r,l);
}

//void camera::Camera::set_position_with_target_lock(
//		const geometry::Vector4d &pos) {
//		
//}

geometry::Matrix4d camera::Camera::get_camera_transform() {
	geometry::Matrix4d ans = geometry::translate(-this->position[0],
			-this->position[1],-this->position[2]);
	ans = geometry::Matrix4d({r[0],r[1],r[2],0,
			u[0],u[1],u[2],0,
			-l[0],-l[1],-l[2],0,
			0,0,0,1})*ans;
	return ans;
}
