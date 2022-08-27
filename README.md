# Rastor Based Graphics Pipeline using C++
A Raster Based Computer Graphics Pipeline that reads a scenefile and a configfile and generates a bmp image of that scene using the zbuffer algorithm.

## Scenefile
The scenefile contains the location and orientation of the camera and the description of the scene. A scene is made form triangles and matrix operations.

The format of a scenefile is as follows:  

```
camera_x camera_y camera_z
focus_x focus_y focus_z
up_x up_y up_z
fovy aspect near far
[scene command]*
end
```

### scene commands
A scene command can be one of the following commands:  

- `triangle` : adds a triangle to the scene given its vertices (with respect to the co-ordinate given by the top of the matrix-stack)

	```
	triangle
	vertex1_x vertex1_y vertex1_z
	vertex2_x vertex2_y vertex2_z
	vertex3_x vertex3_y vertex3_z
	```
- `push`: push the identity matrix to the matrix stack
	
	```
	push
	```  
- `pop`: pops from the matrix stack

	```
	pop
	```
- `scale`: scales the x,y,z axes (updates the top of the matrix stack)

	```
	scale 
	scale_x scale_y scale_z
	```
- `translate`: performs a translation operation (updates the top of the matrix stack)

	```
	translate 
	translate_x translate_y translate_z
	```

- `rotate`: performs a rotation operation around axis `(rotation_x,rotation_y,rotation_z)` by `angle` degrees (updates the top of the matrix stack)

	```
	rotate
	angle rotation_x rotation_y rotation_z
	```

## Configfile
Config file describes the parameters for clipping and creating the image and the image dimensions

Format of a configfile is as follows:

```
width height
x_limit
y_limit
min_z max_z
```

- `x_limit` : points outside the interval `[-x_limit,x_limit]` will be clipped  
- `y_limit` : points outside the interval `[-y_limit,y_limit]` will be clipped  
- `z_min` and `z_max` : points outside the interval `[z_min,z_max]` will be clipped  


# Dependencies
- `bitmap_image.hpp` (https://www.partow.net/programming/bitmap/index.html)  
- `catch2` (only for unit testing)


# Build
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build/
cd build
make raster-pipeline
```

# Usage 
``` 
raster-pipeine scenefile [configfile]
```
The default value of config file is `config.txt`.


# Unit Tests
## Build Unit Tests
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build/
cd build
make raster-pipeline_test
```

## Run Unit Test
From the build directory run the following commands:
``` 
test/raster-pipeline_test
```
