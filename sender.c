/*
 * sender.c
 * sender Multicast udp-packet's
 *  Created on: 28 февр. 2018 г.
 *      Author: jake
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> //для close()
#include <stdlib.h> // для exit()

int main(void)
{
	char msg1[]="Multicast-1 packet";
	char msg2[]="Multicast-2 packet";
	struct sockaddr_in multi1,multi2;
	const int optval = 1;
	int sock = socket(AF_INET,SOCK_DGRAM,0);

	if (sock<0)
	{
		perror("socket error");
		exit(1);
	}
	bzero(&multi1, sizeof(multi1));
	bzero(&multi2, sizeof(multi2));
// Первая группа
	inet_aton("224.0.0.1",&multi1.sin_addr);
	multi1.sin_port = htons(50000);
	multi1.sin_family = AF_INET;
// Вторая группа
	inet_aton("224.0.0.2",&multi2.sin_addr);
	multi2.sin_port = htons(50000);
	multi2.sin_family = AF_INET;

	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
// Отправка в каждую группу по пакету
	sendto(sock,msg1,sizeof(msg1),0,(struct sockaddr*)&multi1,sizeof(multi1));
	sendto(sock,msg2,sizeof(msg2),0,(struct sockaddr*)&multi2,sizeof(multi2));

	close(sock);
	return 0;
}
