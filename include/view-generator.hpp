#include <camera.hpp>
#include <canvas.hpp>
#include <shapes.hpp>
#include <vector>

#ifndef VIEW_GENERATOR
#define VIEW_GENERATOR

namespace view_generator {
	class View_generator {
		public:
		camera::Camera camera;
		double field_of_view_y;
		double aspect;
		double near;
		double far;
		
		View_generator(camera::Camera camera,
				double field_of_view_y, double aspect,
				double near,double far) {
			this->camera = camera;
			this->field_of_view_y = field_of_view_y;
			this->aspect = aspect;
			this->near = near;
			this->far = far;
		}

		std::vector<geometry::Vector4d> generate_view(
				const canvas::Canvas& canvas);
	};
}



#endif
