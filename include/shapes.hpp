#include <geometry.hpp>
#include <color.hpp>
#include <ostream>
#include <sstream>
#include <vector>
#include <string>
#include <strstream>

#ifndef SHAPE
#define SHAPE

namespace shape {

	class Polygon {
		int number_of_vertices;
		color::rgb_24_bit c;
		std::vector<geometry::Vector4d> vertices;
		public:
		Polygon(const std::vector<geometry::Vector4d> & vertices);
		Polygon();
		
		Polygon(int number_of_vertices);		
		unsigned size() const; 
		geometry::Vector4d& operator[](int idx);
		const geometry::Vector4d& operator[](int idx) const;

		bool operator==(const Polygon& s) const;
		bool operator!=(const Polygon& s) const;
		void set_color(const color::rgb_24_bit& c) {
			this->c = c;
		}
		color::rgb_24_bit get_color() const {
			return this->c;
		}
	};

	Polygon apply_transform(const Polygon& shape,
			const geometry::Matrix4d& mat);
	
	Polygon homogenize(const Polygon& shape);

	Polygon clip_against_plane(const Polygon& shape, 
			geometry::Vector4d normal, 
			geometry::Vector4d point);

	std::string to_string(const Polygon& p);

	std::ostream& operator<<(std::ostream& os,const Polygon& p);
	
	geometry::Vector4d get_baricentric_coordinates(
			const geometry::Vector4d& p,
			const Polygon& triangle);

	std::vector<Polygon> triangulate_polygon(const Polygon& p);
}


#endif 
