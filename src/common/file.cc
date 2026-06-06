#include "../include/common/file.h"

namespace simple {
ifstream Open(const char* path, ios_base::openmode mode){
    ifstream file(path, mode);
    if(!file){
        string err {"Unable to open file"};
        err.append(path);
        throw std::runtime_error(err);
    }
    file.exceptions(ifstream::badbit);
    return file;
}
}
