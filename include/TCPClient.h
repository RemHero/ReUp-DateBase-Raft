#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h> 
#include <vector>

using namespace std;

class TCPClient {
private:
    int sock;
    int port;
    string address;
    struct sockaddr_in server;
    
    // 添加连接状态标志
    bool is_connected;
    
public:
    TCPClient();
    ~TCPClient();  // 添加析构函数
    
    // 禁用拷贝构造和赋值操作
    TCPClient(const TCPClient&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;
    
    bool isConnected() const { return is_connected; }
    // ... other existing declarations ...
};