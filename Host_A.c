#include "Sim_Engine.h"

#define SIZE 20
#define BUFFER_SIZE 20
#define INCREMENT 35

static int sending;
static struct pkt packe;
static int seq_pos;

/* Called when A's timer goes off */
void A_timerinterrupt()
{
	tolayer3(0, packe);
	starttimer(0, INCREMENT);
}

int Check_Sum(char *p)
{
	int sum = 0;

	for (int i = 0; i < SIZE; i++)
	{
		sum += p[i];
	}
	return sum;
}

void Generate_Packet(struct msg ms)
{
	strcpy(packe.payload, ms.data);
	packe.seqnum = seq_pos++;
	packe.acknum = 0;
	packe.checksum = Check_Sum(ms.data) + packe.seqnum + packe.acknum;
}

/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
	/* TODO */
	// MSG data structure to be sent to B
	// load message in buffer, check ACK, Generate packet with msg
	if (sending == 0)
	{
		Generate_Packet(message);
		starttimer(0, INCREMENT);
		tolayer3(0, packe);
		sending = 1;
	}
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
	/* TODO */
	// Packet recived from layer 3, might be corupted and have the wrong checksum
	// Räkna ut checksum med struct från Sim_engine samt den med skickade, kolla så dom stämmer överens
	stoptimer(0);
	
	if (packet.acknum == 1 && packet.seqnum == packe.seqnum)	
	{
		sending = 0;
	}
	else
	{
		starttimer(0, INCREMENT);
		tolayer3(0, packe);
	}
}

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init()
{
	/* TODO */
	sending = 0;
	seq_pos = 1;
}
