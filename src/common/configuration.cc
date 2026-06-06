#include "../include/common/configuration.h"

namespace simple{
Configuration::Configuration(string file_path, Logger& logger)
: _file_path(std::move(file_path))
, _logger(logger)
{
    Load();
}

void Configuration::Load(){
    LoadConfig();
    LoadStopWordList();
}

void Configuration::LoadConfig(){
    nlohmann::json myjson;
    std::ifstream jsonfile(_file_path);
    jsonfile >> myjson;
    _config = myjson.at("path").get<std::map<std::string, std::string>>();
}

void Configuration::LoadStopWordList(){
    ifstream stop_word_eng_file = simple::Open(_config.at("stop_word_eng").c_str());
    ifstream stop_word_zh_file = simple::Open(_config.at("stop_word_zh").c_str());

    string word;
    while(stop_word_eng_file >> word){
        _stop_word_list.insert(word);
    }
    while(stop_word_zh_file >> word){
        _stop_word_list.insert(word);
    }
}

map<string, string>& Configuration::GetConfig(){
    return _config;
}

set<string>& Configuration::GetStopWordList(){
    return _stop_word_list;
}
}
