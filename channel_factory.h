#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>        //read,write,close(fd)
#include <string.h>
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
#include "channel.h"
#include <sys/epoll.h>
#include <pthread.h>
#include "chlq.h"
class CHANNEL_FACTORY
{
public:
    CHANNEL_FACTORY();
    CHLQ* init(string& ip, int port, int accept_q_size);
    void del_connect(CHANNEL* chl);
    friend void* run_epoll(void* arg);
    friend void* run_accept(void* arg);
private:
    int epfd;
    int sockfd;
    pthread_t epoll_wait_id;
    pthread_t accept_wait_id;
    CHLQ chlq;
    void epoll_wait_data();
    void accept_wait_data();
    CHANNEL* produce_channel();
};
void* run_epoll(void* arg);
void* run_accept(void* arg);
#endif // TCPSERVER_H
