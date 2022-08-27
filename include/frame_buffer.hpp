#include <vector>
#include <color.hpp>
#include <string>


#ifndef FRAME_BUFFER
#define FRAME_BUFFER


namespace frame_buffer {
	template<class Color>
	class Frame_Buffer {
		int height;
		int width;
		
		std::vector<std::vector<Color>> buffer;
	
		public:
	
		Frame_Buffer<Color>(int width,int height) : height(height), 
			width(width), 
			buffer(height,std::vector<Color>(height)) {}
		
		int get_width() const {return width;}
		int get_height() const {return height;}
		void set_pixel(int i,int j,const Color& c) {
			buffer[j][i] = c;
		}
	
		Color get_pixel(int i,int j) const { return buffer[j][i];}
	
	};
	
	void save_as_bmp(const Frame_Buffer<color::rgb_24_bit>& buffer,
			std::string filename);
}

#endif 
