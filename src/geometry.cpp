#include "geometry.hpp"
#include <cmath>
#include <ostream>
#include <random>
#include <string>

geometry::Vector4d::Vector4d() {this->v[3]=1;}

geometry::Vector4d::Vector4d(double x,double y,double z,double w) {
	this->v[0]=x;
	this->v[1]=y;
	this->v[2]=z;
	this->v[3]=w;
}

geometry::Vector4d::Vector4d(const geometry::Vector4d& vec) {
	for(int i=0;i<4;i++) this->v[i] = vec.v[i];
}


geometry::Vector4d& geometry::Vector4d::operator=(const Vector4d &other) {
	if(this==&other) return *this;
	for(int i=0;i<4;i++) v[i] = other.v[i]; 
	return *this;
}


double& geometry::Vector4d::operator[](int i) {
	return v[i];		
}

double geometry::Vector4d::operator[](int i) const {
	return v[i];
}


geometry::Vector4d geometry::Vector4d::operator+(const Vector4d &rhs) const {
	Vector4d ans;	
	geometry::add(this->v,rhs.v,ans.v,4,1);
	return ans;
}

geometry::Vector4d geometry::Vector4d::operator-(const Vector4d &rhs) const {
	Vector4d ans;	
	geometry::sub(this->v,rhs.v,ans.v,4,1);
	return ans;
}

geometry::Vector4d geometry::Vector4d::operator*(double scale) const {
	Vector4d ans;	
	geometry::scale(scale,this->v,ans.v,4,1);
	return ans;
}

bool geometry::Vector4d::operator==(const Vector4d &rhs) const {
	for(int i=0;i<4;i++) {
		double d = std::fabs(this->v[i]-rhs[i]);
		if(d>EPS || std::isnan(d)) return false;
	}

	return true;
}

bool geometry::Vector4d::operator!=(const Vector4d &rhs) const {
	return !((*this)==rhs);
}

geometry::Vector4d& geometry::Vector4d::operator+=(const Vector4d &other) {
	geometry::add(this->v,other.v,this->v,4,1);
	return *this;
}

geometry::Vector4d& geometry::Vector4d::operator-=(const Vector4d &rhs) {
	geometry::sub(this->v,rhs.v,this->v,4,1);
	return *this;
}

geometry::Vector4d& geometry::Vector4d::operator*=(double scale) {
	geometry::scale(scale,this->v,this->v,4,1);
	return *this;
}

double geometry::dot(const Vector4d &v1, const Vector4d &v2) {
	double ans=0;
	for(int i=0;i<4;i++) ans+=v1[i]*v2[i];
	return ans;
}

double geometry::dot3d(const Vector4d &v1, const Vector4d &v2) {
	double ans=0;
	for(int i=0;i<3;i++) ans+=v1[i]*v2[i];
	return ans;
}

geometry::Vector4d geometry::operator*(double scale,
		const geometry::Vector4d& v) {
	return v*scale;
}

geometry::Vector4d geometry::homogenize(const Vector4d &v) {
	return Vector4d(v[0]/v[3],v[1]/v[3],v[2]/v[3]);
}

geometry::Vector4d geometry::normalize3d(const Vector4d &v) {
	double size = std::sqrt(dot3d(v,v));
	Vector4d ans(v[0]/size,v[1]/size,v[2]/size,v[3]);
	return ans;
	
}

geometry::Vector4d geometry::cross(const Vector4d &v1, const Vector4d &v2) {
	double x = v1[1]*v2[2] - v2[1]*v1[2];
	double y = v1[2]*v2[0] - v2[2]*v1[0];
	double z = v1[0]*v2[1] - v2[0]*v1[1];

	return Vector4d(x,y,z,v1[3]*v2[3]);
}

std::string geometry::to_string(const geometry::Vector4d& v) {
	std::string str = "( " + std::to_string(v[0]) + ", " + 
		std::to_string(v[1]) + ", " + 
		std::to_string(v[2]) + ", " + 
		std::to_string(v[3]) +" )";
	return str;
}

std::ostream& geometry::operator<<(std::ostream &os, const Vector4d &v) {
	os << geometry::to_string(v);
	return os;
}


geometry::Matrix4d::Matrix4d() {}

geometry::Matrix4d::Matrix4d(const std::vector<double>& v) {
	for(int i=0;i<v.size() && i<16;i++) this->v[i] = v[i];	
}

geometry::Matrix4d::Matrix4d(const geometry::Matrix4d& mat) {
	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++) {
			this->v[i*4+j] = mat[i][j];
		}
	}
}

double* geometry::Matrix4d::operator[](int i) {
	return (double*)(this->v)+(i*4);
}

const double* geometry::Matrix4d::operator[](int i) const {
	return (double*)(this->v)+(i*4);
}

geometry::Matrix4d& geometry::Matrix4d::operator=(const Matrix4d &other) {
	if(this == &other) return *this;
	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++) this->v[i*4+j]=other[i][j];
	}
	
	return *this;
}

bool geometry::Matrix4d::operator==(const Matrix4d &rhs) const {
	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++) {
			double d = std::fabs(this->v[i*4+j]-rhs[i][j]); 
			if(d>EPS || std::isnan(d)) 
				return false;
		}
	}
	return true;
}

bool geometry::Matrix4d::operator!=(const Matrix4d &rhs) const {
	return !(*this==rhs);
}

