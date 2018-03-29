#include<winsock2.h>
#include<stdio.h>
#include<stdbool.h>
#include"common.h"
#include<pthread.h>
#include"list.h"
#define MAX_LENGTH_OF_NICKNAME 100
pthread_mutex_t lock ;
struct sockaddr_in createServerAddress(int port) {
    /**
        创建一个internet的address，address包含3部分：
        1，协议簇
        2，端口
        3，可以连接进来的ip
        addrin和sockaddr_in在oo语言中就是父类和之类的关系
    **/
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );
    address.sin_addr.s_addr = INADDR_ANY;
    return address;
}

void linstenTo(SOCKET socket, int lengthOfQueue) {
    //让socket转为被动状态，也就是监听进来的请求
    listen(socket, lengthOfQueue);
}

_Bool isClientForDisplay(struct sockaddr_in * clientAddress) {
    return ntohs(clientAddress->sin_port) == 7777;
}


struct sockAndAddr {
    SOCKET clientSocket;
    struct sockaddr_in clientAddress;
    struct sockAndAddr *ptr;
};

struct threadArgs {
    struct sockAndAddr socketAndAddress;
    char nickname[MAX_LENGTH_OF_NICKNAME];
};

void sendToClientsForDisplay(char *message, int length, struct sockAndAddr *socketAndAddress) {
    if(head==NULL) {
        printf("没有客户端需要发送\n" );
        return;
    }
    //在发送的内容前面加上ip和冒号
    //printf("发送到客服端的内容是========%s \n", message);

    struct clientSocketForDisplay * node = head;
    while(node!=NULL) {

        int  result = send(node->socket, message, length + 16 + 1, 0);
        printf("发送到socket %d的结果是%d\n",node->socket,result);
        //只需要同步链表的操作即可
        pthread_mutex_lock(&lock);
        if(result==-1) {
            removeFromDisplayList(node);
        }
        node = node->next;
        pthread_mutex_unlock(&lock);
    }
    free(message);
};

void getCompletedMessage(char *dest, char * ip, char * nickname, char *message){
  strcpy(dest, ip);
  strcat(dest, nickname);
  strcat(dest, ":");
  strcat(dest, message);
  printf("用户的最终发送消息为%s\n", dest);
}
void getLeaveMessage(char *dest, char *ip, char *nickname){
    strcpy(dest, "========系统消息：ip为");
    strcat(dest, ip);
     strcat(dest, "的用户\"");
    strcat(dest, nickname);
    strcat(dest, "\"断开连接=======");
}

void *chat(void * param) {
    struct threadArgs *arg = (struct threadArgs *)param;
    printf("用户的昵称是：%s", arg->nickname);

    struct sockAndAddr *socketAndAddress  = &(arg->socketAndAddress);
    SOCKET clientSocket = (*socketAndAddress).clientSocket;
    struct sockaddr_in clientAddress = socketAndAddress->clientAddress;
    char buffer[SERVER_BUFFER_SIZE] = {0};//接受用户输入，需要足够大
    char message[SERVER_BUFFER_SIZE]={0};//组装成最终的字符串

    int numberOfRead=0;
    char *ip = inet_ntoa(clientAddress.sin_addr);
    do {
        numberOfRead  = recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("读取到的数据长度是%d\n", numberOfRead);
        if(numberOfRead>0) {
            buffer[numberOfRead] = 0;//修改末尾的内容并没有太大影响？
            getCompletedMessage(message, ip, arg->nickname, buffer);
            printf("ip %s提交的内容-----------》:%s\n",ip, message);
            sendToClientsForDisplay(message, strlen(message), socketAndAddress);
        }
    } while(numberOfRead>0);
    char leaveMessage[100] = {0};
    getLeaveMessage(leaveMessage, ip, arg->nickname);
    sendToClientsForDisplay(leaveMessage, strlen(leaveMessage), socketAndAddress);
    free(socketAndAddress);
    printf("client %s %d关闭连接\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port) );
    return 0;
}
void getNickname(SOCKET clientSocket, char buffer[], int lengthOfBuffer){
     int numberOfRead  = recv(clientSocket, buffer, lengthOfBuffer, 0);
     buffer[numberOfRead] = 0;
}

void createNewThreadToChat(SOCKET clientSocket, struct sockaddr_in clientAddress) {
    struct threadArgs * args =  (struct threadArgs *)malloc(sizeof(struct threadArgs));
    getNickname(clientSocket, args->nickname, MAX_LENGTH_OF_NICKNAME);
    printf("客户端提交的昵称为 %s\n", args->nickname);
    args->socketAndAddress.clientSocket = clientSocket;
    args->socketAndAddress.clientAddress = clientAddress;
    pthread_t thread;
    /**
        注意这里有巨大的坑，和java之类的高级语言不同，这里在stack上分配的内存，在函数退出后立即无效
        因此socketAndAddress在线程中是无效的，因此必须在heap上分配内存

    **/
    int result = pthread_create(&thread, NULL, chat, args);
    //printf("会话线程创建状态为%d\n", result);
}

void handleIncomingRequest(SOCKET serverSocket) {

    int sizeofAddress = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddress;//每个新进来的请求都有对应的socket和address
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &sizeofAddress);
    if(clientSocket==INVALID_SOCKET) {
        printf("非法的客户端连接，错误码是\n",WSAGetLastError());
        return;
    }
    printf("收到客户端连接，ip是%s %u %d\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port),clientSocket);
    if(isClientForDisplay(&clientAddress)) { //分辨是展示型还是输入型
        printf("添加%s:%d到列表", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
        addToDisplayList(clientSocket);
        return;
    } else {
        createNewThreadToChat(clientSocket, clientAddress);
    }
}





int main(void) {
    init();
    SOCKET serverSocket = createSocket();
    struct sockaddr_in serverAddress = createServerAddress(PORT_OF_SERVER);
    bindSocketWithAddress(serverSocket, serverAddress, sizeof(serverAddress));
    linstenTo(serverSocket, SOMAXCONN);
    pthread_mutex_init(&lock, NULL);
    while(true) {
        handleIncomingRequest(serverSocket);
    }
    return 0;
}
