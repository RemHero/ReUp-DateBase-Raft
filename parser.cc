#include "./include/parser.hpp"

void outCommand(Command c){
    printf("***********************\n /add:%s\n/port:%s\n/posi:%s\n/index:%s\n/op:%s\n/value:%s\n/res:%s\n***********************"
    ,c.add.c_str(),c.port.c_str(),c.pos.c_str(),c.index.c_str(),c.op.c_str(),c.value.c_str(),c.res.c_str());
}

//在末尾加上/是为了防止粘包！！！！！！！！！很重要！！！
Command Parser::commandParser(string cmd){//!!!!!!!!!!!!need gengxin !
    // printf("--------------------par.5\n");
    // printf("%s\n",cmd.c_str());
    // printf("--------------------par.7\n");
    int beg=0,typePoi=-1,posPoi=-1,indexPoi=-1,opPoi=-1,valuePoi=-1,portPoi=-1,addPoi=-1,resPoi=-1,endPoi=-1;
    string type,add,op,value,index,pos,port,res;
    if(cmd.find("/add")!=string::npos){addPoi=cmd.find("/add:");}
    if(cmd.find("/port")!=string::npos){portPoi=cmd.find("/port:");}
    if(cmd.find("/posi")!=string::npos){posPoi=cmd.find("/posi:");}
    if(cmd.find("/index:")!=string::npos){indexPoi=cmd.find("/index:");}
    if(cmd.find("/op:")!=string::npos){opPoi=cmd.find("/op:");}
    if(cmd.find("/value:")!=string::npos){valuePoi=cmd.find("/value:");}
    if(cmd.find("/res")!=string::npos){resPoi=cmd.find("/res:");}
    if(cmd.find("/",resPoi+1)!=string::npos){endPoi=cmd.find("/",resPoi+1);}

    if((typePoi=cmd.find("[PRE]"))!=string::npos){
        type=cmd.substr(typePoi,5);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        op=cmd.substr(opPoi+4,resPoi-opPoi-4);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
        // res=cmd.substr(resPoi+5);
    }else if((typePoi=cmd.find("[ROLL]"))!=string::npos){
        type=cmd.substr(typePoi,6);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[COM]"))!=string::npos){
        type=cmd.substr(typePoi,5);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[PRERE]"))!=string::npos){
        type=cmd.substr(typePoi,7);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[ROLLRE]"))!=string::npos){
        type=cmd.substr(typePoi,8);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[COMRE]"))!=string::npos){
        type=cmd.substr(typePoi,7);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[HEART]"))!=string::npos){
        printf("--------------------par.50\n");
        type=cmd.substr(typePoi,7);
        // printf("%s\n",type.c_str());
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        // printf("%s\n",add.c_str());
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        // printf("%s\n",port.c_str());
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        // printf("%s\n",pos.c_str());
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        // printf("%s\n",index.c_str());
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
        // printf("%s\n",res.c_str());
    }else if((typePoi=cmd.find("[HEARTRE]"))!=string::npos){
        printf("--------------------par.57\n");
        type=cmd.substr(typePoi,9);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
        // printf("%s\n",type.c_str());
        // printf("%s\n",add.c_str());
        // printf("%s\n",port.c_str());
        // printf("%s\n",pos.c_str());
        // printf("%s\n",index.c_str());
        // printf("%s\n",res.c_str());
    }else if((typePoi=cmd.find("[VOTE]"))!=string::npos){
        type=cmd.substr(typePoi,6);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[VOTERE]"))!=string::npos){
        type=cmd.substr(typePoi,8);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        port=cmd.substr(portPoi+6,posPoi-portPoi-6);
        pos=cmd.substr(posPoi+6,indexPoi-posPoi-6);
        index=cmd.substr(indexPoi+7,resPoi-indexPoi-7);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }else if((typePoi=cmd.find("[CLIENT]"))!=string::npos){
        type=cmd.substr(typePoi,8);
        add=cmd.substr(addPoi+5,portPoi-addPoi-5);
        op=cmd.substr(opPoi+4,resPoi-opPoi-4);
        res=cmd.substr(resPoi+5,endPoi-resPoi-5);
    }
    outCommand(Command(type,add,port,pos,index,op,value,res));
    return Command(type,add,port,pos,index,op,value,res);
}

