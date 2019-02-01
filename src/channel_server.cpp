//============================================================================
// Name        : server.cpp
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   : 
// Description : MAC for GFDM
//============================================================================

#include "channel_server.h"

ChannelServer::ChannelServer () :
    m_sock (0), m_server_port (8877), m_running (true), m_bufferLength (1024)
{
}

ChannelServer::ChannelServer (uint16_t port) :
    m_sock (0), m_server_port (port), m_running (true), m_bufferLength (1024)
{
}

ChannelServer::~ChannelServer ()
{
}

void
ChannelServer::Initialize ()
{
  CreateServerAddress ();
  CreateSocket ();
  BindSocket ();

  std::cout << PrintAddress (m_server_bind_address, "Server: ", "\n");
}

void
ChannelServer::CreateServerAddress ()
{
  memset (&m_server_bind_address, 0, sizeof(m_server_bind_address)); // socket address used for the server, set all to 0
  m_server_bind_address.sin_family = AF_INET;                        // Ipv4 address space
  m_server_bind_address.sin_port = htons (m_server_port);            // htons: host to network short: transforms a value in host byte order to network byte
  m_server_bind_address.sin_addr.s_addr = htonl (INADDR_ANY);        // htons: host to network long: transforms a value in network byte order to host byte order
}

void
ChannelServer::CreateSocket ()
{
  if ((m_sock = socket (PF_INET, SOCK_DGRAM, 0)) < 0)     // create a UDP socket
    {
      printf ("Server::CreateSocket: Could not create a socket.\n");
      return;
    }
}

void
ChannelServer::BindSocket ()
{
  // bind it to listen to the incoming connections on the created server
  if ((bind (m_sock, (struct sockaddr *) &m_server_bind_address, sizeof(m_server_bind_address))) < 0)
    {
      printf ("Server::BindSocket: Could not bind socket.\n");
      return;
    }
}

// not used so far
#if 0
void
Server::Receive (char* buffer, uint32_t lenBuff)
  {
    struct sockaddr_in client_address; // socket address used to store client address
    socklen_t client_address_len = sizeof(client_address);

    int len = recvfrom (m_sock, buffer, lenBuff, 0, (struct sockaddr *) &client_address, &client_address_len);// read content into buffer from an incoming client

    if (len == -1)
    std::cout << "Error: Server::StartReceiving: recv" << std::endl;

    buffer[len] = '\0';
  }
#endif

void
ChannelServer::StartReceivingLoop ()
{
  struct sockaddr_in client_address; // socket address used to store client address
  socklen_t client_address_len (0);

  // run indefinitely
  while (m_running)
    {
      char buffer[m_bufferLength];
      int len = recvfrom (m_sock, buffer, m_bufferLength, 0, (struct sockaddr *) &client_address, &client_address_len); // read content into buffer from an incoming client

      if (len == -1)
        std::cout << "Error: Server::StartReceivingLoop: recvfrom" << std::endl;

      buffer[len] = '\0'; // add zero character

      // Print Received information
//      std::cout << "Received: " << buffer << std::endl;
      std::cout << PrintAddress (client_address, "Channel Server Received Packet: ");
      std::cout << std::endl;

      AddPortToVector (client_address.sin_port);
      RetransmitToAll (client_address.sin_port, buffer, len, client_address);
    }
}

void
ChannelServer::RetransmitToAll (uint16_t receivedFromPort, char* buffer, uint32_t len, struct sockaddr_in client_address)
{
  // send same content back to all nodes ("echo"), i.e. emulate the channel

  // block the "Initialize" packets that they are not retransmitted, to run: include "#ifdef RUN"
#if 0
  if (Functions::IsStringEqual (buffer, (char*)MessageTypes::InitializeMessage().c_str()))
  return;
#endif

  for (std::vector<uint16_t>::iterator it = m_portsUes.begin (); it != m_portsUes.end (); ++it)
    {
      if (receivedFromPort != (*it))
        {
          client_address.sin_port = (*it); // set port for the receiving node
          std::cout << "Server: Retransmit to Port: " << client_address.sin_port << std::endl;
          sendto (m_sock, buffer, len, 0, (struct sockaddr *) &client_address, sizeof(client_address));
        }
    }
}

void
ChannelServer::AddPortToVector (uint16_t port)
{
  if (IsPortIncluded (port) == false)
    {
      m_portsUes.push_back (port);
    }
}

bool
ChannelServer::IsPortIncluded (uint16_t port)
{
  return (std::find (m_portsUes.begin (), m_portsUes.end (), port) != m_portsUes.end ());
}

uint16_t
ChannelServer::GetServerPort ()
{
  return m_server_port;
}

void
ChannelServer::SetServerPort (uint16_t serverPort)
{
  m_server_port = serverPort;
}

std::string
ChannelServer::PrintAddress (sockaddr_in address, std::string prefix, std::string suffix)
{
  std::stringstream sOutput;

  std::string ipAddress (inet_ntoa (address.sin_addr));
  std::string port = std::to_string (ntohs (address.sin_port));

  sOutput << prefix << "From IP: " << ipAddress << " Port: " << port << suffix;

  return sOutput.str ();
}
