#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
class ChatServer{

public:
    ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg):_server(loop, listenAddr, nameArg), _loop(loop)
    {
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
        _server.setThreadNum(4);
    }
    void start(){
        _server.start();
    }
private:
    void onConnection(const TcpConnectionPtr &conn){
        if(conn->connected()){
            cout << conn->peerAddress().toIpPort() << " -> " <<
                conn->localAddress().toIpPort() << " state:online" << endl;
        }else{
             cout << conn->peerAddress().toIpPort() << " -> " <<
                conn->localAddress().toIpPort() << " state:offline" << endl; 
                conn->shutdown();          
        }
    }
    void onMessage(const TcpConnectionPtr &conn,
                            Buffer *buffer,
                            Timestamp time){
                               
    string buf = buffer->retrieveAllAsString();
    cout << "recv data:" << buf << " time: " << time.toString() << endl;
    conn->send(buf);
    }
    TcpServer _server;
    EventLoop *_loop;
};
int main(){

    EventLoop loop;
    InetAddress addr("0.0.0.0", 8000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();
    return 0;
}