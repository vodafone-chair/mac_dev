#ifndef APP_H
#define APP_H


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
#include <thread>
#include <vector>            // vector

#include "mac.h"
#include "datagram.h"

class Mac;

class App
{
public:

  App();
  virtual~App();

  void SetMac(Mac* mac);                                // set mac (for the send function)
  void SetRunning(bool isRunning);                      // set m_running
  virtual void Initialize ();                           // initialize functions

  virtual void TriggerPackets (uint32_t interval) = 0;  // triggers packets for transmission, loop, implemented in child class
  virtual void SendPacket (PacketData packet) = 0;      // receive packet, implemented in child class
  virtual void ReceivePacket (PacketData packet) = 0;   // receive packet, implemented in child class

protected:
  PacketData GeneratePacket(char* payload, uint32_t payloadSize, uint8_t dstAddress); // generates the payload of the packet, input char
  PacketData GeneratePacket(uint8_t dstAddress, std::string stringIn); // generates the payload of the packet, input string
  Header GenerateHeader (uint8_t version, uint8_t sequNum, uint8_t srcAddress, uint8_t dstAddress, uint8_t crc_header);
  Header GenerateHeader (uint8_t dstAddress);     // generates the header of the packet

  Mac* m_mac;                 // mac object
  bool m_running;             // control the transmission loop
  uint8_t m_nodeAddress;      // address of the node
  uint8_t m_sequNumber;       // sequence number of send packets
  uint64_t m_counter;         // counter of the TX loop

};


#endif /* APP_H */
