#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "../common/logger.h"
#include "../common/configuration.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <utility>

namespace simple{
using std::string;
using std::vector;
using std::set;
using std::map;
using std::unordered_map;
using std::pair;
using std::ifstream;
using std::istringstream;

class Dictionary{
public:
    Dictionary(Logger &logger, IConfiguration &config) noexcept;
    unordered_map<int, int> DoQuery(const string &key);
    vector<pair<string, int>> &GetEnDict();
    vector<pair<string, int>> &GetCnDict();
    map<string, set<int>> &GetEnIndex();
    map<string, set<int>> &GetCnIndex();
    static int CharLen(const string &c);

private:
    int ReadDict();
    int ReadIndex();

    Logger& _logger;
    IConfiguration& _config;
    vector<pair<string, int>> _dict_en;
    vector<pair<string, int>> _dict_zh;
    map<string, set<int>> _index_en;
    map<string, set<int>> _index_zh;
};
}
#endif
