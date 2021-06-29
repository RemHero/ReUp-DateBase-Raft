//#include "../include/core.hpp"
#include "../include/participant.h"
#include "../include/parser.hpp"
#include <string.h>
using namespace std;

sem_t Participant::psem[5];
mutex Participant::mut[10];

std::thread Participant::beginAccpet(){
   return std::move(RUS.beginAccept());
}


void Participant::debug(){
    RUS.debug();
}


int Participant::msgSend(NodeInfo node,string msg){
    RUS.TCPCclose();
    while(RUS.RUsend(node,msg,100000)!=0){
        cerr << "[ERROR]send msg faild <part.cpp 25>\n";
        return ERROR;
    }
    RUS.TCPCclose();
    return OK;
}

/*
the func can just send the heartbeat, it's possible that
you may not receive any msg
*/
void* Participant::sendHeartBeat(void* arg){//这里要修改，一次性发完
    int l=follower_info_list.size();
    // bool flag=false;
    while(1){
        // sem_wait(&psem[2]);
        while(!msgSendQueue.empty()){
            auto it=msgSendQueue.front();
            msgSendQueue.pop();
            printf("--------------------------------------send %s\n",it.second.c_str());
            if(it.second.find("[HEART]")!=string::npos && pstate!=LEADER) continue;
            if(msgSend(it.first,it.second)!=0){
                printf("part.49 send fail \n");
            }
        }
        if(pstate==LEADER){
            heartAll();
        }
        usleep(1000000);
    }
    return NULL;
}

void Participant::rollBackAll(){
    int l=follower_info_list.size();
    string posi,index,state,ans,add;
    int port;
    add=MY.add;port=MY.port;
    posi=to_string(log.getNewposi());
    index=to_string(log.getNewLogIndex());
    for(int i=0;i<l;i++){
        ans="[ROLL]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
        msgSendQueue.push(make_pair(follower_info_list[i],ans));
        sem_post(&psem[2]);
    }
    
}

void Participant::commitAll(){
    int l=follower_info_list.size();
    string posi,index,state,ans,add;
    int port;
    add=MY.add;port=MY.port;
    posi=to_string(log.getNewposi());
    index=to_string(log.getNewLogIndex());
    for(int i=0;i<l;i++){
        ans="[COM]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
        msgSendQueue.push(make_pair(follower_info_list[i],ans));
        sem_post(&psem[2]);
    }
    
}

void Participant::prepareAll(string op,string val){
    int l=follower_info_list.size();
    string posi,index,state,ans,add;
    int port;
    add=MY.add;port=MY.port;
    posi=to_string(log.getNewposi());
    index=to_string(log.getNewLogIndex());
    for(int i=0;i<l;i++){
        ans="[PRE]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/op:"+op+"/res:"+val+"/";
        msgSendQueue.push(make_pair(follower_info_list[i],ans));
        sem_post(&psem[2]);
    }
    
}

void Participant::heartAll(){
    int l=follower_info_list.size();
    string posi,index,state,ans,add;
    int port;
    add=MY.add;port=MY.port;
    posi=to_string(log.getNewposi());
    index=to_string(log.getNewLogIndex());
    for(int i=0;i<l;i++){
        ans="[HEART]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
        msgSendQueue.push(make_pair(follower_info_list[i],ans));
        sem_post(&psem[2]);
    }
    
}

// enum DATASTATE Participant::transState(string s){
//     if(s=="0") return LOG_COMMIT;
//     else if(s=="1") return LOG_PRE;
//     else if(s=="2") return INVALID;
//     else if(s=="3") return LOG_ABORT;
// }

