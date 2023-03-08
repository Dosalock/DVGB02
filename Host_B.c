#include "Sim_Engine.h"

#define SIZE 20
#define INCREMENT 35
struct pkt Last_pkt;

int Check_Sum_B(char *p)
{
  int sum = 0;

  for (int i = 0; i < SIZE; i++)
  {
    sum += p[i];
  }
  return sum;
}

/* Called from layer 5, passed the data to be sent to other side */
void B_output(struct msg message)
{
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet)
{
  /* TODO */
  int checksum = packet.acknum + packet.seqnum + Check_Sum_B(packet.payload);
  //printf("B_input() Checksum: %d, Calculated checksum: %d\n\n", packet.checksum, checksum);

  if (checksum != packet.checksum)
  {
    //printf( "Corrupted packet!!\n\n");
    packet.acknum = 0;
    tolayer3(1, packet);
  }
  else
  {
    packet.acknum = 1;
    if (packet.seqnum > Last_pkt.seqnum)
    {
      tolayer5(1, packet.payload);
      Last_pkt = packet;
    }
    tolayer3(1, packet);
  }
}

/* Called when B's timer goes off */
void B_timerinterrupt()
{
  /* TODO */
}

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init()
{
  /* TODO */
}
