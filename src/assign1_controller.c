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
#include <sys/netmgr.h>

#include "door_entry.h"

int main(int argc, const char* argv[]) {


	int inputID;	// indicates who we should reply to
	int chid;		// the channel ID

	int  displayCOID;
	pid_t displayPID;

	Person person; //person received from inputs
	Display display; //display sent to the display

	if( argc != 2 ){
		perror("Invalid argument");
		return EXIT_FAILURE;
	}
	displayPID = atoi(argv[1]);

	//Create the server channel
	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("failed to create the channel.");
		exit(EXIT_FAILURE);
	}

	printf("The controller is running as PID : %d\n", getpid());
	printf("Waiting for Person...");

	//Connect the display
	displayCOID = ConnectAttach (ND_LOCAL_NODE, displayPID, 1, _NTO_SIDE_CHANNEL, 0);
	if (displayCOID == -1) {
		fprintf (stderr, "Couldn't ConnectAttach\n");
		perror (NULL);
		exit (EXIT_FAILURE);
	}


	while (1) {
		inputID = MsgReceive(chid, &person, sizeof(person), NULL);

		switch(person.state){
		case START_STATE:
			break;
		case SCAN_STATE:
			break;
		case UNLOCK_LEFT_DOOR_STATE:
			break;
		case UNLOCK_RIGHT_DOOR_STATE:
			break;
		case OPEN_LEFT_STATE:
			break;
		case OPEN_RIGHT_STATE:
			break;
		case WEIGHT_STATE:
			break;
		case CLOSE_LEFT_STATE:
			break;
		case CLOSE_RIGHT_STATE:
			break;
		case LOCK_LEFT_DOOR_STATE:
			break;
		case LOCK_RIGHT_DOOR_STATE:
			break;
		case END_STATE:
			break;
		}

		// send the message
		/*if (MsgSend(displayCOID, &display, sizeof(display), &response, sizeof(response)) == -1) {
			fprintf (stderr, "Error during MsgSend\n");
			perror (NULL);
			exit (EXIT_FAILURE);
		}*/

        //MsgReply (inputID, EOK, &response, sizeof(response));
	}


	ChannelDestroy(chid);
	ConnectDetach(displayCOID);

	return EXIT_SUCCESS;
}
