#include "../include/core.hpp"
#include "../include/coordinate.h"
#include "../include/RUSocket.h"

int Coordinator::Init(std::vector<NodeInfo> ps, NodeInfo c)
{
    int len=ps.size();

    port=c.port;
    follower_info_list=ps;
    RUS.RURecvConnection(c,TIMEOUT);

    for(int i=0;i<len;i++){
        follower_socket_list.push_back(RUSocket(follower_info_list[i].add,
                                        follower_info_list[i].port));
        //follower_socket_list[i].setRUconnection(TIMEOUT);
    }
    return 0;
}
void Coordinator::Working(){
    std::thread cAccept=beginAccpet();
    test();

    cAccept.join();
}

std::thread Coordinator::beginAccpet(){
    return std::move(RUS.beginAccept());
}

int Coordinator::getState(){
    return State;
}

bool Coordinator::ifalive(int connid){
    return follower_socket_list[connid].isConn();
}

int Coordinator::test(){
    // RUS.handle();
    if(RUS.RUsend(follower_info_list[0],"1234",TIMEOUT)!=0){
        cerr << "[ERROR]send failed <coo.cpp 42>\n";
    }
    RUS.TCPCclose();
    return OK;
}

Coordinator::Coordinator(){

}

int Coordinator::send_msg_to_participate(int connid,std::string smsg,std::string rmsg){
   
    return OK;
}

