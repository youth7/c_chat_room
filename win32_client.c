#include<stdbool.h>
#include<stdio.h>
#include<winsock2.h>
#include"common.h"
#include"common_client.h"
#include<windows.h>
#include<time.h>


_Bool readFromStdin(char buffer[], int size) {
    char *ptr = fgets(buffer, size, stdin);
    if(ptr==NULL) {
        puts("读取错误\n");
        return false;
    }
    size_t oriLength = strlen(buffer);
    if(oriLength<1) {
        printf("奇怪的长度%u\n", oriLength);
        return false;
    }
    buffer[oriLength-1] = 0;//删除回车
    return true;

}

void chat(SOCKET clientSocket) {
    time_t start,end;
    time(&start);
    char bufferForSpeak[CLIENT_BUFFER_SIZE] = {0};
    int length = 0;
    int resultOfSend = 0;
    while(true) {
        _Bool result = readFromStdin(bufferForSpeak, CLIENT_BUFFER_SIZE);
        if(!result) {
            puts("读取输入失败，请重试\n");
            continue;
        }
        length = strlen(bufferForSpeak);
        if(length>CLIENT_BUFFER_SIZE/2) {
            puts("发言太长，请控制发言长度\n");
            fflush(stdin);
            continue;
        }
        time(&end);
        //printf("%u\n",end-start);
        if(end-start<1) {
            printf("您废话说得太快了-_-%u\n", end-start);
            continue;
        }
        time(&start);
        //printf("发送的数据是%s\n", bufferForSpeak);
        resultOfSend = send(clientSocket, bufferForSpeak, length, 0);
        if(resultOfSend<0) {
            printf("发送失败,错误码是%d，请重试\n", resultOfSend);
            continue;
        }
    }
}
#define BUFFER_SIZE 30

void setNickName(SOCKET clientSocket) {
    char nickname[BUFFER_SIZE] = {0};
    char * nicknamePtr = 0;
    while(true) {
        puts("请输入你在聊天室中的昵称，长度在10英文字母以内\n");
        _Bool result = readFromStdin(nickname, BUFFER_SIZE);
        if(!result) {
            continue;
        }
        int lengthOfNickname = strlen(nickname);
        if(lengthOfNickname==0) {
            puts("昵称不能为空\n");
            continue;
        }
        if(lengthOfNickname>MAX_NICKNAME_LENGTH) {
            printf("昵称太长了 %d\n", lengthOfNickname);
             fflush(stdin);
            continue;
        }
        int resultOfSend = send(clientSocket, nickname, lengthOfNickname, 0);
        printf("设置昵称的结果为%d，可以开始发言\n", resultOfSend);
        return;
    }
}
int main(int argc, char *argv[]) {
    init();
    SOCKET clientSocket = createSocket();
    struct sockaddr_in clientAddress = createClientAddressWithPort(6666);
    struct sockaddr_in serverAddress = getServerAddress();
    bindSocketWithAddress(clientSocket, clientAddress, sizeof(clientAddress));
    connectToServer(clientSocket, serverAddress);
    setNickName(clientSocket);
    chat(clientSocket);
    return 0;
}
