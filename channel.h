#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/socket.h>
#include <pthread.h>
#include "buffer.h"
#include <string> //EAGAIN
class CHANNEL
{
public:
    CHANNEL(int fd);
    bool init();
    void read_from_client();
    void read_from_self(BUFFER& b);
    void write_to_self(const BUFFER& b);
private:
    //{{
    CHANNEL();//不允许默认构造
    CHANNEL(const CHANNEL&);//不允许拷贝构造
    CHANNEL& operator=(const CHANNEL&);//不允赋值
    //预期是只能使用指针或引用，且每一个生产出来的频道是唯一的。
    //}}
    BUFFER buff;
    int fd;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_t cmutex;
};

#endif // CHANNEL_H
