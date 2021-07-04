 Distributed Key-Value Database
=== 
A Key-Value storage system based on RAFT.

<img src="https://img.shields.io/badge/Release-Ver1.0.0-blue.svg" />
<div align=center><img src="https://github.com/RemHero/RemHero/blob/main/png/RUDB-logo.png"  alt="ReUp"/><br/></div>

Usage
---
Compile the RUDB.
``` shell
make
make clean
```

Run the RUDB's client.
``` shell
./kvstore2pcsystem
```

Run the program.
```shell
./kvstore2pcsystem --c <filename>.config
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
DEL key1 key2 ... keyn
```
