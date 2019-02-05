#include "app_ue.h"

AppUe::AppUe () :
    App::App ()
{
}

AppUe::~AppUe ()
{
}

void
AppUe::SendPacket (PacketData packet)
{
  m_sequNumber++;
  m_mac->EnqueuePacket (packet);
}

void
AppUe::TriggerPackets (uint32_t interval)  // Not used so far
{
  while (m_running)
    {
      uint32_t randNum = Functions::GenerateRandomNumber (1000);                          // generate random number
      std::string payload = "UE sends this number: " + std::to_string (randNum);          // data that will be sent to the server

      PacketData packet = GeneratePacket (Header::GetBaseStationAddress (), payload);     // generate packet
      AppUe::SendPacket (packet);                                                         // send data

      m_counter++;        // loop counter
      usleep (interval);  // delayed (in us)
    }
}

void
AppUe::ReceivePacket (PacketData packet)
{
  std::cout << "\nRX-UE-APP " << packet << std::endl;

  if (MessageTypes::IsRequestUeMessage (packet))
    {
      usleep(m_nodeAddress * 2e3);  // wait for nodeAddress * 2000 microseconds
      RetransmitToBs (MessageTypes::UeReplyMessage ());
    }
  else if (MessageTypes::IsGrantD2dMessage (packet))
    {
      TransmitD2dMessage ();
    }
  else if (MessageTypes::IsUeD2dMessage (packet))
    {
      // do nothing
    }
  else
    {
      RetransmitToBs ("UE replies from Address: " + std::to_string (m_nodeAddress));
    }

  packet.DeletePacket();  // delete pointer to char array
}

void
AppUe::RetransmitToBs (std::string payload)
{
  PacketData packet = GeneratePacket (Header::GetBaseStationAddress (), payload);  // generate packet
  std::cout << "TX-UE-Ret-APP " << packet << std::endl;
  AppUe::SendPacket (packet);                                                      // transmit packet
}

void
AppUe::TransmitD2dMessage ()
{
  uint32_t randNum = Functions::GenerateRandomNumber (1000);                       // random number generation
  std::string payload = MessageTypes::UeD2dMessage ();
  payload += ": D2D packet with random number " + std::to_string (randNum);
  payload += " from Node " + std::to_string (m_nodeAddress);                       // generate payload

  PacketData packet = GeneratePacket (Header::GetBroadcastAddress (), payload);    // generate packet
  std::cout << "TX-UE-APP " << packet << std::endl;
  AppUe::SendPacket (packet);                                                      // transmit packet
}

