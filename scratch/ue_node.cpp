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
#include "mac_ue.h"
#include "app_ue.h"

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
      std::cout << "UE: No node address parameter set. Please set the address of the node [2...254]." << std::endl; // 255 is broadcast
      exit (EXIT_FAILURE);
    }

  if (argc >= 3)
    srcPort = (uint16_t) atoi (argv[2]);
  else
    {
      std::cout << "Please set a Port Address." << std::endl;
      exit (EXIT_FAILURE);
    }

  std::cout << "Start: UE with node-address: " << (uint16_t) nodeAddress << std::endl;

  // declaration of the objects for the layer APP, MAC and PHY
  App* appUe = new AppUe ();                        // ue application
  Mac* macUe = new MacUe ();                        // ue mac
  PhyInterface* phyInterface = new PhyClient ();    // phy interface to send data

  // Set interfaces among layers
  appUe->SetMac (macUe);
  macUe->SetPhyInterface (phyInterface);
  macUe->SetApp (appUe);
  phyInterface->SetMac (macUe);

  // Initialize the layers
  phyInterface->Initialize (PhyInfo (srcPort, channelServerPort));
  macUe->Initialize (nodeAddress);   // provide the node address to the layers
  appUe->Initialize ();

  // Set threads for the send (Application) and receive (phyInterface) function
  std::thread receiveThread (&PhyInterface::ReceiveData, phyInterface);   // Rx function in the phyInterface
  //std::thread sendThread (&App::TriggerPackets, appUe, 1000000);        // Tx function in the application

  // start threads
  receiveThread.join ();
  //sendThread.join();

  // delete pointers
  delete appUe;
  delete macUe;
  delete phyInterface;

  return 0;

}
