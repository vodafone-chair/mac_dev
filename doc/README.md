### Description
This program is a framework for a MAC environment. The environment consists of a base station and connected UEs. The program runs on an Ubuntu OS. The purpose is to have a base structure for MAC development.

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
Four consoles will open. One console is the channel server, which functions as Wi-Fi channel. That means, it will retransmit a received message to all nodes (base station and UEs) connected to the channel server. One console is the base station, which organizes and schedules the communication. Two consoles are two UEs (1 console for 1 UE), which reply the received message to the base station.

### Signaling
The program also includes signaling which is used to assign UEs to the base station. The base station sends periodically a "RequestUE" message to all UEs (currently every 20th packet, i.e. 20s). After reception of this message, the UEs will reply with a "UeReply" message. The base station stores all replying UEs in a list, which is used for the round robin scheduling. After 60s the list is purged to remove disappeared UEs, i.e. which does not reply to the "RequestUE" message anymore.

### Main Application: round robin echo server client
The main application running on the base station is an echo server client. The base station transmits a message to a UE, which will send an answer to the base station. The scheduling for the UEs is a simple round robin scheme.

### D2D application
A second application is D2D (device-to-device) communication. The base station sends a "GrantD2d" message to a UE, which is then allowed to send a D2D message. That means, the UE can directly send its message (payload) to its neighbors and the base station just controls the communication. Normally, in cellular communication a UE just sends its message (payload) to the base station (uplink), which distributes this message (downlink) to the destination UE. 

To activate the D2D application: in ```$ src/app_base_station.cpp``` set ```uint32_t appUsed = 1``` to ```uint32_t appUsed = 2``` and run ```make```.

### System specification
This module was tested on the following system:
* Ubuntu: 18.04 LTS
* gcc/g++: 7.3.0

### Authors
Sebastian Kühlmorgen, TU Dresden, Vodafone Chair, <sebastian.kuehlmorgen@tu-dresden.de>
