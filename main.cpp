//
// Created by Rem_Hero on 2021/5/29.
//
#include "./include/core.hpp"
#include "./include/coordinate.h"
#include "./include/participant.h"
#include "./include/TCPServer.h"
#include "./include/conf.hpp"
#include "./include/parser.hpp"
#include "./layer3/ossSocket.hpp"
#include "./include/util.hpp"
#include "./include/global.h"
sem_t ROsem[2];
TCPClient tcpc;
TCPServer tcp;

void getMsg(){
    while(1){
        tcp.accepted();
    }
}

void outMsg(){
    while(1){
        queue<descript_socket*> descT=tcp.getMessage();
        if(!descT.empty()){
            descript_socket* desc=descT.front();
            cout << desc->message << endl;
        }      
    }
}

int main(int argc, char ** argv)
{
    for(int i=0;i<2;i++){
        if (sem_init(&ROsem[i],0,0)) {
            printf("[ERROR] Semaphore initialization failed!! <main 18>\n");
            exit(EXIT_FAILURE);
        }
    }
    

    if(argc == 1) {

        std::string add = "127.0.0.1";
        ossSocket client;
        std::string msg;
        Parser p;

        while(getline(std::cin, msg)) {
            // client = ossSocket(add.c_str(), 8001);

            // client.initSocket();
            // while( client.connect() ) { }

            std::vector<std::string> list;
            split(msg, list, ' ');//split the command into block --> list

            std::string pac = p.getRESPArry(list);

            //*
            // client.send(pac.c_str(), pac.length(), 100000000);//the timeout should be large

            // char * p = new char[1024];
            // memset(p, 0, 1024);
            // int len = 1024;
            // while(client.recvNF(p, len, 100000)) {}//这里看一下超时怎么实现的？？？？？？

            // std::string msg2(p, len);
            // std::cout << msg2;

            // delete[] p;
            // msg.clear();
            //*

            vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };
            tcp.setup(8010,opts);
            std::thread p1(getMsg);
            std::thread p2(outMsg);
            p1.detach();
            p2.detach();
            tcpc.setup("127.0.0.1",8001);
            tcpc.Send(pac);
            tcpc.exit();

        }

        return 0;
    }

    std::vector<NodeInfo>   pinfo;
    NodeInfo                cinfo;
    Mode                    m = MODE_INVALID;
    
    std::string conf = GetOptLong(argc, argv);//运行时的指令
    if( !readConf(conf, pinfo, cinfo, m) ) {
        std::cout << "> ERROR: CHECK THE CONFIGURE FILE. " << std::endl;
        return 0;
    }

    if( m == MODE_INVALID ) {
        std::cout << "> ERROR: UNDEFINE THE TYPE OF THE NODE. " << std::endl;
        return 0;
    }

    // if(m == MODE_C) {
    //     Coordinator c;
    //     c.Init(pinfo, cinfo);
    //     cerr << "Coordinator begin running...─=≡Σ(((つ•̀ω•́)つ\n";
    //     c.Working();
        
    //     //c.Launch();
    // } else if( m == MODE_P) {
        Participant p;
        // if(pinfo.size() != 1) {
        //     std::cout << "> ERROR: UNDEFINE THE PARTICIPANT. " << std::endl;
        //     return 0;
        // }
        p.Init(pinfo, cinfo);
        cerr << "ReRaft begin running...─=≡Σ(((つ•̀ω•́)つ\n";
        p.Working();
        //p.Launch();
    // }

    return 0;
}
