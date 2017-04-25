cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

ExternalProject_Add( 
	SquirrelLang
	URL ${CMAKE_SOURCE_DIR}/squirrel
#	GIT_REPOSITORY https://github.com/albertodemichelis/squirrel.git
	#GIT_TAG v3.1
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE=Debug
            -DINSTALL_INC_DIR=include/squirrel
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(SquirrelLang install_dir)
message(STATUS "install dir: ${install_dir}")
link_directories(${install_dir}/lib)


#add_subdirectory(squirrel)
include_directories(${install_dir}/include)
project(SquirrelTest)
add_executable(${PROJECT_NAME} main.cpp test.nut)
target_link_libraries(${PROJECT_NAME} squirrel_static)


