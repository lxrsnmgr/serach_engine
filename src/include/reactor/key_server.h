#ifndef __KEY_SERVER_H__
#define __KEY_SERVER_H__

#include "../common/configuration.h"
#include "../common/logger.h"
#include "../key/dict_producer.h"
#include "../key/dictionary.h"
#include "../key/key_recommander.h"
#include "../reactor/event_loop.h"
#include "../reactor/tcp_server.h"
#include "../reactor/thread_pool.h"
#include <ctime>
#include <iostream>
#include <limonp/Logging.hpp>
#include <unistd.h>
#include "../cache/redis.h"

namespace simple{
using std::cout;
using std::endl;
using std::string;

class MyTask{
public:
    MyTask(string& msg, const TcpConnectionPtr& con, KeyRecommander& kr,
           Logger& logger, RedisCache& cache)
        : _msg(std::move(msg))
        , _con(std::move(con))
        , _kr(kr)
        , _logger(logger)
        , _cache(cache){

        }

    void Process() const{
        LOG_INFO("{} key word query: {}", _con->GetPeerAddrString(), _msg);

        //msg是要发送给client的内容
        string result = _cache.DoQuery(_msg);
        if(result.empty()){
            _kr.Query(_msg);
            result.append((">>>>>\n")).append(_kr.GetString()).append("<<<<<\n\n");
            _cache.AddData(_msg, result);
        }

    }

private:
    string _msg;
    TcpConnectionPtr _con;
    KeyRecommander &_kr;
    Logger &_logger;
    RedisCache &_cache;
};

class KeyServer{
public:
    KeyServer(size_t threadNum, size_t queSize, const string& ip,
              unsigned short port, Logger& logger, IConfiguration &config,
              Dictionary &dict, KeyRecommander &kr, RedisCache &cache)
        : _pool(threadNum, queSize)
        , _server(ip, port)
        , _logger(logger)
        , _config(config)
        , _dict(dict)
        , _kr(kr)
        , _cache(cache){}

    void Start(){
        _pool.Start();//计算线程全部启动
        _server.SetAllCallback(
            [this](auto&& PH1){OnConnection(std::forward<decltype(PH1) > (PH1));},
            [this](auto&& PH1){OnMessage(std::forward<decltype(PH1) > (PH1));},
            [this](auto&& PH1){OnClose(std::forward<decltype(PH1) > (PH1));});
        _server.Start();
    }

    void Stop(){
        _pool.Stop();
        _server.Stop();
    }

    void OnConnection(const TcpConnectionPtr& con){
        cout << con->ToString() << " has connected" << endl;
        string logmsg;
        logmsg = con->ToString() + " has connected";
        LOG_INFO("connection message: {}", logmsg);
    }

    void OnMessage(const TcpConnectionPtr &con){
        string msg = con->Receive();
        msg.erase(msg.size() - 1, 1);
        cout << "recv msg " << msg << endl;
        MyTask task(msg, con, _kr, _logger, _cache);
        _pool.AddTask([task]{task.Process();});
    }

    void OnClose(const TcpConnectionPtr &con){
        cout << con->ToString() << " has closed" << endl;
        string logmsg;
        logmsg = con->ToString() + " has closed";
        LOG_INFO("close message: {}", logmsg);
    }
private:
    ThreadPool _pool;
    TcpServer _server;
    Logger& _logger;
    IConfiguration& _config;
    Dictionary& _dict;
    KeyRecommander& _kr;
    RedisCache& _cache;
};
}
#endif
