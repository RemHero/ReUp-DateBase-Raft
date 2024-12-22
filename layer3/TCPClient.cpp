#include "TCPClient.h"

TCPClient::TCPClient()
	: sock(-1)
	, port(0)
	, address("")
	, is_connected(false)
{
	memset(&server, 0, sizeof(server));
}

TCPClient::~TCPClient()
{
	if (sock != -1) {
		close(sock);
		sock = -1;
	}
}

bool TCPClient::setup(string address, int port)
{	
	if (is_connected) {
		close(sock);
		sock = -1;
		is_connected = false;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		cerr << "Could not create socket: " << strerror(errno) << endl;
		return false;
	}

	int keepalive = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0) {
		cerr << "Failed to set SO_KEEPALIVE: " << strerror(errno) << endl;
	}

	if ((signed)inet_addr(address.c_str()) == -1)
	{
		struct hostent *he;
		struct in_addr **addr_list;
		if ( (he = gethostbyname( address.c_str() ) ) == NULL)
		{
			cout<<"Failed to resolve hostname\n";
			return false;
		}
		addr_list = (struct in_addr **) he->h_addr_list;
		for(int i = 0; addr_list[i] != NULL; i++)
		{
			server.sin_addr = *addr_list[i];
			break;
		}
	}
	else
	{
		server.sin_addr.s_addr = inet_addr( address.c_str() );
	}
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cerr << "Connect failed: " << strerror(errno) << endl;
		close(sock);
		sock = -1;
		return false;
	}

	is_connected = true;
	this->address = address;
	this->port = port;
	return true;
}

bool TCPClient::Send(string data)
{
	if(sock != -1) 
	{
		int send_len=0;
		send_len=send(sock , data.c_str() , strlen( data.c_str() ) , 0);
		if( send_len < 0)
		{
			// cout << "Send failed : " << data << endl;
			return false;
		}
		if(send_len!=strlen( data.c_str() )){
			cerr << "check if send all? <TCPC.cpp 63>\n";
			return false;
		}
	}
	else
		return false;
	return true;
}

string TCPClient::receive(int size)
{
	if (!is_connected || size <= 0) {
		return "";
	}

	vector<char> buffer(size);
	ssize_t received = recv(sock, buffer.data(), size, 0);
	
	if (received < 0) {
		cerr << "Receive failed: " << strerror(errno) << endl;
		return "";
	}
	
	if (received == 0) {
		is_connected = false;
		close(sock);
		sock = -1;
		return "";
	}

	return string(buffer.data(), received);
}

string TCPClient::read()
{
	char buffer[1] = {};
	string reply;
	while (buffer[0] != '\n') {
		if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
		{
			return nullptr;
		}
		reply += buffer[0];
	}
	return reply;
}

void TCPClient::exit()
{
	if (sock != -1) {
		close(sock);
		sock = -1;
	}
	is_connected = false;
}