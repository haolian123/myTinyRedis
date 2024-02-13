#ifndef EPOLL_MANAGER
#define EPOLL_MANAGER

#include<iostream>
#include<sys/epoll.h>
#include<fcntl.h>
#include<vector>
#include<errno.h>
#include<unistd.h>
#include<cassert>
#define MAXEVENT 1024

class EpollManager {
private:
    int epollFd;                            // epoll 文件描述符
    std::vector<struct epoll_event> events;  // epoll 事件集合

public:
    // 构造函数，默认参数为最大事件数 MAXEVENT
    EpollManager(int maxEvent = MAXEVENT);

    // 析构函数
    ~EpollManager();

    // 添加文件描述符和事件到 epoll 实例中
    bool addFd(int fd, uint32_t events);

    // 修改文件描述符的关注事件
    bool modifyFd(int fd, uint32_t events);

    // 从 epoll 实例中删除文件描述符
    bool deleteFd(int fd);

    // 等待 epoll 事件的发生，可指定超时时间
    int wait(int timeout = -1);

    // 获取指定事件的文件描述符
    int getEventFd(size_t i) const;

    // 获取指定事件的事件类型
    uint32_t getEvents(size_t i) const;
};

#endif 