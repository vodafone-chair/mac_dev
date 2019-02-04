#include "mac.h"

Mac::Mac () :
    m_app (0), m_phyInterface (0), m_nodeAddress (0), m_macQueue (0)
{
  m_macQueue = new MacQueue ();
}

Mac::~Mac ()
{
  delete m_macQueue;
}

void
Mac::SetPhyInterface (PhyInterface* phyInterface)
{
  m_phyInterface = phyInterface;
}

void
Mac::SetApp (App* app)
{
  m_app = app;
}

uint8_t
Mac::GetNodeAddress()
{
  return m_nodeAddress;
}

void
Mac::Initialize (uint8_t nodeAddress)
{
  if (m_phyInterface == 0)
    {
      std::cout << "Mac::Initialize: phyInterface object is 0. Enter 0 to exit." << std::endl;

      std::string s;
      std::cin >> s; // stop console to be closed

      exit(EXIT_FAILURE);
    }

  if (m_app == 0)
    {
      std::cout << "Mac::Initialize: app object is 0. Enter 0 to exit." << std::endl;

      std::string s;
      std::cin >> s; // stop console to be closed

      exit(EXIT_FAILURE);
    }

  m_nodeAddress = nodeAddress;
}

void
Mac::SendData (char* buffer, uint32_t lenHeadAndPayl)
{
  m_phyInterface->SendData (buffer, lenHeadAndPayl);
}


