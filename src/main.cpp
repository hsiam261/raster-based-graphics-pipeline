#include <fstream>
#include <geometry.hpp>
#include <iomanip>
#include <ios>
#include <ostream>
#include <shapes.hpp>
#include <canvas.hpp>
#include <iostream>
#include <projector.hpp>
#include <camera.hpp>
#include <frame_buffer.hpp>
#include <rasterizer.hpp>
#include <random>
#include <string>
using namespace std;

canvas::Canvas cnvs;
camera::Camera cmra;

void read_scene(string scene_file_name,string config_file_name) {
	fstream scene_file;
	scene_file.open(scene_file_name,ios::in);

	geometry::Vector4d eye;
	geometry::Vector4d look;
	geometry::Vector4d up;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); 	
	uniform_int_distribution<> dist(0,255);

	scene_file  >> eye[0] >> eye[1] >> eye[2];
	cout << eye[0] << " " << eye[1] << " " << eye[2] << endl;
	scene_file  >> look[0] >> look[1] >> look[2];
	cout << look[0] << " " << look[1] << " " << look[2] << endl;
	scene_file  >> up[0] >> up[1] >> up[2];
	up[3]=0;
	cout << up[0] << " " << up[1] << " " << up[2] << endl;
	
	cmra.set_position(eye);
	cmra.look_at(look,up);

	double fovy,aspect, near, far;
	scene_file >> fovy >> aspect >> near >> far;
	cout << fovy << " " <<  aspect << " " << near << " " << far << endl; 
	fovy = fovy/180*3.14159;
	while (!scene_file.eof()) {
		string command;
		scene_file >> command;
		cout << command <<  " " << (command=="end") << endl;
		if(command=="triangle") {
			shape::Polygon triangle(3);
			for(int i=0;i<3;i++) {
				for(int j=0;j<3;j++) {
					scene_file >> triangle[i][j]; 
					cout << triangle[i][j] << " ";

				}
				cout << endl;
				int r = dist(gen);
				int g = dist(gen);
				int b = dist(gen);
				triangle.set_color({r,g,b});
			}
			
			cnvs.add_shape(triangle);
			std::cout<< "&&&&&&&&" << cnvs[cnvs.size()-1].get_color() << std::endl;
		} else if (command=="translate") {
			double x,y,z;
			scene_file >> x >> y >> z;
			cnvs.update_transform(geometry::translate(x,y,z));
		} else if(command=="scale") {
			double x,y,z;
			scene_file >> x >> y >> z;
			cnvs.update_transform(geometry::scale(x,y,z));
		} else if(command=="rotate") {
			double theta,x,y,z;
			scene_file >> theta >> x >> y >> z;
			cnvs.update_transform(geometry::
					rotate(theta/180*3.14159265358979323846,{x,y,z,0}));
		} else if(command=="push") {
			cnvs.push(geometry::identity());
		} else if (command=="pop") {
			cnvs.pop();
		} else if(command=="end") {
			break;
		} else {
			cout << "why? :(" << endl;
			break;
		}			

	}

	cout << "Read scene file" << endl;
	scene_file.close();

	fstream config_file;
	config_file.open(config_file_name,ios::in);
	
	int width,height;
	config_file >> width >> height;
	double minx,miny,minz,maxx,maxy,maxz;
	config_file >> minx;
	maxx = -minx;
	config_file >> miny;
	maxy = -miny;
	config_file >> minz >> maxz;

	config_file.close();
	cout << "Read config file" << endl;
	fstream output_stage1;
	output_stage1.open("stage1.txt",ios::out);
	for(int i=0;i<cnvs.size();i++) {
		for(int j=0;j<cnvs[i].size();j++) {
			for(int k=0;k<3;k++) {
				output_stage1 << std::fixed << std::setprecision(7)  << cnvs[i][j][k];
				if(k!=2) output_stage1 << " ";
			}
			output_stage1 << endl;
		}
		output_stage1 << endl;
	}
	output_stage1.close();
	cout << endl;


	cout << "Finished Stage 1" << endl;

	fstream output_stage2;
	output_stage2.open("stage2.txt",ios::out);
	for(int i=0;i<cnvs.size();i++) {
		auto t = shape::apply_transform(cnvs[i],cmra.get_camera_transform());
		for(int j=0;j<t.size();j++) {
			for(int k=0;k<3;k++) {
				output_stage2 <<  std::fixed << std::setprecision(7) <<  t[j][k];
				if(k!=2) output_stage2 << " ";
			}
			output_stage2 << endl;
		}
		output_stage2 << endl;
	}
	output_stage2.close();
	cout << endl;
	

	cout << "Finished Stage 2" << endl;

	fstream output_stage3;
	output_stage3.open("stage3.txt",ios::out);
	for(int i=0;i<cnvs.size();i++) {
		auto t = shape::apply_transform(cnvs[i],cmra.get_camera_transform());
		t = shape::homogenize(shape::apply_transform(t,projector::get_canonical_view_transform(fovy,aspect,near,far)));
		for(int j=0;j<t.size();j++) {
			for(int k=0;k<3;k++) {
				output_stage3 << std::fixed << std::setprecision(7)<<  t[j][k];
				if(k!=2) output_stage3 << " ";
			}				
			output_stage3 << endl;
		}
		output_stage3 << endl;
	}
	output_stage3.close();
	cout << endl;
	cout << "Finished Stage 3" << endl;
	
	
	rastorizer::Zbuffer_rasterizor<frame_buffer::Frame_Buffer<color::rgb_24_bit>> rastorizer(width,height,minx,maxx,miny,maxy,20);
	for(int i=0;i<cnvs.size();i++) {
		auto t = shape::apply_transform(cnvs[i],cmra.get_camera_transform());
		std::cout << t << std::endl;
		t = shape::clip_against_plane(t,{0,0,-1},{0,0,-near}); 		
		std::cout << t << std::endl;
		t = shape::clip_against_plane(t,{0,0,1},{0,0,-far});
		std::cout << t << std::endl;

		auto p = shape::apply_transform(t,projector::get_canonical_view_transform(fovy,aspect,near,far));
		cout << "#" << p << endl;
		p  = shape::homogenize(p);
		p = shape::clip_against_plane(p,{0,0,1},{0,0,minz});	
		p = shape::clip_against_plane(p,{0,0,-1},{0,0,maxz});	
		std::cout << "************************************" <<  p << std::endl;
		auto v = shape::triangulate_polygon(p);
		cout << "hello" << endl;
		for(auto triangle:v) {
			std::cout << triangle << std::endl;
			rastorizer.rasterize_triangle(triangle);
		}
		frame_buffer::save_as_bmp(rastorizer.get_frame_buffer(),"test"+std::to_string(i));
	}

	frame_buffer::save_as_bmp(rastorizer.get_frame_buffer(),"out.bmp");

	fstream zbuffer_file;
	zbuffer_file.open("z_buffer.txt",ios::out);
	
	auto zbuff = rastorizer.get_zbuffer();
	for(int i=0;i<height;i++) {
		for(int j=0;j<width;j++) {
			auto t = zbuff[height-1-i][j];
			if(t<2.0)
				zbuffer_file << std::fixed << std::setprecision(6) << t << "\t";
		}
		zbuffer_file << std::endl;
	}
	
	zbuffer_file.close();





}



int main(int argc,char** argv) {
	if(argc!=2 and argc!=3) {
		std::cerr << "Usage: raster-pipeline scenefile [configfile]" << std::endl;
	} 

	string scenefile = argv[1];
	string configfile = "config.txt";
	if(argc==3) configfile = argv[2];
	read_scene(scenefile,configfile);	

	return 0;
}
