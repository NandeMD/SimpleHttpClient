
compiler := clang++
opt_level:= -O3
include_flags := -Iinclude -Isrc
flags := -lstdc++

main_file := test.cpp
source_dir := src
src_files = $(wildcard $(source_dir)/*.cpp)
out_name = test

code := $(compiler) $(include_flags) $(flags) $(opt_level) $(main_file) $(src_files) -o $(out_name)

main:
	rm -f $(out_name)
	$(code)
