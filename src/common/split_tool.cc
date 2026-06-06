#include "../include/common/split_tool.h"

namespace simple{
SplitTool::SplitTool(Logger& logger, IConfiguration& config)
: _logger{logger}
, _config{config}
, _seg{_config.GetConfig()["jieba.dict"], _config.GetConfig()["hmm_model"]}
, _jieba{_config.GetConfig()["jieba.dict"],
         _config.GetConfig()["hmm_model"],
         _config.GetConfig()["user.dict"],
         _config.GetConfig()["idf"],
         _config.GetConfig()["stop_words"]}{

}

void SplitTool::Cut(const string& str, vector<string>& wordsvec, bool flag){
    if(flag){
        _jieba.Cut(str, wordsvec, true);
    } else {
        _seg.Cut(str, wordsvec);
    }
}

void SplitTool::Cut(const string& str, unordered_map<string, uint64_t>& wordsmap, bool flag){
    vector<string> tmpwordsvec;
    if(flag){
        _jieba.Cut(str, tmpwordsvec, true);
    } else {
        _seg.Cut(str, tmpwordsvec);
    }

    //加载停用词
    set<string> stopwordset;
    ReadStopWord(stopwordset, _config.GetConfig()["stop_word_zh"]);
    //遍历tmpwordsvec中的每一个单词(词语)
    for(const auto& elem : tmpwordsvec){
        //= 0表示不是停用词
        if(stopwordset.count(elem) == 0){
            auto iter = wordsmap.find(elem);
            if(iter != wordsmap.end()){
                ++iter->second; //如果当前单词在dictmap中，词频++
            } else {
                wordsmap[elem] = 1; //否则，词频=1
            }
        }
    }
}

void SplitTool::ReadStopWord(set<string>& stopword, const string& stopwordpath){
    ifstream ifs(stopwordpath);
    if(!ifs){
        string msg = string("open") + stopwordpath + string(" fail.\n");
        LOG_ERROR("open {} fail.", stopwordpath);
        return;
    }
    string line;
    while(ifs >> line){
        stopword.insert(line);
    }
    ifs.close();
}
}
