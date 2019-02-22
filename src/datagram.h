/*
 * datagram.h
 *
 *  Created on: Jan 22, 2019
 *      Author: sebastian Kühlmorgen
 */

#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <stdint.h>
#include <string>
#include <cstring> // strcpy, .c_str ()
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <limits>
#include <ctime>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // inet_ntoa

class PhyInfo
{
public:
  PhyInfo () :
      m_srcPort (0), m_dstPort (0), m_srcIpAddress ("0.0.0.0"), m_dstIpAddress ("127.0.0.1")
  {
  }

  PhyInfo (uint16_t srcPort, uint16_t dstPort) :
      m_srcPort (srcPort), m_dstPort (dstPort), m_srcIpAddress ("0.0.0.0"), m_dstIpAddress ("127.0.0.1")
  {
  }

  PhyInfo (uint16_t srcPort, uint16_t dstPort, std::string srcAddress, std::string dstAddress) :
      m_srcPort (srcPort), m_dstPort (dstPort), m_srcIpAddress (srcAddress), m_dstIpAddress (dstAddress)
  {
  }

  uint16_t m_srcPort;
  uint16_t m_dstPort;
  std::string m_srcIpAddress;
  std::string m_dstIpAddress;
};
/* PhyInfo */



class UeInfo
{
public:
  UeInfo (uint8_t ueAddress) :
      m_ueAddress (ueAddress), m_timeEnqueued (time (NULL)), m_maxQueuingTime (20)
  {
  }

  UeInfo (uint8_t ueAddress, time_t maxQueuingTime) :
      m_ueAddress (ueAddress), m_timeEnqueued (time (NULL)), m_maxQueuingTime (maxQueuingTime)
  {
  }

  UeInfo (uint8_t ueAddress, time_t time, time_t expire) :
      m_ueAddress (ueAddress), m_timeEnqueued (time), m_maxQueuingTime (expire)
  {
  }

  uint8_t m_ueAddress;       // UE address
  time_t m_timeEnqueued;     // m_time(time(NULL)) means now
  time_t m_maxQueuingTime;   // max queuing time in the vector
};
/* UeInfo */



class Header
{
public:
  uint8_t m_version;     // version of the header
  uint8_t m_sequNum;     // sequence number of the packet
  uint8_t m_srcAddress;  // source address of the node
  uint8_t m_dstAddress;  // destination address of the node
  uint8_t m_crc_header;  // CRC value for the header

  Header ();
  Header (uint8_t version, uint8_t sequNum, uint8_t srcAddress, uint8_t dstAddress, uint8_t crc_header);

  static uint32_t
  GetHeaderSize ();
  static uint8_t
  GetBroadcastAddress ();
  static uint8_t
  GetZeroAddress ();
  static uint8_t
  GetBaseStationAddress ();

  friend std::ostream &
  operator<< (std::ostream &out, const Header &header);

};
/* Header */

std::ostream &
operator<< (std::ostream &out, const Header &header); // to print Header with std::cout

class PacketData
{
public:
  Header m_header;          // packet header
  char* m_payload;          // packet payload (data)
  uint32_t m_payloadSize;   // payload size

  PacketData ();
  PacketData (char* payload, uint32_t payloadSize);
  virtual
  ~PacketData ();

  void
  SetData (char* payload, uint32_t payloadSize); // set payload
  void
  SetHeader (Header header);                      // set header
  void
  DeletePacket ();                                // delete char* pointer, free the memory

  uint32_t
  GetPayloadSize ();                         // get payload size
  uint32_t
  GetSizeHeaderAndPayload ();                // get header And payload size

  uint32_t
  Serialize (char* outBuffer);               // add header to the payload
  static PacketData
  Deserialize (char* inBuffer, uint32_t lenHeadAndPayl); // remove header from the payload

  friend std::ostream &
  operator<< (std::ostream &out, const PacketData &packet);

};
/* PacketData */

std::ostream &
operator<< (std::ostream &out, const PacketData &packet); // to print PacketData with std::cout



class Functions
{
public:
  Functions ()
  {
  }

  static void
  ConvertStringToChar (char* charOut, std::string stringIn);  // converts a string to a char array
  static int
  GenerateRandomNumber (uint64_t maxNum);                     // generates a random number between 0 and maxNum
  static bool
  IsStringEqual (std::string string1, std::string string2);   // compares string1 with string2 if it is equal
  static bool
  IsStringEqual (char* char1, char* char2);                   // compares char with char2 if it is equal
  static bool
  IsStringIncluded (char* charTotal, char* charIncluded);     // checks if charIncluded is in charTotal
  static uint16_t
  ComputeCrc (const char* data, uint32_t len);                // compute the CRC of a packet
  static std::string
  PrintSocketAddress (sockaddr_in address, std::string prefix = "", std::string suffix = ""); // print the Ipv4 and Port of the socket address
};
/* Functions */



class MessageTypes
{
public:
  MessageTypes ()
  {
  }

  // message types for signaling

  // Initialize message that the channel server knows all nodes
  static std::string
  InitializeMessage ()
  {
    return "Initialize";
  }

  static bool
  IsInitializeMessage (PacketData packet)
  {
    bool isInitializeMsg = Functions::IsStringEqual (packet.m_payload, (char*) MessageTypes::InitializeMessage ().c_str ());
    return isInitializeMsg;
  }

  // UEs reply of the RequestUeMessage from the BS
  static std::string
  UeReplyMessage ()
  {
    return "UeReplyMsg";
  }

  static bool
  IsUeReplyMessage (PacketData packet)
  {
    bool isUeReplyMsg = Functions::IsStringEqual (packet.m_payload, (char*) MessageTypes::UeReplyMessage ().c_str ());
    return isUeReplyMsg;
  }

  // BS triggers the UEs to send a UeReplyMessage
  static std::string
  RequestUeMessage ()
  {
    return "RequestUE";
  }

  static bool
  IsRequestUeMessage (PacketData packet)
  {
    bool isRequestUeMsg = Functions::IsStringEqual (packet.m_payload, (char*) MessageTypes::RequestUeMessage ().c_str ());
    return isRequestUeMsg;
  }

  // BS triggers a UE to send a D2D message
  static std::string
  GrantD2dMessage ()
  {
    return "GrantD2d";
  }

  static bool
  IsGrantD2dMessage (PacketData packet)
  {
    bool isGrantD2dMsg = Functions::IsStringEqual (packet.m_payload, (char*) MessageTypes::GrantD2dMessage ().c_str ());
    return isGrantD2dMsg;
  }

  // UE sends a D2D message
  static std::string
  UeD2dMessage ()
  {
    return "D2dUE";
  }

  static bool
  IsUeD2dMessage (PacketData packet)
  {
    bool isUeD2dMsg = Functions::IsStringIncluded (packet.m_payload, (char*) MessageTypes::UeD2dMessage ().c_str ());
    return isUeD2dMsg;
  }

};
/* MessageTypes */


#endif /* DATAGRAM_H */
