# Smoothed Particle Hydrodynamics solver 
## work in progress

as of last commit this simulation is still very much unbalanced and need more work

inspiration:  
[Seb Lague's fluid simulator](https://github.com/SebLague/Fluid-Sim)  
[lijenicol's SPH fluid simulator](https://github.com/lijenicol/SPH-Fluid-Simulator)  


current issue:  
1. Particles arent visible before simulation started
2. particle generation needs improvement
3. particles never settle, when and if they do they dont stack on top of each other
4. the whole simulation is not scaled properly, especially the smoothing radius to model size ratio
1. some particle position will return 0 when the simulation is started
1. some particles have 0 density, even though it is clamped to 0.1 as the minimum, suggesting that there might be memory barrier issue

## How to build

1. please change every single directory path before trying to build this as the path are absolute, since idk how to set it in cmakelists to use relative path
2. download vcpkg

### Windows
install dependencies

	D:/dev/vcpkg/vcpkg/vcpkg install glfw3 assimp glm glslang volk meshoptimizer gli rapidjson taskflow bullet3 imguizmo glad --triplet x64-windows

then run  

	mkdir build  
	cd build 
	cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="D:/dev/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake"

this should generate the visual studio solution and you can build it from there

### Linux

I have not tested this in linux, but in theory it should work since there are not platform independent code

install vcpkg

	git clone https://github.com/microsoft/vcpkg.git
	cd vcpkg
	./bootstrap-vcpkg.sh

install dependencies

	./vcpkg install glfw3 assimp glm glslang volk meshoptimizer gli rapidjson taskflow bullet3 imguizmo glad --triplet x64-linux

then run 

	mkdir build
	cd build
	cmake .. -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
	cmake --build .
	./3Drenderer
