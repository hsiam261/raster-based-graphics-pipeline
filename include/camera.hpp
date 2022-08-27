#include "geometry.hpp"
#ifndef CAMERA 
#define CAMERA

namespace camera {
	class Camera {
		geometry::Vector4d u,r,l;
		geometry::Vector4d position;
		geometry::Vector4d point_to_look_at;


		public:
		
		Camera();
		Camera(const geometry::Vector4d& pos);
		Camera(const geometry::Vector4d& pos,
				const geometry::Vector4d& point_to_look_at,
				const geometry::Vector4d& up_direction);
	
		void set_position(const geometry::Vector4d& pos);
		void look_at(const geometry::Vector4d& point_to_look_at,
				const geometry::Vector4d& up_direction);
		
		void set_position_with_target_lock(const geometry::Vector4d& pos);
	
		geometry::Matrix4d get_camera_transform();
	
	};
}
#endif
