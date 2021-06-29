
#include "../include/logger.h"

    // private:
    //     int newLogIndex;
    //     int CommitLogIndex;
    //     vector<Log> log;

Logger::Logger(){
    newLogIndex=CommitLogIndex=posi=maxPosi=0;
}

void Logger::parseDelVal(string key){
    int lpoi=0,fpoi=0;
    int len=key.length();
    while(key.find(",",lpoi)!=string::npos){
        fpoi=key.find(",",lpoi);
        string tkey=key.substr(lpoi,fpoi-lpoi);
        delqueue.push(tkey);
        lpoi=fpoi+1;
        if(lpoi>=len) break;
    }
}

void Logger::parseSetVal(string cmd,string &key,string &val){
    int pos=cmd.find(",");
    key=cmd.substr(0,pos);
    val=cmd.substr(pos+1);
}

Log Logger::createLog(int index,string op,string val,string oldval){
    return Log(index,Operation(op,val,oldval));
}

//"/index:12/op:set/value:10,10"
Log Logger::str2Log(string str){
    Log tlog;
    Operation opt;
    int index;
    string indexString,op,value;

    int indexPoi=str.find("/index:");
    int opPoi=str.find("/op:");
    int valuePoi=str.find("/value:");

    indexString=str.substr(indexPoi+7,opPoi-indexPoi-7);
    index = strtol(indexString.c_str(), nullptr, 10);//if is ok to transfer the long to int
    op=str.substr(opPoi+4,valuePoi-opPoi-4);
    value=str.substr(indexPoi+7);
    opt.setValue(op,value,"");
    tlog.setValue(index,opt);
    return tlog;
}

int Logger::getNewLogIndex(){
    if(posi!=0)
        return log[posi-1].LogIndex;
    else return 0;
}

int Logger::getNewposi(){
    return posi;
}

Log Logger::getLog(int posi){
    return log[posi];
}

bool Logger::writeLog(Log _l){
    log.push_back(_l);
    posi++;
    cout << "\n[log 71] " << log[posi-1].op.value << endl;
    newLogIndex=_l.LogIndex;
    log[posi-1].state=LOG_PRE;
    return true;
}

bool Logger::commitLog(string &ans){
    if(posi==0){
        cerr << "[ERROR] no log to commmit <logger.cpp 57>\n";
        return false;
    }
    if(log[posi-1].state==LOG_COMMIT){
        cerr << "[ERROR] have commited <logger.cpp 57>\n";
        return false;
    }
    string oper=log[posi-1].op.operation;
    if(oper=="GET"){
        cout << "get get get get get get get get get get get get get get get get " << log[posi-1].op.value << endl;
        if(db.getValue(log[posi-1].op.value,ans)!=0){
            return false;
        }
    }
    if(oper=="DEL"){
        string key=log[posi-1].op.value;
        string oldval;
        parseDelVal(key);
        while(!delqueue.empty()){
            string tkey=delqueue.front();
            delqueue.pop();
            if(db.delValue(tkey,oldval)!=0){
                cerr << "[ERROR] del wrong <logger.cpp 74>\n";
                return false;
            }else{
                log[posi-1].op.delVal.push_back(oldval);
            }
        }
    }
    if(oper=="SET"){
        printf("--------------------logger.96\n");
        string _key,_val,oldval;
        cout << "[log 111] " << log[posi-1].op.value << endl;
        parseSetVal(log[posi-1].op.value,_key,_val);
        cout << "sadfasdf: " << _key << ' ' << _val << endl;
        if(db.setValue(_key,_val,oldval)!=0){
            printf("--------------------logger.107\n");
            return false;
        }else{
            log[posi-1].op.oldVal=oldval;
        }
    }
    log[posi-1].state=LOG_COMMIT;
    printf("--------------------logger.113\n");
    return true;
}

bool Logger::rollbackLog(){
    int _posi=posi-1;
    string op=log[_posi].op.operation;
    string key,val,oldval;
    printf("--------------------logger.127\n");
    if(log[_posi].state==LOG_PRE){
        log.pop_back();
    }else if(log[_posi].state==LOG_COMMIT){
        printf("--------------------logger.131\n");
        if(op=="GET"){
            posi--;
        }else if(op=="DEL"){
            int delpos=0;
            parseDelVal(log[_posi].op.value);
            while(!delqueue.empty()){
                string tkey=delqueue.front();
                delqueue.pop();
                if(db.setValue(tkey,log[_posi].op.delVal[delpos++])!=0){
                    cerr << "[ERROR] recover del operation failed <logger.cpp 120>\n";
                    return false;
                }
            }
        }else if(op=="SET"){
            printf("--------------------logger.146\n");
            int pos;
            string s=log[_posi].op.value;
            pos=s.find(",");
            key=s.substr(0,pos);
            val=log[_posi].op.oldVal.substr(pos+1);
            if(db.setValue(key,val)!=0){
                return false;
            }
        }
        log.pop_back();
    }
    printf("--------------------logger.158\n");
    posi--;
    return true;
}

string Logger::getState(){
    if(posi==0)
        return "~";
    else if(log[posi-1].state==LOG_COMMIT)
        return "0";
    else if(log[posi-1].state==LOG_PRE)
        return "1";
    else if(log[posi-1].state==INVALID)
        return "2";
    else if(log[posi-1].state==LOG_ABORT)
        return "3";
    
    return "ERROR";
}


