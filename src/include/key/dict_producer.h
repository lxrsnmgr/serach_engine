#ifndef __DICT_PRODUCER_H__
#define __DICT_PRODUCER_H__
#include "../common/configuration.h"
#include "../common/logger.h"
#include "../common/split_tool.h"
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <string>

namespace simple{
using std::ifstream;
using std::vector;
using std::string;
using Type = enum{CN, ENG};

class DictProducer{
public:
    explicit DictProducer(Logger& logger, IConfiguration &conf);
    void BuildEnDict();
    void BuildCnDict();
    void CreateIndex(Type type);
    int Store(Type type);
    void GetFileName(vector<string> &vec, string & path);
    void ReadStopWord(set<string> & stopword, const string &stopwordpath);

private:
    static void PreProcess(string &word);
    static bool IsAlphaNumber(string &word);
    size_t GetIdx(string &word);

    Logger & _logger;
    IConfiguration& _conf;
    SplitTool _st;
    std::vector<std::string> _file;
    std::vector<std::pair<std::string, int>> _dict;
    std::map<std::string, std::set<int>> _index;
};
}
#endif
