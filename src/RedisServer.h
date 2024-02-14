#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <future>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <signal.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include <cstring> 
#include "ParserFlyweightFactory.h"
#include "EpollManager.h"
#include "ThreadPool.h"
class RedisServer {
private:
    std::unique_ptr<ParserFlyweightFactory> flyweightFactory;
    int serverSocket, newSocket;
    struct sockaddr_in server, client;
    socklen_t c;
    int port;
    std::atomic<bool> stop{false};
    pid_t pid;
    std::string logoFilePath;

    std::unique_ptr<EpollManager> epollManager; // epoll 管理器
    std::unique_ptr<ThreadPool> threadPool; // epoll 管理器
    uint32_t listenEvent=EPOLLET;   // 监听事件类型

private:
    RedisServer(int port = 8888, const std::string& logoFilePath = "logo");

    void handleClient(int clientSocket);
    static void signalHandler(int sig);
    void createSocket();
    void initServer();
    bool bindSocket();
    void printLogo();
    void printStartMessage();
    void replaceText(std::string &text, const std::string &toReplaceText, const std::string &replaceText);
    std::string getDate();
    int setFdNoBlock(int fd);
    void executeTransaction(std::queue<std::string>&commandsQueue,int clientSocket);
public:
    static std::shared_ptr<RedisServer> getInstance();
    void start();
};

#endif 
