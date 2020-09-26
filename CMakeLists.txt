cmake_minimum_required(VERSION 3.5)

project(qpc)

if(NOT DEFINED QPC_PLATFORM)
	set(QPC_PLATFORM "WINDOWS")
endif()

if(NOT DEFINED QPC_ARCH)
	set(QPC_ARCH "AMD64")
endif()

add_subdirectory(scripts)
