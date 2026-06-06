#ifndef __SPLIT_TOOL_H__
#define __SPLIT_TOOL_H__

#include "configuration.h"
#include "logger.h"
#include "cppjieba/Jieba.hpp"
#include <cppjieba/MixSegment.hpp>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

namespace simple{
using std::set;
using std::string;
using std::vector;
using cppjieba::Jieba;
using cppjieba::MixSegment;
using std::unordered_map;
class SplitTool{
public:
    explicit SplitTool(Logger& logger, IConfiguration& config);
    void Cut(const string &str, vector<string> &wordsvec, bool flag);
    void Cut(const string &str, unordered_map<string, uint64_t> &wordsmap, bool flag);

private:
    void ReadStopWord(set<string> &stopword, const string &stopwordpath);

    Logger& _logger;
    IConfiguration& _config;
    cppjieba::MixSegment _seg;
    cppjieba::Jieba _jieba;
};
}
#endif
