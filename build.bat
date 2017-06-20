@echo off

REM # Create build directory
call cmake -E make_directory "build"

REM # Create build generator files
call cmake -E chdir "build" cmake -E time cmake --clean ".." -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Release

REM # Build project
call cmake -E chdir "build" cmake -E time cmake --build "." --config Release
