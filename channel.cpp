#include "channel.h"

CHANNEL::CHANNEL(int afd)
{
    fd = afd;

}

bool CHANNEL::init()
{
    if(0 != pthread_cond_init(&cond, NULL))
        return false;
    if(0 != pthread_mutex_init(&mutex,NULL))
        return false;
    if(0 != pthread_mutex_init(&cmutex,NULL))
        return false;
    return true;
}

void CHANNEL::read_from_client()
{
    pthread_mutex_lock(&mutex);
    char b[1024];
    int size = 1024;
    while(true){
        int count = recv(fd, b, size, MSG_DONTWAIT);
        if(count > 0 ){
            buff.append(count, b);
            continue;
        }
        if(count < 0)
        {
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&cond);
            return;
        }
        //EAGAIN,表示当前缓冲区已无数据可读
        if(errno == EAGAIN){
            break;
        }
        if(count == 0)
        {
            // 这里表示对端的socket已正常关闭.
            //fix me
        }
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
}

void CHANNEL::read_from_self(BUFFER &b)
{
    pthread_mutex_lock(&mutex);
    if(buff.sizef() != 0){
        b = buff;
        buff.clear();
        pthread_mutex_unlock(&mutex);
        return;
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_wait(&cond, &cmutex);
    pthread_mutex_lock(&mutex);
    b = buff;
    buff.clear();
    pthread_mutex_unlock(&mutex);
}

void CHANNEL::write_to_self(const BUFFER &b)
{
    int size = send(fd, b.buff_f(), b.sizef(), 0);
    if(size > 0){
    //发送了。暂时不考虑 size < b.sizef()的情况
    }else if(size == 0){
//表示对方主动关闭了连接过程
    }else{
        //有错误
    }
}
