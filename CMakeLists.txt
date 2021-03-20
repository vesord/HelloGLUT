cmake_minimum_required(VERSION 3.17)
project(HelloGLUT)

set(CMAKE_C_STANDARD 99)

add_compile_options("-Wall -Wextra -Werror")

add_executable(TwoDimensionTransform TwoDimensionTransform/main.c)
target_link_options(TwoDimensionTransform PUBLIC -framework GLUT)

add_executable(ThreeDimensionTransform ThreeDimensionTransform/main.c)
target_link_options(ThreeDimensionTransform PUBLIC -framework GLUT)

add_executable(Polygons Polygons/main.c)
target_link_options(Polygons PUBLIC -framework GLUT)

add_executable(SimpleFractal SimpleFractal/main.c)
target_link_options(SimpleFractal PUBLIC -framework GLUT)

add_executable(LightAndMaterials LightAndMaterials/main.c LightAndMaterials/materials.c)
target_link_options(LightAndMaterials PUBLIC -framework GLUT)

add_executable(Textures Textures/main.c)
target_link_options(Textures PUBLIC -framework GLUT)