int Participant::performComm(){
    Command com;
    string index,posi,state,ans,add;
    int port;
    add=MY.add;port=MY.port;
    NodeInfo des;
    while(1){
        ans="";
        sem_wait(&psem[1]);
        printf("--------------------part.103\n");
        bool flag=false;
        if(!comQueue.empty()){
            com=comQueue.front();
            if(com.type!="[VOTE]" ||com.type!="[VOTERE]" ) timeOut=0;
            comQueue.pop();
            des.add=com.add,des.port=atoi(com.port.c_str());
            if(com.type=="[CLIENT]"){
                if(pstate!=LEADER){
                    des.add=LEA.add,des.port=LEA.port;
                    ans=com.reCmd;
                    cout << "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE " << ans << endl;
                    goto LASTL;
                }else{
                    Client.add=com.add;Client.port=8010;
                    log.writeLog(log.createLog(round,com.op,com.res));
                    dataState=LOG_PRE;

                    prepareAll(com.op,com.res);
                    // std::thread ppre(&Participant::preTimer,this,5);
                    // ppre.detach();
                    
                    continue;
                }
            }
            else if(com.type=="[VOTE]"){
                printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ %d %d\n",atoi(com.res.c_str()),round);
                if(atoi(com.res.c_str())>=round){
                    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% switch to follower\n");
                    if(atoi(com.res.c_str())>round){
                        pstate=FOLLOWER;
                        // sleep(10);
                    }
                    // mut[0].lock();
                    // for(int i=0;i<50;i++){
                    //     cout << "old:" << round<< endl;
                    // }
                    round=atoi(com.res.c_str());
                    // for(int i=0;i<50;i++){
                    //     cout << "new:" << round<< endl;
                    // }
                    timeOut=0;
                    voteNum=1;
                    // mut[0].unlock();
                    flag=true;
                }
                else flag=false;
                posi=to_string(log.getNewposi());
                index=to_string(log.getNewLogIndex());
                if(flag){
                    cout << "part.161\n"; 
                    ans="[VOTERE]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
                }
            }else if(com.type=="[VOTERE]" && pstate==CANDIDATE && atoi(com.res.c_str())==round){
                for(int i=0;i<10;i++){
                        cout << voteNum << endl;
                        cout << "==============================================================================================================\n";
                    }
                flag=false;
                voteNum++;
                if(voteNum>=memberNum/2+1){
                    for(int i=0;i<50;i++){
                        cout << voteNum << endl;
                        cout << "==============================================================================================================\n";
                    }
                    voteNum=1;
                    pstate=LEADER;
                    heartAll();
                }
                continue;
            }
            else if(pstate==FOLLOWER || pstate==CANDIDATE || pstate==LEADER){//if the node'state is follower
                printf("--------------------part.113\n");
                if(com.type=="[HEART]"){
                    if(com.res.find("{")==string::npos)
                        if(atoi(com.res.c_str())>=round && atoi(com.pos.c_str())>=log.getNewposi() && atoi(com.index.c_str())>=log.getNewLogIndex()){
                            pstate=FOLLOWER;
                            timeOut=0;
                            round=atoi(com.res.c_str());
                            LEA.add=com.add;
                            LEA.port=atoi(com.port.c_str());
                        }else{
                            continue;
                        }
                    // for(int i=0;i<500;i++)
                    //     cout << "198 ============= folower\n";
                    printf("--------------------part.115\n");
                    if(dataState!=INVALID && com.pos==to_string(log.getNewposi()) && com.index==to_string(log.getNewLogIndex())){
                        flag=true;
                        string s=log.getState();
                        if(s=="0" || s=="~") dataState=LOG_COMMIT;
                        else if(s=="1") dataState=LOG_PRE;
                        else if(s=="2") dataState=INVALID;//恢复状态
                        else if(s=="3") dataState=LOG_ABORT;
                        printf("--------------------part.120\n");
                    }
                    else if(dataState!=INVALID && (com.pos!=to_string(log.getNewposi()) || com.index!=to_string(log.getNewLogIndex()))){
                        printf("--------------------part.125\n");
                        flag=true;
                        dataState=INVALID;//进入恢复阶段
                        printf("[WORNING] data does not fit and get into recover <part.cpp 68>\n");
                    }else if(dataState==INVALID){
                        printf("--------------------part.129\n");         
                        flag=true;
                        if(com.pos==to_string(log.getNewposi()) && com.index==to_string(log.getNewLogIndex()) && com.res.size()<20){
                            dataState=LOG_ABORT;
                            printf("[WORNING] recover finish! <part.cpp 216>\n");
                            continue;
                        }
                        if(com.res.empty() || com.res.size()<10){
                            //flag=false;
                            printf("waiting for the recover heart beat... <part.cpp 76>\n");
                            ans="[HEARTRE]";
                            goto FINALL;
                            //continue;
                        }
                        printf("[WORNING] recover.... <part.cpp 223>\n");
                        string op,val,oldval,_state;
                        P.reComPar(com.res,op,val,oldval,_state);
                        cout << "###### " << op << ' ' << val << ' ' << oldval << ' ' << _state << endl;
                        if(atoi(com.pos.c_str())>=log.getNewposi()){
                            Log l_=log.createLog(atoi(com.index.c_str()),op,val,oldval);
                            if(log.writeLog(l_)!=false){
                                printf("--------------------part.225\n");    
                                if(_state=="0"){//commit state
                                    printf("--------------------part.227\n");    
                                    string ts;
                                    flag=log.commitLog(ts);
                                    cout << "[part 260 result] " << log.getLog(log.getNewposi()-1).op.operation << ' ' << log.getLog(log.getNewposi()-1).op.value << endl; 
                                    if(!flag)
                                        printf("[ERROR] recver or commit wrong <part.cpp 285>\n");
                                }
                            }
                        }
                    }
                    ans="[HEARTRE]";
                }else if(com.type=="[PRE]"){
                    flag=log.writeLog(log.createLog(atoi(com.index.c_str()),com.op,com.res));
                    posi=to_string(log.getNewposi());
                    index=to_string(log.getNewLogIndex());
                    ans="[PRERE]";
                    if(flag){
                        dataState=LOG_PRE;
                        ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:ok/";
                    }else{
                        ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:error/";
                    }
                    goto LASTL;
                }else if(com.type=="[ROLL]"){
                    flag=log.rollbackLog();
                    posi=to_string(log.getNewposi());
                    index=to_string(log.getNewLogIndex());
                    ans="[ROLLRE]";
                    if(flag){
                        dataState=LOG_ABORT;
                        ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:ok/";
                    }else{
                        ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:error/";
                    }
                    goto LASTL;
                }else if(com.type=="[COM]"){
                    string ts;
                    flag=log.commitLog(ts);
                    posi=to_string(log.getNewposi());
                    index=to_string(log.getNewLogIndex());
                    ans="[COMRE]";
                    if(flag){
                        dataState=LOG_ABORT;
                        ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:ok/";
                    }else{
                        ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:error/";
                    }
                    goto LASTL;
                }else{
                    printf("[WARNING] type not fit? <part.cpp 148>\n");
                    flag=false;
                }
                FINALL:
                printf("--------------------part.152\n");
                posi=to_string(log.getNewposi());
                index=to_string(log.getNewLogIndex());
                if(flag){
                    ans+="/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
                }
            }
            if(pstate==LEADER){
                if(com.type=="[HEARTRE]"){
                    printf("--------------------part.257\n");
                    if(com.index==to_string(log.getNewLogIndex()) && com.pos==to_string(log.getNewposi())){
                        flag=true;
                        posi=to_string(atoi(com.pos.c_str()));
                        index=to_string(log.getNewLogIndex());
                        continue;
                        ans="[HEART]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
                    }else{
                        flag=true;
                        printf("--------------------part.265\n");
                        if(atoi(com.pos.c_str())!=log.getNewposi()){
                            printf("--------------------part.267\n");
                            posi=to_string(atoi(com.pos.c_str()));
                            Log l_=log.getLog(atoi(posi.c_str()));
                            index=to_string(l_.LogIndex);
                            ans="[HEART]/add:"+add+"/port:"+to_string(port)+"/posi:"+posi+"/index:"+index+"/res:{op:"+l_.op.operation+"val:"+l_.op.value+"oldval:"+l_.op.oldVal+"state:"+l_.getState()+"}"+"/";
                        }
                    }
                }else if(com.type=="[PRERE]"){
                    printf("--------------------part.293\n");
                    if(com.res=="ok"){
                        printf("--------------------part.295\n");
                        succNum++;
                        if(succNum>=memberNum/2+1){
                            // ans="[CLENTRE]/res:ok";//raft算法修改为二阶段
                            dataState=LOG_COMMIT;
                            commitAll();
                            succNum=1;
                            continue;
                        }
                    }else{
                        printf("--------------------part.303\n");
                        failNum++;
                        if(failNum>=memberNum/2+1){
                            printf("--------------------part.306\n");
                            des.add=Client.add,des.port=Client.port;
                            ans="[CLENTRE]/add:"+add+"/port:"+to_string(port)+"/res:error/";
                            failNum=0;
                            rollBackAll();
                            log.rollbackLog();
                            continue;
                        }
                    }
                }else if(com.type=="[ROLLRE]"){
                    dataState=LOG_ABORT;//鲁棒性！！！！！！！！！！！！！！！！！！
                    if(com.res=="ok"){
                        flag=true;
                    }else{
                        printf("roll back fail <pact.cpp 170>\n");
                        flag=false;
                    }
                }else if(com.type=="[COMRE]"){
                     printf("--------------------part.348\n");
                    if(com.res=="ok"){
                        printf("--------------------part.350\n");
                        succNum++;
                        if(succNum>=memberNum/2+1){
                            dataState=LOG_ABORT;
                            printf("--------------------part.355\n");
                            string ts;
                            succNum=1;
                            des.add=Client.add,des.port=Client.port;
                            if(log.commitLog(ts)){
                                if(!ts.empty())
                                    ans="[CLENTRE]/add:"+add+"/port:"+to_string(port)+"/res:ok"+"/res:"+ts+"/";
                                else
                                    ans="[CLENTRE]/add:"+add+"/port:"+to_string(port)+"/res:ok/";
                            }
                            else ans="[CLENTRE]/add:"+add+"/port:"+to_string(port)+"/res:error/";
                        }
                    }
                    else{
                        failNum++;
                        if(failNum>memberNum/2+1){
                            dataState=LOG_ABORT;
                            des.add=Client.add,des.port=Client.port;
                            ans="[CLENTRE]/add:"+add+"/port:"+to_string(port)+"/res:error/";
                            failNum=0;
                            rollBackAll();
                            log.rollbackLog();
                        }
                    }
                }
                else{
                    printf("[ERROR] type: %s not fit? <part.cpp 266>\n",com.type.c_str());
                    continue;
                }
            }
            LASTL:
            printf("-----------------ans: %s\n",ans.c_str());
            if(!ans.empty()){
                msgSendQueue.push(make_pair(des,ans));
                sem_post(&psem[2]);
            }
        }
    }
    return OK;
}

