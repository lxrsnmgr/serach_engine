#include "../include/cache/redis.h"
#include "../include/common/configuration.h"
#include "../include/common/logger.h"
#include <cstdint>

namespace simple{
using sw::redis::OptionalString;

RedisCache::RedisCache(Logger& logger, IConfiguration& conf)
: _logger{logger}
, _conf{conf}
, _redis{_conf.GetConfig()["redis.init"]}
, _ttl{std::atoi(_conf.GetConfig()["redis.ttl"].c_str())}{
    LOG_INFO("Redis cache connected");
}

string RedisCache::DoQuery(const string& key){
    OptionalString value = _redis.get(key);
    if(value){
        //存在
        LOG_INFO("Query key:\"{}\" Cache hit", key);
        _redis.expire(key, _ttl);
        return value.value();
    }
    //不存在，返回空字符串
    std::cout << "hello" << std::endl;
    LOG_INFO("Query key:\"{}\" Cache missed", key);
    return "";
}

void RedisCache::AddData(const string& key, const string& value){
    _redis.setex(key, _ttl, value);
    LOG_INFO("New data added to cache. key:\"{}\"", key);
}
}
