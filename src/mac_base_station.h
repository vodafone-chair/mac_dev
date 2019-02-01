#ifndef MAC_BASE_STATION_H
#define MAC_BASE_STATION_H

#include <arpa/inet.h>       //close
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>          //strlen
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>          //close
#include <iostream>

#include "mac.h"
#include "datagram.h"

class MacBs : public Mac
{
public:

  MacBs();
  virtual~MacBs();

  void EnqueuePacket(PacketData packet);              // enqueue packets from higher layer, e.g. application
  void ReceiveData(char* receivedData, uint32_t len); // receive data from lower layer, e.g. phy interface

private:
  void DeQueuePacket();                               // dequeue packet
  void AddHeader(PacketData packet);                  // add the header
};


#endif /* MAC_BASE_STATION_H */
