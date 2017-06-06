/*
 * Server.c
 *
 *  Created on: Jun 5, 2017
 *      Author: chaoticace
 */
#include "Server.h"

static const int BUFFER_SIZE = 32768;

int main(int argc, char **argv) {
	struct sockaddr saddr;
	struct sockaddr_in srcaddr, destaddr;
	int running, status, sd, saddr_len = sizeof(saddr);
	unsigned char* packet_buffer = (unsigned char*) calloc(1, BUFFER_SIZE);

	sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (sd < 0) {
		perror("socket()");
	}

	running = 1;
	while (running) {
		memset(&saddr, 0, sizeof(saddr));

		status = recvfrom(sd, packet_buffer, BUFFER_SIZE, 0, &saddr,
				(socklen_t*) &saddr_len);

		if (status < 1) {
			perror("recvfrom()");
			running = 0;
		}

		struct ethhdr* ethhdr;
		struct iphdr* iphdr;

		ethhdr = (struct ethhdr*) packet_buffer;
		iphdr = (struct iphdr*) (packet_buffer + sizeof(struct ethhdr));
		memset(&srcaddr, 0, sizeof(srcaddr));
		memset(&destaddr, 0, sizeof(destaddr));
		srcaddr.sin_addr = ((struct sockaddr_in*) &saddr)->sin_addr;
		destaddr.sin_addr.s_addr = iphdr->daddr;
		if (ethhdr->h_proto == htons(ETH_P_IP)) {
			if (iphdr->protocol == IPPROTO_TCP) {
				printf("size: %d src: %s, dest: %s \n ", status,
						inet_ntoa(srcaddr.sin_addr),
						inet_ntoa(destaddr.sin_addr));

			}
		}
		memset(packet_buffer, 0, (size_t) BUFFER_SIZE);

	}

	close(sd);
	free(packet_buffer);
}
