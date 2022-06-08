#ifndef BUFFER_H
#define BUFFER_H
#include <stdio.h>
#include <string.h>
class BUFFER
{
public:
    BUFFER();
    BUFFER(int size, char* b);
    BUFFER(const BUFFER& b);
    BUFFER& operator=(const BUFFER& b);
    ~BUFFER();
    void append(int s, char* b);
    void* buff_f()const{
        return buff;
    }

    int sizef()const{
        return size;
    }
    void clear();
private:
    int size;
    char* buff;

};

#endif // BUFFER_H