int Participant::PCommandParser(){
    string cmd;
    Command com;
    while(1){
        printf("--------------------part.260\n");
        sem_wait(&psem[0]);
        printf("--------------------part.261\n");
        if(!cmdQueue.empty()){
            printf("--------------------part.264\n");
            cmd=cmdQueue.front();
            printf("%s\n",cmd.c_str());
            printf("--------------------part.428\n");
            cmdQueue.pop();
            printf("--------------------part.267\n");
            com=P.commandParser(cmd);
            if(com.type=="[CLIENT]")
                com.reCmd=cmd;
            // if(com.type=="[HEART]"){
            //     for(int i=0;i<10000;i++)
            //         cout << "why!!!\n";
            // }
            printf("--------------------part.271\n");
            comQueue.push(com);
            sem_post(&psem[1]);
        }
    }
    printf("--------------------part.268\n");
    return OK;
}

int Participant::getCommand(){//need 
    string cmd;
    while(1){
        if(RUS.getCommand(cmd)){
            printf("--------------------receive cmd:%s\n",cmd.c_str());
            cmdQueue.push(cmd);
            sem_post(&psem[0]);
        }
    }
    return ERROR;
}

int Participant::pSend(string dest,string msg){

    return OK;
}

int Participant::Init(std::vector<NodeInfo> ps,NodeInfo node){
    int len=ps.size();
    MY=node;
    memberNum=len+1;//!!!!!!!!!!!!!!!!!!!!!!!有多个参与者要在这里写明！！待修改
    pstate=FOLLOWER;
    voteNum=1;
    if(RUS.RURecvConnection(node,TIMEOUT)!=0){
        cerr << "[---ERROR---] init failed <parti.cpp 306>\n";
    }
    for(int i=0;i<len;i++){
        follower_info_list.push_back(ps[i]);
    }
        
    return 0;
}

