
#ifndef SHAREFILE_COORDINATE_H
#define SHAREFILE_COORDINATE_H
#include "RUSocket.h"
#include "KVDB.h"
using namespace std;

class Coordinator
{
    private:
        enum{OK,ERROR} State=OK;
        unsigned int port;
        RUSocket RUS;
        KVDB DB;
        std::vector<NodeInfo> follower_info_list;
        std::vector<RUSocket> follower_socket_list;
        int send_msg_to_participate(int connid,std::string smsg,std::string rmsg);
    public:
        Coordinator();
        //Coordinator begin working
        void Working();
        //create a thread to accept in while
        std::thread beginAccpet();
        int getState();
        int Init(std::vector<NodeInfo> ps, NodeInfo c);
        bool ifalive(int connid);
        // int getCommand();
        int test();
};

#endif //SHAREFILE_COORDINATE_H
