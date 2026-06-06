#ifndef __ECHO_SERVER_H__
#define __ECHO_SERVER_H__

#include "event_loop.h"
#include "tcp_connection.h"
#include "tcp_server.h"
#include "thread_pool.h"
#include <iostream>
#include <unistd.h>

namespace simple{
using std::cout;
using std::endl;
using std::string;

class MyTask{
public:
    MyTask(const string& msg, const TcpConnectionPtr& con)
        : _msg(std::move(msg))
          , _con(std::move(con)){

          }

    void Process() const{
        _con->SendInLoop(_msg);
    }

private:
    string _msg;
    TcpConnectionPtr _con;
};

class EchoServer{
public:
    EchoServer(size_t threadNum, size_t queSize, const string& ip, unsigned short port)
        : _pool(threadNum, queSize)
        , _server(ip, port){}

    void Start(){
        _pool.Start();
        _server.SetAllCallback(
            [](auto&& PH1){OnConnection}
                              )
    }

    void Stop(){
        _pool.Stop();
        _server.Stop();
    }

    static void OnConnection(const TcpConnectionPtr& con){
        cout << con->ToString() << "has connected" << endl;
    }

    void OnMessage(const TcpConnectionPtr& con){
        string msg = con->Receive();
        cout << "recv msg" << msg << endl;

        MyTask task(msg, con);
        _pool.AddTask([task]{task.Process();});
    }

    static void Onclose(const TcpConnectionPtr& con){
        cout << con->ToString() << "has closed" << endl;
    }

private:
    ThreadPool _pool;
    TcpServer _server;
};
}
#endif
