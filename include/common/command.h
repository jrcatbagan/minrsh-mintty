

#ifndef COMMAND_H
#define COMMAND_H

#define CMD_MORE_DATA 	0x55
#define CMD_NO_DATA	0x22
#define ACK 		0x66

unsigned char receive_controlcommand(int masterfd);
void send_controlcommand(int clientfd, unsigned char controlcommand);

#endif /* COMMAND_H */
