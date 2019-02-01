#include "mac_queue.h"

MacQueue::MacQueue () :
    m_maxSize (1000), m_maxDelay (60)
{
}

MacQueue::MacQueue (uint32_t maxSize, time_t maxDelay) :
    m_maxSize (maxSize), m_maxDelay (maxDelay)
{

}

MacQueue::~MacQueue ()
{
}

void
MacQueue::Enqueue (PacketData packet)
{
  Cleanup ();

  if (GetSize () == m_maxSize)
    {
      std::cout << "MacQueue::Enqueue: buffer overflow" << std::endl;
      return;
    }

  m_queue.push_back (QueueEntry (packet, time (NULL)));
}

PacketData
MacQueue::Dequeue ()
{
  Cleanup ();

  if (!m_queue.empty ())
    {
      QueueEntry queueEntry = m_queue.front ();
      m_queue.pop_front ();

      return queueEntry.m_packet;
    }

  return PacketData ();
}

void
MacQueue::Cleanup (void)
{
  for (std::list<QueueEntry>::iterator iterQueueEntry = m_queue.begin (); iterQueueEntry != m_queue.end ();)
    {
      if (iterQueueEntry->m_arrivalTime + m_maxDelay <= time (NULL))
        {
          iterQueueEntry = m_queue.erase (iterQueueEntry);
        }
      else
        break; // packets are ordered, when the first packet is not expired, we can stop
    }
}

void
MacQueue::SetMaxSize (uint32_t maxSize)
{
  m_maxSize = maxSize;
}

uint32_t
MacQueue::GetMaxSize (void) const
{
  return m_maxSize;
}

void
MacQueue::SetMaxDelay (time_t delay)
{
  m_maxDelay = delay;
}

time_t
MacQueue::GetMaxDelay (void) const
{
  return m_maxDelay;
}

uint32_t
MacQueue::GetSize (void) const
{
  return m_queue.size ();
}

bool
MacQueue::IsEmpty (void)
{
  Cleanup ();
  return m_queue.empty ();
}

