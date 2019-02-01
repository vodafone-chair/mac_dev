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

#include "phy_client.h"
#include "mac_base_station.h"
#include "app_base_station.h"


int
main (int argc, char *argv[])
{
  uint8_t nodeAddress(0);
  if(argc == 2)
    nodeAddress = (uint8_t)atoi(argv[1]);
  else
    std::cout << "Base Station: No node address parameter. Please set the address of the node to 1." << std::endl;

  std::cout << "Start: Base Station with node address: " << (uint16_t)nodeAddress << std::endl;

  // declaration of the objects for the layer APP, MAC and PHY
  App* appBs = new AppBs();                        // base station application
  Mac* macBs = new MacBs ();                       // base station mac
  PhyInterface* phyInterface = new PhyClient ();   // phy interface to send data

  // Set interfaces among layers
  appBs->SetMac(macBs);
  macBs->SetPhyInterface(phyInterface);
  macBs->SetApp(appBs);
  phyInterface->SetMac(macBs);

  // Initialize the layers
  phyInterface->Initialize ();
  macBs->Initialize(nodeAddress);
  appBs->Initialize();

  // Set threads for the send (Application) and receive (phyInterface) function
  std::thread receiveThread (&PhyInterface::ReceiveData, phyInterface);   // Rx function in the phyInterface
  std::thread sendThread (&App::TriggerPackets, appBs, 1e6);              // Tx function in the application, interval in us

  receiveThread.join();
  sendThread.join();

  return 0;

}
