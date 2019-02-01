//============================================================================
// Name        : channel_server.cpp
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   : 
// Description : MAC for GFDM
//============================================================================

#include <string.h>
#include <time.h>
#include <iostream>
#include "channel_server.h"


int
main (int argc, char *argv[])
{
  std::cout << "Start: Channel Server." << std::endl;

  // port to start the server on
  int SERVER_PORT = 8877;

  // Emulation of the wifi channel;
  ChannelServer* channelServer = new ChannelServer();
  channelServer->SetServerPort(SERVER_PORT);
  channelServer->Initialize();
  channelServer->StartReceivingLoop();


  return 0;

}
