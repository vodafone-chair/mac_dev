//============================================================================
// Name        : client.cpp
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   : 
// Description : MAC for GFDM
//============================================================================

#include "phy_client.h"

PhyClient::PhyClient () :
    PhyInterface::PhyInterface (), m_sock (0), m_sourcePort (0), m_sourceIp ("0.0.0.0"), m_serverPort (8877), m_serverIp ("127.0.0.1"), m_running (true), m_bufferLength (1024)
{
}

PhyClient::~PhyClient ()
{
  StopClient ();
}

void
PhyClient::StopClient ()
{
  m_running = false;

  close (m_sock);
}

uint16_t
PhyClient::GetServerPort ()
{
  return m_serverPort;
}

void
PhyClient::SetServerIp (std::string serverIp)
{
  m_serverIp = serverIp;
}

void
PhyClient::SetServerPort (uint16_t serverPort)
{
  m_serverPort = serverPort;
}

void
PhyClient::SetSourceIp (std::string sourceIp)
{
  m_sourceIp = sourceIp;
}

void
PhyClient::SetSourcePort (uint16_t sourcePort)
{
  m_sourcePort = sourcePort;
}

void
PhyClient::SetRunning (bool isRunnung)
{
  m_running = isRunnung;
}

void
PhyClient::Initialize (PhyInfo phyInfo)
{
  SetServerPort (phyInfo.m_dstPort);     // set port of the channel server (we want to reach the server)
  SetServerIp (phyInfo.m_dstIpAddress);  // set Ipv4 address of the server
  SetSourcePort (phyInfo.m_srcPort);     // set port of the node
  SetSourceIp (phyInfo.m_srcIpAddress);  // set Ipv4 address of the node

  CreateServerAddress ();       // creates the server socket address to which the data is sent
  CreateSourceSocketAddress (); // creates the node socket address
  CreateNodeSocket ();          // builds the socket of the node
  BindSourceSocketAddress ();   // binds node socket address

  std::cout << Functions::PrintSocketAddress (m_nodeSocketAddress, "Node socket address: ", "\n");
  std::cout << Functions::PrintSocketAddress (m_serverSocketAddress, "Server to reach: ", "\n");

  if (m_mac == 0)
    {
      std::cout << "PhyClient::Initialize: mac object is 0. Set the object in main.cpp. Enter 0 to exit." << std::endl;

      std::string s;
      std::cin >> s; // stop console to be closed

      exit (EXIT_FAILURE);
    }
}

void
PhyClient::CreateServerAddress ()
{
  memset (&m_serverSocketAddress, 0, sizeof(m_serverSocketAddress));    // socket address used for the server, set all to 0
  m_serverSocketAddress.sin_family = AF_INET;                           // Ipv4 address space
  inet_aton (m_serverIp.c_str (), &m_serverSocketAddress.sin_addr);     // set local host Ipv4
  m_serverSocketAddress.sin_port = htons (m_serverPort);                // htons: host to network short: transforms a value in host byte order to network byte
}

void
PhyClient::CreateSourceSocketAddress ()
{
  memset (&m_nodeSocketAddress, 0, sizeof(m_nodeSocketAddress));
  m_nodeSocketAddress.sin_family = AF_INET;
  inet_aton (m_sourceIp.c_str (), &m_nodeSocketAddress.sin_addr);     // set local host Ipv4
  m_nodeSocketAddress.sin_port = htons (m_sourcePort);
}

void
PhyClient::CreateNodeSocket ()
{
  if ((m_sock = socket (PF_INET, SOCK_DGRAM, 0)) < 0)           // create a UDP socket
    {
      printf ("Client::CreateSocket: Could not create a socket\n");
      return;
    }
}

void
PhyClient::BindSourceSocketAddress ()
{
  if ((bind (m_sock, (struct sockaddr *) &m_nodeSocketAddress, sizeof(m_nodeSocketAddress))) < 0)
    {
      printf ("PhyClient::BindSocket: Could not bind socket.\n");
      return;
    }
}

void
PhyClient::SendData (char* data_to_send, uint32_t len)
{
  sendto (m_sock, data_to_send, len, 0, (struct sockaddr*) &m_serverSocketAddress, sizeof(m_serverSocketAddress));
}

void
PhyClient::ReceiveData ()
{
  struct sockaddr_in address; // address of the packet's source
  socklen_t address_len (0);

  while (m_running)
    {
      char buffer[m_bufferLength];
      int len = recvfrom (m_sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &address, &address_len); // 0, NULL, NULL);

      if (len == -1)
        std::cout << "Error: Client::StartReceiving: recv" << std::endl;

      m_mac->ReceiveData (buffer, len);  // pass to MAC
    }
}


