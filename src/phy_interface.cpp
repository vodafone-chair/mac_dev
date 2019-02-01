#include "phy_interface.h"

PhyInterface::PhyInterface () :
    m_mac (0)
{
}

PhyInterface::~PhyInterface ()
{
  delete m_mac;
  m_mac = 0;
}

void
PhyInterface::SetMac (Mac* mac)
{
  m_mac = mac;
}

