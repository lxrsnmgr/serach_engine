#ifndef __REDIS_H__
#define __REDIS_H__
#include "../common/configuration.h"
#include <sw/redis++/redis++.h>

namespace simple{
using std::string;
using sw::redis::Redis;

class RedisCache{
public:
    RedisCache(Logger& logger, IConfiguration& conf);
    string DoQuery(const string& key);
    void AddData(const string& key, const string& value);

private:
    Logger& _logger;
    IConfiguration& _conf;
    Redis _redis;
    int64_t _ttl;
};
}
#endif
