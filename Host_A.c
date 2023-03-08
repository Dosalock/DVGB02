#include "Sim_Engine.h"

#define SIZE 20
#define BUFFER_SIZE 20
#define INCREMENT 35

int ACK0, sending;
struct pkt packe;
struct pkt l_pkt;
int seq_pos;

/* Called when A's timer goes off */
void A_timerinterrupt()
{
	//printf("TIMER WENT OFF IDK HOW LOL\n");
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
	//printf("seq_pos ++ %d\n message %s ms.data\n", seq_pos, ms.data);
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
		//printf("sending: %s\n", packe.payload);
		starttimer(0, INCREMENT);
		tolayer3(0, packe);
		sending = 1;
		l_pkt = packe;
	}
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
	/* TODO */
	// Packet recived from layer 3, might be corupted and have the wrong checksum
	// Räkna ut checksum med struct från Sim_engine samt den med skickade, kolla så dom stämmer överens
	stoptimer(0);
	int checksum = packet.acknum + packet.seqnum + Check_Sum(packet.payload);
	//printf("seq_pos %d before A_input\n", seq_pos);
	//printf("A_input() packet Checksum: %d, Calculated checksum: %d, Packe Checksum %d\n\n", packet.checksum, checksum, packe.checksum);
	if (packet.acknum == 1 && packet.seqnum == l_pkt.seqnum)	
	{
		//seq_pos--;
		sending = 0;

		if(seq_pos == 0){
			sending = 1;
			starttimer(0, INCREMENT);	
			tolayer3(0, packe);
		}
	}
	else
	{
		starttimer(0, INCREMENT);
		tolayer3(0, l_pkt);
	}
	//printf("seq_pos %d after A_input\n", seq_pos);
}

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init()
{
	/* TODO */
	sending = 0;
	ACK0 = 1;
	seq_pos = 1;
}
