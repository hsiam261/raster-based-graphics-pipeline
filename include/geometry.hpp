#include <ostream>
#include <regex>
#include <string>
#include <vector>

#ifndef GEOMETRY
#define GEOMETRY

namespace geometry{	
	const double EPS = 1e-5;


	template <class basetype>
	void add(const basetype* mat1,const basetype* mat2,basetype* ans,
			int row_cnt,int col_cnt) {
		for(int i=0;i<row_cnt;i++) {
			for(int j=0;j<col_cnt;j++) {
				int idx = i*col_cnt+j;
				ans[idx] = mat1[idx] + mat2[idx];
			} 
		}
	}

	template <class base_type>
	void sub(const base_type* mat1,const base_type* mat2,base_type* ans,
			int row_cnt,int col_cnt) {
		for(int i=0;i<row_cnt;i++) {
			for(int j=0;j<col_cnt;j++) {
				int idx = i*col_cnt+j;
				ans[idx] = mat1[idx] - mat2[idx];
			} 
		}
	}

	template <class base_type>
	void mul(const base_type* mat1,const base_type* mat2,base_type* ans,
			int row_cnt1,int col_cnt1,int col_cnt2) {
		for(int i=0;i<row_cnt1;i++) {
			for(int j=0;j<col_cnt2;j++) {
				int idx_ans = i*col_cnt2+j;
				ans[idx_ans] = 0;
				for(int k=0;k<col_cnt1;k++) {
					int idx1 = i*col_cnt1+k;
					int idx2 = k*col_cnt2+j;
					ans[idx_ans] += mat1[idx1]*mat2[idx2];
				}
			}
		}

	}
	
	template <class base_type>
	void scale(base_type scale_factor,
			const base_type* mat,base_type* ans,
			int row_cnt,int col_cnt) {
		for(int i=0;i<row_cnt;i++) {
			for(int j=0;j<col_cnt;j++) {
				int idx= i*col_cnt+j;
				ans[idx] = mat[idx]*scale_factor;
			}
		}

	}

	template <class base_type>
	void transpose(const base_type* mat,base_type* ans,
			int row_cnt,int col_cnt) {
		for(int i=0;i<row_cnt;i++) {
			for(int j=0;j<col_cnt;j++) {
				int idx = i*col_cnt+j;
				int ans_idx = j*row_cnt+i;
				ans[ans_idx] = mat[idx];
			}
		}
	}


	class Vector4d {
		double v[4];
		friend class Matrix4d;
		public:
		Vector4d(); 
		Vector4d(const Vector4d& vec);
		Vector4d(double x,double y,double z,double w=1);
			

		double& operator[](int i);
		double operator[](int i) const;
		
		Vector4d& operator=(const Vector4d& other);
		Vector4d& operator+=(const Vector4d& other);
		Vector4d& operator-=(const Vector4d& other);
		Vector4d& operator*=(double scale);

		bool operator==(const Vector4d& rhs) const;
		bool operator!=(const Vector4d& rhs) const;

		Vector4d operator+(const Vector4d& rhs) const;
		Vector4d operator-(const Vector4d& rhs) const;
		Vector4d operator*(double scale) const;
	};


	double dot(const Vector4d& v1,const Vector4d& v2);
	double dot3d(const Vector4d& v1,const Vector4d& v2);
	Vector4d operator*(double scale,const Vector4d& v);
	Vector4d homogenize(const Vector4d&);
	Vector4d cross(const Vector4d& v1, const Vector4d& v2);
	Vector4d normalize3d(const Vector4d& v);

	std::string to_string(const Vector4d& v);
	std::ostream& operator<<(std::ostream &os,const Vector4d& v);

	class Matrix4d {
		double v[16]; 
		
		friend Matrix4d transpose(const Matrix4d& v);

		public:
		Matrix4d();
		Matrix4d(const std::vector<double>& v);
		Matrix4d(const Matrix4d& m);
		
		Matrix4d& operator=(const Matrix4d& other);
		Matrix4d& operator+=(const Matrix4d& other);
		Matrix4d& operator-=(const Matrix4d& other);
		Matrix4d& operator*=(const Matrix4d& other);
		Matrix4d& operator*=(double scale);

		bool operator==(const Matrix4d& rhs) const;
		bool operator!=(const Matrix4d& rhs) const;

		const double* operator[](int i) const;
		double* operator[](int i);
		
		Matrix4d operator+(const Matrix4d& rhs) const;
		Matrix4d operator-(const Matrix4d& rhs) const;
		Matrix4d operator*(const Matrix4d& rhs) const;
		Matrix4d operator*(double scale) const;
		Vector4d operator*(const Vector4d& rhs) const;

	};

	

	Matrix4d operator*(double scale,const Matrix4d& rhs);
	Matrix4d transpose(const Matrix4d& mat);


		
	std::string to_string(const Matrix4d&);
	std::ostream& operator<<(std::ostream &os, const Matrix4d& mat);

	Matrix4d identity();
	Matrix4d zeros();
	
	Matrix4d translate(double x,double y,double z);
	Vector4d translate(double x,double y,double z,const Vector4d& v);
	Vector4d translate(const Vector4d& translation_vector, 
			const Vector4d& v);
	Matrix4d translate(const Vector4d& translation_vector);

	Matrix4d rotate(double angle_in_radian,const Vector4d& axis);	
	Vector4d rotate(double angle_in_radian, const Vector4d &axis,
			const Vector4d& v);

	Matrix4d scale(double x,double y,double z);
	Vector4d scale(double x,double y,double z,const Vector4d& v);
	
};




#endif
