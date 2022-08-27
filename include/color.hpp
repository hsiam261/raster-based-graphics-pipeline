#include <ostream>
#include <string>

#ifndef COLOR 
#define COLOR

namespace color{
	class rgb_24_bit {
		public:
		int red,green,blue;
		
		rgb_24_bit(int r=0,int g=0,int b=0) : red(r),
		green(g), blue(b) {}
	};

	std::string to_string(const rgb_24_bit& color);	
	std::ostream& operator<<(std::ostream& os,const rgb_24_bit& color); 
		

}


#endif