geometry::Matrix4d geometry::Matrix4d::operator+(const Matrix4d &rhs) const {
	Matrix4d ans;
	geometry::add(this->v,rhs.v,ans.v,4,4);
	return ans;
}

geometry::Matrix4d geometry::Matrix4d::operator-(const Matrix4d &rhs) const {
	Matrix4d ans;
	geometry::sub(this->v,rhs.v,ans.v,4,4);
	return ans;
}

geometry::Matrix4d geometry::Matrix4d::operator*(double scale) const {
	Matrix4d ans;
	geometry::scale(scale,this->v,ans.v,4,4);
	return ans;
}

geometry::Matrix4d geometry::operator*(double scale,const Matrix4d& rhs) {
	return rhs*scale;
}

geometry::Matrix4d geometry::Matrix4d::operator*(const Matrix4d& rhs) const {
	Matrix4d ans;
	geometry::mul(this->v,rhs.v,ans.v,4,4,4);
	return ans;
}

geometry::Vector4d geometry::Matrix4d::operator*(const Vector4d& v) const {
	Vector4d ans;
	mul(this->v,v.v,ans.v,4,4,1);
	return ans;
}

geometry::Matrix4d& geometry::Matrix4d::operator+=(const Matrix4d &other) {
	add(this->v,other.v,this->v,4,4);
	return *this;
}

geometry::Matrix4d& geometry::Matrix4d::operator-=(const Matrix4d &other) {
	sub(this->v,other.v,this->v,4,4);
	return *this;
}

geometry::Matrix4d& geometry::Matrix4d::operator*=(double scale) {
	geometry::scale(scale,this->v,this->v,4,4);
	return *this;
}

geometry::Matrix4d& geometry::Matrix4d::operator*=(const Matrix4d& other) {
	return *this=((*this)*other);
}

geometry::Matrix4d geometry::transpose(const geometry::Matrix4d& mat) {
	Matrix4d ans;
	transpose(mat.v,ans.v,4,4);
	return ans;
}

std::string geometry::to_string(const Matrix4d& mat) {
	std::string s= "{";
	for(int i=0;i<4;i++) {
		s+= "{ ";
		for(int j=0;j<4;j++) {
			s+=std::to_string(mat[i][j]);
			if(j!=3) s+=",";
			s+=" ";
		}
		s+="}";
		if(i!=3) s+="\n";
	}
	s+="}";

	return s;
}

std::ostream& geometry::operator<<(std::ostream& os,const Matrix4d& mat)  {
		os << geometry::to_string(mat);
		return os;
}


geometry::Matrix4d geometry::identity() {
	return Matrix4d({1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1});
}


geometry::Matrix4d geometry::zeros() {
	return Matrix4d({0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0});
}

geometry::Matrix4d geometry::translate(double x,double y,double z) {
	return Matrix4d({1,0,0,x,
			0,1,0,y,
			0,0,1,z,
			0,0,0,1});
}

geometry::Vector4d geometry::translate(double x,double y,double z, 
		const Vector4d& v) {
	Vector4d ans = v;
	ans[0]+=x*ans[3];
	ans[1]+=y*ans[3];
	ans[2]+=z*ans[3];

	return ans;
}

geometry::Matrix4d geometry::translate(const geometry::Vector4d& 
		translation_vector) {
	return Matrix4d({1,0,0,translation_vector[0],
			0,1,0,translation_vector[1],
			0,0,1,translation_vector[2],
			0,0,0,1});
}

geometry::Vector4d geometry::translate(const geometry::Vector4d& 
		translation_vector, const geometry::Vector4d& v) {
	Vector4d ans = v;
	ans[0]+=translation_vector[0]*ans[3]; 
	ans[1]+=translation_vector[1]*ans[3]; 
	ans[2]+=translation_vector[2]*ans[3]; 

	return ans;
}

geometry::Vector4d geometry::rotate(double angle_in_radian, 
		const geometry::Vector4d& axis, const Vector4d& v) {
	auto normalized_axis = normalize3d(axis);
	auto homogenized_vertex = homogenize(v);

	auto ans = homogenized_vertex*std::cos(angle_in_radian) 
		+ ( cross(normalized_axis,homogenized_vertex)
		  * std::sin(angle_in_radian) ) 
		+ (normalized_axis*dot3d(normalized_axis,homogenized_vertex)
				*(1-std::cos(angle_in_radian)));
	return ans;
}

geometry::Matrix4d geometry::rotate(double angle_in_radian,
		const geometry::Vector4d& axis) {
	Vector4d arr[3];
	arr[0] = rotate(angle_in_radian,axis,Vector4d(1,0,0));	
	arr[1] = rotate(angle_in_radian,axis,Vector4d(0,1,0));	
	arr[2] = rotate(angle_in_radian,axis,Vector4d(0,0,1));	

	Matrix4d ans = identity();
	for(int j=0;j<3;j++) {
		for(int i=0;i<3;i++) ans[i][j] = arr[j][i];
		ans[3][j]=0;
	}

	ans[0][3] = ans[1][3] = ans[2][3] = 0;
	ans[3][3] = 1;

	return ans;
}

geometry::Matrix4d geometry::scale(double x,double y,double z) {
	return Matrix4d({x,0,0,0,
			0,y,0,0,
			0,0,z,0,
			0,0,0,1
		});
}

