#include <stack>
#include <vector>
#include <geometry.hpp>
#include <shapes.hpp>
#include <string>
#include <ostream>

#ifndef CANVAS
#define CANVAS 

namespace canvas {
	class Canvas {
		std::stack<geometry::Matrix4d> matrix_stack;
		std::vector<shape::Polygon> shapes;
		public:
		Canvas();
		void push(const geometry::Matrix4d& mat);
		void pop();
		void update_transform(const geometry::Matrix4d& mat);

		void add_shape(const shape::Polygon& p);

		unsigned size() const;
		shape::Polygon& operator[](int idx);
		const shape::Polygon& operator[](int idx) const;
		

		std::vector<shape::Polygon> get_display_list() const;
	};

	std::ostream& operator<<(std::ostream& os,const Canvas& canvas);

}

#endif
