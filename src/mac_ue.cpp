#include "mac_ue.h"

MacUe::MacUe () :
    Mac::Mac ()
{
}

MacUe::~MacUe ()
{
}

void
MacUe::EnqueuePacket (PacketData packet)
{
  m_macQueue->Enqueue (packet);

  DeQueuePacket ();
}

void
MacUe::DeQueuePacket ()
{
  if (m_macQueue->IsEmpty ())
    return;

  AddHeader (m_macQueue->Dequeue ());
}

void
MacUe::AddHeader (PacketData packet)
{
  char* buffer = new char[packet.GetSizeHeaderAndPayload ()];    // create empty buffer for the combination of header and payload
  uint32_t lenHeadAndPayl = packet.Serialize (buffer);           // add packet header to packet
  packet.DeletePacket();                                         // delete pointer to char array

  SendData (buffer, lenHeadAndPayl);                             // send data
  delete[] buffer;                                               // delete pointer
}

void
MacUe::ReceiveData (char* receivedData, uint32_t lenHeadAndPayl)
{
  PacketData packet = PacketData::Deserialize (receivedData, lenHeadAndPayl);  // generate packet at this node, remove packet header

  // address check
  if (packet.m_header.m_dstAddress == m_nodeAddress || packet.m_header.m_dstAddress == Header::GetBroadcastAddress ())
    m_app->ReceivePacket (packet);     // call receive function in the application
  else
    packet.DeletePacket();             // delete pointer to char array
}
