#ifndef __WEB_PAGE_SEARCHER_H__
#define __WEB_PAGE_SEARCHER_H__
#include "page_lib_preprocessor.h"
#include "web_page.h"
#include "web_page_query.h"
#include "../common/logger.h"
#include "../reactor/tcp_connection.h"
#include "../cache/redis.h"
#include <type_traits>

namespace simple{
using TcpConnectionPtr = shared_ptr<TcpConnection>;
class WebPageSearcher{
public:
    explicit WebPageSearcher(string sought, TcpConnectionPtr conn,
                             const PageLibPreprocessor& pre, Logger& logger,
                             IConfiguration& config, RedisCache& cache);
    string DoQuery() const;

    void Process() const{
        //在process函数中去进行真正的业务逻辑的处理
        LOG_INFO("{} search sought: {}", _tcp_connection->GetPeerAddrString(), _sought);
        //msg是要发送给client的内容
        string result = _cache.DoQuery(_sought);
        if(result.empty()){
            result = DoQuery();
            _cache.AddData(_sought, result);
        }
        _tcp_connection->SendInLoop(result);
    }

private:
    string _sought;
    TcpConnectionPtr _tcp_connection;
    const PageLibPreprocessor& _pre;
    Logger &_logger;
    IConfiguration &_config;
    RedisCache &_cache;
};
}
#endif
