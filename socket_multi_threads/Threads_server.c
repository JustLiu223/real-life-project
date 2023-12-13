#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
struct  SockInfo
{
    /* data */
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
};
struct SockInfo infos[128];

void* working(void* arg){
    //和客户端连接
    while(1){
        struct SockInfo* info = (struct SockInfo*)arg;
        char buf[1024];
        int ret = read(info->fd,buf,sizeof(buf));
        if(ret == 0){
            printf("client disconnect...\n");
            info->fd = -1;
            break;
        }
        else if (ret == -1){
            printf("recv data falied");
            info->fd = -1;
            break;
        }
        else{
            printf("client say: %s\n",buf);
            write(info->fd,buf,strlen(buf)+1);
        }
        
    }
    return NULL;
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
    //阻塞等待并接受客户端连接
    int len = sizeof(struct sockaddr);
    //数据初始化
    int max = sizeof(infos)/sizeof(infos[0]);
    for(int i=0;i<max;++i){
        bzero(&infos[i],sizeof(infos[i]));
        infos[i].fd =-1;
        infos[i].tid = -1;
    }
    //父进程监听，子进程通信
    while (1)
    {
        /* code */
        //创建子线程
        struct SockInfo* pinfo;
        for(int i=0;i<max;i++){
            if(infos[i].fd == -1){
                pinfo = &infos[i];
                break;
            }
            if(i == max-1){
                sleep(1);
                i--;
            }
        }
        int connfd = accept(lfd,(struct sockaddr*)&pinfo->addr,&len);
        printf("parent thread connfd: %d\n",connfd);
        if(connfd ==-1){
            perror("accept");
            exit(0);
        }
        pinfo->fd = connfd;
        pthread_create(&pinfo->tid,NULL,working,pinfo);
        pthread_detach(pinfo->tid);
    }
    close(lfd);
    return 0;
}
