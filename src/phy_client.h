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

#include "phy_interface.h"

typedef uint32_t socklen_t;

class PhyClient: public PhyInterface
{
public:
  
  PhyClient();
  virtual~PhyClient();

  void Initialize(PhyInfo phyInfo);                   // initialize client
  void SendData(char* data_to_send, uint32_t len);    // send data packet from higher layer (is called in the MAC)
  void ReceiveData();                                 // receive data packet from lower layer (calls a MAC function)

private:
  void StopClient();                                  // stop client
  uint16_t GetServerPort();                           // get port number of the server to which the data is sent
  void SetServerPort(uint16_t serverPort);            // set port number of the server to which the data is sent
  void SetServerIp(std::string serverIp);             // set Ipv4 of the server (destination)
  void SetSourcePort(uint16_t sourcePort);            // set port number of this node
  void SetSourceIp(std::string sourceIp);             // set Ipv4 of the node (source)
  void SetRunning(bool isRunnung);                    // set loop running for the reception of packets

  int m_sock;                                // socket of this node
  struct sockaddr_in m_nodeSocketAddress;    // node socket address
  uint16_t m_sourcePort;                     // port of this node
  std::string m_sourceIp;                    // source ip address, "0.0.0.0" is INADDR_ANY
  struct sockaddr_in m_serverSocketAddress;  // server address
  uint16_t m_serverPort;                     // server port
  std::string m_serverIp;                    // server ip address
  bool m_running;                            // receive loop running
  uint32_t m_bufferLength;                   // max buffer length

  void CreateNodeSocket();                   // create a socket
  void CreateServerAddress();                // create the server address
  void CreateSourceSocketAddress();          // create node socket address
  void BindSourceSocketAddress();            // bind node socket address
};

#endif /* CLIENT_H */



