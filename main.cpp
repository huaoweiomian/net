#include <QCoreApplication>
#include <iostream>
#include <string>
using namespace std;
#include "channel_factory.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CHANNEL_FACTORY cf;
    string ip("192.168.106.148");
    CHLQ* chlq = cf.init(ip,6666,10);
    cout<<"init channel factory:"<<(chlq != NULL)<<endl;
    if(chlq == NULL){
        cout<<"over!"<<endl;
        return a.exec();
    }

    BUFFER buf;
    int flag=0;
    CHANNEL* chl = NULL;
    while(true){
        cout<<"0:start new round;1:show client message;2:over;3:write to chl;"<<endl;
        cin>>flag;
        switch (flag) {
        case 0:
            break;
        case 1:
            chl = chlq->block_read();
            chl->read_from_self(buf);
            cout<<string((char*)buf.buff_f())<<endl;
            break;
        case 2:
            goto OUTER;
        case 3:
            if(chl != NULL)
                chl->write_to_self(buf);
        }
    }
    OUTER:
    cout<<"over!"<<endl;
    return a.exec();
}
