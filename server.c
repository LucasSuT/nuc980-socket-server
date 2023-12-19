#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include "led_cgi.c"

int port = 7000;

typedef struct Command{
    unsigned int dev : 8;
    unsigned int dev_num : 7;
    unsigned int value : 1;
}command;

command ParseCmd(char* s,int len)
{
    command cmd;
    int cmd_hex;

    cmd_hex = strtol(s,NULL,16);
    printf("cmd_hex = 0x%x \n",cmd_hex);
    cmd.dev = cmd_hex>>8;
    printf("dev = 0x%x\n",cmd.dev);
    cmd.dev_num = cmd_hex>>1;
    printf("dev_num = 0x%x \n",cmd.dev_num);
    cmd.value = cmd_hex;
    printf("value = 0x%x\n",cmd.value);
    return cmd;
}

void Order(char* s,int len)
{
    command cmd;

    cmd = ParseCmd(s,len);
    switch(cmd.dev)
    {
        case 0x5:
        {
            SetLedValue(cmd.dev_num,cmd.value);
            break;
        }        
        default:
            printf("cmd error\n");
            break;
    }
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, n;
    socklen_t addrlen;
    struct sockaddr_in serv_addr, client_addr; 

    char sendBuff[1025], recvBuff[1024];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
    {
        printf("\n Error : Could not create socket \n");
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&client_addr, '0', sizeof(client_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("\n Error : Binding error \n");
    }

    if(listen(listenfd, 10) == -1)
    {
        printf("\n Error : Listening error \n");
    } 
    char server_ip[22];
    inet_ntop(AF_INET, &serv_addr.sin_addr, server_ip, sizeof(server_ip));
    printf("\n server start at: %s:%d \n",server_ip, port);
    printf("\n wait for connection... \n");
    addrlen = sizeof(client_addr);
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &addrlen); 
        char client_ip[22];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("connected by %s:%d\n", client_ip,ntohs(client_addr.sin_port));
        
        while(1){
            n = read(connfd, recvBuff, sizeof(recvBuff)-1);
	        if(n < 0)
    	    {
                printf("\n client close connection. \n");
                close(connfd);
	            break;
    	    }
            recvBuff[n] = 0;
            printf("recv: %s\n", recvBuff);
            Order(recvBuff,n+1);
            snprintf(sendBuff, sizeof(sendBuff), "echo %s", recvBuff);
            write(connfd, sendBuff, strlen(sendBuff)); 
        }
     }
     close(connfd);
     sleep(1);
}
