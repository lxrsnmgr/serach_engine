#ifndef __WEB_SERVER_H__
#define __WEB_SERVER_H__
#include "../common/configuration.h"
#include "../common/logger.h"
#include "../key/dict_producer.h"
#include "../key/dictionary.h"
#include "../key/key_recommander.h"
#include "../key/mytimer.h"
#include "../reactor/tcp_server.h"
#include "../reactor/thread_pool.h"
#include "../cache/redis.h"
#include "../web/web_page_searcher.h"
#include <iostream>
#include <unistd.h>

namespace simple{
using std::cout;
using std::endl;
using std::string;

struct Setting{
    Setting(Dictionary &dict, KeyRecommander &kr, PageLibPre)
}
}
#endif
