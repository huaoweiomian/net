#ifndef CHLQ_H
#define CHLQ_H
#include<pthread.h>
#include<list>
using std::list;
#include "channel.h"
class CHLQ
{
public:
    CHLQ();
    bool init();
    CHANNEL* read();
    CHANNEL* block_read();
    void write(CHANNEL* v);
private:
    list<CHANNEL*> cl;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_t cmutex;
};

#endif // CHLQ_H
