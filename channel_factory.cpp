#include "channel_factory.h"

CHANNEL_FACTORY::CHANNEL_FACTORY()
{
    sockfd = 0;
}

CHLQ* CHANNEL_FACTORY::init(std::string &ip, int port, int accept_q_size)
{
    //1.创建tcp监听套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd){
        return NULL;
    }
    //2.绑定sockfd
    struct sockaddr_in my_addr;
    memset(&my_addr,0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    int ret = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if(-1 == ret){
        close(sockfd);
        return NULL;
    }
    //3.监听listen
    ret = listen(sockfd, accept_q_size);
    if(-1 == ret){
        close(sockfd);
        return NULL;
    }
    epfd = epoll_create(1); // 创建一个 epoll 的句柄，参数要大于 0， 没有太大意义
    pthread_create(&epoll_wait_id, NULL, run_epoll, this);
    pthread_create(&accept_wait_id, NULL, run_accept, this);
    if(chlq.init()==false)
        return NULL;
    return &chlq;
}

CHANNEL* CHANNEL_FACTORY::produce_channel()
{
    struct sockaddr_in my_addr;
    socklen_t len = sizeof (my_addr);
    int newfd = accept(sockfd,(struct sockaddr*)&my_addr,&len);
    CHANNEL* pchl = new CHANNEL(newfd);
    if(pchl==NULL&&!pchl->init()){
        return NULL;
    }
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = newfd; //监听套接字
    event.events = EPOLLIN|EPOLLET; // 表示对应的文件描述符可以读
    event.data.ptr = pchl;
    //5.事件注册函数，将监听套接字描述符 sockfd 加入监听事件
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &event);
    if(ret != 0)
        return NULL;
    return pchl;
}

void CHANNEL_FACTORY::del_connect(CHANNEL *chl)
{
    delete chl;
}

void CHANNEL_FACTORY::epoll_wait_data()
{
    epoll_event wait_event; //内核监听完的结果
    memset(&wait_event,0,sizeof (wait_event));
    while(1)
    {
        // 监视并等待多个文件（标准输入，udp套接字）描述符的属性变化（是否可读）
        // 没有属性变化，这个函数会阻塞，直到有变化才往下执行，这里没有设置超时
        int count = epoll_wait(epfd, &wait_event, 1, -1);
        if(count < 1){
            cout<<"error:"<<errno<<endl;
            continue;
        }
        CHANNEL* ptr = (CHANNEL*)wait_event.data.ptr;
        ptr->read_from_client();
    }
}


void *run_accept(void *arg)
{
    CHANNEL_FACTORY* p = (CHANNEL_FACTORY*)arg;
    while(true){
        CHANNEL* chl = p->produce_channel();
        if(chl == NULL)
            continue;
        p->chlq.write(chl);
    }
    return NULL;
}

void *run_epoll(void *arg)
{
    CHANNEL_FACTORY* p = (CHANNEL_FACTORY*)arg;
    p->epoll_wait_data();
    return NULL;
}
