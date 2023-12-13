#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main(){
    //创建连接的套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    //将socket()的返回值与本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    inet_pton(AF_INET,"172.24.29.43",&addr.sin_addr.s_addr);
    // addr.sin_addr.s_addr = INADDR_ANY;
    int ret = connect(lfd,(struct sockaddr*)&addr,sizeof(addr));
    int number=0;
    //和服务端通信
    while(1){
        //接收数据
        char buf[1024];
        
        sprintf(buf,"hello world %d\n",number++);
        send(lfd,buf,strlen(buf)+1,0);
        memset(buf,0,sizeof(buf));
        int len = read(lfd,buf,sizeof(buf));
        if(len>0){
            printf("server say: %s\n",buf);
        }
        else if(len ==0){

            printf("server disconnect...\n");
            break;
        }else{
            perror("read");
            break;
        }
        sleep(1);
    }
    close(lfd);
    return 0;
}