void Participant::Timer(int t){
    while(1){
        if(pstate==LEADER){
            sleep(t);
            // sem_post(&psem[4]);
            continue;
        }
        int tk=(rand()%2==1)?3:5;
        cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " <<round << endl; 
        if(timeOut>=tk){
            pstate=CANDIDATE;
            mut[0].lock();
            round++;
            timeOut=0;
            voteNum=1;
            mut[0].unlock();
            sem_post(&psem[4]);
        }
        sleep(t);
        timeOut++;
    }
}

void Participant::preTimer(int t){
    string ans,add;
    int port;
    add=MY.add;port=MY.port;
    for(int i=0;i<t;i++){
        if(dataState!=LOG_PRE) return;
        sleep(1);
    }
    if(dataState!=LOG_PRE) return;
    if(succNum<memberNum/2+1){
        rollBackAll();
        ans="[CLIENT]/add:"+add+"/port:"+to_string(port)+"/res:error/";
        msgSendQueue.push(make_pair(Client,ans));
        succNum=1;
        dataState=LOG_ABORT;
    }
}

void Participant::comTimer(int t){
    string ans,add;
    int port;
    add=MY.add;port=MY.port;
    for(int i=0;i<t;i++){
        if(dataState!=LOG_COMMIT) return;
        sleep(1);
    }
    if(dataState!=LOG_COMMIT) return;
    if(succNum<memberNum/2+1){
        rollBackAll();
        ans="[CLIENT]/add:"+add+"/port:"+to_string(port)+"/res:error/";
        msgSendQueue.push(make_pair(Client,ans));
        succNum=1;
        dataState=LOG_ABORT;
    }
}

