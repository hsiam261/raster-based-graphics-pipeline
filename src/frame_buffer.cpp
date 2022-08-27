#include "bitmap_image.hpp"
#include <frame_buffer.hpp>
#include <iostream>
void frame_buffer::save_as_bmp(const Frame_Buffer<color::rgb_24_bit> &buffer, 
		std::string filename) {
	bitmap_image image(buffer.get_width(),buffer.get_height());
	for(int i=0;i<buffer.get_width();i++) {
		for(int j=0;j<buffer.get_height();j++) {
			auto c = buffer.get_pixel(i,j);
			image.set_pixel(i,buffer.get_height()
					-1-j,c.red,c.green,c.blue);
		}
	}
	
	image.save_image(filename);
}
