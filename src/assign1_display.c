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

	//Create the server channel
	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("failed to create the channel.");
		exit(EXIT_FAILURE);
	}

	printf("The display is running as PID : %d\n", getpid());

	while (1) {

		controllerID = MsgReceive(chid, &display, sizeof(display), NULL);


		switch(display.msgIndex){
		case SCAN_ACK:
			printf(outMessage[display.msgIndex], display.person.personID);
			break;
		case WEIGHT_ACK:
			printf(outMessage[display.msgIndex], display.person.weight);
			break;

		case SCAN_REQUEST:
		case UNLOCK_LEFT_DOOR:
		case UNLOCK_RIGHT_DOOR:
		case OPEN_LEFT_DOOR:
		case OPEN_RIGHT_DOOR:
		case WEIGHT_REQUEST:
		case CLOSE_LEFT_DOOR:
		case CLOSE_RIGHT_DOOR:
		case LOCK_LEFT_DOOR:
		case LOCK_RIGHT_DOOR:
			printf(outMessage[display.msgIndex]);
			break;
		}

        //MsgReply (controllerID, EOK, &response, sizeof(response));
	}

    ChannelDestroy(chid);
	return EXIT_SUCCESS;
}
