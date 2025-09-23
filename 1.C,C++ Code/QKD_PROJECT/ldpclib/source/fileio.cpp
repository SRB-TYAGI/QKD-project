// fileio.cpp
#include <cstdio>
FILE* open_file_std(const char* name, const char* mode) {
return fopen(name, mode);
}