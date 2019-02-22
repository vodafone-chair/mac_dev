//============================================================================
// Name        : channel_server.h
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   : 
// Description : MAC for GFDM
//============================================================================

#ifndef CHANNEL_SERVER_H
#define CHANNEL_SERVER_H

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
#include <vector>            // vector
#include <algorithm>         // find

#include "datagram.h"

typedef uint32_t socklen_t;

class ChannelServer
{
public:
  
  ChannelServer();
  ChannelServer(uint16_t port);
  virtual~ChannelServer();

  void Initialize(); // initialize server
  uint16_t GetServerPort();
  void SetServerPort(uint16_t serverPort);

//  void Receive(char* buffer, uint32_t lenBuff);   // receive function, not used
  void StartReceivingLoop();    // start the receiving loop and send data to all nodes

private:

  int m_sock;
  uint16_t m_server_port;
  bool m_running;
  uint16_t m_bufferLength;
  struct sockaddr_in m_server_bind_address; // server address
  std::vector<uint16_t> m_portsUes;         // ports of UEs and BS

  void CreateServerAddress();   // create the server address

  void CreateSocket();          // create a socket
  void BindSocket();            // bind the socket

  void AddPortToVector(uint16_t port);   // add port to vector
  bool IsPortIncluded(uint16_t port);    // check if port is already stored

  void RetransmitToAll(uint16_t receivingPort, char* buffer, uint32_t len, struct sockaddr_in); // retransmits to all nodes stored in the vector
};




#endif /* CHANNEL_SERVER_H */



