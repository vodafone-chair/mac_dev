#ifndef APP_UE_H
#define APP_UE_H

#include <arpa/inet.h>       //close
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>          //strlen
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>          //close
#include <iostream>

#include "app.h"
#include "datagram.h"

class AppUe : public App
{
public:

  AppUe ();
  virtual~AppUe ();

  void TriggerPackets (uint32_t interval);
  void SendPacket (PacketData packet);
  void ReceivePacket (PacketData packet);

private:
  void RetransmitToBs (std::string dataString);
  void TransmitD2dMessage();

};

#endif /* APP_UE_H */
