struct clientSocketForDisplay {
    SOCKET socket;
    struct clientSocketForDisplay *pre;
    struct clientSocketForDisplay *next;

};

struct clientSocketForDisplay * head = NULL;
struct clientSocketForDisplay * tail = NULL;

struct clientSocketForDisplay * createNewNode (SOCKET socket) {
    struct clientSocketForDisplay *newNode = (struct clientSocketForDisplay *)malloc(sizeof(struct clientSocketForDisplay));
    newNode->socket = socket;
    newNode->pre = newNode->next = NULL;
    return newNode;
}


void showSockets(void) {
    struct clientSocketForDisplay * node = head;
    if(node==NULL) {
        return;
    }
    printf("当前链表为:");
    do {
        printf("%d->",node->socket);
        node = node->next;
    } while(node!=NULL);
    printf("\n");
}
void addToDisplayList(SOCKET socket) {
    printf("添加socket %d\n",socket);
    if(head==NULL) {
        head = tail = createNewNode(socket);
    } else {
        struct clientSocketForDisplay *newNode =  createNewNode(socket);
        tail->next = newNode;
        newNode->pre = tail;
        tail = newNode;
    }
    showSockets();
}


void removeFromDisplayList(struct clientSocketForDisplay *client) {
    if(client==NULL) {
        printf("NULL节点\n");
        return;
    }
    printf("准备移除节点%d\n", client->socket);
    if(client->next !=NULL && client->pre!=NULL) {
        printf("移除普通节点%d\n", client->socket);
        client->pre->next = client->next;
        client->next->pre = client->pre;
    } else if(client->next ==NULL && client->pre==NULL) {
        printf("链表只有1个节点%d\n", client->socket);
        head = tail = NULL;
    } else if(client->next==NULL) {
        printf("移除tail节点%d\n", client->socket);
        client->pre->next = NULL;
        tail = client->pre;
    } else {
        printf("移除head节点%d\n", client->socket);
        head = client->next;
        client->next->pre = NULL;
    }
    free(client);
    showSockets();
}

