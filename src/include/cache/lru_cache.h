#ifndef __LRU_CACHE_H__
#define __LRU_CACHE_H__

#include <string>
#include <unordered_map>
#include <list>

namespace simple{
using std::list;
using std::string;
using std::unordered_map;
using std::pair;

class LruCache{
public:
    explicit LruCache(int cache_size = 1000);
    void ReadFromFile(const string& file);
    void AddRecord(const string &key, const string &result);
    void Update(const LruCache &);
    void WriteToFile(const string &file);

    void LruTest();

private:
    void AddFront(const string &key, const string &value);
    void DeleteRear();
    void MoveToFront(const string &key);
    void AddToPending(const string &key, const string &value);

    list<pair<string, string>> _result_list;
    unordered_map<string, list<pair<string, string>>::iterator> _hash_map;
    unordered_map<int, list<pair<string, string>>::iterator> _freq_map;
    list<pair<string, string>> _pending_update_list;
    int _cache_size;
    int _min_freq;
};
}
#endif
