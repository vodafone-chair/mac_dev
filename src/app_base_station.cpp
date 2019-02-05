#include "app_base_station.h"

AppBs::AppBs () :
    App::App (), m_rr_counter (0), m_expirationTimeNodeVector (60)
{
}

AppBs::~AppBs ()
{
}

void
AppBs::TriggerPackets (uint32_t interval)
{
  uint32_t appUsed = 1; // set application (see enum apps)

  enum apps
  {
    AppRoundRobin = 1, AppD2d = 2, AppTestData = 3
  };

  while (m_running)
    {
      if (m_counter % 20 == 2)             // after 20 rounds, send a UeRequest message, after 2s send the first request
        {
          SendUeRequest ();                // send UE request after 10 packets
        }
      else
        {
          switch (appUsed)
            {
            case AppRoundRobin:
              SendRoundRobin ();           // send random data to UEs in round robin order
              break;
            case AppD2d:
              SendGrantD2dMsg ();          // send a D2D grant message, then UE send a D2D message to its neighbors
              break;
            case AppTestData:
              SendTestData ();             // send test data to node 2
              break;
            default:
              std::cout << "No application set." << std::endl;
            }
        }

      m_counter++;                     // loop counter
      usleep (interval);               // delayed by 1us
    }
}

void
AppBs::ReceivePacket (PacketData packet)
{
  std::cout << "RX-BS-APP " << packet << std::endl;         // print packet size and payload

  if (MessageTypes::IsUeReplyMessage (packet))
    {
      AddUeAddressToVector (packet.m_header.m_srcAddress);  // add the UE address to the m_packetHeaderList
    }

  packet.DeletePacket ();                                   // delete pointer to char array
}

void
AppBs::SendPacket (PacketData packet)
{
  m_sequNumber++;
  m_mac->EnqueuePacket (packet);
}

void
AppBs::SendUeRequest ()
{
  PacketData packet = GeneratePacket (Header::GetBroadcastAddress (), MessageTypes::RequestUeMessage ()); // send UE request which triggers a reply message from the UE

  std::cout << "TX-BS-APP " << packet << std::endl; // output what is sent
  AppBs::SendPacket (packet); // send data
}

void
AppBs::SendTestData ()
{
  int randNum = Functions::GenerateRandomNumber (1000);                        // generate random number
  std::string payload = "BS sends this number: " + std::to_string (randNum);   // data for the packet
  PacketData packet = GeneratePacket (2, payload);                             // generate packet

  std::cout << "TX-BS-APP " << packet << std::endl;                            // output what is sent
  AppBs::SendPacket (packet);                                                  // send data
}

void
AppBs::SendRoundRobin ()
{
  int randNum = Functions::GenerateRandomNumber (1000);          // generate random number
  uint8_t dstAddress = GetNextDstAddress ();                     // get next UE address, round robin scheme
  std::string payload = "BS sends number: " + std::to_string (randNum) + " to Node " + std::to_string (dstAddress);      // data for the packet

  PacketData packet = GeneratePacket (dstAddress, payload);      // generate packet

  std::cout << "TX-BS-APP " << packet << std::endl;              // output what is sent
  AppBs::SendPacket (packet);
}

void
AppBs::SendGrantD2dMsg ()
{
  uint8_t dstAddress = GetNextDstAddress ();                                           // get next UE address, round robin scheme
  PacketData packet = GeneratePacket (dstAddress, MessageTypes::GrantD2dMessage ());   // send UE a grand to send a D2D message

  std::cout << "TX-BS-APP " << packet << std::endl;                                    // output what is sent
  AppBs::SendPacket (packet);
}

uint8_t
AppBs::GetNextDstAddress ()
{
  if (m_ueAddressList.size () == 0)
    return 2;  // 2 because base station has source address 1

  m_rr_counter++;  // increment round robin counter
  RestrictRrCounter ();

  return m_ueAddressList[m_rr_counter].m_ueAddress;
}

void
AppBs::RestrictRrCounter ()
{
  if (m_rr_counter >= m_ueAddressList.size ())
    m_rr_counter = 0;
}

void
AppBs::AddUeAddressToVector (uint8_t ueAddress)
{
  Purge (); // purge vector of expired entries

  if (IsUeAddressIncluded (ueAddress) == false)
    {
      m_ueAddressList.push_back (UeInfo (ueAddress, m_expirationTimeNodeVector));  // after XX seconds is a node removed from the list
    }
}

bool
AppBs::IsUeAddressIncluded (uint8_t ueAddress)
{
  for (std::vector<UeInfo>::iterator it = m_ueAddressList.begin (); it != m_ueAddressList.end (); ++it)
    {
      if (it->m_ueAddress == ueAddress)
        return true;
    }

  return false;
}

void
AppBs::Purge ()
{
  for (std::vector<UeInfo>::iterator iter = m_ueAddressList.begin (); iter != m_ueAddressList.end ();)
    {
      if (iter->m_timeEnqueued + iter->m_maxQueuingTime <= time (NULL))
        {
          iter = m_ueAddressList.erase (iter);
        }
      else
        ++iter;
    }
}

