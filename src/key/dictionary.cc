#include "../include/key/dictionary.h"
#include <cstdio>
#include <vector>

namespace simple{
Dictionary::Dictionary(Logger& logger, IConfiguration &config) noexcept
    : _dict_en{}
    , _dict_zh{}
    , _index_en{}
    , _index_zh{}
    , _logger{logger}
    , _config{config}{
        ReadDict();
        ReadIndex();
    }

vector<pair<string, int>> &Dictionary::GetEnDict(){
    return _dict_en;
}

vector<pair<string, int>> &Dictionary::GetCnDict(){
    return _dict_zh;
}

map<string, set<int>> &Dictionary::GetEnIndex(){
    return _index_en;
}

map<string, set<int>> &Dictionary::GetCnIndex(){
    return _index_zh;
}

unordered_map<int, int> Dictionary::DoQuery(const string& key){
    int char_len = CharLen(key);
    unordered_map<int, int> line_freq_map;

    set<string> tmp_set;
    //遍历字母
    for(int i = 0; i < key.size(); i += char_len){
        string ch = string(key, i, char_len);
        tmp_set.insert(ch);
    }
    //统计频数
    for(const auto & ch : tmp_set){
        if(CharLen(ch) == 3){
            for(const auto &elem : _index_zh[ch]){
                ++line_freq_map[elem];
            }
        } else if(CharLen(ch) == 1){
            for(const auto &elem : _index_en[ch]){
                ++line_freq_map[elem];
            }
        }
    }
    return line_freq_map;
}

int Dictionary::CharLen(const string &c){
    char firstbyte = c.c_str()[0];
    int ret = -1;
    if((firstbyte & 0x80) == 0x0){
        ret = 1;
    } else if((firstbyte & 0xE0) == 0xE0){
        ret = 3;
    }
    return ret;
}

int Dictionary::ReadDict(){
    vector<string> filenamevec = {
        _config.GetConfig()["dict_eng"],
        _config.GetConfig()["dict_zh"]
    };
    for(const auto &filename : filenamevec){
        ifstream ifs(filename);
        if(!ifs){
            LOG_ERROR("open {} fail.", filename);
            return -1;
        }
        string line;
        string word;
        int freq;
        while(getline(ifs, line)){
            istringstream iss(line);
            iss >> word >> freq;

            //读取词典
            if(filename == _config.GetConfig()["dict_eng"]){
                _dict_en.emplace_back(make_pair(word, freq));
            } else {
                _dict_zh.emplace_back(make_pair(word, freq));
            }
        }
        ifs.close();
    }
    return 0;
}

int Dictionary::ReadIndex(){
    vector<string> filenamevec = {
        _config.GetConfig()["index_eng"],
        _config.GetConfig()["index_zh"]
    };
    for(const auto & filename : filenamevec){
        ifstream ifs(filename);
        if(!ifs){
            LOG_ERROR("open {} fail", filename);
            return -1;
        }
        string line;
        string ch;
        while(getline(ifs, line)){
            istringstream iss(line);
            iss >> ch;
            int index;
            set<int> indice_set;
            while(iss >> index){
                indice_set.insert(index);
            }

            //读取索引
            if(filename == _config.GetConfig()["index_eng"]){
                _index_en.insert(make_pair(ch, indice_set));
            } else {
                _index_zh.insert(make_pair(ch, indice_set));
            }
        }
        ifs.close();
    }
    return 0;
}
}
