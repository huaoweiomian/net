#include "chlq.h"

CHLQ::CHLQ()
{

}

bool CHLQ::init()
{
    if(0 != pthread_cond_init(&cond, NULL))
        return false;
    if(0 != pthread_mutex_init(&mutex,NULL))
        return false;
    if(0 != pthread_mutex_init(&cmutex,NULL))
        return false;
    return true;
}

CHANNEL* CHLQ::read()
{
    pthread_mutex_lock(&mutex);
    if (cl.empty()){
        return NULL;
    }
    CHANNEL* ret = cl.front();
    cl.pop_front();
    pthread_mutex_unlock(&mutex);
    return ret;
}

CHANNEL *CHLQ::block_read()
{
    pthread_mutex_lock(&mutex);
    if (!cl.empty()){
        CHANNEL* ret = cl.front();
        cl.pop_front();
        pthread_mutex_unlock(&mutex);
        return ret;
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_wait(&cond, &cmutex);
    pthread_mutex_lock(&mutex);
    CHANNEL* ret = cl.front();
    cl.pop_front();
    pthread_mutex_unlock(&mutex);
    return ret;
}

void CHLQ::write(CHANNEL *v)
{
    pthread_mutex_lock(&mutex);
    cl.push_back(v);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
}
