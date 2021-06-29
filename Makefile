VPATH =./src
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g 
OBJS = main.o parser.o KVDB.o logger.o conf.o ossSocket.o  RUSocket.o TCPServer.o TCPClient.o coordinate.o participant.o 
INCLUDE = -I./include  -I./layer3/ossSocket 
LIB = -lpthread

kvstore2pcsystem : $(OBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(OBJS) $(LIB) $(INCLUDE)
	@rm *.o

main.o : main.cpp
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

parser.o : parser.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

KVDB.o : ./layer2/KVDB.cpp
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

logger.o : ./layer2/logger.cpp
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

coordinate.o : ./layer1/coordinate.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIB) $(INCLUDE)

# logger.o : logger.cc
# 	$(CPP) $(CXXFLAGS) -c $^ $(LIB) $(INCLUDE)

participant.o :  ./layer1/participant.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIB)  $(INCLUDE)

conf.o : conf.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

ossSocket.o : ./layer3/ossSocket.cc
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

RUSocket.o : ./layer3/RUSocket.cpp
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

TCPServer.o : ./layer3/TCPServer.cpp
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

TCPClient.o : ./layer3/TCPClient.cpp
	$(CPP) $(CXXFLAGS) -c $^  $(LIB) $(INCLUDE)

PHONY: clean

clean:
	rm -f ./kvstore2pcsystem ./*.o ./src/*.gch