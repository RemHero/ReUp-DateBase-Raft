
#ifndef SHAREFILE_PARTICIPANT_H
#define SHAREFILE_PARTICIPANT_H
#include "RUSocket.h"
#include "logger.h"
#include "parser.hpp"
using namespace std;

class Participant
{
    private:
        enum{OK,ERROR};
        enum DATASTATE {LOG_COMMIT,LOG_PRE,INVALID,LOG_ABORT} dataState=LOG_COMMIT;
        enum{FOLLOWER,CANDIDATE,LEADER} pstate=FOLLOWER;
        //0 for cmdQueue,1 for comQueue,2 for msgSendQueue,3 for the func handleCommand()
        //4 for timeout
        static sem_t psem[5];
        static mutex mut[10];
        int round,voteNum,succNum,failNum,memberNum;
        int timeOut;
        map<string,bool> ifRes;
        Logger log;
        Parser P;
        RUSocket RUS;
        NodeInfo MY,LEA,Client;
        std::queue<string> cmdQueue;
        std::queue< pair<NodeInfo,string> > msgSendQueue;
        std::queue<Command> comQueue;
        std::vector<NodeInfo> follower_info_list;
        std::string send_msg_to_participate(int connid,std::string smsg,std::string rmsg);
        //call the func to get the command,return 0 OK
        int getCommand();
        //parser the command and change the state ,set the Log if necessary
        int PCommandParser();
        //perform the command and cheange the state,you can create more thread to call this func
        int performComm();
        //send msg to someone
        int msgSend(NodeInfo R,string msg);
        // heart beat to sync the log,only the leader can call this func
        void* sendHeartBeat(void *arg);
        //get the command fron comQueue and handle the call back 
        // int handleCommand();
        //roll back the log by adding the cmd in msgSendQueue
        void rollBackAll();
        //commit all the log by adding the cmd in msgSendQueue
        void commitAll();
        //make all the log prepared by adding the cmd in msgSendQueue
        void prepareAll(string op,string val);
        //send heart beat to all
        void heartAll();
        //timer to switch the state when time out 
        void Timer(int t);
        //candidiate's func to fight for vote
        void voteAll();
        //timer to count if recv any [PRERE] 
        void preTimer(int t);
        //timer to count if recv any [COMRE] 
        void comTimer(int t);

        

    public:
        Participant();
        void debug();
        int getState();
        //call this func to perform the Participant
        void Working();
        
        int Init(std::vector<NodeInfo>ps,NodeInfo node);
        //create a thread to accept in while
        std::thread beginAccpet();
        //recv the msg after
        // int pRecv();
        //send the msg to destnation
        int pSend(string dest,string msg);
        void test();
};

#endif //SHAREFILE_COORDINATE_H
