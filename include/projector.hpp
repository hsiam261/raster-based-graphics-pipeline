#include "geometry.hpp"


#ifndef  PROJECTOR
#define PROJECTOR
namespace projector {	
	geometry::Matrix4d get_canonical_view_transform(double field_of_view_y,
			double aspect, double near, double far); 
}

#endif
