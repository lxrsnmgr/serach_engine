#ifndef __CACHE_MANAGER_H__
#define __CACHE_MANAGER_H__

#include "lru_cache.h"
#include <vector>
#include <string>

namespace simple{
using std::vector;
using std::string;

class CacheManager{
public:
    void Init(const string &file);
    LruCache& GetCache(int);
    void UpdateCache();

private:
    vector<LruCache> _cache;
};
}
#endif
