 Distributed Key-Value Database
=== 
A Key-Value storage system based on RAFT.

<img src="https://img.shields.io/badge/Release-Ver2.0.0-blue.svg" />
<div align=center><img src="https://github.com/RemHero/RemHero/blob/main/png/RUDB-logo.png"  alt="ReUp"/><br/></div>
“如果喜欢，感觉有用，麻烦随手star一下嗷~”
Usage
---
The compiling environment.
```shell
Ubuntu 20.04
```

Compile the RUDB.
``` shell
make
make clean
```

Run the RUDB's client.
``` shell
./kvstore2pcsystem
```

Run the RUDB's servers.
```shell
./kvstore2pcsystem --c <filename>.conf
```

Configure node.
``` shell
mode coordinator
coordinator_info 127.0.0.1:8001
participant_info 127.0.0.1:8002
participant_info 127.0.0.1:8003
participant_info 127.0.0.1:8004
```

You can send the command:
```
SET key value
GET key
DEL key
```

Example Usage
---
Example Usage of client.
> ./kvstore2pcsystem

Example Usage of servers.
You can run this command line **in sequence** in different Windows.
> ./kvstore2pcsystem --c ./other/coordinator.conf

> ./kvstore2pcsystem --c ./other/coordinator1.conf

> ./kvstore2pcsystem --c ./other/coordinator2.conf

> ./kvstore2pcsystem --c ./other/coordinator3.conf
