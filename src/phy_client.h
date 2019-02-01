//============================================================================
// Name        : client.h
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   : 
// Description : MAC for GFDM
//============================================================================

#ifndef CLIENT_H
#define CLIENT_H

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
#include <thread>

#include "phy_interface.h"

typedef uint32_t socklen_t;

class PhyClient: public PhyInterface
{
public:
  
  PhyClient();
  virtual~PhyClient();

  void Initialize();                                  // initialize client
  void SendData(char* data_to_send, uint32_t len);    // send data packet from higher layer (is called in the MAC)
  void ReceiveData();                                 // receive data packet from lower layer (calls a MAC function)

private:
  void StopClient();                                  // stop client
  uint16_t GetServerPort();                           // get port number of the server to which the data is sent
  void SetServerPort(uint16_t serverPort);            // set port number of the server to which the data is sent
  void SetRunning(bool isRunnung);                    // set loop running for the reception of packets

  int m_sock;
  struct sockaddr_in m_server_address; // server address
  uint16_t m_server_port;              // server port
  const char* m_server_name;           // server name (localhost, 127.0.0.1)
  bool m_running;                      // receive loop running
  uint32_t m_bufferLength;             // max buffer length

  void CreateSocket();                 // create a socket
  void CreateServerAddress();          // create the server address

  std::string PrintAddress(sockaddr_in address, std::string prefix, std::string suffix = ""); // print sockaddr_in address (IP, Port)
};

#endif /* CLIENT_H */



