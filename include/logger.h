#ifndef SHAREFILE_LOGGER_H
#define SHAREFILE_LOGGER_H

#include "string.h"
#include "../include/core.hpp"
#include "KVDB.h"
#include "vector"
using namespace std;

typedef enum logState: uint8_t 
{LOG_COMMIT,LOG_PRE,INVALID,LOG_ABORT} logState;

struct Operation{
    Operation(string op="",string val="",string old=""){
        operation=op,value=val,oldVal=old;
    }
    void setValue(string op,string val,string old){
        operation=op,value=val,oldVal=old;
    }
    vector<string> delVal;
    string operation;
    string value;
    string oldVal;
};

struct Log{
    Log(){
        LogIndex=0;
        state=LOG_PRE;
    }
    Log(int index,Operation opt){
        LogIndex=index,op=opt;
    }
    void setValue(int index,Operation opt){
        LogIndex=index,op=opt;
    }
    string getState(){
        if(state==LOG_COMMIT)
            return "0";
        else if(state==LOG_PRE)
            return "1";
        else if(state==INVALID)
            return "2";
        else if(state==LOG_ABORT)
            return "3";
        return "ERROR";
    }
    int LogIndex;//对应的term号
    logState state=LOG_PRE;
    Operation op;
};
/*
Index
OP
value
*/
class Logger{
    private:
        int newLogIndex,posi,maxPosi;
        int CommitLogIndex;
        KVDB db;
        vector<Log> log;
        queue<string> delqueue;
    public:
        Logger();
        //parse the every del val into queue
        void parseDelVal(string val);
        ////parse the set's val into queue
        void parseSetVal(string cmd,string &key,string &val);
        Log createLog(int index,string op,string val,string oldval="");
        //geu the log from the server
        Log str2Log(string str);
        //get the new Index
        int getNewLogIndex();
        //get the new posi
        int getNewposi();
        //get the State of the follower(the new log's state),-1 means wrong
        string getState();
        //get the specific log with index
        Log getLog(int posi);
        //if it's ok to write the log
        bool writeLog(Log _l);
        //if it's ok to commit the log
        bool commitLog(string &ans);
        //if it's ok to rollback the log
        bool rollbackLog();
};


#endif 