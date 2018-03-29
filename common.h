#define CLIENT_BUFFER_SIZE 100//客户端输入的缓冲区大小
#define SERVER_BUFFER_SIZE 1024//服务端缓冲区大小
#define MAX_NICKNAME_LENGTH 10//昵称的最大长度
const char IP_OF_SERVER[] = "192.168.0.123";
const unsigned short PORT_OF_SERVER = 8888;
void init() { //windows的特有初始化，linux 并没有
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("初始化失败，错误码是: %d",WSAGetLastError());
        exit(1);
    }
}

int createSocket() {

    /**
        创建一个空白的socket，socket是一个communication中的endpoint，
        1个communication包含2个endpoint，一个是server端，一个是client端
        1个独立的communication由以下两部分标记（协议类型+ 源ip+ port） + （协议类型+ 目的ip+ port）

        第1个参数表示socket支持的协议簇
            Linux下可用的类型如下:
            AF_UNIX, AF_LOCAL   Local communication              unix(7)
            AF_INET             IPv4 Internet protocols          ip(7)
            AF_INET6            IPv6 Internet protocols          ipv6(7)
            AF_IPX              IPX - Novell protocols
            AF_NETLINK          Kernel user interface device     netlink(7)
            AF_X25              ITU-T X.25 / ISO-8208 protocol   x25(7)
            AF_AX25             Amateur radio AX.25 protocol
            AF_ATMPVC           Access to raw ATM PVCs
            AF_APPLETALK        AppleTalk                        ddp(7)
            AF_PACKET           Low level packet interface       packet(7)
            AF_ALG              Interface to kernel crypto API

            windows可用类型参见这份文档：
            https://msdn.microsoft.com/en-us/library/windows/desktop/ms740506(v=vs.85).aspx

        第2个参数表示socket的类型，有流式的、数据报式的等等
            Linux可用类型如下：
            windows可用类型如下：
    **/
    SOCKET serverSocket =  socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == INVALID_SOCKET) {
        printf("创建服务端socket失败，错误码是%d", WSAGetLastError());
        exit(0);
    }
    return serverSocket;
}

void bindSocketWithAddress(SOCKET socket, struct sockaddr_in address, unsigned int length) {
    /**
        bind方法将一个socket和一个address关联起来，
        从csapp中的介绍可以看出，
        socket_in和 sockaddr很相似，并且前者还专门用了8个0字节来填充自己使得兼容后者
    **/
    int result = bind(socket, (struct sockaddr *)&address, length);
    if(result == SOCKET_ERROR) {
        printf("绑定address失败，错误码是%d", WSAGetLastError());
        exit(0);
    }
}
