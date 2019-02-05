/*
 * datagram.cpp
 *
 *  Created on: Jan 22, 2019
 *      Author: sebastian Kühlmorgen
 */

#include "datagram.h"

//=================Header==========================================================================

Header::Header () :
    m_version (0), m_sequNum (0), m_srcAddress (0), m_dstAddress (0), m_crc_header (0)
{
}

Header::Header (uint8_t version, uint8_t sequNum, uint8_t srcAddress, uint8_t dstAddress, uint8_t crc_header) :
    m_version (version), m_sequNum (sequNum), m_srcAddress (srcAddress), m_dstAddress (dstAddress), m_crc_header (crc_header)
{
}

uint32_t
Header::GetHeaderSize ()
{
  return 5;  // 5 byte for the header version 1
}

uint8_t
Header::GetBroadcastAddress ()
{
  return std::numeric_limits<uint8_t>::max ();
}

uint8_t
Header::GetZeroAddress ()
{
  return 0;
}

uint8_t
Header::GetBaseStationAddress ()
{
  return 1;
}

std::ostream &
operator<< (std::ostream &out, const Header &header)
{
  out << " Version: " << (uint16_t) header.m_version;
  out << " SequNum: " << (uint16_t) header.m_sequNum;
  out << " SrcAddr: " << (uint16_t) header.m_srcAddress;
  out << " DstAddr: " << (uint16_t) header.m_dstAddress;
  out << " CrcMac:  " << (uint16_t) header.m_crc_header;
  return out;
}

//=================PacketData======================================================================

PacketData::PacketData () :
    m_header (Header ()), m_payload (0), m_payloadSize (0)
{
}

PacketData::PacketData (char* payload, uint32_t payloadSize) :
    m_header (Header ()), m_payload (0), m_payloadSize (0)
{
  SetData (payload, payloadSize);
}

PacketData::~PacketData ()
{
}

void
PacketData::SetData (char* payload, uint32_t payloadSize)
{
  m_payloadSize = payloadSize;               // Set payload size

  delete[] m_payload;                        // delete the pointer
  m_payload = new char[payloadSize];         // create new buffer that is connected to the packet object
  memcpy (m_payload, payload, payloadSize);  // copy payload to m_payload
}

void
PacketData::DeletePacket ()
{
  delete[] m_payload;
  m_payload = 0;
}

void
PacketData::SetHeader (Header header)
{
  m_header = header;
}

uint32_t
PacketData::GetPayloadSize ()
{
  return m_payloadSize;
}

uint32_t
PacketData::GetSizeHeaderAndPayload ()
{
  return (Header::GetHeaderSize () + PacketData::GetPayloadSize ());
}

uint32_t
PacketData::Serialize (char* outBuffer)
{
  uint8_t header[] =
    { m_header.m_version, m_header.m_sequNum, m_header.m_srcAddress, m_header.m_dstAddress, m_header.m_crc_header };

  // combine header and payload
  memcpy (outBuffer, header, Header::GetHeaderSize ());                      // write header to buffer
  memcpy (outBuffer + Header::GetHeaderSize (), m_payload, m_payloadSize);   // add offset of the header and write payload data

  return GetSizeHeaderAndPayload ();
}

PacketData
PacketData::Deserialize (char* inBuffer, uint32_t lenHeadAndPayl)
{
  // remove header and write values to new packet object
  PacketData packet;                                   // create new packet object
  packet.m_header.m_version = (uint8_t) *inBuffer;     // write values to packet header object
  inBuffer += sizeof(m_header.m_version);              // shift pointer to next address (header value)

  packet.m_header.m_sequNum = (uint8_t) *inBuffer;
  inBuffer += sizeof(m_header.m_sequNum);

  packet.m_header.m_srcAddress = (uint8_t) *inBuffer;
  inBuffer += sizeof(m_header.m_srcAddress);

  packet.m_header.m_dstAddress = (uint8_t) *inBuffer;
  inBuffer += sizeof(m_header.m_dstAddress);

  packet.m_header.m_crc_header = (uint8_t) *inBuffer;
  inBuffer += sizeof(m_header.m_crc_header);

  packet.SetData (inBuffer, lenHeadAndPayl - Header::GetHeaderSize ()); // write payload data

  return packet;
}

std::ostream &
operator<< (std::ostream &out, const PacketData &packet) // implement here, otherwise the One Definition Rule is broken
{
  out << "Size: " << packet.m_payloadSize << " Payload: " << packet.m_payload;
  return out;
}

//=================Functions=======================================================================

void
Functions::ConvertStringToChar (char* charOut, std::string stringIn)
{
  strcpy (charOut, stringIn.c_str ());
}

int
Functions::GenerateRandomNumber (uint64_t maxNum)
{
  srand (time (NULL));                 // initialize random seed
  int iSecret = rand () % maxNum + 1;  // generate secret number between 1 and maxNum:
  return iSecret;
}

bool
Functions::IsStringEqual (std::string string1, std::string string2)
{
  if (strcmp (string1.c_str (), string2.c_str ()) == 0) // check if string is equal
    return true;

  return false;
}

bool
Functions::IsStringEqual (char* char1, char* char2)
{
  if (strcmp (char1, char2) == 0) // check if char is equal
    return true;

  return false;
}

bool
Functions::IsStringIncluded (char* charTotal, char* charIncluded)
{
  if (strstr (charTotal, charIncluded) != 0) // check if char is equal
    return true;

  return false;
}

uint16_t
Functions::ComputeCrc (const char* data, uint32_t len)
{
  char x;
  uint16_t crc = 0xFFFF;

  while (len--)
    {
      x = crc >> 8 ^ *data++;
      x ^= x >> 4;
      crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x << 5)) ^ ((uint16_t) x);
    }

  return crc;
}

