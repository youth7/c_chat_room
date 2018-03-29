struct sockaddr_in getServerAddress() {
    /**
        创建远程服务器的address包括：
        1，协议簇
        2，端口
        3，可以连接进来的ip
    **/
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( PORT_OF_SERVER );
    address.sin_addr.s_addr = inet_addr(IP_OF_SERVER);
    return address;

}

struct sockaddr_in createClientAddressWithPort(unsigned short port) {
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons( port);
    char hostName[255] = {0};
    gethostname(hostName, 255);
    /*
    struct hostent结构定义如下
    struct hostent {
        char    *h_name;         //official name of host
        char    **h_aliases;     //alias list
        int     h_addrtype;      //host address type
        int     h_length;        //length of address
        char    **h_addr_list;   //list of addresses
    }
    而为了向后兼容，因此有#define h_addr h_addr_list[0]，所以h_addr其实就是h_addr_list[0]

    h_addr_list在man中说的是：
    An  array  of  pointers  to network addresses for the host (in network byte order), terminated by a NULL pointer.
    因此可知h_addr_list就是指向了网络地址的指针，这和struct in_addr的结构是兼容的，因此强转没有问题

    struct in_addr 位于/usr/include/linux/in.h ,具体是：

    struct in_addr {
            __be32  s_addr;//Internet address
    };
    说明s_addr是一个大端顺序表示的地址，和h_addr_list是兼容的
    */
    struct hostent *host = gethostbyname(hostName);
    printf("ip 是 %s\n", inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
    clientAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
    return clientAddress;
};

void connectToServer(SOCKET clientSocket, struct sockaddr_in serverAddress) {
    printf("正在连接到服务器%s\n", inet_ntoa(serverAddress.sin_addr));
    int result = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (result < 0) {
        printf("连接服务器失败，错误码是%d\n", WSAGetLastError());
        exit(0);
    }
    puts("已经成功连接服务器\n");

}
