void Participant::voteAll(){
    while(1){
        sem_wait(&psem[4]);
        if(pstate!=CANDIDATE) continue;
        printf("===============================new candidate of %d begin===============================\n",round);
        int l=follower_info_list.size(),port;
        string add,posi,index,ans;
        NodeInfo des;
        posi=to_string(log.getNewposi());
        index=to_string(log.getNewLogIndex());
        for(int i=0;i<l;i++){
            des.add=follower_info_list[i].add;
            des.port=follower_info_list[i].port;
            ans="[VOTE]/add:"+MY.add+"/port:"+to_string(MY.port)+"/posi:"+posi+"/index:"+index+"/res:"+to_string(round)+"/";
            msgSendQueue.push(make_pair(des,ans));
            sem_post(&psem[2]);
        }
    }
}

void Participant::Working(){
    if(MY.port==8001){
        pstate=LEADER;
    }
    //     log.writeLog(log.createLog(1,"SET","LHZ,HERO"));
    //     string ts;
    //     log.commitLog(ts);
    //     printf("--------------------part.543\n");
    //     string cmd="[client]/op:GET/res:LHZ/";
    //     cmdQueue.push(cmd);
        
    // }else if(MY.port==8002){
    //     pstate=FOLLOWER;
    //     log.writeLog(log.createLog(1,"SET","LHZ,HERO"));
    //     string ts;
    //     log.commitLog(ts);
    // }

    std::thread pAccept=beginAccpet();//消息放入Message队列
    std::thread pHandle=RUS.handle();//从Message取信息放入message
    std::thread pGetCommand(&Participant::getCommand,this);//放入cmd队列
    std::thread pParseCom(&Participant::PCommandParser,this);//从cmd取信息解析后放入com队列
    std::thread pPerformCom(&Participant::performComm,this);//从com队列取命令并实施
    std::thread pSend(&Participant::sendHeartBeat,this,(void*)0);//发送回应
    std::thread pTimer1(&Participant::Timer,this,1);//follower超时选举
    std::thread pVote(&Participant::voteAll,this);//follower超时发送选票

    

    pHandle.join();
    pAccept.join();
    pGetCommand.join();
    pParseCom.join();
    pPerformCom.join();
    pSend.join();
    pVote.join();
    pTimer1.join();
}

// void Participant::test(){
//     std::thread tHandle=RUS.handle();
//     descript_socket* Info;
//     while(1){
//         if(RUS.getCommand(Info)){
//             cerr << Info->message<< endl;
//             RUS.TCPclose(Info);
//         }
//     }
//     tHandle.join();
// }

Participant::Participant(){
    round=0,voteNum=1,succNum=1,failNum=0,memberNum=0,timeOut=0;
    for(int i=0;i<5;i++){
        if (sem_init(&psem[i],0,0)) {
            printf("[ERROR] Semaphore initialization failed!! <parti.cpp 118>\n");
            exit(EXIT_FAILURE);
        }
    }
}

int Participant::getState(){
    return pstate;
}


