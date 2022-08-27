#include <iostream>
#include <math.h>
#include <ostream>
#include <sstream>
#include <vector>
#include <geometry.hpp>
#include <shapes.hpp>
#include <cmath>

#ifndef RASTERIZOR
#define RASTERIZOR

namespace rastorizer {
	template<class Frame_Buffer>
	class Zbuffer_rasterizor {
		double minx,maxx;
		double miny,maxy;
		double dx, dy;
		
		double inf;

		int width, height;
		Frame_Buffer buffer;
		std::vector<std::vector<double>> z_buffer;
	
		public:
		Zbuffer_rasterizor(int width,int height,double minx,
				double maxx, double miny,double maxy,
				double inft = 2.0) : 
			width(width), 
			height(height), minx(minx), maxx(maxx), miny(miny), 
			maxy(maxy), buffer(width,height), 
			z_buffer(height,std::vector<double>(width,inft)) {
			dx = (maxx-minx)/width;
			dy = (maxy-miny)/height;
		}
	
		
		int get_row(double y) {return (y-miny)/dy;}
		int get_col(double x) {return (x-minx)/dx;}
		
		bool point_in_triangle(const geometry::Vector4d& p, 
			const shape::Polygon& triangle) {
			auto bary = shape::get_baricentric_coordinates(
					p,triangle);
			for(int i=0;i<3;i++) if(bary[i]>1 || bary[i]<0) 
				return false;
			return true;
		}

		int bari_point_inside_triangle(const geometry::Vector4d bary,
				const shape::Polygon& triangle) {
			int inside = 0;
			int zeros = 0;
			for(int i=0;i<3;i++) {
				if(bary[i]-1>-geometry::EPS || 
						bary[i]<-geometry::EPS) 
					return -1;
				if(fabs(bary[i])<geometry::EPS) zeros++;
			}

			if(zeros) return 0;
			return 1;

		}
	
		void draw_row(const geometry::Vector4d& left, 
				const geometry::Vector4d& right,
				const shape::Polygon& triangle) {

			double dz = (right[2]-left[2])/(right[0]-left[0]);
			int xleft = get_col(left[0]);
			int xright = get_col(right[0]);
	
			int y = get_row(left[1]);
			
			double center_x = minx + dx*xleft + dx/2;
			double center_y = miny + dy*y + dy/2;
			double center_z = left[2] + dz*(center_x-left[0]);
			
			//left and right clip
			if(xleft<0) {
				center_z = center_z + dz*(minx+dx/2-
						center_x);
				xleft = 0;
				center_x = minx+dx/2;
			}

			if(xright>=width) {
				xright = width-1;
			}

			for(int i=xleft;i<=xright;i++) {
				if(z_buffer[y][i]-center_z>
						geometry::EPS) {
					auto bari = 
					shape::get_baricentric_coordinates(
						{center_x,center_y,center_z}, 
						triangle);
					int flag = bari_point_inside_triangle(
							bari,triangle);
					if(flag==1 || flag==0 && (
						i!=xright || (i==xright) &&
						(right[0]!=(xright*dx
						+minx+dx/2)))) {
						z_buffer[y][i] = center_z;
					//	std::cout << i << " " << y << " " << triangle.get_color() << std::endl;
						buffer.set_pixel(i,y,
							triangle.get_color());
					}
				} 
	
				center_z+=dz*dx;
				center_x+=dx;
			}
		}
	
