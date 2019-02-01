//============================================================================
// Name        : client.cpp
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   : 
// Description : MAC for GFDM
//============================================================================

#include "phy_client.h"

PhyClient::PhyClient () :
    PhyInterface::PhyInterface(), m_sock (0), m_server_port (8877), m_server_name ("localhost"), m_running (true), m_bufferLength (1024)
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
  return m_server_port;
}

void
PhyClient::SetServerPort (uint16_t serverPort)
{
  m_server_port = serverPort;
}

void
PhyClient::SetRunning (bool isRunnung)
{
  m_running = isRunnung;
}

void
PhyClient::Initialize ()
{
  uint16_t SERVER_PORT = 8877;  // port of the channel server (we want to reach the server)
  SetServerPort (SERVER_PORT);

  CreateServerAddress ();  // builds the server address to which the data is sent
  CreateSocket ();         // builds the socket

  std::cout << PrintAddress (m_server_address, "Server to reach:", "\n");

  if (m_mac == 0)
    {
      std::cout << "Client::Initialize: mac object is 0. Enter 0 to exit." << std::endl;

      std::string s;
      std::cin >> s; // stop console to be closed

      exit(EXIT_FAILURE);
    }
}

void
PhyClient::CreateServerAddress ()
{
  //  struct sockaddr_in server_address;
  memset (&m_server_address, 0, sizeof(m_server_address));        // socket address used for the server, set all to 0
  m_server_address.sin_family = AF_INET;                          // Ipv4 address space
  m_server_address.sin_port = htons (m_server_port);              // htons: host to network short: transforms a value in host byte order to network byte
  inet_pton (AF_INET, m_server_name, &m_server_address.sin_addr); // converts IPv4 or IPv6 from textual to network (binary) format
}

void
PhyClient::CreateSocket ()
{
  if ((m_sock = socket (PF_INET, SOCK_DGRAM, 0)) < 0)     // create a UDP socket
    {
      printf ("Client::CreateSocket: Could not create a socket\n");
      return;
    }
}

void
PhyClient::SendData (char* data_to_send, uint32_t len)
{
  sendto (m_sock, data_to_send, len, 0, (struct sockaddr*) &m_server_address, sizeof(m_server_address));
}

void
PhyClient::ReceiveData ()
{
  struct sockaddr_in address;
  socklen_t address_len (0);

  while (m_running)
    {
      char buffer[m_bufferLength];
      int len = recvfrom (m_sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &address, &address_len); // 0, NULL, NULL);

      if (len == -1)
        std::cout << "Error: Client::StartReceiving: recv" << std::endl;

      buffer[len] = '\0';   // set NULL character at the end, at this point strlen() stops counting

      m_mac->ReceiveData (buffer, len);
    }
}

std::string
PhyClient::PrintAddress (sockaddr_in address, std::string prefix, std::string suffix)
{
  std::stringstream ss;

  std::string ipAddress (inet_ntoa (address.sin_addr));
  std::string port = std::to_string (ntohs (address.sin_port));

  ss << prefix << " To IP: " << ipAddress << " Port: " << port << suffix;

  return ss.str ();
}

