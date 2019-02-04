#include "phy_interface.h"

PhyInterface::PhyInterface () :
    m_mac (0)
{
}

PhyInterface::~PhyInterface ()
{
}

void
PhyInterface::SetMac (Mac* mac)
{
  m_mac = mac;
}

