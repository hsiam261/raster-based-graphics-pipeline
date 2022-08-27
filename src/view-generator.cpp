#include <vector>
#include <view-generator.hpp>

std::vector<geometry::Vector4d> view_generator::View_generator::generate_view(
		const canvas::Canvas &canvas) {
	std::vector<geometry::Vector4d> ans(canvas.size());
	for(int i=0;i<canvas.size();i++) {
		ans[i] = canvas[i];
		for(int j=0;j<canvas[i].size();i++) {

		}
	}
}
