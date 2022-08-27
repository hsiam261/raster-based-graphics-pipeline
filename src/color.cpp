#include <color.hpp>
#include <ostream>

std::ostream& color::operator<<(std::ostream &os, const rgb_24_bit &color) {
	os << color::to_string(color);
	return  os;

}

std::string color::to_string(const rgb_24_bit& color) {
	return "(" + std::to_string(color.red) + ", " 
		+ std::to_string(color.green) + ", "
		+ std::to_string(color.blue) + ") ";
}
