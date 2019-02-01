#ifndef MAC_H
#define MAC_H

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

#include "app.h"
#include "phy_interface.h"
#include "datagram.h"
#include "mac_queue.h"

class App;
class PhyInterface;

class Mac
{
public:

  Mac();
  virtual~Mac();

  void SetPhyInterface(PhyInterface* phyInterface);                 // set phy interface (for the send function)
  void SetApp(App* app);                                            // set application, e.g. UE or base station (for the receive function)
  virtual void Initialize(uint8_t nodeAddress);                     // initialize functions and check objects

  virtual void EnqueuePacket(PacketData packet) = 0;                // is implemented in the child class
  virtual void ReceiveData(char* receivedData, uint32_t len) = 0;   // is implemented in the child class

  uint8_t GetNodeAddress();                                         // get the address of the node

protected:
  virtual void DeQueuePacket() = 0;                                 // is implemented in the child class
  virtual void AddHeader(PacketData packet) = 0;                    // is implemented in the child class
  virtual void SendData(char* buffer, uint32_t lenHeadAndPayl);     // is implemented in the child class

  App* m_app;                      // interface to the application
  PhyInterface* m_phyInterface;    // interface to the phy
  uint8_t m_nodeAddress;           // node address
  MacQueue* m_macQueue;            // MAC queue object
};


#endif /* MAC_H */
