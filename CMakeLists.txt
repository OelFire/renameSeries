cmake_minimum_required(VERSION 3.9)
project(renameSeries)

set(CMAKE_CXX_STANDARD 14)

add_executable(renameSeries main.cpp RenameSeriesFile.cpp RenameSeriesFile.hpp)