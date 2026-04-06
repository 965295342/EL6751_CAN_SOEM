#ifndef SENDUDP_H
#define SENDUDP_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <math.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>


typedef struct udp_struct
{
	int udp_socket_fd;
	struct sockaddr_in local_addr;
	struct sockaddr_in dest_addr;
} udp_struct;

uint8_t init_udp(int port, udp_struct *udp);
int sendUdpMsg(float msg[], int length, udp_struct *udp);

#endif /* SENDUDP_H */