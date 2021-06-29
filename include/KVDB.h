//
// Created by Rem_Hero on 2021/5/31.
//

#ifndef SHAREFILE_KVDB_H
#define SHAREFILE_KVDB_H
#include <map>
#include <string.h>
#include <iostream>
using namespace std;

class KVDB{
    private:
        enum{OK,ERROR};
        map<string,string> DB;
    public:
        KVDB();
        int setValue(string key,string value,string oldval="");
        int delValue(string key,string oldval="");
        int getValue(string key,string &value);
};


#endif //SHAREFILE_KVDB_H
