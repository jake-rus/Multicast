/*
 * receiver.c
 *	receiver multicast udp-packet's
 *  Created on: 28 февр. 2018 г.
 *      Author: jake
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <linux/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> //для close()
#include <stdlib.h> // для exit()

#define MULTIADDR "224.0.0.2"

int main(void)
{
	char buf[BUFSIZ];
	const int optval=1;
	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	struct ip_mreq mr;

	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if (sock < 0)
	{
		perror("socket error");
		exit(1);
	}
	bzero(&local, sizeof(local));
	bzero(&from, fromlen);
	bzero(&mr, sizeof(mr));

// Биндимся к адресу Группы (иначе будем принимать все)
	inet_aton(MULTIADDR,&local.sin_addr);
	local.sin_port = htons(50000);
	local.sin_family = AF_INET;
	if (bind(sock, (struct sockaddr*) &local, sizeof(local)) < 0)
	{
		perror("binding error");
		exit(2);
	}

// вступаем в группу Мультикаста
	inet_aton(MULTIADDR,&mr.imr_multiaddr);
	mr.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mr,sizeof(mr));
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

// получаем пакет группы
	if (recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&from,&fromlen) <0)
		{
			perror("recvfrom error");
			exit(3);
		}

	printf("%s	from %s-%d\n",buf,inet_ntoa(from.sin_addr),from.sin_port);
	close(sock);
	return 0;
}
