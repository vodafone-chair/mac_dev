### Description
This program is a framework for a MAC environment. It consists of a base station and connected UEs. The program runs on an Ubuntu OS. The purpose is to have a base structure for MAC development.

### Usage
Build/rebuild the program.
```sh
$ cmake CMakeLists.txt
$ make
```
To visualize the scenario in Ubuntu 18.04.
```sh
$ ./run_example.sh
```
### Remarks
Four consoles will open. One console is the channel server, which functions as channel. That means it will retransmit a message to all nodes connected to the channel server. One console is the base station, which organizes and schedules the communication. Two consoles are two UEs, which reply to the base station.

After a certain time, the base station sends a RequestUE Message. After reception of this message, the UEs will reply with a UeReply Message. This procedure is used to assign the UEs to the base station.

The main application running on the base station is an echo server client. The base station transmits data to the UEs, which will send an answer to the base station. The scheduling is a simple round robin scheme.

### System specification
This module was tested on the following system:
* Ubuntu: 18.04 LTS
* gcc/g++: 7.3.0

### Authors
Sebastian Kühlmorgen, TU Dresden, Vodafone Chair, <sebastian.kuehlmorgen@tu-dresden.de>
