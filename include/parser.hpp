#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <cassert>
using namespace std;
struct Command{
    string reCmd;
    string type;
    string add,port;
    string op,value,index,pos,res;
    Command(string t_="",string a_="",string po_="",string p_="",string i_="",string o_="",string v_="",string r_=""){
        type=t_,port=po_,op=o_,value=v_,index=i_,pos=p_,res=r_,add=a_;
    }
};

class Parser
{
private:
public:
    
    //parse the cmd into Command
    Command commandParser(string cmd);
    //parse the recover cmd's val
    void reComPar(string &cmd,string &op,string &val,string &oldval,string &state);
    /*
        @param raw the raw string you want to warp into the string message
        @return a string message
            e.g. CS06142 -> $7\r\nCS06142\r\n
    */
    string getStringMessage(string & raw);

    /*
        @return a success message
    */
    string getSuccessMessage();

    /*
        @ return an error message
    */
    string getErrorMessage();

    /*
        @param raw the raw interger you wanna warp into the interger message
        @return a interger message
            e.g. 10 -> :10\r\n
    */
    string getIntergerMessage(int raw);

    /*
        @param raw the raw string vactor you wanna warp into the RESP message
        @return a string vactor message
            e.g. Cloud Computing-> *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
    string getRESPArry(vector<string> & raw);
    string getRESPArry(string raw);

    /*
        @para message the raw message recieved
        @return the string parsered from message
            e.g. CS06142 <- $7\r\nCS06142\r\n
    */
    string parseStringMessage(char * message, int * length=nullptr);

    /*
        @para message the raw message recieved
        @return the interger parsered from message
            e.g. 10 <- :10\r\n
    */
    int parserIntergerMessage(char * message, int * length=nullptr);

    /*
        @para message the raw message recieved
        @return the status of the excution
            e.g. +OK\r\n -> true
                 -ERROR\r\n -> false
    */
    bool parserSuccessMessage(char * message, int * length=nullptr);

    /*
        @para message the raw message recieved
        @return the string vector parsered from message
            e.g. Cloud Computing <- *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
    void parserRESPArry(char * message, vector<string> & rc, int * length=nullptr);


};


#endif






























