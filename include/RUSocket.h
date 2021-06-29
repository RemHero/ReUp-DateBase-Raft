
#ifndef SHAREFILE_RUSOCKET_H
#define SHAREFILE_RUSOCKET_H

#include "TCPServer.h"
#include "TCPClient.h"
#include "../include/core.hpp"
#include "string.h"
using namespace std;
#define TIMEOUT 1000000
#define RECVLEN 1000
#define socketInfo pair< descript_socket*,NodeInfo>

class RUSocket{
    private:
        static pthread_mutex_t mutex1;
        static mutex RUSmt;
        vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };
        enum{OK,ERROR,LOSTCONN} connState=OK;
        queue< descript_socket* > message;
        string Address;
        int Port;
        TCPServer tcp;
        TCPClient tcpc;
        //perform as the server to accept <this fun was designed to be called in thread!!>
        void* RUaccept(void* arg);
        void outputMsg(descript_socket* desc);
        int RUparser(descript_socket* desc,NodeInfo node);
        void* handle_while(void* arg);
        //to creat the ReUp's protol
        string RESP2Reup(descript_socket* desc);
    public:
        RUSocket();
        //init as the server to bind the port 
        RUSocket( string pHostname, int port, int timeout = 0 );
        void debug();
        //check if you can connecte the server
        bool isConn();
        //perform as the client to bind the port to send
        int RURecvConnection(NodeInfo node, int timeout = 0 );
        //perform as the client to bind the port to send
        int RUconnection(NodeInfo node, int timeout = 0 );
        //other way to perform as the client to bind the port to send
        int setRUconnection( int timeout = 0 );
        //call the RUconnection() before you send the msg
        int RUsend(NodeInfo node,std::string msg,int time_out);
        //<thread> handle the request
        std::thread handle();
        //close the tcp connection and release the source
        void TCPclose(descript_socket* Info);
        //close the tcpc connection and release the source
        void TCPCclose();
        //upper call to get the command
        bool getCommand(string &cmd);
        //<thread> accept int while 
        std::thread beginAccept();
};

#endif //SHAREFILE_RUSOCKET_H
