#include <canvas.hpp>
#include <ostream>
#include <vector>

canvas::Canvas::Canvas() {
	this->matrix_stack.push(geometry::identity());
}

void canvas::Canvas::push(const geometry::Matrix4d &mat) {
	auto t = matrix_stack.top();
	t*=mat;
	matrix_stack.push(t);
}


void canvas::Canvas::pop() {
	if(matrix_stack.size()!=1) matrix_stack.pop();
}

void canvas::Canvas::update_transform(const geometry::Matrix4d &mat) {
	matrix_stack.top()*=mat;
}

unsigned canvas::Canvas::size() const{
	return shapes.size();
}

void canvas::Canvas::add_shape(const shape::Polygon &p) {
	shapes.push_back(shape::apply_transform(p,matrix_stack.top()));
}

shape::Polygon&  canvas::Canvas::operator[](int idx) {
	return shapes[idx];
}

const shape::Polygon&  canvas::Canvas::operator[](int idx) const {
	return shapes[idx];
}

std::vector<shape::Polygon> canvas::Canvas::get_display_list() const {
	return shapes;
}

std::ostream& canvas::operator<<(std::ostream &os, const Canvas &canvas) {
	os << "{\n";
	for(int i=0;i<canvas.size();i++) os << canvas[i] << "\n";
	os << "}";
	return os;
}



