#ifndef APP_BASE_STATION_H
#define APP_BASE_STATION_H

#include <arpa/inet.h>       //close
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>          //strlen
#include <cstring>
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

#include "app.h"
#include "datagram.h"

class AppBs : public App
{
public:

  AppBs ();
  virtual~AppBs ();

  void TriggerPackets (uint32_t interval);
  void SendPacket (PacketData packet);
  void ReceivePacket (PacketData packet);

private:
  void SendTestData();    // send test data
  void SendUeRequest();   // asks for all UEs in the system
  void SendGrantD2dMsg(); // grant a node to send a D2D message
  void SendRoundRobin();  // send packets in round robin scheme

  void AddUeAddressToVector (uint8_t ueAddress);  // adds UE address to vector
  bool IsUeAddressIncluded (uint8_t ueAddress);   // checks if UE address is already stored in vector
  uint8_t GetNextDstAddress ();                   // returns the next UE address for the round robin schedule
  void Purge();                                   // purge vector with UE addresses, all entries which are expired are dropped

  uint16_t m_rr_counter;               // counter for the round robin schedule
  time_t m_expirationTimeNodeVector;   // expiration time, when the nodes are removed from the round robin node vector
  std::vector<UeInfo> m_ueAddressList; // list of UE nodes (source addresses) in the channel
};

#endif /* APP_BASE_STATION_H */
