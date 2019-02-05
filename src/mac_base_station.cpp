#include "mac_base_station.h"

MacBs::MacBs () :
    Mac::Mac ()
{
}

MacBs::~MacBs ()
{
}

void
MacBs::EnqueuePacket (PacketData packet)
{
  m_macQueue->Enqueue (packet);

  DeQueuePacket ();
}

void
MacBs::DeQueuePacket ()
{
  if (m_macQueue->IsEmpty ())
    return;

  AddHeader (m_macQueue->Dequeue ());
}

void
MacBs::AddHeader (PacketData packet)
{
  char* buffer = new char[packet.GetSizeHeaderAndPayload ()];    // create empty buffer for the combination of header and payload
  uint32_t lenHeadAndPayl = packet.Serialize (buffer);           // add packet header to packet
  packet.DeletePacket();                                         // delete pointer to char array

  SendData (buffer, lenHeadAndPayl);                             // send data
  delete[] buffer;                                               // delete pointer
}

void
MacBs::ReceiveData (char* receivedData, uint32_t lenHeadAndPayl)
{
  PacketData packet = PacketData::Deserialize (receivedData, lenHeadAndPayl);     // Remove MAC header from the received packet

  // address check
  if (packet.m_header.m_dstAddress == m_nodeAddress || packet.m_header.m_dstAddress == Header::GetBroadcastAddress ())
    m_app->ReceivePacket (packet);      // call receive function in the application
  else
    packet.DeletePacket();             // delete pointer to char array
}

