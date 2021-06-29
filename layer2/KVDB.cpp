//
// Created by Rem_Hero on 2021/5/31.
//

#include "../include/KVDB.h"
using namespace std;

KVDB::KVDB(){

}

int KVDB::setValue(string key,string value,string oldval){
    if(DB.find(key)!=DB.end()){
        oldval=DB[key];
    }
    DB[key]=value;
    return OK;
}

int KVDB::delValue(string key,string oldval){
    if(DB.find(key)!=DB.end()){
        oldval=DB[key];
    }
    if(DB.erase(key)==0){
        cerr << "[ERROR] del not find in DB <KVDB.cpp 28>\n";
        return ERROR;
    }
    return OK;
}

int KVDB::getValue(string key,string &value){
    if(DB.find(key)!=DB.end()){
        cout << "[DB33]get okokokokokokokokokokokokokokokokokokokokokokokok\n";
        value=DB[key];
        return OK;
    }
    return ERROR;
}
