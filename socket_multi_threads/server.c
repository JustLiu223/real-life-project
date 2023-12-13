#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
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
    struct sockaddr_in cliaddr;
    int cilen = sizeof(cliaddr);
    int cfd = accept(lfd,(struct sockaddr*)&cliaddr,&cilen);
    //和客户端连接
    while(1){
        //接收数据
        char buf[1024];
        memset(buf,0,sizeof(buf));
        int len = read(cfd,buf,sizeof(buf));
        if(len>0){
            printf("client say: %s\n",buf);
            write(cfd,buf,len);
        }
        else if(len ==0){

            printf("client disconnect...\n");
            break;
        }else{
            perror("client read");
            break;
        }
    }
    close(cfd);
    close(lfd);
    return 0;
}
