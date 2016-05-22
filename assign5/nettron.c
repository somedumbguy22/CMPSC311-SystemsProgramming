#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "nettron.h"

/*
 * Networked tronctl.  Uses the MultiTron message protocol to communicate with a
 * MultiTron server.
 *
 * Parameters:
 *   sock     File descriptor of a socket which is connected to the server.
 *            This parameter is passed by reference.  If the descriptor is
 *            negative, this function should connect to the default server/port
 *            and store the socket descriptor here before sending any data.
 *
 *   instr    MultiTron instruction (the parameter formerly known as "op").  The
 *            format of this parameter is exactly the same as in previous
 *            assignments.
 *
 *   data     Data buffer for the operation.  This parameter should function
 *            exactly as in previous assignments.
 *
 *   datalen  Size in bytes of the data buffer.
 *
 * Return value: 0 for success, nonzero for failure
 */
int tronctl_net(int *sock, uint32_t instr, void *data, int datalen)
{
	/*a variable for testing if functions function*/
	int success;

	/*Specifying the address into a client side struct*/
	char *ip = MULTITRON_DEFAULT_IP;
	unsigned short port = MULTITRON_DEFAULT_PORT;
	struct sockaddr_in soccer;
	soccer.sin_family = AF_INET;
	soccer.sin_port = htons(port);

	success = inet_aton(ip, &soccer.sin_addr);
	if(success == 0)
	{
		printf("Error: Failed setup of address info.\n");
		return ( -1 );
	}

	/*Creates a socket file descriptor, 
	*if current one is null*/
	if(*sock == -1)
	{
		*sock = socket(AF_INET, SOCK_STREAM, 0);
		if (*sock == -1)
		{
			printf("Error: Socket Creation");
			return ( -1 );
		}

		/*This establishes a connection if the sock passed in is not valid*/
		/*It uses the default port and IP Address*/
		success = connect(*sock, (const struct sockaddr*) &soccer, sizeof(struct sockaddr));
		if(success == -1)
		{
			printf("Error: Connection setup error.\n");
			return ( -1 );
		}
	}

	
	//------------------------Sending Response Packet--------------------------//
	/*Making sure everything is in correct byte order*/
	uint32_t opCode = htonl(instr);
	uint16_t dLength = htons(datalen);

	/*Writing the instruction set*/
	success = write(*sock, &opCode, sizeof(opCode));
	if(success != sizeof(opCode))
	{
		printf("Error: Failure to write Instruction Code\n");
		return ( -1 );
	}

	/*Writing the legnth of the data*/
	success = write(*sock, &dLength, sizeof(dLength));
	if(success != sizeof(dLength))
	{
		printf("Error: Failure to write data length\n");
		return ( -1 );
	}

	/*Creating a return value to pass in*/
	uint16_t returnValue = 0x0;

	/*Writing the return value*/
	success = write(*sock, &returnValue, sizeof(returnValue));
	if(success != sizeof(uint16_t))
	{
		printf("Error: Failure to write Return value\n");
	}

	/*Writing the data*/
	success = write(*sock, data, datalen);
	if(success != datalen)
	{
		printf("Error: Failure to write Data\n");
		return ( -1 );
	}

	//------------------------------------------------------------------//

	//----------------------Reading Response Packet---------------------//

	/*Reading the instruction and converting to correct byte order*/
	success = read(*sock, &opCode, sizeof(opCode));
	if(success != sizeof(opCode))
	{
		printf("Error: Failure to read instruction\n");
		return ( -1 );
	}
	instr = ntohl(opCode);

	/*Reading the data length and converting to correct byte order*/
	success = read(*sock, &dLength, sizeof(dLength));
	if(success != sizeof(dLength))
	{
		printf("Error: Failure to read Data Length\n");
		return ( -1 );
	}
	datalen = ntohs(dLength);

	/*Reading the return value and making sure it is correct*/
	success = read(*sock, &returnValue, sizeof(returnValue));
	if(success != sizeof(returnValue))
	{
		printf("Error: Failure to read return value\n");
		return ( -1 );
	}

	if(ntohs(returnValue) != 0)
	{
		return ( -1 );
	}
	
	/*Reading the data*/
	success = read(*sock, data, datalen);
	if(success != datalen)
	{
		printf("Error: Failure to read Data\n");
		return ( -1 );
	}
	
	//-----------------------------------------------------------//
	
	return 0;
}