
#include <unistd.h>
#include <common/util.h>
#include <common/command.h>

unsigned char receive_controlcommand(int masterfd)
{
	ssize_t bytesread;
	unsigned char controlcommand;

	debug("trying to receive control command\n");
	bytesread = read(masterfd, &controlcommand, sizeof(controlcommand));
	debug("control command received\n");

	return controlcommand;
}

void send_controlcommand(int clientfd, unsigned char controlcommand)
{
	ssize_t byteswritten;

	debug("sending control command\n");
	byteswritten = write(clientfd, &controlcommand, sizeof(controlcommand));
	debug("control command sent\n");
}
