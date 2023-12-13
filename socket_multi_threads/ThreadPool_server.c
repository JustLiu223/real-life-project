#include "ThreadPool.h"

struct  SockInfo
{
    /* data */
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
};
typedef struct PoolInfo
{
    ThreadPool* p;
    int fd;
}PoolInfo;
void acceptConn(void* arg);
void working(void* arg){
    struct SockInfo* info = (struct SockInfo*)arg;
    char ip[32];
    printf("client ip: %s,port: %d\n",inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(info->addr.sin_port));
    //和客户端连接
    while(1){
        
        char buf[1024];
        int ret = read(info->fd,buf,sizeof(buf));
        if(ret == 0){
            printf("client disconnect...\n");
            break;
        }
        else if (ret == -1){
            printf("recv data falied");
            break;
        }
        else{
            printf("client say: %s\n",buf);
            write(info->fd,buf,strlen(buf)+1);
        }
        
    }
    close(info->fd);
}
int main(){
    //创建监听的套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    //将socket()的返回值与本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(lfd,(struct sockaddr*)&addr,sizeof(addr));
    //设置监听
    ret = listen(lfd,128);
    
    //创建线程池
    ThreadPool *pool = threadPoolCreate(3,8,100);
    PoolInfo* pInfo = (PoolInfo*)malloc(sizeof(PoolInfo));
    pInfo->p = pool;
    pInfo->fd = lfd;
    threadPoolAdd(pool,acceptConn,pInfo);
    pthread_exit(NULL);
    return 0;
}
//子线程任务函数
void acceptConn(void* arg){
    PoolInfo* poolInfo = (PoolInfo*)arg;
    int len = sizeof(struct sockaddr_in);
    while (1)
    {
        struct SockInfo* pinfo;
        pinfo = (struct SockInfo*)malloc(sizeof(struct SockInfo));
        pinfo->fd = accept(poolInfo->fd,(struct sockaddr*)&pinfo->addr,&len);
        printf("parent thread connfd: %d\n",pinfo->fd);
        if(pinfo->fd ==-1){
            perror("accept");
            exit(0);
        }
        //添加通信的任务
        threadPoolAdd(poolInfo->p,working,pinfo);
    }
    close(poolInfo->fd);
}
