#include "../include/TCPServer.h"

char TCPServer::msg[MAXPACKETSIZE];
int TCPServer::num_client;
int TCPServer::last_closed;
bool TCPServer::isonline;
queue<descript_socket*> TCPServer::Message;
vector<descript_socket*> TCPServer::newsockfd;
std::mutex TCPServer::mt;
std::mutex TCPServer::mut[10];
long long recNum=0;

void TCPServer::CloseConnection(descript_socket *desc){
	isonline = false;//!!!
	if(DEBUG)
	cerr << "close client[ id:"<< desc->id <<" ip:"<< desc->ip <<" socket:"<< desc->socket<<" ]" << endl;
	last_closed = desc->id;
	if(desc->socket==0 || close(desc->socket)!=0){
		cerr <<"socket close failed";
		exit(0);
	}

	int id = desc->id;

	mut[1].lock();//LZH
	// auto new_end = std::remove_if(newsockfd.begin(), newsockfd.end(),
	// 				[id](descript_socket *device)
	// 				{ return device->id == id; });
	// newsockfd.erase(new_end, newsockfd.end());
	if(num_client>0) num_client--;
	mut[1].unlock();//LZH

	if(desc != NULL)//这里怎么可能能释放呢？肯定要用完再释放。
	//这里是DEBUG时的LHZ，是我见解太浅了，大神NB，我悟了
	//这是再后来的LHZ，这TM什么垃圾代码，根本就不支持多线程
		delete(desc);
	if(DEBUG){
		cerr << "exit thread: " << this_thread::get_id() << endl;
		cerr << "-----------------------------------\n" << endl;
	}
	
}

void* TCPServer::Task(void *arg)
{
	char msgT[MAXT];//???MAXT
	int n=-1;
	struct descript_socket *desc = (struct descript_socket*) arg;
	pthread_detach(pthread_self());
	if(DEBUG)
    cerr << "open client[ id:"<< desc->id <<" ip:"<< desc->ip <<" socket:"<< desc->socket<<" send:"<< desc->enable_message_runtime <<" ]" << endl;
	int getTime=0;
	timeval tv_out;
	tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;
    setsockopt(desc->socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
	while(1) 
	{
		// cout <<"------------begin\n";
		// cout << "n---"<<n <<endl;
		// sleep(1);         
		// cout << "socket---" <<  desc->socket << endl;
		n = recv(desc->socket, msgT, MAXT, 0);//???MAXT
		// printf("errno is: %d\n",errno);
		// if(errno!=0){
		// 	printf("errno is: %d\n",errno);
		// 	perror("11");
		// 	close(desc->socket);
		// 	pthread_exit(NULL);
		// 	// continue;
		// 	// exit(1);
		// }
		if(n!=-1) 
		{
			if(n==0) 
			{
				// isonline = false;//!!!
				// cerr << "close client[ id:"<< desc->id <<" ip:"<< desc->ip <<" socket:"<< desc->socket<<" ]" << endl;
				// last_closed = desc->id;
				// close(desc->socket);

				// int id = desc->id;

				// mut[1].lock();//LZH
				// auto new_end = std::remove_if(newsockfd.begin(), newsockfd.end(),
				// 									[id](descript_socket *device)
				// 										{ return device->id == id; });
			   	// newsockfd.erase(new_end, newsockfd.end());
			   	// if(num_client>0) num_client--;
			   	// mut[1].unlock();//LZH
			   	break;
			}
			msg[n]='\0';
			if(DEBUG)
			cout << "[DEBUG]> tcpser.95 strlen: " << strlen(msgT);
			desc->message = string(msgT);
			// cout << "================================================ \n" <<  msgT << "\n=============================================\n";
	        //std::lock_guard<std::mutex> guard(mt);//LZH
			mut[2].lock();//LZH
			Message.push( desc );
			sem_post(&ROsem[0]);
			mut[2].unlock();//LZH
			// cout << "----------------end\n";
			break;//为了压力测试，之后要删掉
		}
		//usleep(600);
    }
	// if(desc != NULL)//这里怎么可能能释放呢？肯定要用完再释放。
	// //这里是DEBUG时的LHZ，是我见解太浅了，大神NB，我悟了
	// free(desc);
	// cerr << "exit thread: " << this_thread::get_id() << endl;
	// cerr << "-----------------------------------\n" << endl;
	pthread_exit(NULL);
	return 0;
}

int TCPServer::showSocketfd(){
	cout << "[DEBUG] tcpserver sockfd " << sockfd << endl;
	return 0;
}

int TCPServer::setup(int port, vector<int> opts)
{
	int opt = 1;
	isonline = false;
	last_closed = -1;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
 	memset(&serverAddress,0,sizeof(serverAddress));

	for(unsigned int i = 0; i < opts.size(); i++) {
		if( (setsockopt(sockfd, SOL_SOCKET, opts.size(), (char *)&opt, sizeof(opt))) < 0 ) {
			cerr << "Errore setsockopt" << endl; 
      			return -1;
	      	}
	}

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port        = htons(port);

	if((::bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress))) < 0){
		cerr << "Errore bind" << endl;
		return -1;
	}
	
 	if(listen(sockfd,5) < 0){
		cerr << "Errore listen" << endl;
		return -1;
	}
	num_client = 0;
	isonline = true;
	return 0;
}

