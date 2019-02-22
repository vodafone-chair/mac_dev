//============================================================================
// Name        : ue_node.cpp
// Author      : Sebastian Kühlmorgen
// Version     :
// Copyright   :
// Description : MAC for GFDM
//============================================================================

#include <string.h>
#include <time.h>
#include <iostream>
#include <thread>

#include "phy_client.h"
#include "mac_base_station.h"
#include "app_base_station.h"

int
main (int argc, char *argv[])
{
  uint8_t nodeAddress (0);
  uint16_t srcPort (0);
  uint16_t channelServerPort (8877);

  if (argc >= 2)
    nodeAddress = (uint8_t) atoi (argv[1]);
  else
    {
      std::cout << "Base Station: No node address parameter. Please set the address of the node to 1." << std::endl; // 255 is broadcast
      exit (EXIT_FAILURE);
    }

  if (argc >= 3)
    srcPort = (uint16_t) atoi (argv[2]);
  else
    {
      std::cout << "Please set a Port Address." << std::endl;
      exit (EXIT_FAILURE);
    }

  std::cout << "Start: Base Station with node-address: " << (uint16_t) nodeAddress << std::endl;

  // declaration of the objects for the layer APP, MAC and PHY
  App* appBs = new AppBs ();                        // base station application
  Mac* macBs = new MacBs ();                        // base station mac
  PhyInterface* phyInterface = new PhyClient ();    // phy interface to send data

  // Set interfaces among layers
  appBs->SetMac (macBs);
  macBs->SetPhyInterface (phyInterface);
  macBs->SetApp (appBs);
  phyInterface->SetMac (macBs);

  // Initialize the layers
  phyInterface->Initialize (PhyInfo (srcPort, channelServerPort));
  macBs->Initialize (nodeAddress);
  appBs->Initialize ();

  // Set threads for the send (Application) and receive (phyInterface) function
  std::thread receiveThread (&PhyInterface::ReceiveData, phyInterface);   // Rx function in the phyInterface
  std::thread sendThread (&App::TriggerPackets, appBs, 1e6);              // Tx function in the application, interval in us

  // start threads
  receiveThread.join ();
  sendThread.join ();

  // delete pointers
  delete appBs;
  delete macBs;
  delete phyInterface;

  return 0;

}