		void rasterize_triangle_helper_bottom(
			geometry::Vector4d top, geometry::Vector4d mid,
				geometry::Vector4d bottom,
				color::rgb_24_bit color={0,0,0}) {
			auto triangle = shape::Polygon({top,mid,bottom});
			triangle.set_color(color);

			auto left = mid;
			auto right = top;
	
			if(left[0]>right[0]) std::swap(left,right); 
	
			auto left_boundary = bottom;
			auto right_boundary = bottom;
			
			if(mid[0]<left_boundary[0]) left_boundary = mid;
			else right_boundary = mid;
			
			auto dleft = top - left_boundary;
			dleft = dleft * (1.0/dleft[1]);
	
			auto dright = top - right_boundary;
			dright = dright * (1.0/dright[1]);
			
			//clip miny	
			if(bottom[1]<miny) {
				left_boundary = left_boundary + (miny+dy/2
						-left_boundary[1])*dleft;
				right_boundary = right_boundary + (miny+dy/2
						-right_boundary[1])*dright;
			}

			//draw_bottom_row
			left_boundary = left_boundary + (get_row(
					left_boundary[1])*dy+dy/2+miny
					-left_boundary[1])*dleft;
			right_boundary = right_boundary + (get_row(
					right_boundary[1])*dy+dy/2+miny
					-right_boundary[1])*dright;

			if((bottom[1]-left_boundary[1]) < -geometry::EPS)  {
				draw_row(left_boundary, right_boundary,
						triangle);
			}

			left_boundary += dleft*dy;
			right_boundary += dright*dy;

			if(get_row(top[1])>=height) 
				top[1] = maxy-dy/2;
			
			while (left_boundary[1]<=top[1]) {
				draw_row(left_boundary, right_boundary,
						triangle);
				left_boundary += dleft*dy;
				right_boundary += dright*dy;
			}
		}
	
		void rasterize_triangle_helper_top(
			geometry::Vector4d top, geometry::Vector4d mid,
				geometry::Vector4d bottom,
				color::rgb_24_bit color={0,0,0}) {
			auto triangle = shape::Polygon({top,mid,bottom});
			triangle.set_color(color);
			

			auto left = mid;
			auto right = top;
	
			if(left[0]>right[0]) std::swap(left,right); 
	
			auto left_boundary = bottom;
			auto right_boundary = bottom;
			
			auto dleft = left - left_boundary;
			dleft = dleft * (1.0/dleft[1]);

			auto dright = right - right_boundary;
			dright = dright * (1.0/dright[1]);
			
			//clip miny	
			if(bottom[1]<miny) {
				left_boundary = left_boundary + (miny+dy/2
						-left_boundary[1])*dleft;
				right_boundary = right_boundary + (miny+dy/2
						-right_boundary[1])*dright;
			}

			//draw_bottom_row
			left_boundary = left_boundary + (get_row(
					left_boundary[1])*dy+dy/2+miny
					-left_boundary[1])*dleft;
			right_boundary = right_boundary + (get_row(
					right_boundary[1])*dy+dy/2+miny
					-right_boundary[1])*dright;

			if((bottom[1]-left_boundary[1]) 
					< -geometry::EPS)  {
				draw_row(left_boundary, right_boundary,
						triangle);
			}

			left_boundary += dleft*dy;
			right_boundary += dright*dy;


			if(get_row(top[1])>=height) 
				top[1] = maxy-dy/2;

			while (left_boundary[1]<=top[1]) {
				draw_row(left_boundary, right_boundary,
						triangle);
				left_boundary += dleft*dy;
				right_boundary += dright*dy;
			}
	
		}

		std::vector<std::vector<double>> get_zbuffer() {
			return z_buffer;
		}

		Frame_Buffer get_frame_buffer() const {
			return buffer;
		}
	
		void rasterize_triangle(const shape::Polygon& p) {
			
			auto bottom = p[0];
			auto mid = p[1];
			auto top = p[2];
	
			auto c = p.get_color();
			
			if(top[1]<mid[1]) std::swap(top,mid);
			if(top[1]<bottom[1]) std::swap(top,bottom);
			if(mid[1]<bottom[1]) std::swap(mid,bottom);

			if(top[1]==bottom[1]) return;
			
			if(bottom[1]==mid[1]) { 
				rasterize_triangle_helper_bottom(top,mid
						,bottom,c);
			} else if(top[1]==mid[1]) { 
				rasterize_triangle_helper_top(top,mid,
						bottom,c);
			} else {
				auto dtop = (top-bottom);
				dtop = dtop * (1/dtop[1]);
				auto clip_vertex = (mid-bottom)[1]*dtop 
					+ bottom;
				rasterize_triangle_helper_top(clip_vertex,
						mid,bottom,c);

				rasterize_triangle_helper_bottom(top,
						clip_vertex,mid,c);

			}

			
		}	
	};
}


#endif
