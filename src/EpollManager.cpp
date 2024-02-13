
#include"EpollManager.h"

EpollManager::EpollManager(int maxEvent):
epollFd(epoll_create(512)),events(maxEvent)
{
    assert(epollFd>=0&&events.size()>0);
}

EpollManager::~EpollManager(){
    close(epollFd);
}

bool EpollManager::addFd(int fd, uint32_t events){
    if(fd<0){
        return false;
    }
    epoll_event epollEvent={0};
    epollEvent.data.fd=fd;
    epollEvent.events=events;
    return epoll_ctl(epollFd,EPOLL_CTL_ADD,fd,&epollEvent)==0;
}

bool EpollManager::modifyFd(int fd, uint32_t events) {
    if (fd < 0) {
        return false;
    }
    epoll_event epollEvent = {0};
    epollEvent.data.fd = fd;
    epollEvent.events = events;
    return epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &epollEvent) == 0; 
}

bool EpollManager::deleteFd(int fd) {
    if (fd < 0) {
        return false;
    }
    epoll_event epollEvent = {0};
    return epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, &epollEvent) == 0; 
}


int EpollManager::wait(int timeout){
    return epoll_wait(epollFd,&events[0],static_cast<int>(events.size()),timeout);
}

int EpollManager::getEventFd(size_t i) const{
    assert(i<events.size()&&i>=0);
    return events[i].data.fd;
}

uint32_t EpollManager::getEvents(size_t i) const{
    assert(i<events.size()&&i>=0);
    return events[i].events;
}