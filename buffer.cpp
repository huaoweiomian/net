#include "buffer.h"

BUFFER::BUFFER()
{
    size = 0;
    buff = NULL;
}

BUFFER::BUFFER(int size, char *b)
{
    this->size = size;
    buff = new char[size];
    memcpy(buff, b, size);
}

BUFFER::BUFFER(const BUFFER &b)
{
    size = b.size;
    buff = new char[size];
    memcpy(buff, b.buff, size);
}

BUFFER &BUFFER::operator=(const BUFFER &b)
{
    size = b.size;
    buff = new char[size];
    memcpy(buff, b.buff, size);
    return *this;
}

BUFFER::~BUFFER()
{
    clear();
}

void BUFFER::append(int s, char* b)
{
    char* btmp = new char[s+size];
    memcpy(btmp, buff,size);
    memcpy(btmp+size,b,s);
    delete[] buff;
    buff = btmp;
    size += s;
}

void BUFFER::clear()
{
    if(size == 0)
        return;
    size = 0;
    delete[] buff;
    buff = NULL;
}



