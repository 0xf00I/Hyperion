/*
 * Header which gives access to the modules initiator routines
 */

#ifndef __C2_H
#define __C2_H

/* Default buffer size for communication */
#define BUF_SIZE	1024

/* Setup and run the data extraction module */
int dext_init(int port);

#endif/*__C2_H */