#include "../include/key/dict_producer.h"
#include "cppjieba/Jieba.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <dirent.h>

namespace simple{
using std::ifstream;
using std::istringstream;
using std::ofstream;
using std::string;

DictProducer::DictProducer(Logger& logger, IConfiguration& conf)
    : _logger{logger}
    , _conf{conf}
    , _st{_logger, _conf}{
        _dict.reserve(100);
    }

void DictProducer::BuildEnDict(){
    if(!_dict.empty()){
        _dict.clear();
    }

    std::map<std::string, int> dictmap;
    std::set<std::string> stopword;

    //读取停用词
    const string stopwordpath = _conf.GetConfig()["stop_word_eng"];
    DictProducer::ReadStopWord(stopword, std::move(string(stopwordpath)));

    const string path = _conf.GetConfig()["english"];
    ifstream ifs(path);
    if(!ifs){
        LOG_ERROR("open {} fail", path);
        return;
    }

    string line;
    while(getline(ifs, line)){
        PreProcess(line);
        istringstream iss(line);
        string word;
        while(iss >> word){
            if(stopword.count(word) == 0){//表示不是停用词
                auto iter = dictmap.find(word);
                if(iter != dictmap.end()){
                    ++iter->second;//如果当前单词在dictmap中，词频++
                } else {
                    dictmap[word] = 1;
                }
            }
        }
    }

    _dict.reserve(dictmap.size());
    for(const auto& elem : dictmap){
        _dict.emplace_back(std::make_pair(elem.first, elem.second));
    }
}

void DictProducer::BuildCnDict(){
    if(!_dict.empty()){
        _dict.clear();
    }

    std::vector<std::string> filenamevec;
    std::map<std::string, int> dictmap;
    std::set<std::string> stopword;

    const string stopwordpath = _conf.GetConfig()["stop_word_zh"];
    DictProducer::ReadStopWord(stopword, std::move(string(stopwordpath)));
    DictProducer::GetFileName(filenamevec, _conf.GetConfig()["dir_art"]);
    const string path = _conf.GetConfig()["dir_art"];

    for(auto& filename : filenamevec){
        filename = string(path).append(filename);
        ifstream ifs(filename);
        if(!ifs){
            LOG_ERROR("open {} fail", filename);
            return;
        }
        string line;
        while(getline(ifs, line)){
            _file.push_back(line);
        }
    }

    vector<string> wordvec;
    for(auto& line : _file){
        _st.Cut(line, wordvec, true);
        for(auto&elem : wordvec){
            if(IsAlphaNumber(elem)){
                continue;
            }
            if(stopword.count(elem) == 0){
                auto iter = dictmap.find(elem);
                if(iter != dictmap.end()){
                    ++iter->second;//如果当前单词在dictmap中,词频++
                } else {
                    dictmap[elem] = 1;
                }
            }
        }
    }
    _dict.reserve(dictmap.size());//将map中的数据转到vector中
    for(const auto& elem : dictmap){
        _dict.emplace_back(std::make_pair(elem.first, elem.second));
    }
}

void DictProducer::CreateIndex(Type type){
    for(size_t idx = 0; idx < _dict.size(); ++idx){
        //遍历单词的字母加入set
        int letter_size;
        if(type == CN){
            letter_size = 3;
        } else {
            letter_size = 1;
        }
        for(int pos = 0; pos < _dict[idx].first.size(); pos += letter_size){
            string letter(_dict[idx].first, pos, letter_size);
            _index[letter].insert(static_cast<int>(idx));
        }
    }
}

int DictProducer::Store(Type type){
    ofstream ofs_dict;
    ofstream ofs_index;
    string dictfile;
    string indexfile;
    if(type == CN){
        dictfile = _conf.GetConfig()["dict_zh"];
        indexfile = _conf.GetConfig()["index_zh"];
    } else {
        dictfile = _conf.GetConfig()["dict_eng"];
        indexfile = _conf.GetConfig()["index_eng"];
    }
    ofs_dict.open(dictfile);
    ofs_index.open(indexfile);
    if(ofs_dict.good() && ofs_index.good()){
        for(auto& item : _dict){
            ofs_dict << item.first << "\t" << item.second << std::endl;
        }
        
        for(auto& item : _index){
            ofs_index << item.first << "\t";
            for(const auto& idx : _index[item.first]){
                ofs_index << " " << idx;
            }
            ofs_index << "\n";
        }
    } else {
        LOG_ERROR("open {} or {} fail", dictfile, indexfile);
        return -1;
    }
    return 0;
}

void DictProducer::PreProcess(string& word){
    for(auto& elem : word){
        if(isalpha(elem)){
            elem = static_cast<char>(tolower(elem));//是字母，转化为小写
        } else {
            elem = ' ';//不是字母，替换成空格
        }
    }
}

size_t DictProducer::GetIdx(string& word){
    size_t idx;
    for(idx = 0; idx < _dict.size(); ++idx){
        if(_dict[idx].first == word){
            return idx;
        }
    }
    return _dict.size() + 1;
}

void DictProducer::GetFileName(vector<string>& vec, string& path){
    DIR* ptr_dir = opendir(path.c_str());
    if(ptr_dir == nullptr){
        LOG_ERROR("open directory: {} fail", path);
        return;
    }
    struct dirent* ptrdirent;
    struct stat ptrstat;

    while((ptrdirent = readdir(ptr_dir)) != nullptr){
        int ret = stat(ptrdirent->d_name, &ptrstat);
        string filename(ptrdirent->d_name);
        if(filename != "." && filename != ".."){
            vec.emplace_back(filename);
        }
    }
    closedir(ptr_dir);
}

void DictProducer::ReadStopWord(std::set<std::string>& stopword, const string& stopwordpath){
    //读取停用词
    ifstream ifs(stopwordpath);
    if(!ifs){
        string msg = string("open ") + stopwordpath + string(" fail\n");
        LOG_ERROR("open {} fail", stopwordpath);
        return;
    }
    string line;
    while(ifs >> line){
        stopword.insert(line);
    }
    ifs.close();
}

bool DictProducer::IsAlphaNumber(string& word){
    char firstbyte = word.c_str()[0];
    bool flag = true;
    if((firstbyte & 0x80) == 0x0){
        flag = true;
    } else if((firstbyte & 0xE0) == 0xE0){
        flag = false;
    }
    return flag;
}
}
