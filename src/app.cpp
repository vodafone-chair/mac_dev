#include "app.h"

App::App () :
    m_mac (0), m_running (true), m_nodeAddress (0), m_sequNumber (0), m_counter (0)
{
}

App::~App ()
{
  delete m_mac;
  m_mac = 0;
}

void
App::SetMac (Mac* mac)
{
  m_mac = mac;
}

void
App::SetRunning (bool isRunning)
{
  m_running = isRunning;
}

void
App::Initialize ()
{
  if (m_mac == 0)
    {
      std::cout << "App::Initialize: mac object is 0. Enter 0 to exit." << std::endl;

      std::string s;
      std::cin >> s; // stop console to be closed

      exit (EXIT_FAILURE);
    }

  m_nodeAddress = m_mac->GetNodeAddress (); // get node address

  if (m_nodeAddress == 0)
    std::cout << "App::Initialize: Node address is 0. Please Initialize() the MAC first." << std::endl;

  // send initial packet to assign (register) the node to the channel server
  // (the channel server works as channel and retransmits the received packet to all registered nodes)
  PacketData packet = GeneratePacket (Header::GetBroadcastAddress (), MessageTypes::InitializeMessage ());   // generate packet
  SendPacket (packet);
}

PacketData
App::GeneratePacket (char* payload, uint32_t payloadSize, uint8_t dstAddress)
{
  PacketData packet (payload, payloadSize);
  packet.SetHeader (GenerateHeader (dstAddress));

  return packet;
}

PacketData
App::GeneratePacket (uint8_t dstAddress, std::string stringIn)
{
  uint32_t payloadSize = stringIn.size () + 1;           // payload size, "+1" because of the '\0' character
  char* payload = new char[payloadSize];                 // declare char array
  Functions::ConvertStringToChar (payload, stringIn);    // include string to char array
  PacketData packet (payload, payloadSize);              // create packet
  packet.SetHeader (GenerateHeader (dstAddress));        // set packet header
  delete[] payload;

  return packet;
}

Header
App::GenerateHeader (uint8_t version, uint8_t sequNum, uint8_t srcAddress, uint8_t dstAddress, uint8_t crc_header)
{
  Header header;
  header.m_version = version;          // version of the header
  header.m_sequNum = sequNum;          // sequence number of the packet
  header.m_srcAddress = srcAddress;    // source address
  header.m_dstAddress = dstAddress;    // destination address
  header.m_crc_header = crc_header;    // MAC header CRC check (tbd)

  return header;
}

Header
App::GenerateHeader (uint8_t dstAddress)
{
  // TODO generate header CRC
  Header header = GenerateHeader (1, m_sequNumber, m_nodeAddress, dstAddress, 0);
  return header;
}

