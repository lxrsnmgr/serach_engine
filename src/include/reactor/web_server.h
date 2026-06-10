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
#include "event_loop.h"
#include <iostream>
#include <unistd.h>

namespace simple{
using std::cout;
using std::endl;
using std::string;

struct Setting{
    Setting(Dictionary &dict, KeyRecommander &kr, PageLibPreprocessor &plp, RedisCache &cache)
        : dict{std::move(dict)},
          key_recommander{std::move(kr)},
          page_lib_preprocessor{std::move(plp)},
          cache{std::move(cache)}{}

    Dictionary dict;
    KeyRecommander key_recommander;
    PageLibPreprocessor page_lib_preprocessor;
    RedisCache cache;
};

class MyTask{
public:
    MyTask(string &msg, const TcpConnectionPtr &con, KeyRecommander &key_recommand, Logger &logger)
        : _msg(std::move(msg)), _con(std::move(con)), _kr(key_recommand), _logger(logger){}

    void Process() const{
        LOG_INFO("{} key word query: {} in process", _con->GetPeerAddrString(), _msg);
        //msg是要发送给client的内容
        _kr.Query(_msg);
        string msg;
        msg.append((">>>>>\n")).append(_kr.GetString()).append("<<<<<\n\n");
        _con->SendInLoop(msg);
    }
private:
    string _msg;
    TcpConnectionPtr _con;
    Logger &_logger;
    KeyRecommander& _kr;
};

class WebServer{
public:
    WebServer(size_t threadNum, size_t queSize, const string &ip,
              unsigned short port, Logger &logger, IConfiguration &config,
              Setting &setting)
        : _pool(threadNum, queSize),
          _server(ip, port),
          _logger(logger),
          _config(config),
          _setting(setting){}

    void Start(){
        _pool.Start();
        _server.SetAllCallback(
            [this](auto&& PH1){OnConnection(std::forward<decltype(PH1)>(PH1));},
            [this](auto&& PH1){OnMessage(std::forward<decltype(PH1)>(PH1));},
            [this](auto&& PH1){OnClose(std::forward<decltype(PH1)>(PH1));});
        _server.Start();
    }

    void stop(){
        _pool.Stop();
        _server.Stop();
    }

    void OnConnection(const TcpConnectionPtr &con){
        string logmsg{con->ToString() + " has connected"};
        LOG_INFO("Connection message: {}", logmsg);
    }

    void OnMessage(const TcpConnectionPtr &con){
        string msg = con->Receive();
        msg.erase(msg.size() - 1, 1);
        cout << "recv msg " << msg << endl;

        WebPageSearcher task(msg, con, _setting.page_lib_preprocessor, _logger,
                             _config, _setting.cache);

        _pool.AddTask(
                      [task]{task.Process();});
    }

    void OnClose(const TcpConnectionPtr &con){
        string logmsg{con->ToString() + " has closed"};
        LOG_INFO("Close message: {}", logmsg);
    }
private:
    ThreadPool _pool;
    TcpServer _server;
    Logger &_logger;
    IConfiguration &_config;
    Setting &_setting;
};
}
#endif
