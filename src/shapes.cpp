#include <ostream>
#include <shapes.hpp>
#include <string>
#include <strstream>


shape::Polygon::Polygon(const std::vector<geometry::Vector4d> & vertices) :
	number_of_vertices(vertices.size()), vertices(vertices) {}

shape::Polygon::Polygon() {}
		
shape::Polygon::Polygon(int number_of_vertices) : 
	number_of_vertices(number_of_vertices), 
	vertices(number_of_vertices) {}
		
unsigned shape::Polygon::size() const {
	return number_of_vertices;
}

geometry::Vector4d& shape::Polygon::operator[](int idx) {
	return vertices[idx];
}	

const geometry::Vector4d& shape::Polygon::operator[](int idx) const {
	return vertices[idx];
}	

shape::Polygon shape::apply_transform(const Polygon &shape,
		const geometry::Matrix4d &mat) {
	Polygon ans = shape;
	for(int i=0;i<ans.size();i++) {
		ans[i] = mat*ans[i];
	}
	ans.set_color(shape.get_color());
	return ans;
}

shape::Polygon shape::homogenize(const Polygon &shape) {
	Polygon ans = shape;
	for(int i=0;i<ans.size();i++) {
		ans[i] = geometry::homogenize(ans[i]);
	}

	ans.set_color(shape.get_color());
	return ans;
}
bool shape::Polygon::operator==(const Polygon &s) const {
	if(number_of_vertices!=s.number_of_vertices) return false;
	for(int i=0;i<number_of_vertices;i++) {
		if(vertices[i]!=s.vertices[i]) return false;
	}

	return true;
}

bool shape::Polygon::operator!=(const Polygon &s) const {
	return !(*this==s);
}

std::string shape::to_string(const Polygon &p)  {
	std::stringstream ss;
	ss << "(\n";
	for(int i=0;i<p.size();i++) {
		ss << p[i] << "\n";
	}
	ss << ")";
	return ss.str();
}	

std::ostream& shape::operator<<(std::ostream &os, const Polygon &p) {
	os << "(\n";
	for(int i=0;i<p.size();i++) {
		os << p[i] << "\n";
	}
	os << ")";
	return os;
}


std::vector<shape::Polygon> shape::triangulate_polygon(const Polygon &p) {
	std::vector<Polygon> v;
	for(int i=1;i<((long long)p.size())-1;i++) {
		auto temp = Polygon({p[0],p[i],p[i+1]});
		temp.set_color(p.get_color());
		v.push_back(temp);
	}

	return v;
}



shape::Polygon shape::clip_against_plane(const Polygon &shape, 
		geometry::Vector4d normal, 
		geometry::Vector4d point) {
	normal = geometry::normalize3d(normal);

	std::vector<geometry::Vector4d> vertices;
	if(shape.size()<3) return vertices;	
	double D = geometry::dot3d(normal,point);
	double dot_prev = geometry::dot3d(normal,shape[0]);
	auto prev = shape[0];

	if(dot_prev>=D) {
		vertices.push_back(shape[0]);
	}
	


	for(int i=1;i<=shape.size();i++) {
		auto curr = shape[i%shape.size()];
		double dot_curr = geometry::dot3d(normal,curr);

		if(dot_curr>=D) {
			if(dot_curr!=D && dot_prev<D) {
				auto t = (dot_prev-D)/(dot_prev-dot_curr);
				auto temp = prev + t*(curr-prev);
				vertices.push_back(temp);
			}

			if(i!=shape.size()) vertices.push_back(curr);
		} else {
			if(dot_prev>D) {
				auto t = (dot_prev-D)/(dot_prev-dot_curr);
				auto temp = prev + t*(curr-prev);
				vertices.push_back(temp);
			}
		}

		prev = curr;
		dot_prev = dot_curr;
	}

	auto ans =  Polygon(vertices);
	ans.set_color(shape.get_color());

	return ans;
}


geometry::Vector4d shape::get_baricentric_coordinates(
		const geometry::Vector4d &p, 
		const shape::Polygon &triangle) {
	geometry::Vector4d bary;
	
	auto normal = geometry::cross(triangle[1]-triangle[0],
			triangle[2]-triangle[0]);

	
	// The area of a triangle is 
	double areaABC = geometry::dot3d(normal,normal);
	double areaPBC = geometry::dot3d(normal,geometry::cross(
				triangle[1]-p,triangle[2]-p));
	double areaPCA = geometry::dot3d(normal,geometry::cross(
				triangle[2]-p,triangle[0]-p));
	
	bary[0] = areaPBC/areaABC; // alpha
	bary[1] = areaPCA/areaABC; // beta
	bary[2] = 1.0f - bary[0] - bary[1]; // gamma
	
	return bary ;	
}
