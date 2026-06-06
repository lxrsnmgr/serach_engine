#include "../include/common/dir_scanner.h"
#include <filesystem>

namespace simple{
using std::filesystem::directory_iterator;
using std::filesystem::path;

DirScanner::DirScanner(Logger& logger, IConfiguration& config)
: _logger{logger}, _config{config}
{

}

void DirScanner::operator()(){
    LOG_INFO("{}", _config.GetConfig()["dir_people"]);
    return Traverse(_config.GetConfig()["dir_people"]);
}

vector<string>& DirScanner::GetFilesPath(){
    return _file_path_list;
}

void DirScanner::Traverse(const string& dir_path){
    const path test_dir_path = {dir_path};
    for(const auto& entry : directory_iterator{test_dir_path}){
        _file_path_list.emplace_back(entry.path().string());
    }
}
}
