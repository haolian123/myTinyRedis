#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H
#include <iostream>
#include <sys/socket.h> // 提供socket函数及数据结构
#include <netinet/in.h> // 定义数据结构sockaddr_in
#include <arpa/inet.h> // 提供IP地址转换函数
#include <unistd.h> // 提供close函数
#include <cstring> // 提供memset函数
#include <string>

#define DEFAULT_HOSTNAME "127.0.0.1"
#define DEFAULT_PORT 8888

class RedisClient{
private:
    int clientSocket;
    struct sockaddr_in server;
    std::string message;
    char serverMessage[2000];
    ssize_t recvSize;
    std::string hostName="";
    int port;
    std::string PORT="";
public:
    RedisClient(const std::string& Host, int port);
    RedisClient();
    void start();
};

#endif 
