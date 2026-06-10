#include "../include/web/web_page_searcher.h"
#include <utility>

namespace simple{
WebPageSearcher::WebPageSearcher(string sought, TcpConnectionPtr conn,
                                 const PageLibPreprocessor &pre, Logger &logger,
                                 IConfiguration &config, RedisCache &cache)
    : _sought{std::move(sought)},
      _tcp_connection{std::move(conn)},
      _pre{pre},
      _logger{logger},
      _config{config},
      _cache{cache}{}

string WebPageSearcher::DoQuery() const{
    WebPageQuery query(_pre, _logger, _config);
    return query.DoQuery(_sought);
}
}
