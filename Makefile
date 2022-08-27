all:
	mkdir -p build/
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build/ && cd build/ && make && cd ../ && build/test/raster-pipeline_test
	mv build/compile_commands.json ./ 

gen_test_file: 
	cd others/ && ls && unzip 'Test Cases (Updated).zip' && find . -name '*.txt'|xargs -I {} sed -i 's/\r$$//' {}	 
	cp others/1/ others/1-out/ -r
	cp others/2/ others/2-out/ -r 
	cp others/3/ others/3-out/ -r 
	cp others/4/ others/4-out/ -r
	cp build/raster-pipeline others/1-out/ && cd others/1-out && ./raster-pipeline && sxiv test[0-4]
	cp build/raster-pipeline others/2-out/ && cd others/2-out && ./raster-pipeline && sxiv test[0-4]
	cp build/raster-pipeline others/3-out/ && cd others/3-out && ./raster-pipeline && sxiv test[0-9] test1[0-1]
	cp build/raster-pipeline others/4-out/ && cd others/4-out && ./raster-pipeline && sxiv test[0-9] test1[0-1] 
