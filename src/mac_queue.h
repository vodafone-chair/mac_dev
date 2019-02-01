
#ifndef MAC_QUEUE_H
#define MAC_QUEUE_H

#include <list>
#include <utility>
#include <ctime>

#include "datagram.h"

class MacQueue
{
public:

  class QueueEntry
  {
  public:
    PacketData m_packet; // Actual packet
    time_t m_arrivalTime; // time stamp when the packet arrived at the queue

    QueueEntry (PacketData packet, time_t arrivalTime):
      m_packet(packet),
      m_arrivalTime(arrivalTime)
    {
    }
  };

  MacQueue ();
  MacQueue (uint32_t maxSize, time_t maxDelay);
  virtual~MacQueue ();

  void SetMaxSize (uint32_t maxSize);
  uint32_t GetMaxSize (void) const;
  uint32_t GetSize (void) const;

  void SetMaxDelay (time_t delay);
  time_t GetMaxDelay (void) const;

  bool IsEmpty (void);

  void Enqueue (PacketData packet);
  PacketData Dequeue ();

private:
  void Cleanup (void);

  uint32_t m_maxSize;              // Max queue capacity
  time_t m_maxDelay;               // Time to live for packets in the queue

  std::list<QueueEntry> m_queue;  // Packet (QueueEntry) queue
};


#endif /* MAC_QUEUE_H */
