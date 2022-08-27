#include <projector.hpp>
#include <cmath>

geometry::Matrix4d projector::get_canonical_view_transform(
		double field_of_view_y,double aspect, double near, double far) {
	double f = 1/std::tan(field_of_view_y/2);
	return geometry::Matrix4d({f/aspect,0,0,0,
			0,f,0,0,
			0,0,(near+far)/(near-far),2*near*far/(near-far),
			0,0,-1,0
		});
}

