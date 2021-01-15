/*
 * assign1_display.h
 *
 *  Created on: Feb 25, 2019
 *  Author: Tiago Rascazzi, Armando Arenas
 */

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>

#include "../../Real-Time_Assign1_Controller/src/door_entry.h"

int main(void) {

	int controllerID;	// indicates who we should reply to
	int chid;			// the channel ID

	Display display;
	int running = 1;

	//Create the server channel
	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("failed to create the channel.");
		exit(EXIT_FAILURE);
	}

	printf("The display is running as PID : %d\n", getpid());
	fflush(stdout);

	while (running) {

		controllerID = MsgReceive(chid, &display, sizeof(display), NULL);

		switch(display.msgIndex){
		case SCAN_ACK:
			printf("%s %d\n", outMessage[display.msgIndex], display.person.personID);
			break;
		case WEIGHT_ACK:
			printf("%s %d\n", outMessage[display.msgIndex], display.person.weight);
			break;

		case UNLOCKED_LEFT_DOOR:
		case UNLOCKED_RIGHT_DOOR:
		case OPENED_LEFT_DOOR:
		case OPENED_RIGHT_DOOR:
		case CLOSED_LEFT_DOOR:
		case CLOSED_RIGHT_DOOR:
		case LOCKED_LEFT_DOOR:
		case LOCKED_RIGHT_DOOR:
		case INVALID_INPUT:
			printf("%s\n", outMessage[display.msgIndex]);
			break;
		case EXIT_MSG:
			printf("%s\n", outMessage[display.msgIndex]);
			running = 0;
			break;
		}
		fflush(stdout);

        MsgReply (controllerID, EOK, NULL, 0);
	}

    ChannelDestroy(chid);
	return EXIT_SUCCESS;
}