void Parser::reComPar(string &cmd,string &op,string &val,string &oldval,string &state){
    int opPos,valPos,oldvalPos,statePos,endPoi;
    printf("--------------------par.117\n");
    printf("%s\n",cmd.c_str());
    opPos=cmd.find("op:");valPos=cmd.find("val:");oldvalPos=cmd.find("oldval:");
    statePos=cmd.find("state:");endPoi=cmd.find("}");

    printf("cmd:  %s\n",cmd.c_str());
    printf("--------------------par.120\n");
    op=cmd.substr(opPos+3,valPos-opPos-3);
    printf("--------------------par.122\n");
    printf("op:%d  val:%d  oldval:%d  state:%d  ",opPos,valPos,oldvalPos,statePos);
    val=cmd.substr(valPos+4,oldvalPos-valPos-4);
    printf("--------------------par.124\n");
    printf("op:%d  val:%d  oldval:%d  state:%d  ",opPos,valPos,oldvalPos,statePos);
    oldval=cmd.substr(oldvalPos+7,statePos-oldvalPos-7);
    printf("--------------------par.126\n");
    printf("op:%d  val:%d  oldval:%d  state:%d  ",opPos,valPos,oldvalPos,statePos);
    state=cmd.substr(statePos+6,endPoi-statePos-6);
    printf("--------------------par.128\n");
    printf("op:%d  val:%d  oldval:%d  state:%d  ",opPos,valPos,oldvalPos,statePos);
}
/*
        @param raw the raw string you want to warp into the string message
        @return a string message
            e.g. CS06142 -> $7\r\nCS06142\r\n
*/
string Parser::getStringMessage(string & raw)//��������ĸ�ʽ 
{
    string rc = "$";
    int length = (int) raw.length();
    rc += std::to_string(length);
    rc += "\r\n";
    rc += raw;
    rc += "\r\n";
    return rc;
}

/*
        @return a success message
    */
string Parser::getSuccessMessage()//����ɹ� 
{
    string rc = "+OK\r\n";
    return rc;
}

/*
        @ return an error message
    */
string Parser::getErrorMessage()//����ʧ�� 
{
    string rc = "-ERROR\r\n";
    return rc;
}

/*
        @param raw the raw interger you wanna warp into the interger message
        @return a interger message
            e.g. 10 -> :10\r\n
    */
string Parser::getIntergerMessage(int raw)//�����ʽ 
{
    string rc = ":";
    rc += std::to_string(raw);
    rc += "\r\n";
    return rc;
}

/*
        @param raw the raw string vactor you wanna warp into the RESP message
        @return a string vactor message
            e.g. Cloud Computing-> *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
string Parser::getRESPArry(std::vector<string> & raw)//ת����RESP���и�ʽ������ֵ 
{
    string rc = "*";
    int number = raw.size();

    rc += std::to_string(number);
    rc += "\r\n";
    
    for(size_t i = 0; i < raw.size(); i++) 
    {
        rc += getStringMessage(raw[i]);
    }

    return rc;
}

string Parser::getRESPArry(string raw)//��ֵ 
{
    string rc = "*";
    int number = 1;

    rc += std::to_string(number);
    rc += "\r\n";
    rc += getStringMessage(raw);

    return rc;
}

/*
        @para message the raw message recieved
        @return the string parsered from message
            e.g. CS06142 <- $7\r\nCS06142\r\n
    */
string Parser::parseStringMessage(char * message, int * length)//�Ӹ�ʽ����ȡ������Ϣ����key 
{
    string rc = "";
    int strl = 0;
    int pos = 0;
    assert(message[0] == '$');

    strl = atoi(message + 1);
    pos = 3 + std::to_string(strl).size();
    rc = string(message + pos, strl);

    if(length) {
        *length = pos + strl + 2;
    }

    return rc;
}

/*
        @para message the raw message recieved
        @return the interger parsered from message
            e.g. 10 <- :10\r\n
    */
int Parser::parserIntergerMessage(char * message, int * length)//�õ�������Ϣ 
{
    int rc = 0;
    assert(message[0] == ':');
    rc = atoi(message + 1);

    if(length) {
        *length = 3 + std::to_string(rc).length();
    }
    return rc;
}

/*
        @para message the raw message recieved
        @return the status of the excution
            e.g. +OK\r\n -> true
                 -ERROR\r\n -> false
    */
bool Parser::parserSuccessMessage(char * message, int * length)//�ӳɹ�/ʧ������ȡ��Ϣ 
{
    assert(message[0] == '+' || message[0] == '-');
    if(message[0] == '+') {
        if(length) {
            *length = 5;
        }
        return true;
    }
    else {
        if(length) {
            *length = 8;
        }
        return false;
    }
}

/*
        @para message the raw message recieved
        @return the string vector parsered from message
            e.g. Cloud Computing <- *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
void Parser::parserRESPArry(char * message, std::vector<string> & rc, int * length)//�õ����ݿ���value��Ϣ 
{
    int number = 0;
    int pos = 0;

    assert(message[0] == '*');
    number = atoi(message + 1);
    pos += std::to_string(number).length() + 3;

    for(int i = 0; i < number; i++) {
        int len = 0;
        string tmp = parseStringMessage(message + pos, &len);
        rc.push_back(tmp);
        pos += len;
    }

    if(length) {
        *length = pos;
    }
}
