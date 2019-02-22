#ifndef PHY_INTERFACE_H
#define PHY_INTERFACE_H


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
#include <vector>            // vector

#include "mac.h"
#include "datagram.h"

class Mac;

class PhyInterface
{
public:

  PhyInterface();
  virtual~PhyInterface();

  virtual void Initialize (PhyInfo phyInfo) = 0;                   // Initialize functions
  virtual void SendData (char* data_to_send, uint32_t len) = 0;    // send data, implemented in child class
  virtual void ReceiveData () = 0;                                 // receive data, implemented in child class

  void SetMac(Mac* mac);                                           // set mac (for the send function)

protected:
  Mac* m_mac;                                                      // mac object

};


#endif /* PHY_INTERFACE_H */