void TCPServer::accepted()//这里需要对队列进行加锁访问
{		
		socklen_t sosize    = sizeof(clientAddress);
		descript_socket *so = new descript_socket;
		so->socket=-1;//初始化
		so->socket = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
		if(so->socket<0)
		{
			perror("accept() error! <TCPS.cpp 157>\n");
			exit(1);
		}

		struct sockaddr_in sa;
		socklen_t len = sizeof(sa);
		if(!getpeername(so->socket, (struct sockaddr *)&sa, &len))
		{
			so->port=ntohs(sa.sin_port);
		}
		cout << "+++++++++++++++++++++" << so->port << endl;

		// so->socket          = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
		
		//cout <<"--------------\n";

		mut[0].lock();//LZH
		// cout << "------------ip:"<<inet_ntoa(clientAddress.sin_addr)<< endl;
		// cout << "------------socket:"<<so->socket<< endl;
		so->id              = num_client;
		so->ip              = inet_ntoa(clientAddress.sin_addr);
		// newsockfd.push_back( so );
		if(DEBUG)
		cerr << "accept client[ id:" << so->id << 
							" ip:" << so->ip << 
						" handle:" << so->socket << " ]" << endl;
		pthread_create(&serverThread[num_client], NULL, &Task, (void *)so);//!!!这里的优化
		isonline=true;
		num_client++;
		mut[0].unlock();//LZH
}

// vector<descript_socket*> TCPServer::getMessage()//Version 1.0只返回一个值进行处理
// {
// 	std::lock_guard<std::mutex> guard(mt);
// 	return Message;
// }

queue<descript_socket*> TCPServer::getMessage()//Version 1.0只返回一个值进行处理,这样也不需要clean函数
{
	std::lock_guard<std::mutex> guard(mt);
	// return Message;
	queue<descript_socket*> MT;
	int k=0;
	while(!Message.empty()){
		k++;
		MT.push(Message.front());
		Message.pop();
		if(k>=10) break;
	}
	return MT;
}

void TCPServer::Send(descript_socket* desc,string msg)
{
	send(desc->socket,msg.c_str(),msg.length(),0);
}

int TCPServer::get_last_closed_sockets()
{
	return last_closed;
}

// void TCPServer::clean(int id)
// {
// 	Message[id]->message = "";
// 	memset(msg, 0, MAXPACKETSIZE);
// }

string TCPServer::get_ip_addr(int id)
{
	return newsockfd[id]->ip;
}

bool TCPServer::is_online() 
{
	return isonline;
}

void TCPServer::detach(int id)
{
	close(newsockfd[id]->socket);
	newsockfd[id]->ip = "";
	newsockfd[id]->id = -1;
	newsockfd[id]->message = "";
} 

void TCPServer::closed() 
{
	close(sockfd);
}

