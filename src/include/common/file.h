#ifndef __FILE_H__
#define __FILE_H__

#include <fstream>
#include <ios>
#include <string>

namespace simple {
using std::ifstream;
using std::string;
using std::ios_base;

ifstream Open(const char* path, ios_base::openmode mode = ios_base::in);
}
#endif
