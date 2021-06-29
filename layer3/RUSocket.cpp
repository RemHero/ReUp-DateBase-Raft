#include "RUSocket.h"
#include "../include/global.h"
mutex RUSocket::RUSmt;
pthread_mutex_t RUSocket::mutex1;
RUSocket::RUSocket(){
    
}

RUSocket::RUSocket (string pHostname, int port, int timeout){
    Address=pHostname;
    Port=port;
}

bool RUSocket::isConn(){
    if(tcpc.Send("~")==false){
        cerr << "conn failed <RUS.cpp 15>\n";
        return false;
    }
    return true;
}

void RUSocket::outputMsg(descript_socket* desc){
    cout << "id:      " << desc->id      << endl
        << "ip:      " << desc->ip      << endl
        << "message: " << desc->message << endl
        << "socket:  " << desc->socket  << endl;
}

//no use any more!!!
int RUSocket::RUparser(descript_socket* desc,NodeInfo node){
    node.add="127.0.0.1";
    return OK;
}

void RUSocket::TCPclose(descript_socket* Info){
    tcp.CloseConnection(Info);
}

void RUSocket::TCPCclose(){
    tcpc.exit();
}

bool RUSocket::getCommand(string &cmd){
    sem_wait(&ROsem[1]);
    cout << "-----------------RUS.45\n";
    descript_socket* t;
    RUSmt.lock();
    if(!message.empty()){
        t=message.front();
        if(t->message[0]=='*'){
            RESP2Reup(t);
        }
        cout << message.front()->message << endl;
        cmd=message.front()->message;
        message.pop();
        RUSmt.unlock();
        
        std::thread pCash(&RUSocket::TCPclose,this,t);
        pCash.detach();
        
        return true;
    }
    return false;
}

std::thread RUSocket::handle(){
    return std::thread (&RUSocket::handle_while,this,(void*)0);
    //printf("--------------------RUS.58\n");
}

//*4\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
string RUSocket::RESP2Reup(descript_socket* desc){
    string ts=desc->message,tval,op,_key,_val;
    vector<string> list;
    int pos1=0,num=0,tp=0,l=0;
    pos1=ts.find("\r\n");
    num=atoi(ts.substr(1,pos1-1).c_str());

    for(int i=0;i<num;i++){
        tp=ts.find("$",tp+1);
        cout << "tp: " << tp << endl;
        pos1=ts.find("\r\n",pos1+1);
        cout << "pos1: " << pos1 << endl;
        l=atoi(ts.substr(tp+1,pos1-tp-1).c_str());
        cout << "len: " << l << endl;
        tval=ts.substr(pos1+2,l);
        cout << "val: " << tval << endl;
        list.push_back(tval);
        pos1=ts.find("\r\n",pos1+1);
        cout << "pos1next: " << pos1 << endl;
    }
    op=list[0];
    int _len=list.size();
    for(int i=1;i<_len;i++){
        _val+=list[i];
        if(i!=_len-1) _val+=",";
    }
    desc->message="[CLIENT]/add:"+desc->ip+"/port:8010"+"/op:"+op+"/res:"+_val+"/";
    cout<< "~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << desc->message << endl;
    return desc->message;
}

void* RUSocket::handle_while(void * arg){
    queue<descript_socket*> descT;
    while(1){
        printf("--------------------RUSs65\n");
        sem_wait(&ROsem[0]);
        pthread_mutex_lock(&mutex1);
        descT = tcp.getMessage();
        pthread_mutex_unlock(&mutex1);
        printf("--------------------RUSs69\n");
        while(!descT.empty()) {
            descript_socket* desc;
            desc=descT.front();
            descT.pop();
            NodeInfo node;
            if(!desc->enable_message_runtime)
            {
                desc->enable_message_runtime = true;
                if(DEBUG) outputMsg(desc);
                message.push(desc);
                cout << desc->message << endl;
                printf("--------------------RUSs77\n");
                sem_post(&ROsem[1]);
            }
        }
        printf("--------------------RUSs83\n");
    }
    
    return 0;
}

int RUSocket::RUsend(NodeInfo node,std::string msg,int time_out){
    if(RUconnection(node)!=0){
        return ERROR;
    }
    if(tcpc.Send(msg)==false){
        cerr << "send failed <RUS.cpp 23>\n";
        return ERROR;
    }
    return OK;
}

int RUSocket::RURecvConnection(NodeInfo node, int timeout){
    //printf("++++++  %d  %s\n",node.port,node.add.c_str());
    if(tcp.setup(node.port,opts)!=0){
        cerr << "$tcp$ bind failed <RUS.cpp 90>\n";
        return ERROR;
    }
    return OK;
}

int RUSocket::RUconnection(NodeInfo node, int timeout){
    printf("++++++%d-%s\n",node.port,node.add.c_str());
    if(!tcpc.setup(node.add,node.port)){
        cerr << "$tcpc$ bind failed <RUS.cpp 120>\n";
        return ERROR;
    }
    return OK;
}

int RUSocket::setRUconnection( int timeout){
    if(!tcpc.setup(Address,Port)){
        cerr << "[ERROR] $tcpc$ bind port [" << Port << "] failed <RUS.cpp 104>\n";
        return ERROR;
    }
    return OK;
}

void RUSocket::debug(){
    tcp.showSocketfd();
}

std::thread RUSocket::beginAccept(){
    return std::move(std::thread (&RUSocket::RUaccept,this,(void*)0));
}

void* RUSocket::RUaccept(void* arg){
    while(1){
        tcp.accepted();
    }
}


