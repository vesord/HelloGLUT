cmake_minimum_required(VERSION 3.17)
project(HelloGLUT)

set(CMAKE_C_STANDARD 99)

set(CMAKE_EXE_LINKER_FLAGS "-framework OpenGL -framework GLUT")

add_compile_options("-Wall -Wextra -Werror")

add_executable(TwoDimensionTransform TwoDimensionTransform/main.c)

add_executable(ThreeDimensionTransform ThreeDimensionTransform/main.c)

add_executable(Polygons Polygons/main.c)

add_executable(SimpleFractal SimpleFractal/main.c)

add_executable(LightAndMaterials LightAndMaterials/main.c LightAndMaterials/materials.c)

add_executable(Textures Textures/main.c)
